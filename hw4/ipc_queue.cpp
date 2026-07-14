#include "ipc_queue.h"

#include <algorithm>
#include <cstring>
#include <thread>

namespace hw4 {

namespace {

std::size_t buffer_offset() {
    return offsetof(SharedQueueLayout, data);
}

void write_ring_bytes(unsigned char* buffer,
                      std::size_t capacity,
                      std::uint64_t position,
                      const void* src,
                      std::size_t length) {
    if (length == 0) {
        return;
    }

    std::size_t start = static_cast<std::size_t>(position % capacity);
    std::size_t first_part = std::min(length, capacity - start);

    std::memcpy(buffer + start, src, first_part);

    if (first_part < length) {
        std::memcpy(buffer, static_cast<const unsigned char*>(src) + first_part, length - first_part);
    }
}

void read_ring_bytes(const unsigned char* buffer,
                     std::size_t capacity,
                     std::uint64_t position,
                     void* dst,
                     std::size_t length) {
    if (length == 0) {
        return;
    }

    std::size_t start = static_cast<std::size_t>(position % capacity);
    std::size_t first_part = std::min(length, capacity - start);

    std::memcpy(dst, buffer + start, first_part);

    if (first_part < length) {
        std::memcpy(static_cast<unsigned char*>(dst) + first_part, buffer, length - first_part);
    }
}

} // namespace

std::size_t get_min_queue_memory_size() {
    return buffer_offset();
}

std::size_t get_data_capacity(std::size_t shm_size) {
    if (shm_size <= get_min_queue_memory_size()) {
        return 0;
    }

    return shm_size - get_min_queue_memory_size();
}

void init_queue(SharedQueueLayout* queue, std::size_t shm_size) {
    std::size_t capacity = get_data_capacity(shm_size);

    queue->meta.magic = PROTOCOL_MAGIC;
    queue->meta.version = PROTOCOL_VERSION;
    queue->meta.shm_size = shm_size;
    queue->meta.data_capacity = capacity;

    queue->reserve_head.store(0, std::memory_order_relaxed);
    queue->publish_head.store(0, std::memory_order_relaxed);
    queue->tail.store(0, std::memory_order_relaxed);

    std::memset(queue->data, 0, capacity);
}

bool is_queue_valid(const SharedQueueLayout* queue, std::size_t shm_size) {
    if (queue == nullptr) {
        return false;
    }

    if (queue->meta.magic != PROTOCOL_MAGIC) {
        return false;
    }

    if (queue->meta.version != PROTOCOL_VERSION) {
        return false;
    }

    if (queue->meta.shm_size != shm_size) {
        return false;
    }

    if (queue->meta.data_capacity != get_data_capacity(shm_size)) {
        return false;
    }

    return true;
}

bool try_push_message(SharedQueueLayout* queue,
                      MessageType type,
                      const void* data,
                      std::size_t length) {
    if (queue == nullptr) {
        return false;
    }

    if (length > 0 && data == nullptr) {
        return false;
    }

    const std::size_t capacity = static_cast<std::size_t>(queue->meta.data_capacity);
    const std::size_t total_size = sizeof(MessageHeader) + length;

    if (capacity == 0 || total_size > capacity) {
        return false;
    }

    std::uint64_t start_position = 0;

    while (true) {
        std::uint64_t tail_snapshot = queue->tail.load(std::memory_order_acquire);
        std::uint64_t reserve_snapshot = queue->reserve_head.load(std::memory_order_relaxed);

        std::uint64_t used = reserve_snapshot - tail_snapshot;
        if (used + total_size > capacity) {
            return false;
        }

        std::uint64_t next_reserve = reserve_snapshot + total_size;

        if (queue->reserve_head.compare_exchange_weak(
                reserve_snapshot,
                next_reserve,
                std::memory_order_acq_rel,
                std::memory_order_relaxed)) {
            start_position = reserve_snapshot;
            break;
        }
    }

    MessageHeader header{};
    header.type = static_cast<std::uint32_t>(type);
    header.length = static_cast<std::uint32_t>(length);

    write_ring_bytes(queue->data, capacity, start_position, &header, sizeof(header));
    write_ring_bytes(queue->data, capacity, start_position + sizeof(header), data, length);

    while (queue->publish_head.load(std::memory_order_acquire) != start_position) {
        std::this_thread::yield();
    }

    queue->publish_head.store(start_position + total_size, std::memory_order_release);
    return true;
}

bool try_pop_message(SharedQueueLayout* queue,
                     MessageType expected_type,
                     std::vector<char>* out_data,
                     MessageHeader* out_header) {
    if (queue == nullptr || out_data == nullptr || out_header == nullptr) {
        return false;
    }

    const std::size_t capacity = static_cast<std::size_t>(queue->meta.data_capacity);
    if (capacity == 0) {
        return false;
    }

    std::uint64_t tail_snapshot = queue->tail.load(std::memory_order_relaxed);
    std::uint64_t published_snapshot = queue->publish_head.load(std::memory_order_acquire);

    if (published_snapshot - tail_snapshot < sizeof(MessageHeader)) {
        return false;
    }

    MessageHeader header{};
    read_ring_bytes(queue->data, capacity, tail_snapshot, &header, sizeof(header));

    std::size_t total_size = sizeof(MessageHeader) + header.length;

    if (header.length > capacity) {
        return false;
    }

    if (published_snapshot - tail_snapshot < total_size) {
        return false;
    }

    if (header.type == static_cast<std::uint32_t>(expected_type)) {
        out_data->resize(header.length);

        if (header.length > 0) {
            read_ring_bytes(
                queue->data,
                capacity,
                tail_snapshot + sizeof(MessageHeader),
                out_data->data(),
                header.length
            );
        }

        *out_header = header;
    }

    queue->tail.store(tail_snapshot + total_size, std::memory_order_release);

    if (header.type != static_cast<std::uint32_t>(expected_type)) {
        return false;
    }

    return true;
}

}

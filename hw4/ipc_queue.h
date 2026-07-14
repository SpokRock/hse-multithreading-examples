#ifndef IPC_QUEUE_H
#define IPC_QUEUE_H

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace hw4 {

const std::uint32_t PROTOCOL_MAGIC = 0x48573431;
const std::uint32_t PROTOCOL_VERSION = 1;

enum class MessageType : std::uint32_t {
    UNKNOWN = 0,
    TEXT = 1,
    NUMBER = 2
};

struct MessageHeader {
    std::uint32_t type;
    std::uint32_t length;
};

struct QueueMeta {
    std::uint32_t magic;
    std::uint32_t version;
    std::uint64_t shm_size;
    std::uint64_t data_capacity;
};

struct SharedQueueLayout {
    QueueMeta meta;

    alignas(64) std::atomic<std::uint64_t> reserve_head;
    alignas(64) std::atomic<std::uint64_t> publish_head;
    alignas(64) std::atomic<std::uint64_t> tail;

    alignas(64) unsigned char data[1];
};

std::size_t get_min_queue_memory_size();
std::size_t get_data_capacity(std::size_t shm_size);

void init_queue(SharedQueueLayout* queue, std::size_t shm_size);
bool is_queue_valid(const SharedQueueLayout* queue, std::size_t shm_size);

bool try_push_message(SharedQueueLayout* queue,
                      MessageType type,
                      const void* data,
                      std::size_t length);

bool try_pop_message(SharedQueueLayout* queue,
                     MessageType expected_type,
                     std::vector<char>* out_data,
                     MessageHeader* out_header);

}

#endif

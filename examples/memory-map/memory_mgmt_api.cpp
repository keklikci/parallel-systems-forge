#include <algorithm>
#include <condition_variable>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <vector>

struct Request {
    int thread_id;
    std::size_t size;
};

class MemoryManager {
  public:
    explicit MemoryManager(std::size_t capacity)
        : memory_(capacity, '.'), next_offset_(0), stopping_(false) {}
    void start() { server_ = std::thread(&MemoryManager::serve, this); }
    void stop() {
        {
            std::lock_guard lock(mutex_);
            stopping_ = true;
        }
        condition_.notify_one();
        server_.join();
    }
    int allocate(int thread_id, std::size_t size) {
        std::unique_lock lock(mutex_);
        requests_.push({thread_id, size});
        replies_.resize(std::max(replies_.size(), static_cast<std::size_t>(thread_id + 1)), -2);
        condition_.notify_one();
        reply_condition_.wait(lock, [&] { return replies_[thread_id] != -2; });
        const int offset = replies_[thread_id];
        replies_[thread_id] = -2;
        return offset;
    }
    void write(int offset, int thread_id, std::size_t size) {
        std::lock_guard lock(mutex_);
        if (offset >= 0)
            for (std::size_t i = 0; i < size; ++i)
                memory_[offset + i] = char('0' + thread_id);
    }
    void dump() const {
        for (std::size_t i = 0; i < memory_.size(); ++i) {
            if (i % 30 == 0)
                std::cout << '\n';
            std::cout << memory_[i] << ' ';
        }
        std::cout << '\n';
    }

  private:
    void serve() {
        for (;;) {
            std::unique_lock lock(mutex_);
            condition_.wait(lock, [&] { return stopping_ || !requests_.empty(); });
            if (requests_.empty() && stopping_)
                return;
            const Request request = requests_.front();
            requests_.pop();
            int offset = -1;
            if (next_offset_ + request.size <= memory_.size()) {
                offset = static_cast<int>(next_offset_);
                next_offset_ += request.size;
            }
            replies_[request.thread_id] = offset;
            reply_condition_.notify_all();
        }
    }
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<Request> requests_;
    std::vector<char> memory_;
    std::vector<int> replies_;
    std::condition_variable reply_condition_;
    std::size_t next_offset_;
    bool stopping_;
    std::thread server_;
};

int main(int argc, char **argv) {
    const int thread_count = argc > 1 ? std::atoi(argv[1]) : 10;
    const unsigned seed = argc > 2 ? static_cast<unsigned>(std::strtoul(argv[2], nullptr, 10)) : 1;
    const std::size_t capacity =
        argc > 3 ? static_cast<std::size_t>(std::strtoul(argv[3], nullptr, 10)) : 150;
    if (thread_count <= 0) {
        std::cerr << "thread count must be positive\n";
        return EXIT_FAILURE;
    }
    if (capacity == 0) {
        std::cerr << "capacity must be positive\n";
        return EXIT_FAILURE;
    }
    MemoryManager manager(capacity);
    manager.start();
    std::mt19937 random(seed);
    std::vector<std::size_t> sizes;
    std::uniform_int_distribution<int> size_distribution(1, 25);
    for (int id = 0; id < thread_count; ++id)
        sizes.push_back(static_cast<std::size_t>(size_distribution(random)));
    std::vector<std::thread> workers;
    for (int id = 0; id < thread_count; ++id)
        workers.emplace_back([&, id] {
            const std::size_t size = sizes[id];
            const int offset = manager.allocate(id, size);
            if (offset < 0)
                std::cout << "Thread " << id << ": Not enough memory\n";
            manager.write(offset, id, size);
        });
    for (auto &worker : workers)
        worker.join();
    manager.stop();
    manager.dump();
    return EXIT_SUCCESS;
}

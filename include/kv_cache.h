#pragma once
#include <cstddef>
#include <cstdint>
#include <unordered_map>

class KvCacheTracker {
public:
    explicit KvCacheTracker(std::size_t capacity_tokens);
    bool reserve(std::uint64_t request_id, std::size_t tokens);
    void release(std::uint64_t request_id);
    std::size_t used_tokens() const { return used_tokens_; }
    double utilization() const;

private:
    std::size_t capacity_tokens_;
    std::size_t used_tokens_{0};
    std::unordered_map<std::uint64_t, std::size_t> allocations_;
};

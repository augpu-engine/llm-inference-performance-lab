#include "kv_cache.h"

KvCacheTracker::KvCacheTracker(std::size_t capacity_tokens)
    : capacity_tokens_(capacity_tokens) {}

bool KvCacheTracker::reserve(std::uint64_t request_id, std::size_t tokens) {
    if (used_tokens_ + tokens > capacity_tokens_) return false;
    allocations_[request_id] = tokens;
    used_tokens_ += tokens;
    return true;
}

void KvCacheTracker::release(std::uint64_t request_id) {
    const auto it = allocations_.find(request_id);
    if (it == allocations_.end()) return;
    used_tokens_ -= it->second;
    allocations_.erase(it);
}

double KvCacheTracker::utilization() const {
    return capacity_tokens_ == 0 ? 0.0 :
        static_cast<double>(used_tokens_) / static_cast<double>(capacity_tokens_);
}

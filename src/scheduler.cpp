#include "scheduler.h"
#include <algorithm>

DynamicBatchScheduler::DynamicBatchScheduler(std::size_t max_batch_size, int max_batch_tokens)
    : max_batch_size_(max_batch_size), max_batch_tokens_(max_batch_tokens) {}

std::vector<Batch> DynamicBatchScheduler::build_batches(std::vector<InferenceRequest> requests) const {
    std::stable_sort(requests.begin(), requests.end(),
        [](const auto& a, const auto& b) { return a.prompt_tokens < b.prompt_tokens; });

    std::vector<Batch> batches;
    Batch current;

    for (const auto& r : requests) {
        const bool full_by_count = current.requests.size() >= max_batch_size_;
        const bool full_by_tokens =
            !current.requests.empty() &&
            current.total_prompt_tokens + r.prompt_tokens > max_batch_tokens_;

        if (full_by_count || full_by_tokens) {
            batches.push_back(std::move(current));
            current = Batch{};
        }

        current.total_prompt_tokens += r.prompt_tokens;
        current.requests.push_back(r);
    }

    if (!current.requests.empty())
        batches.push_back(std::move(current));

    return batches;
}

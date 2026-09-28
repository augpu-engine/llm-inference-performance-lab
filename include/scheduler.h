#pragma once
#include "request.h"
#include <vector>

struct Batch {
    std::vector<InferenceRequest> requests;
    int total_prompt_tokens{};
};

class DynamicBatchScheduler {
public:
    DynamicBatchScheduler(std::size_t max_batch_size, int max_batch_tokens);
    std::vector<Batch> build_batches(std::vector<InferenceRequest> requests) const;

private:
    std::size_t max_batch_size_;
    int max_batch_tokens_;
};

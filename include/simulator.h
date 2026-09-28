#pragma once
#include "scheduler.h"
#include "kv_cache.h"
#include "metrics.h"
#include <vector>

struct SimulationConfig {
    std::size_t max_batch_size{8};
    int max_batch_tokens{4096};
    std::size_t kv_capacity_tokens{32768};
};

class InferenceSimulator {
public:
    explicit InferenceSimulator(SimulationConfig config);
    std::vector<SampleMetrics> run(std::vector<InferenceRequest> requests);

private:
    SimulationConfig config_;
};

#include "scheduler.h"
#include <cassert>
#include <chrono>

int main() {
    DynamicBatchScheduler scheduler(2, 500);
    const auto now = std::chrono::steady_clock::now();

    std::vector<InferenceRequest> requests = {
        {1, 100, 32, now},
        {2, 120, 32, now},
        {3, 400, 32, now}
    };

    const auto batches = scheduler.build_batches(requests);
    assert(batches.size() == 2);
    assert(batches[0].requests.size() == 2);
    assert(batches[1].requests.size() == 1);
}

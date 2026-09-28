#include "simulator.h"
#include <algorithm>

InferenceSimulator::InferenceSimulator(SimulationConfig config) : config_(config) {}

std::vector<SampleMetrics> InferenceSimulator::run(std::vector<InferenceRequest> requests) {
    DynamicBatchScheduler scheduler(config_.max_batch_size, config_.max_batch_tokens);
    KvCacheTracker cache(config_.kv_capacity_tokens);
    const auto batches = scheduler.build_batches(std::move(requests));

    std::vector<SampleMetrics> samples;
    double simulated_clock_ms = 0.0;

    for (const auto& batch : batches) {
        int max_prompt = 0;
        int max_decode = 0;

        for (const auto& r : batch.requests) {
            max_prompt = std::max(max_prompt, r.prompt_tokens);
            max_decode = std::max(max_decode, r.max_new_tokens);
            cache.reserve(r.id, static_cast<std::size_t>(r.prompt_tokens + r.max_new_tokens));
        }

        // Synthetic cost model: batching amortizes fixed launch/dispatch overhead.
        const double prefill_ms = 2.5 + 0.018 * max_prompt * batch.requests.size();
        const double decode_ms = 0.42 * max_decode;
        const double batch_latency = prefill_ms + decode_ms;

        for (const auto& r : batch.requests) {
            SampleMetrics m;
            m.queue_ms = simulated_clock_ms;
            m.ttft_ms = simulated_clock_ms + prefill_ms;
            m.latency_ms = simulated_clock_ms + batch_latency;
            m.generated_tokens = r.max_new_tokens;
            samples.push_back(m);
            cache.release(r.id);
        }

        simulated_clock_ms += batch_latency;
    }

    return samples;
}

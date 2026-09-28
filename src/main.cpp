#include "simulator.h"
#include "metrics.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    std::vector<InferenceRequest> workload;
    for (std::uint64_t i = 0; i < 48; ++i) {
        workload.push_back({
            i,
            64 + static_cast<int>((i * 97) % 900),
            32 + static_cast<int>((i * 31) % 160),
            std::chrono::steady_clock::now()
        });
    }

    for (std::size_t batch_size : {1u, 4u, 8u}) {
        SimulationConfig config;
        config.max_batch_size = batch_size;
        config.max_batch_tokens = 4096;

        InferenceSimulator simulator(config);
        const auto samples = simulator.run(workload);

        double elapsed_s = 0.0;
        for (const auto& s : samples)
            elapsed_s = std::max(elapsed_s, s.latency_ms / 1000.0);

        const auto summary = summarize(samples, elapsed_s);

        std::cout << "batch=" << batch_size
                  << " p50_ms=" << std::fixed << std::setprecision(1) << summary.p50_latency_ms
                  << " p95_ms=" << summary.p95_latency_ms
                  << " avg_ttft_ms=" << summary.avg_ttft_ms
                  << " tok/s=" << summary.tokens_per_second << '\n';
    }
}

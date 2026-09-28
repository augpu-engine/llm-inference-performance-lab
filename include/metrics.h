#pragma once
#include <vector>
#include <cstddef>

struct SampleMetrics {
    double queue_ms{};
    double ttft_ms{};
    double latency_ms{};
    int generated_tokens{};
};

struct SummaryMetrics {
    double p50_latency_ms{};
    double p95_latency_ms{};
    double avg_ttft_ms{};
    double tokens_per_second{};
};

SummaryMetrics summarize(const std::vector<SampleMetrics>& samples, double elapsed_seconds);

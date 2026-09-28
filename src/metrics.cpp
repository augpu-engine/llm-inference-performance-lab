#include "metrics.h"
#include <algorithm>
#include <numeric>

static double percentile(std::vector<double> v, double p) {
    if (v.empty()) return 0.0;
    std::sort(v.begin(), v.end());
    const auto i = static_cast<std::size_t>((v.size() - 1) * p);
    return v[i];
}

SummaryMetrics summarize(const std::vector<SampleMetrics>& samples, double elapsed_seconds) {
    SummaryMetrics out;
    std::vector<double> latencies;
    double ttft_sum = 0.0;
    int total_tokens = 0;

    for (const auto& s : samples) {
        latencies.push_back(s.latency_ms);
        ttft_sum += s.ttft_ms;
        total_tokens += s.generated_tokens;
    }

    out.p50_latency_ms = percentile(latencies, 0.50);
    out.p95_latency_ms = percentile(latencies, 0.95);
    out.avg_ttft_ms = samples.empty() ? 0.0 : ttft_sum / samples.size();
    out.tokens_per_second = elapsed_seconds <= 0.0 ? 0.0 : total_tokens / elapsed_seconds;
    return out;
}

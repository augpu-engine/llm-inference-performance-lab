#pragma once
#include <cstdint>
#include <chrono>

struct InferenceRequest {
    std::uint64_t id{};
    int prompt_tokens{};
    int max_new_tokens{};
    std::chrono::steady_clock::time_point arrival;
};

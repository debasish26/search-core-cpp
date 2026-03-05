#ifndef BENCHMARKS_H
#define BENCHMARKS_H

#include <string>
#include <unordered_map>
#include <chrono>

void benchmarks(
    const std::string &fileName,
    size_t tokens,
    const std::unordered_map<std::string,int> &frequencies,
    std::chrono::duration<double, std::milli> tokenizer_time,
    std::chrono::duration<double, std::milli> frequency_time
);

#endif

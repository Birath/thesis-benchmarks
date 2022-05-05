#pragma once
#include <skepu>
#include <fstream>
#include <string>


auto get_next_result_file(std::string const& prefix) -> std::string {
    auto next_name{prefix + ".dat"};
    std::ifstream f{next_name};
    size_t index = 1;
    while (f) {
        std::stringstream ss {};
        ss << prefix << index++ << ".dat";
        next_name = ss.str();
        f = std::ifstream{next_name};
    }
    return next_name;
}

template<typename BF>
void run_benchmark(BF func, size_t repeats, size_t start_size, size_t end_size, size_t increment, std::string name) {
    auto const output_name{get_next_result_file(name)};
    std::cout << "Running benchmark " << name << "\n" << " From " << start_size << " To " << end_size << "\n In increments of " << increment << " and " << repeats << " repeats\n" << "Writing result to " << output_name << "\n";
    std::ofstream result{output_name};

    result << "SIZE " << "time (s)\n";
    for (size_t size = start_size; size <= end_size; size += increment) {
        auto median_dur = skepu::benchmark::basicBenchmark(repeats, size, func);
        std::cout << size << " : " << (median_dur.count() / 10E3) << " ms.\n"; 
        result << size << " " << (median_dur.count() / 10E3) << "\n";
    }
    result.close();
}
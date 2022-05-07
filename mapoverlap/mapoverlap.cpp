#include <skepu>
#include <skepu-lib/io.hpp>
#include <skepu-lib/util.hpp>
#include <numeric>
#include <functional>
#include <fstream>
#include <tuple>

#include "run_benchmark.hpp"
 
template<typename T>
T over_1d (skepu::Region1D<T>r)
{
    return (r(-3) + r(-2) + r (-1) + r (0) + r (1) + r(2) + r(3)) / (r.oi * 2 + 1);
}


template<typename T>
auto run_mapoverlap(skepu::BackendSpec spec, size_t min, size_t max, size_t repeats, size_t increment) -> void {
	auto map_overlap = skepu::MapOverlap(over_1d<T>);
    spec.setDevices(1);
	map_overlap.setBackend(spec);
    map_overlap.setOverlap(3);
    auto func = [&](size_t current_size) {
        skepu::Vector<T> a(current_size);
        skepu::Vector<T> result(current_size);
        skepu::Vector<T> result2(current_size);
        T counter = 0;
        std::transform(a.begin(), a.end(), a.begin(), [&counter](T in) {
            return T{counter++};
        });
        map_overlap(result, a);
        result.flush();

    };

    run_benchmark(func, repeats, min, max, increment, "mapoverlap");
}


int main(int argc, char *argv[])
{
	if (argc == 2 && std::string(argv[1]) == "help")
	{
		skepu::io::cout << "Usage: " << argv[0] << "min max repeats backend\n";
		exit(1);
	}
	size_t min, max, repeats;
    std::tie(min, max, repeats) = skepu::benchmark::parseArgs(argc, argv, 100000, 4000000, 10);
    size_t increment = argc > 4 ? atoi(argv[4]) : 100000;
	auto spec = argc > 5 ? skepu::BackendSpec{argv[5]} : skepu::BackendSpec{skepu::Backend::Type::FPGA};
    run_mapoverlap<float>(spec, min, max, repeats, increment);
    
	
	return 0;
}

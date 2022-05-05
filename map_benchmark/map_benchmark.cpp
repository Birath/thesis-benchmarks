#include <skepu>
#include <skepu-lib/io.hpp>
#include <skepu-lib/util.hpp>
#include <numeric>
#include <functional>
#include <fstream>
#include <tuple>

#include "run_benchmark.hpp"
 
template<typename T>
T map_func(T a, T b)
{
	return a * a + b * b;
}

template<typename T>
auto run_map(skepu::BackendSpec spec, size_t min, size_t max, size_t repeats, size_t increment) -> void {
	auto map = skepu::Map(map_func<T>);
	map.setBackend(spec);

    auto func = [&](size_t current_size) {
        skepu::Vector<T> a(current_size);
        skepu::Vector<T> b(current_size);
        skepu::Vector<T> result(current_size);
        T counter = 0;
        std::transform(a.begin(), a.end(), a.begin(), [&counter](T in) {
            return T{counter++};
        });
        std::transform(b.begin(), b.end(), b.begin(), [&counter](T in) {
            return T{counter++};
        });
        map(result, a, b);
        result.flush();
    };
    run_benchmark(func, repeats, min, max, increment, "map");
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
    run_map<float>(spec, min, max, repeats, increment);
    
	
	return 0;
}

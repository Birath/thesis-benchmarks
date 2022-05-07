#include <skepu>
#include <skepu-lib/io.hpp>
#include <skepu-lib/util.hpp>
#include <numeric>
#include <functional>
#include <fstream>
#include <tuple>

#include "run_benchmark.hpp"
 
template<typename T>
T add(T a, T b)
{
	return a + b;
}

template<typename T>
auto run_scan(skepu::BackendSpec spec, size_t min, size_t max, size_t repeats, size_t increment) -> void {
	auto scan = skepu::Scan(add<T>);
	scan.setBackend(spec);

    auto func = [&](size_t current_size) {
        skepu::Vector<T> a(current_size);
        skepu::Vector<T> res(current_size);
        T counter = 0;
        std::transform(a.begin(), a.end(), a.begin(), [&counter](T in) {
            return T{counter++};
        });
        scan(res, a);
        res.flush();
    };
    run_benchmark(func, repeats, min, max, increment, "scan");
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
    run_scan<float>(spec, min, max, repeats, increment);
    
	
	return 0;
}

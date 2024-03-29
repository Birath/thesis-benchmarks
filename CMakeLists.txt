cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0...3.18.4)
project(benchmarks)

find_package(SkePU REQUIRED)

execute_process (COMMAND bash -c "aocl link-config"
                 OUTPUT_VARIABLE AoclLinkConfig)

execute_process (COMMAND bash -c "aocl compile-config"
                 OUTPUT_VARIABLE AoclCompileConfig)

string(STRIP "${AoclCompileConfig}" AoclCompileConfig)
string(STRIP "${AoclLinkConfig}" AoclLinkConfig)

MESSAGE(STATUS "Compile config: ${AoclCompileConfig}")
MESSAGE(STATUS "Link config: ${AoclLinkConfig}")

add_library(skepu_config_base INTERFACE)
target_compile_definitions(skepu_config_base INTERFACE SKEPU_DEBUG=3 USE_INTEL_FPGA_OPENCL)
target_compile_options(skepu_config_base INTERFACE ${AoclCompileConfig} ${AoclLinkConfig})
target_include_directories(skepu_config_base INTERFACE "")


add_subdirectory(reduce_benchmark)
add_subdirectory(map_benchmark)
add_subdirectory(mapreduce_benchmark)
add_subdirectory(scan_benchmark)
add_subdirectory(mapoverlap)
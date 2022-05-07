skepu_add_executable(mapreduce FPGA OpenMP SKEPUSRC mapreduce.cpp)

target_link_libraries(mapreduce PRIVATE skepu_config_base)
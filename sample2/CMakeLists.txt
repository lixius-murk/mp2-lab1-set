cmake_minimum_required(VERSION 3.10)
set(target "CrossRoad")

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

add_executable(${target} ${srcs} ${hdrs})
target_link_libraries(${target} ${MP2_LIBRARY})

#создаем библиотеку из всего, что есть в src

set(target ${MP2_LIBRARY})#извлечение значения

file(GLOB hdrs "*.h*" "${MP2_INCLUDE}/*.h*")#
file(GLOB srcs "*.cpp")

add_library(${target} STATIC ${srcs} ${hdrs})
target_link_libraries(${target} ${LIBRARY_DEPS})

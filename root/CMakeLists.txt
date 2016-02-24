cmake_minimum_required(VERSION 2.8)
project(april_sdf_generator)
find_package( OpenCV REQUIRED )

set(test_path ${PROJECT_SOURCE_DIR}/test)
set(bin_path ${PROJECT_BINARY_DIR}/bin)

include_directories(./libraries)
set(HEADERS ./libraries/xml_gen.hpp)
set(SOURCES ./libraries/xml_gen.cpp)

add_library(xml_genlib ${SOURCES} ${HEADERS})

add_executable( april_sdf_generator april_sdf_generator.cpp ${HEADERS})
target_link_libraries( april_sdf_generator ${OpenCV_LIBS} xml_genlib )

enable_testing()
add_subdirectory(${test_path})
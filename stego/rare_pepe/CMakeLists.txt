project(stego)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native -mtune=native -pthread -Wall -Wextra -pedantic -pedantic-errors -std=c++14 -Wfloat-equal -Wundef -Winit-self -Wstrict-overflow=2 -Wunreachable-code -Winline")

include_directories(.)

find_package(png++ REQUIRED)

add_executable(stego_encode encode.cpp)
target_link_libraries(stego_encode ${png++_LIBRARIES})

add_executable(stego_decode decode.cpp)
target_link_libraries(stego_decode ${png++_LIBRARIES})

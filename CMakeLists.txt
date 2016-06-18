project(libvinyl)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)

add_library(vinyl
	src/Common.cpp
	src/Folder.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(vinyl ${CMAKE_THREAD_LIBS_INIT})

add_executable(VinylTests
	Tests/main.cpp
)

target_link_libraries(VinylTests vinyl)
include_directories(
	src
)

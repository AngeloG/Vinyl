project(libvinyl)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)

add_library(vinyl
	src/Folder.cpp
)

add_executable(VinylTests
	Tests/main.cpp
)

target_link_libraries(VinylTests vinyl)
include_directories(src)

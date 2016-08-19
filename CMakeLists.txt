project(libvinyl)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)

add_library(vinyl
	src/Common.cpp
	src/Common.h
	src/Mountpoint.cpp
	src/Mountpoint.h
	src/MountpointFs.cpp
	src/MountpointFs.h
	src/MountpointZip.cpp
	src/MountpointZip.h
	src/Folder.cpp
	src/Folder.h
	src/File.cpp
	src/File.h
	src/Logging.cpp
	src/Logging.h
	src/ZipFile.cpp
	src/ZipFile.h

	src/ThirdParty/miniz.c
	src/ThirdParty/miniz.h
)

find_package(Threads REQUIRED)
target_link_libraries(vinyl ${CMAKE_THREAD_LIBS_INIT})

add_executable(VinylTests
	Tests/main.cpp
)

target_link_libraries(VinylTests vinyl)
include_directories(
	src
	src/Scratch/
)

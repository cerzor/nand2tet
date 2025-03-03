cmake_minimum_required(VERSION 3.8)
project(VMRunner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_COMPILER "clang++")  # Use Clang
set(CMAKE_OSX_ARCHITECTURES "x86_64")  # Ensure x86_64 architecture
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++17-extensions")

add_executable(VMRunner vm_runner.cpp)
target_link_libraries(VMRunner stdc++)  # Link against libstdc++

project(Client)
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(./Logger.hpp ./thread_pool.hpp)

add_executable(Client client.cpp)
add_executable(Server server.cpp Logger.cpp)



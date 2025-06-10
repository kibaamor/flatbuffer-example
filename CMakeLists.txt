cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(flatbuffer-example VERSION 1.0 LANGUAGES CXX)

add_executable(flatbuffer-example main.cpp)

find_package(Flatbuffers REQUIRED)

set(FLATBUFFERS_SCHEMAS ${CMAKE_CURRENT_SOURCE_DIR}/protos/proto.fbs)
flatbuffers_generate_headers(
  TARGET protos
  SCHEMAS ${FLATBUFFERS_SCHEMAS}
  FLAGS --cpp-std c++17)

target_link_libraries(flatbuffer-example
  PRIVATE protos flatbuffers::flatbuffers)

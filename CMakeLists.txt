cmake_minimum_required(VERSION 3.20)
project(hadd)

set(CMAKE_C_STANDARD 11)

add_executable(hadd
  src/main.c)

target_compile_options(hadd
  PRIVATE -Wall)

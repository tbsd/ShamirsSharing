cmake_minimum_required(VERSION 3.17)
project(shamir)

set(CMAKE_CXX_STANDARD 11)

add_executable(shamir main.cpp ShamirsScheme.h Rational.h Key.cpp Key.h CalcT.h)
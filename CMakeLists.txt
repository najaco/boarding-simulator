cmake_minimum_required(VERSION 3.13)
project(boarding-problem)
set(CMAKE_CXX_STANDARD 14)
add_executable(boarding-problem src/main.cc src/Plane.cc src/Plane.h src/Passenger.h)

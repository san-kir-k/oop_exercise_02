cmake_minimum_required(VERSION 3.17)

project(oop_exercise_02)

set(SOURCE lr2.cpp)
set(EXEC lr2)

add_definitions(-Wall -std=c++17)

add_executable(${EXEC} ${SOURCE})

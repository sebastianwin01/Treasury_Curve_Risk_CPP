cmake_minimum_required(VERSION 3.16)
project(TreasuryCurveRisk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(treasury_curve_cli src/main.cpp)

if (MSVC)
    target_compile_options(treasury_curve_cli PRIVATE /W4)
else()
    target_compile_options(treasury_curve_cli PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Add separately tested library sources, includes and CTest targets after
# Milestone 1 implementations are written and validated.

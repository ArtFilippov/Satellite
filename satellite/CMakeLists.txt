cmake_minimum_required(VERSION 3.5)

project(Satellite)

set(SATELLITE_SRC satellite.cpp satellite.hpp)

find_package(Boost 1.40 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_library(Satellite STATIC ${SATELLITE_SRC})

target_link_libraries(Satellite PRIVATE ${Boost_LIBRARIES})

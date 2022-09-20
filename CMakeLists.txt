cmake_minimum_required(VERSION 3.8)

add_executable(ellipsimg ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_link_libraries(ellipsimg ${CMAKE_SOURCE_DIR}/lib/sfml-graphics.lib)
target_link_libraries(ellipsimg ${CMAKE_SOURCE_DIR}/lib/sfml-window.lib)
target_link_libraries(ellipsimg ${CMAKE_SOURCE_DIR}/lib/sfml-system.lib)

include_directories(${CMAKE_SOURCE_DIR}/include)
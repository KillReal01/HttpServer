cmake_minimum_required(VERSION 3.10)

project(WebServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(http)

set(SOURCES
    main.cpp
    http/mongoose.c
)

add_executable(WebServer ${SOURCES})

target_link_libraries(WebServer pthread)

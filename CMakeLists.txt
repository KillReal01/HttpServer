cmake_minimum_required(VERSION 3.10)

project(WebServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)
include_directories(src/http)
include_directories(src/spdlog)

set(SOURCES
    main.cpp
    src/http/mongoose.c
    src/http/Router.cpp
)

add_executable(WebServer ${SOURCES})

link_directories(${CMAKE_SOURCE_DIR}/src/spdlog/lib)

target_link_libraries(WebServer pthread ${CMAKE_SOURCE_DIR}/src/spdlog/lib/libspdlog.a)

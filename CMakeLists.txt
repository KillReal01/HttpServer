cmake_minimum_required(VERSION 3.10)

project(WebServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/http)
include_directories(${CMAKE_SOURCE_DIR}/src/utils)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/spdlog)

set(SOURCES
    main.cpp
    src/http/mongoose.c
    src/http/HttpHandler.cpp
    src/http/HttpServer.cpp
    src/http/Router.cpp
    src/utils/LoggerManager.cpp
)

add_executable(WebServer ${SOURCES})

target_link_libraries(WebServer pthread spdlog::spdlog)
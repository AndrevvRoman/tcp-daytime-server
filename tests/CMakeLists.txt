cmake_minimum_required(VERSION 3.0.0)
project(tcp-datetime-server-tests VERSION 0.1.0)
 
file(GLOB SOURCES
    "libs/catch/*.hpp"
    "libs/catch/*.cpp"
    "sources/*.cpp"
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PRIVATE
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/catch>
)

target_link_libraries(${PROJECT_NAME}
tcp-datetime-server-lib)
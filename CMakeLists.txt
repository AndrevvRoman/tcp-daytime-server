cmake_minimum_required(VERSION 3.0.0)
project(tcp-datetime-server VERSION 0.1.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -Wall -Wextra -Wpedantic -Werror)

SET(Boost_USE_STATIC_LIBS ON)
# sudo apt install libboost-system1.71-dev
find_package(Boost 1.71 COMPONENTS system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()
 
file(GLOB HEADERS
     "app/include/endpoints/*.hpp"
     "app/include/managment/*.hpp"
     "app/include/misc/*.hpp"
)
file(GLOB SOURCES
    "app/sources/endpoints/*.cpp"
    "app/sources/managment/*.cpp"
    "app/sources/misc/*.cpp"
    "app/sources/*.cpp"
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
add_library(${PROJECT_NAME}-lib STATIC ${HEADERS} ${SOURCES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME}-lib PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PRIVATE
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/include/endpoints>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/include/managment>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/include/misc>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app>
)

target_include_directories(${PROJECT_NAME}-lib PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/include/endpoints>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/include/managment>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/include/misc>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app>
$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/include/endpoints>
$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/include/managment>
$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/include/misc>
$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app>
)

target_link_libraries(${PROJECT_NAME}
${Boost_LIBRARIES}
pthread)

target_link_libraries(${PROJECT_NAME}-lib
${Boost_LIBRARIES}
pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(tests)
cmake_minimum_required(VERSION 3.0.0)
project(tcp-datetime-server VERSION 0.1.0)

SET(Boost_USE_STATIC_LIBS OFF)
# sudo apt install libboost-system1.71-dev libboost-timer1.71.0
find_package(Boost 1.71 COMPONENTS system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()
 
file(GLOB HEADERS
     "*.hpp"
)
file(GLOB SOURCES
     "*.cpp"
)


include(CTest)
enable_testing()

add_executable(tcp-datetime-server ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    ${Boost_LIBRARIES}
    pthread)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

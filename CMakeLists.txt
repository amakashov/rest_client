cmake_minimum_required(VERSION 3.0.0)
project(rest_client VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cpprestsdk REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
include(CTest)
enable_testing()

add_subdirectory(emulator)

add_executable(rest_client main.cpp rozumexchange.hpp rozumexchange.cpp)
target_link_libraries(rest_client PRIVATE cpprestsdk::cpprest Qt5::Core)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_minimum_required(VERSION 3.27)
project(cv-bmstu)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(cv-bmstu Railway/main.cpp
        homographyPrepare/main.cpp)
target_link_libraries(cv-bmstu ${OpenCV_LIBS})


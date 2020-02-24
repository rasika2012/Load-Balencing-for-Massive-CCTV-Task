cmake_minimum_required(VERSION 3.1)
# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_WITH_CUDA FALSE)

project( mainServer )
find_package( OpenCV REQUIRED )
add_executable( mainServer main.cpp )
target_link_libraries( mainServer ${OpenCV_LIBS})
SET(CMAKE_THREAD_LIBS_INIT "${CMAKE_THREAD_LIBS_INIT} -std=c++11 -pthread")

target_link_libraries( mainServer ${CMAKE_THREAD_LIBS_INIT}) 
cmake_minimum_required(VERSION 3.1)
# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_WITH_CUDA FALSE)
set(CMAKE_INSTALL_PREFIX /home/chathuranga/Public/Acadamic/FYP/CV/installation/OpenCV-3.4.4/)
project( server )
find_package( OpenCV REQUIRED )
add_executable( server server.cpp )
# add_executable( server server.cpp )
target_link_libraries( server ${OpenCV_LIBS})
SET(CMAKE_THREAD_LIBS_INIT "${CMAKE_THREAD_LIBS_INIT} -std=c++11 -pthread")

target_link_libraries( server ${CMAKE_THREAD_LIBS_INIT}) 
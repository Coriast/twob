cmake_minimum_required(VERSION 3.16)

set(CMAKE CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(false)
endif()

configure_file("graphic/wall.jpg" "Debug/wall.jpg" COPYONLY)
configure_file("graphic/brick_wall.jpg" "Debug/brick_wall.jpg" COPYONLY)

configure_file("graphic/stanford_bunny/bunny.obj" "Debug/stanford_bunny/bunny.obj" COPYONLY)


add_executable(helloAbstraction "graphic/test_abstraction.cpp")
target_include_directories(helloAbstraction PUBLIC twob)
target_link_libraries(helloAbstraction PUBLIC twob)
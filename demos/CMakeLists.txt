cmake_minimum_required(VERSION 3.16)

set(CMAKE CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(false)
endif()

configure_file("graphic/wall.jpg" "wall.jpg" COPYONLY)
configure_file("graphic/brick_wall.jpg" "brick_wall.jpg" COPYONLY)

configure_file("graphic/stanford_bunny/bunny.obj" "stanford_bunny/bunny.obj" COPYONLY)


add_executable(redBlack "basic/test_redblacktree.cpp")
target_include_directories(redBlack PUBLIC twob)
target_link_libraries(redBlack PUBLIC twob)

# setting up WIN32 to load GUI only application
add_executable(helloTriangle "graphic/test_helloTriangle.cpp") # WIN32
target_include_directories(helloTriangle PUBLIC twob)
target_link_libraries(helloTriangle PUBLIC twob)

# setting up WIN32 to load GUI only application
add_executable(helloQuad "graphic/test_helloQuad.cpp") # WIN32
target_include_directories(helloQuad PUBLIC twob)
target_link_libraries(helloQuad PUBLIC twob)

add_executable(helloShaderClass "graphic/test_helloShaderClass.cpp")
target_include_directories(helloShaderClass PUBLIC twob)
target_link_libraries(helloShaderClass PUBLIC twob)

add_executable(helloTextures "graphic/test_helloTextures.cpp")
target_include_directories(helloTextures PUBLIC twob)
target_link_libraries(helloTextures PUBLIC twob)

add_executable(helloTransformations "graphic/test_transformations.cpp")
target_include_directories(helloTransformations PUBLIC twob)
target_link_libraries(helloTransformations PUBLIC twob)

add_executable(helloCamera "graphic/test_camera.cpp")
target_include_directories(helloCamera PUBLIC twob)
target_link_libraries(helloCamera PUBLIC twob)

add_executable(helloAbstraction "graphic/test_abstraction.cpp")
target_include_directories(helloAbstraction PUBLIC twob)
target_link_libraries(helloAbstraction PUBLIC twob)
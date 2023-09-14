cmake_minimum_required(VERSION 3.16)

set(CMAKE CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")

project(twob)

# can choose STATIC | SHARED | MODULE
add_library(twob STATIC)

target_sources(twob 
    PRIVATE
                   
        "src/twob_redblacktree.cpp"
        "src/twob_image.cpp"
        "src/twob_filesystem.cpp"
        "src/twob_time.cpp"
        "src/internal/twob_canvas.cpp"
        "src/twob_app.cpp"
        "src/internal/twob_renderer_opengl.cpp" 
        "src/twob_cluster.cpp" 
        "src/twob_input.cpp"
        "src/twob_camera.cpp")
 
target_include_directories(twob 
    PUBLIC
        include

    PRIVATE
        src
        src/internal
)

find_package(glfw3 REQUIRED)

find_package(glm REQUIRED)

find_package(glad REQUIRED)

find_package(OpenGL REQUIRED)

target_link_libraries(twob PUBLIC glfw glad::glad glm::glm ${OPENGL_LIBRARY})

#compile demos executables
add_subdirectory(demos)

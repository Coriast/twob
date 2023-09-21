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
        "src/twob_camera.cpp"
)

include(FetchContent)

FetchContent_Declare(
    glfw  
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3-stable
)
FetchContent_GetProperties(glfw)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG c
)
FetchContent_MakeAvailable(glad)

add_library(glad "${glad_SOURCE_DIR}/src/glad.c" "${glad_SOURCE_DIR}/include/glad/glad.h" )
target_include_directories(glad PUBLIC "${glad_SOURCE_DIR}/include")

find_package(OpenGL REQUIRED)

target_include_directories(twob 
    PUBLIC
        include
        ${glfw_SOURCE_DIR}/include
        ${glm_SOURCE_DIR}
        ${glad_SOURCE_DIR}/include

    PRIVATE
        src
        src/internal
)
message(${glad_SOURCE_DIR})
target_link_libraries(twob PRIVATE glfw glad glm::glm ${OPENGL_LIBRARIES})

#compile demos executables
add_subdirectory(demos)

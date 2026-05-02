cmake_minimum_required(VERSION 3.15)
project(VoxelGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(include)
include_directories(${OPENGL_INCLUDE_DIR})

# Source files
set(SOURCES
    src/main.cpp
    src/engine/world.cpp
    src/engine/chunk.cpp
    src/engine/block.cpp
    src/graphics/shader.cpp
    src/graphics/texture.cpp
    src/graphics/renderer.cpp
    src/physics/collision.cpp
    src/utils/noise.cpp
)

# Create executable
add_executable(voxel_game ${SOURCES})

# Link libraries
target_link_libraries(voxel_game
    ${OPENGL_LIBRARIES}
    glfw
    glm::glm
)

# Compiler flags
if(MSVC)
    target_compile_options(voxel_game PRIVATE /W4)
else()
    target_compile_options(voxel_game PRIVATE -Wall -Wextra -Wpedantic)
endif()

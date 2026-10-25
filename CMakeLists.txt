cmake_minimum_required(VERSION 3.10) # Define the minimum cmake version that can run this file.
project(SWGCG301-Graphics-Project VERSION 0.1.0) # Define a name & a version for our project. This is unrelated to the executable that we will define later.

# Tell cmake that we want to use C++ 20
set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)        # Don't build Documentation
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)       # Don't build Tests
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)    # Don't build Examples
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)           # Don't build Installation Information
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)     # Add variables to use High Performance Graphics Card if available
# This tells cmake to include GLFW's cmake file into our cmake project.
# By doing so, the "glfw" library target will be defined, and we will be able to link it to our executable.
add_subdirectory(vendor/glfw EXCLUDE_FROM_ALL)

# This creates a new exectuble called "lab1" (e.g., on windows, it will be called "lab1.exe")
# The executable is created by compiling the following c/c++ files and linking so libraries (which we will define later)
add_executable(SWGCG301-Graphics-Project main.cpp vendor/glad/src/gl.c "Shapes/shapes.h" "Shapes/shapes.cpp" "Shapes/TriangleShape.h" "Shapes/TriangleShape.cpp" "Shapes/StarShape.h" "Shapes/StarShape.cpp" "Shapes/HexagonShape.cpp" "Shapes/HexagonShape.h"  "Shapes/HeartShape.h" "Shapes/HeartShape.cpp" "Shapes/SmilyFace.h" "Shapes/SmilyFace.cpp")
# This tells the compiler where to find the header files for GLFW and glad.
target_include_directories(SWGCG301-Graphics-Project PRIVATE 
   vendor/glfw/include
   vendor/glad/include
)
# This tells the linker to link the GLFW library to our executable.
target_link_libraries(SWGCG301-Graphics-Project PRIVATE glfw)
# Compilers on windows don't like the fopen function, so we disable the c-runtime security warnings
# This is like adding #define _CRT_SECURE_NO_WARNINGS at the start of each file
target_compile_definitions(SWGCG301-Graphics-Project PRIVATE _CRT_SECURE_NO_WARNINGS)

# Run the executable from the project root so shader paths work.
add_custom_command(TARGET SWGCG301-Graphics-Project POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:SWGCG301-Graphics-Project>/shaders"
)

#pragma once

#include <glad/gl.h>

// Compiles and links a shader program from a vertex and a fragment shader file.
// Returns 0 (and prints the reason) if a file can't be read or compilation/linking fails.
GLuint loadProgram(const char* vertexPath, const char* fragmentPath);

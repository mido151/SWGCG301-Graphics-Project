#include "ShaderLoader.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

// Loads and compiles one shader from a file. Returns 0 on failure.
static GLuint loadShader(const char* filePath, GLenum shaderType) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::printf("Failed to open file: %s\n", filePath);
        return 0;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    const std::string source = buffer.str();
    const char* sourcePtr = source.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(static_cast<size_t>(logLength > 0 ? logLength : 1), '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        std::printf("Shader \"%s\" compilation failed: %s\n", filePath, log.c_str());
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint loadProgram(const char* vertexPath, const char* fragmentPath) {
    GLuint vertexShader = loadShader(vertexPath, GL_VERTEX_SHADER);
    if (!vertexShader) {
        return 0;
    }

    GLuint fragmentShader = loadShader(fragmentPath, GL_FRAGMENT_SHADER);
    if (!fragmentShader) {
        glDeleteShader(vertexShader);   // don't leak the vertex shader if the fragment shader failed
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // The shaders are already linked into the program, so they are no longer needed.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(static_cast<size_t>(logLength > 0 ? logLength : 1), '\0');
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        std::printf("Linking %s & %s failed: %s\n", vertexPath, fragmentPath, log.c_str());
        glDeleteProgram(program);
        return 0;
    }
    return program;
}


#pragma once

#include <glad/gl.h>
#include <cstdint>

struct Vector3D {
    float x, y, z;
};

struct Color {
    uint8_t r, g, b, a;
};

struct Vertex {
    Vector3D position;
    Color color;
};

class Shape {
protected:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    GLuint shaderProgram = 0;
    GLint mousePositionLocation = -1;
    GLsizei indexCount = 0;

    void setupGeometry(
        const Vertex* vertices,
        size_t vertexCount,
        const uint16_t* indices,
        size_t indexCount
    );

public:
    Shape(GLuint shaderProgram);
    virtual ~Shape();

    virtual void init() = 0;
    virtual void draw(float mouseX, float mouseY);
};

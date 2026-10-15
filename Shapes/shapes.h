
#pragma once

#include <glad/gl.h>

class Shape {
protected:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    GLuint shaderProgram = 0;

    GLsizei indexCount = 0;

public:
    Shape(GLuint shaderProgram);
    virtual ~Shape();

    virtual void init() = 0;
    virtual void draw(float mouseX, float mouseY) = 0;
};

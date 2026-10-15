
#include "shapes.h"
#include <cstddef>

Shape::Shape(GLuint shaderProgram)
    : shaderProgram(shaderProgram) {
    mousePositionLocation =
        glGetUniformLocation(shaderProgram, "mousePosition");
}

Shape::~Shape() {
    if (ebo != 0) {
        glDeleteBuffers(1, &ebo);
    }

    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }

    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
}

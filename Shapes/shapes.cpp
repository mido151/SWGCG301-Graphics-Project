
#include "shapes.h"

Shape::Shape(GLuint shaderProgram)
    : shaderProgram(shaderProgram) {
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

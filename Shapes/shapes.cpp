
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

void Shape::setupGeometry(
    const Vertex* vertices,
    size_t vertexCount,
    const uint16_t* indices,
    size_t indexCount
)

{
    this->indexCount = static_cast<GLsizei>(indexCount);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertexCount * sizeof(Vertex),
        vertices,
        GL_STATIC_DRAW
    );
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indexCount * sizeof(uint16_t),
        indices,
        GL_STATIC_DRAW
    );
    // Position attribute
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, position)
    );
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(
        1,
        4,
        GL_UNSIGNED_BYTE,
        GL_TRUE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color)
    );
    glEnableVertexAttribArray(1);
    // Unbind the VAO to prevent accidental modification
    glBindVertexArray(0);
}


void Shape::draw(float mouseX, float mouseY) {
    glUseProgram(shaderProgram);

    if (mousePositionLocation != -1) {
        glUniform2f(
            mousePositionLocation,
            mouseX,
            mouseY
        );
    }

    glBindVertexArray(vao);

    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_SHORT,
        nullptr
    );

    glBindVertexArray(0);
}
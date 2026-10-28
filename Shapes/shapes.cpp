#include "shapes.h"
#include <cstddef>

Shape::Shape(GLuint shaderProgram)
    : shaderProgram(shaderProgram) {
    // Look the uniforms up once instead of every frame.
    offsetLocation = glGetUniformLocation(shaderProgram, "mousePosition");
    aspectRatioLocation = glGetUniformLocation(shaderProgram, "aspectRatio");
}

Shape::~Shape() {
    glDeleteBuffers(1, &ebo);          // deleting the name 0 is silently ignored by OpenGL
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Shape::setupGeometry(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) {
    indexCount = static_cast<GLsizei>(indices.size());

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // The EBO binding is stored inside the VAO, so it must stay bound until the VAO is unbound.
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW);

    // Attribute 0: position (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);

    // Attribute 1: color (4 bytes, normalized to 0..1 by OpenGL)
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO to prevent accidental modification.
    glBindVertexArray(0);
}

void Shape::draw(float offsetX, float offsetY, float aspectRatio) const {
    glUseProgram(shaderProgram);
    glUniform2f(offsetLocation, offsetX, offsetY);
    glUniform1f(aspectRatioLocation, aspectRatio);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}

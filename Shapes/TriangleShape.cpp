#include "TriangleShape.h"

TriangleShape::TriangleShape(GLuint shaderProgram)
    : Shape(shaderProgram) {
    const std::vector<Vertex> vertices = {
        {{ 0.0f,  0.5f, 0.0f}, {255,   0,   0, 255}},
        {{-0.5f, -0.5f, 0.0f}, {  0, 255,   0, 255}},
        {{ 0.5f, -0.5f, 0.0f}, {  0,   0, 255, 255}},
    };
    const std::vector<uint16_t> indices = {0, 1, 2};

    setupGeometry(vertices, indices);
}

#include "TriangleShape.h"

TriangleShape::TriangleShape(GLuint shaderProgram)
    : Shape(shaderProgram) {
}

void TriangleShape::init() {
    Vertex vertices[] = {
        {{ 0.0f,  0.5f, 0.0f}, {255,   0,   0, 255}},
        {{-0.5f, -0.5f, 0.0f}, {  0, 255,   0, 255}},
        {{ 0.5f, -0.5f, 0.0f}, {  0,   0, 255, 255}}
    };

    uint16_t indices[] = {
        0, 1, 2
    };

    setupGeometry(
        vertices,
        3,
        indices,
        3
    );
}
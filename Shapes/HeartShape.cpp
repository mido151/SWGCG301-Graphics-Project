#include "HeartShape.h"

HeartShape::HeartShape(GLuint shaderProgram)
    : Shape(shaderProgram) {
}

void HeartShape::init() {
    Vertex vertices[] = {

        {{  0.0f, -0.6f, 0.0f }, { 255, 0, 0, 255 }},


        {{ -0.5f,  0.2f, 0.0f }, { 255, 0, 0, 255 }},


        {{  0.5f,  0.2f, 0.0f }, { 255, 0, 0, 255 }},

        {{  0.0f,  0.2f, 0.0f }, { 255, 0, 0, 255 }},


        {{ -0.25f, 0.5f, 0.0f }, { 255, 0, 0, 255 }},


        {{  0.25f, 0.5f, 0.0f }, { 255, 0, 0, 255 }}
    };

    uint16_t indices[] = {
        0, 1, 2,
        1, 4, 3,
        3, 5, 2 
    };

    setupGeometry(
        vertices,
        6,
        indices,
        9
    );
}
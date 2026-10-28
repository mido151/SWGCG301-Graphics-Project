#include "HeartShape.h"

HeartShape::HeartShape(GLuint shaderProgram)
    : Shape(shaderProgram) {
    constexpr Color RED = {255, 0, 0, 255};

    const std::vector<Vertex> vertices = {
        {{  0.0f,  -0.6f, 0.0f}, RED},  // 0: bottom tip
        {{ -0.5f,   0.2f, 0.0f}, RED},  // 1: left side
        {{  0.5f,   0.2f, 0.0f}, RED},  // 2: right side
        {{  0.0f,   0.2f, 0.0f}, RED},  // 3: dip between the two lobes
        {{ -0.25f,  0.5f, 0.0f}, RED},  // 4: top of the left lobe
        {{  0.25f,  0.5f, 0.0f}, RED},  // 5: top of the right lobe
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2,   // big triangle forming the body
        1, 4, 3,   // left lobe
        3, 5, 2,   // right lobe
    };

    setupGeometry(vertices, indices);
}

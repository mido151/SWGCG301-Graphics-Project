#include "StarShape.h"

#include <array>
#include <cmath>

StarShape::StarShape(GLuint shaderProgram)
    : Shape(shaderProgram) {
}

void StarShape::init() {
    constexpr float pi = 3.14159265358979323846f;
    constexpr float outerRadius = 0.5f;
    constexpr float innerRadius = outerRadius * 0.4f;

    std::array<Vertex, 11> vertices{};
    for (int i = 0; i < 10; ++i) {
        const float angle = pi / 2.0f + static_cast<float>(i) * pi / 5.0f;
        const float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        vertices[i] = {
            {radius * std::cos(angle), radius * std::sin(angle), 0.0f},
            {255, 210, 0, 255}
        };
    }
    vertices[10] = {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}};

    std::array<uint16_t, 30> indices{};
    for (uint16_t i = 0; i < 10; ++i) {
        const uint16_t triangle = static_cast<uint16_t>(i * 3);
        indices[triangle] = 10;
        indices[triangle + 1] = i;
        indices[triangle + 2] = static_cast<uint16_t>((i + 1) % 10);
    }

    setupGeometry(vertices.data(), vertices.size(), indices.data(), indices.size());
}

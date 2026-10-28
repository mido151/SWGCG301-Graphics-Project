#include "StarShape.h"
#include <cmath>

StarShape::StarShape(GLuint shaderProgram)
    : Shape(shaderProgram) {
    constexpr int POINTS = 5;
    constexpr int OUTLINE_VERTICES = POINTS * 2;   // alternating outer tips and inner corners
    constexpr float OUTER_RADIUS = 0.5f;
    constexpr float INNER_RADIUS = OUTER_RADIUS * 0.4f;

    // The outline vertices go first (indices 0..9), the center vertex is last (index 10).
    std::vector<Vertex> vertices;
    for (int i = 0; i < OUTLINE_VERTICES; ++i) {
        // Start at 90 degrees so one tip points straight up.
        const float angle = PI / 2.0f + static_cast<float>(i) * PI / POINTS;
        const float radius = (i % 2 == 0) ? OUTER_RADIUS : INNER_RADIUS;
        vertices.push_back({{radius * std::cos(angle), radius * std::sin(angle), 0.0f}, {255, 210, 0, 255}});
    }
    vertices.push_back({{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}});
    const uint16_t center = OUTLINE_VERTICES;

    // One triangle per outline edge: center, vertex i, next vertex.
    std::vector<uint16_t> indices;
    for (uint16_t i = 0; i < OUTLINE_VERTICES; ++i) {
        indices.push_back(center);
        indices.push_back(i);
        indices.push_back(static_cast<uint16_t>((i + 1) % OUTLINE_VERTICES));
    }

    setupGeometry(vertices, indices);
}

#include "SmileyFace.h"
#include <cmath>

// Appends a filled circle (a triangle fan) to the vertex/index lists.
static void addCircle(
    std::vector<Vertex>& vertices,
    std::vector<uint16_t>& indices,
    float centerX, float centerY,
    float radius,
    Color color,
    int segments = 20)
{
    const uint16_t centerIndex = static_cast<uint16_t>(vertices.size());
    vertices.push_back({{centerX, centerY, 0.0f}, color});

    for (int i = 0; i < segments; i++) {
        const float angle = 2.0f * PI * i / segments;
        vertices.push_back({{centerX + radius * std::cos(angle), centerY + radius * std::sin(angle), 0.0f}, color});
    }

    // Circles are drawn in the order they are added, so later circles appear on top of earlier ones.
    for (int i = 0; i < segments; i++) {
        indices.push_back(centerIndex);
        indices.push_back(static_cast<uint16_t>(centerIndex + 1 + i));
        indices.push_back(static_cast<uint16_t>(centerIndex + 1 + (i + 1) % segments));
    }
}

SmileyFace::SmileyFace(GLuint shaderProgram)
    : Shape(shaderProgram) {
    constexpr Color YELLOW = {255, 220, 0, 255};
    constexpr Color WHITE = {255, 255, 255, 255};
    constexpr Color BLACK = {0, 0, 0, 255};
    constexpr Color RED = {255, 0, 0, 255};

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    // Face, eye whites, then pupils (drawn last so they stay on top).
    addCircle(vertices, indices, 0.0f, 0.0f, 0.5f, YELLOW, 40);
    addCircle(vertices, indices, -0.18f, 0.12f, 0.09f, WHITE);
    addCircle(vertices, indices, 0.18f, 0.12f, 0.09f, WHITE);
    addCircle(vertices, indices, -0.18f, 0.12f, 0.045f, BLACK);
    addCircle(vertices, indices, 0.18f, 0.12f, 0.045f, BLACK);

    // Mouth: a smile made of thin triangles that follow a parabola from x = -0.24 to x = 0.24.
    constexpr int MOUTH_SEGMENTS = 12;
    constexpr float MOUTH_HALF_WIDTH = 0.24f;
    constexpr float MOUTH_STEP = 2.0f * MOUTH_HALF_WIDTH / MOUTH_SEGMENTS;
    constexpr float MOUTH_TOP = -0.10f;     // height of the mouth corners
    constexpr float MOUTH_DEPTH = 0.13f;    // how far the middle of the smile dips
    constexpr float MOUTH_THICKNESS = 0.025f;

    auto mouthY = [&](float x) {
        return MOUTH_TOP - MOUTH_DEPTH * (1.0f - (x * x) / (MOUTH_HALF_WIDTH * MOUTH_HALF_WIDTH));
    };

    for (int i = 0; i < MOUTH_SEGMENTS; i++) {
        const float x1 = -MOUTH_HALF_WIDTH + i * MOUTH_STEP;
        const float x2 = x1 + MOUTH_STEP;
        const uint16_t start = static_cast<uint16_t>(vertices.size());

        vertices.push_back({{x1, mouthY(x1), 0.0f}, RED});
        vertices.push_back({{x2, mouthY(x2), 0.0f}, RED});
        vertices.push_back({{(x1 + x2) / 2.0f, (mouthY(x1) + mouthY(x2)) / 2.0f - MOUTH_THICKNESS, 0.0f}, RED});

        indices.push_back(start);
        indices.push_back(static_cast<uint16_t>(start + 1));
        indices.push_back(static_cast<uint16_t>(start + 2));
    }

    setupGeometry(vertices, indices);
}

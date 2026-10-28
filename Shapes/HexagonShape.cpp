#include "HexagonShape.h"
#include <cmath>

HexagonShape::HexagonShape(GLuint shaderProgram)
    : Shape(shaderProgram) {
    constexpr int SIDES = 6;

    // Distance from the center to each corner (in normalized device coordinates).
    constexpr float RADIUS = 0.4f;

    // One color per corner. The rasterizer interpolates between them,
    // which gives a smooth rainbow gradient across the hexagon.
    const Color cornerColors[SIDES] = {
        {255,   0,   0, 255}, // red
        {255, 255,   0, 255}, // yellow
        {  0, 255,   0, 255}, // green
        {  0, 255, 255, 255}, // cyan
        {  0,   0, 255, 255}, // blue
        {255,   0, 255, 255}  // magenta
    };

    // Vertex 0 is the center (white so the colors blend toward the middle).
    std::vector<Vertex> vertices;
    vertices.push_back({{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}});

    // Corners are placed every 60 degrees (360 / 6) around the center.
    // Starting at angle 0 gives a "flat-top" hexagon.
    for (int i = 0; i < SIDES; i++) {
        const float angle = 2.0f * PI * i / SIDES;
        vertices.push_back({{RADIUS * std::cos(angle), RADIUS * std::sin(angle), 0.0f}, cornerColors[i]});
    }

    // 6 triangles, each made of: the center, corner i, and the next corner.
    // The last triangle wraps around to connect the last corner back to the first.
    std::vector<uint16_t> indices;
    for (int i = 0; i < SIDES; i++) {
        indices.push_back(0);
        indices.push_back(static_cast<uint16_t>(i + 1));
        indices.push_back(static_cast<uint16_t>((i + 1) % SIDES + 1));
    }

    setupGeometry(vertices, indices);
}

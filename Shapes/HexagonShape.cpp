#include "HexagonShape.h"
#include <cmath>

HexagonShape::HexagonShape(GLuint shaderProgram)
    : Shape(shaderProgram) {
}

void HexagonShape::init() {
    // Number of corners of the hexagon.
    const int SIDES = 6;

    // Distance from the center to each corner (in normalized device coordinates).
    const float RADIUS = 0.4f;

    // The window is 1920x1080, but NDC always goes from -1 to 1 on both axes.
    // Without correction the hexagon would look stretched horizontally,
    // so we shrink the x coordinates by (height / width).
    const float ASPECT_CORRECTION = 1080.0f / 1920.0f;

    const float PI = 3.14159265358979f;

    // 1 center vertex + 6 corner vertices = 7 vertices.
    Vertex vertices[SIDES + 1];

    // Vertex 0 is the center of the hexagon (white so the colors blend toward the middle).
    vertices[0] = {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}};

    // One color per corner. The rasterizer interpolates between them,
    // which gives a smooth rainbow gradient across the hexagon.
    Color cornerColors[SIDES] = {
        {255,   0,   0, 255}, // red
        {255, 255,   0, 255}, // yellow
        {  0, 255,   0, 255}, // green
        {  0, 255, 255, 255}, // cyan
        {  0,   0, 255, 255}, // blue
        {255,   0, 255, 255}  // magenta
    };

    // Corners are placed every 60 degrees (360 / 6) around the center.
    // Starting at angle 0 gives a "flat-top" hexagon.
    for (int i = 0; i < SIDES; i++) {
        float angle = 2.0f * PI * i / SIDES;
        float x = RADIUS * std::cos(angle) * ASPECT_CORRECTION;
        float y = RADIUS * std::sin(angle);
        vertices[i + 1] = {{x, y, 0.0f}, cornerColors[i]};
    }

    // 6 triangles, each made of: the center, corner i, and the next corner.
    // The last triangle wraps around to connect corner 6 back to corner 1.
    uint16_t indices[SIDES * 3];
    for (int i = 0; i < SIDES; i++) {
        indices[i * 3 + 0] = 0;                              // center
        indices[i * 3 + 1] = static_cast<uint16_t>(i + 1);   // current corner
        indices[i * 3 + 2] = static_cast<uint16_t>((i + 1) % SIDES + 1); // next corner
    }

    // Upload the vertices and indices to the GPU (VAO, VBO, EBO are created in the base class).
    setupGeometry(
        vertices,
        SIDES + 1,
        indices,
        SIDES * 3
    );
}

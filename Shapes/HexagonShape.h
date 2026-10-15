#pragma once

#include "shapes.h"

// A regular hexagon centered at the origin.
// It is built as a "triangle fan": one vertex in the center and 6 vertices
// on the outline, connected into 6 triangles (one per edge of the hexagon).
// The shape follows the mouse through the "mousePosition" uniform that the
// base Shape class sends to the vertex shader.
class HexagonShape : public Shape {
public:
    HexagonShape(GLuint shaderProgram);

    void init() override;
};

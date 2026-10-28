#pragma once

#include "shapes.h"

// A triangle with a red, a green and a blue corner (the colors blend across the face).
class TriangleShape : public Shape {
public:
    explicit TriangleShape(GLuint shaderProgram);
};

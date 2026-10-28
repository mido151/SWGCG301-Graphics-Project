#pragma once

#include "shapes.h"

// A red heart made of three triangles.
class HeartShape : public Shape {
public:
    explicit HeartShape(GLuint shaderProgram);
};

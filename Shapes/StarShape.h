#pragma once

#include "shapes.h"

// A yellow five-pointed star with a white center.
class StarShape : public Shape {
public:
    explicit StarShape(GLuint shaderProgram);
};

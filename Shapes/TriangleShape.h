#pragma once

#include "Shapes.h"

class TriangleShape : public Shape {
public:
    TriangleShape(GLuint shaderProgram);

    void init() override;
};
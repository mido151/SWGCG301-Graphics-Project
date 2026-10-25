#pragma once

#include "shapes.h"

class HeartShape : public Shape {
public:
    HeartShape(GLuint shaderProgram);
    void init() override;
};
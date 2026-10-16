#pragma once

#include "shapes.h"

class SmilyFace : public Shape {
public:
    SmilyFace(GLuint shaderProgram);
    void init() override;
};

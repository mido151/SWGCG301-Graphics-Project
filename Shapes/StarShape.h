#pragma once

#include "shapes.h"

class StarShape : public Shape {
public:
    explicit StarShape(GLuint shaderProgram);

    void init() override;
};

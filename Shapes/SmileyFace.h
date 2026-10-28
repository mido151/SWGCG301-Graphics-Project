#pragma once

#include "shapes.h"

// A yellow smiley face: a face disc, two eyes (white + pupil) and a red mouth.
class SmileyFace : public Shape {
public:
    explicit SmileyFace(GLuint shaderProgram);
};

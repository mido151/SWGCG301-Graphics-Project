#include "SmilyFace.h"
#include <cmath>
#include <cstdint>

SmilyFace::SmilyFace(GLuint shaderProgram)
    : Shape(shaderProgram) {
}

static void addCircle(
    Vertex* vertices,
    uint16_t* indices,
    int& vertexCount,
    int& indexCount,
    float centerX,
    float centerY,
    float radius,
    Color color,
    int segments = 20)
{
    int centerIndex = vertexCount++;

    vertices[centerIndex] = {
        {centerX, centerY, 0.0f},
        color
    };

    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * 3.14159265f * i / segments;

        vertices[vertexCount++] = {
            {
                centerX + radius * cos(angle),
                centerY + radius * sin(angle),
                0.0f
            },
            color
        };
    }

    for (int i = 0; i < segments; i++) {
        indices[indexCount++] = centerIndex;
        indices[indexCount++] = centerIndex + 1 + i;
        indices[indexCount++] =
            centerIndex + 1 + (i + 1) % segments;
    }
}

void SmilyFace::init() {
    Vertex vertices[200];
    uint16_t indices[600];

    int vertexCount = 0;
    int indexCount = 0;

    addCircle(
        vertices, indices,
        vertexCount, indexCount,
        0.0f, 0.0f,
        0.5f,
        { 255, 220, 0, 255 },
        40
    );

    addCircle(
        vertices, indices,
        vertexCount, indexCount,
        -0.18f, 0.12f,
        0.09f,
        { 255, 255, 255, 255 }
    );

    addCircle(
        vertices, indices,
        vertexCount, indexCount,
        0.18f, 0.12f,
        0.09f,
        { 255, 255, 255, 255 }
    );

    addCircle(
        vertices, indices,
        vertexCount, indexCount,
        -0.18f, 0.12f,
        0.045f,
        { 0, 0, 0, 255 }
    );

    addCircle(
        vertices, indices,
        vertexCount, indexCount,
        0.18f, 0.12f,
        0.045f,
        { 0, 0, 0, 255 }
    );

    for (int i = 0; i < 12; i++) {
        float x1 = -0.24f + i * 0.04f;
        float x2 = x1 + 0.04f;

        float y1 = -0.10f - 0.13f *
            (1.0f - (x1 * x1) / (0.24f * 0.24f));

        float y2 = -0.10f - 0.13f *
            (1.0f - (x2 * x2) / (0.24f * 0.24f));

        int start = vertexCount;

        vertices[vertexCount++] = {
            {x1, y1, 0.0f},
            {255, 0, 0, 255}
        };

        vertices[vertexCount++] = {
            {x2, y2, 0.0f},
            {255, 0, 0, 255}
        };

        vertices[vertexCount++] = {
            {
                (x1 + x2) / 2.0f,
                (y1 + y2) / 2.0f - 0.025f,
                0.0f
            },
            {255, 0, 0, 255}
        };

        indices[indexCount++] = start;
        indices[indexCount++] = start + 1;
        indices[indexCount++] = start + 2;
    }

    setupGeometry(
        vertices,
        vertexCount,
        indices,
        indexCount
    );
}

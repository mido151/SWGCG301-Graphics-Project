#pragma once

#include <glad/gl.h>
#include <cstdint>
#include <vector>

constexpr float PI = 3.14159265358979323846f;

struct Vector3D {
    float x, y, z;
};

struct Color {
    uint8_t r, g, b, a;
};

struct Vertex {
    Vector3D position;
    Color color;
};

// Base class for all 2D shapes.
// A derived shape builds its vertices/indices in its constructor and hands them to
// setupGeometry(), which uploads them to the GPU once. Drawing afterwards only binds
// the VAO and issues one draw call, so shapes must be created once (not every frame).
// The OpenGL objects are owned by the shape, so it cannot be copied.
class Shape {
public:
    virtual ~Shape();

    Shape(const Shape&) = delete;
    Shape& operator=(const Shape&) = delete;

    // offsetX/offsetY: where to move the shape, in normalized device coordinates (-1 to 1).
    // aspectRatio: window width / height. The vertex shader uses it so that shapes keep
    // their proportions (a circle stays a circle) whatever the window size is.
    void draw(float offsetX, float offsetY, float aspectRatio) const;

protected:
    explicit Shape(GLuint shaderProgram);

    // Creates the VAO, VBO and EBO and uploads the geometry. Must be called once by the derived class.
    void setupGeometry(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLsizei indexCount = 0;

    GLuint shaderProgram = 0;
    GLint offsetLocation = -1;
    GLint aspectRatioLocation = -1;
};

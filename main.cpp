#include <cstdio>
#include <memory>
#include <vector>

// We must include GLAD before GLFW, otherwise GLFW may include its own version of some OpenGL functions
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "ShaderLoader.h"
#include "Shapes/TriangleShape.h"
#include "Shapes/HexagonShape.h"
#include "Shapes/StarShape.h"
#include "Shapes/HeartShape.h"
#include "Shapes/SmileyFace.h"

// The initial window size. The window can be resized afterwards.
constexpr int WINDOW_W = 1920;
constexpr int WINDOW_H = 1080;

// Keep the OpenGL viewport in sync with the window when it is resized.
static void onFramebufferResize(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        std::printf("Failed to initialize GLFW\n");
        return 1;
    }

    // We want OpenGL 3.3, the core profile (no backward compatibility with OpenGL 2.0 or earlier)
    // and forward compatibility (all deprecated functions will be disabled).
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_W, WINDOW_H, "SWGCG301 - Phase 1", nullptr, nullptr);
    if (!window) {
        std::printf("Failed to create Window\n");
        glfwTerminate();
        return 1;
    }
    // Make the window's context current. This is necessary before we can call any OpenGL functions.
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onFramebufferResize);
    glfwSwapInterval(1);    // vsync: wait for the monitor refresh instead of drawing as fast as possible

    // Load the OpenGL function pointers using GLAD. This is necessary because GLFW does not provide these pointers.
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::printf("Failed to initialize GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // The shader program is needed to draw anything (other than clearing).
    GLuint program = loadProgram("./shaders/main.vert", "./shaders/main.frag");
    if (!program) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Create every shape once, before the game loop. Number key i + 1 selects shapes[i].
    // The shapes must be destroyed before the OpenGL context, so they live inside this scope.
    {
        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::make_unique<TriangleShape>(program));   // key 1
        shapes.push_back(std::make_unique<HexagonShape>(program));    // key 2
        shapes.push_back(std::make_unique<StarShape>(program));       // key 3
        shapes.push_back(std::make_unique<HeartShape>(program));      // key 4
        shapes.push_back(std::make_unique<SmileyFace>(program));      // key 5

        size_t currentShape = 0;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            // ESC quits the game.
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            // Keys 1..N switch to the corresponding shape.
            for (size_t i = 0; i < shapes.size(); ++i) {
                if (glfwGetKey(window, GLFW_KEY_1 + static_cast<int>(i)) == GLFW_PRESS) {
                    currentShape = i;
                }
            }

            // Convert the mouse position (pixels, origin at the top-left) to normalized device
            // coordinates (-1 to 1, origin at the center, y pointing up).
            int windowW, windowH;
            glfwGetWindowSize(window, &windowW, &windowH);
            if (windowW == 0 || windowH == 0) {
                continue;   // the window is minimized; nothing to draw
            }
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            const float ndcX = static_cast<float>(2.0 * mouseX / windowW - 1.0);
            const float ndcY = static_cast<float>(1.0 - 2.0 * mouseY / windowH);
            const float aspectRatio = static_cast<float>(windowW) / static_cast<float>(windowH);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shapes[currentShape]->draw(ndcX, ndcY, aspectRatio);

            // Swap the front and back buffers so what we drew appears on the screen.
            glfwSwapBuffers(window);
        }
    }

    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

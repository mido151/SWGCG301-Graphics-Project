#include <stdio.h>
#include <stdlib.h>

// We must include GLAD before GLFW, otherwise GLFW may include its own version of some OpenGL functions
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Shapes/TriangleShape.h"



// Load a shader from a file
// file_path: the path to the shader code
// shader_type: the type of shader to compile (either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
GLuint load_shader(const char* file_path, GLenum shader_type) {
    // Load and read the file
    FILE* file = fopen(file_path, "rb");
    if(!file) {
        printf("Failed to open file: %s\n", file_path);
        return 0;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* file_content = (char*)malloc(file_size + 1);
    fread(file_content, file_size, 1, file);
    file_content[file_size] = '\0';
    fclose(file);

    // Create the shader
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &file_content, NULL);
    glCompileShader(shader);
    free(file_content);

    // Check if we have compilation errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        int log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);    
        GLchar* info_log = (GLchar*)malloc(log_length * sizeof(GLchar));
        glGetShaderInfoLog(shader, log_length, NULL, info_log);
        printf("Shader \"%s\" compilation failed: %s\n", file_path, info_log);
        free(info_log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

// Load a program from a vertex and fragment shader
// vs_path: path to the vertex shader file
// fs_path: path to the fragment shader file
GLuint load_program(const char* vs_path, const char* fs_path) {
    // Load the vertex and fragment shaders
    GLuint vertex_shader = load_shader(vs_path, GL_VERTEX_SHADER);
    if(!vertex_shader) {
        printf("Failed to load vertex shader\n");
        return 0;
    }
    
    GLuint fragment_shader = load_shader(fs_path, GL_FRAGMENT_SHADER);
    if(!fragment_shader) {
        printf("Failed to load fragment shader\n");
        // Remember to delete the vertex shader if we failed to create the fragment shader
        glDeleteShader(vertex_shader);
        return 0;
    }

    // Create the program, attach the 2 shaders and link the program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    // We no longer need the shaders; their source code has been compiled and linked into the program.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Check if we have linking errors
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        int log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        char* log = (char*)malloc(log_length * sizeof(char));
        glGetProgramInfoLog(program, log_length, NULL, log);
        printf("Linking %s & %s failed: %s\n", vs_path, fs_path, log);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

// removed to be in individual files

// The window dimensions
#define WINDOW_W 1920
#define WINDOW_H 1080

int main(int argc, char** argv) {
    if(!glfwInit()) {
       printf("Failed to initialize GLFW\n");
       return 1; 
    }
    
    // Here, we tell GLFW that we want to use OpenGL 3.3 and the core profile (no backward compatibility with OpenGL 2.0 or earliere).
    // And that we want forward compatibility (all deprecated functions will be disabled).
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Hello World", NULL, NULL);
    if(!window) {
        printf("Failed to create Window\n");
        glfwTerminate();
        return 1;
    }
    // Make the window's context current. This is necessary before we can call any OpenGL functions.
    // Now all the upcoming OpenGL calls will be directed to this window's context.
    glfwMakeContextCurrent(window);

    // Load the OpenGL function pointers using GLAD. This is necessary because GLFW does not provide these pointers.
    if(!gladLoadGL(glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Load the shader program. This is necessary because we cannot draw anything without a shader program (other than clearing).
    GLuint program = load_program("./shaders/main.vert", "./shaders/main.frag");
    if(!program) {
        return 1;
    }


	//removed to be in individual files

    // First we define our geometry, which is a 2D rectangle in this case.
    // The rectangle will be centered around the window and will cover half the width and height of the window.
    // Each vertex will have a different color
    
	//removed to be in individual files

    // The elements array defines how we use the vertices we defined above to construct 2D triangles
    
	//removed to be in individual files

    // The data in the arrays "vertices" & "elements" above are on the RAM, so they are not visible to the GPU
    // So next, we create buffers for them on the GPU and copy the data there.
    // We will create two buffer objects: Vertex Buffer Object (VBO) and Element Buffer Object (EBO)
   
	//removed to be in individual files

    // No we enter the main game loop until the window is closed (the X button is pressed)
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

		// If the escape key is pressed, we set the window to close. # mohamed made this
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }


        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float ndcX = static_cast<float>(
            2.0 * mouseX / WINDOW_W - 1.0
            );

        float ndcY = static_cast<float>(
            1.0 - 2.0 * mouseY / WINDOW_H
            );

        // First, we set the clear color (Note that this function doesn't actually clear anything).
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // Then we clear the color buffer. We need to specify GL_COLOR_BUFFER_BIT because there are other buffers that we can clear. More on that later.
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the program we created earlier.
        glUseProgram(program);


        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {

            TriangleShape triangle(program);
            triangle.init();

            triangle.draw(ndcX, ndcY);
        }
		//removed to be in individual files

        // Finally, we swap the front and back buffers. This is what makes the triangle that we drew on the back buffer appear on the screen (the front buffer).
        glfwSwapBuffers(window);
    }

	//removed to be in individual files
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
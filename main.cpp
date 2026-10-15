#include <stdio.h>
#include <stdlib.h>

// We must include GLAD before GLFW, otherwise GLFW may include its own version of some OpenGL functions
#include <glad/gl.h>
#include <GLFW/glfw3.h>

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

// Let's define a 3D vector struct and a Color struct to use in our vertices.
struct Vector3D {
    float x, y, z;
};

struct Color {
    uint8_t r, g, b, a;
};

// This will be our vertex struct which contains a position and a color.
struct Vertex {
    Vector3D position;
    Color color;
};

// The window dimensions
#define WINDOW_W 640
#define WINDOW_H 480

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

    // First we define our geometry, which is a 2D rectangle in this case.
    // The rectangle will be centered around the window and will cover half the width and height of the window.
    // Each vertex will have a different color
    Vertex vertices[] = {
        { {-0.5f, -0.5f, 0.0f}, {255,   0,   0, 255} }, // Red Vertex
        { { 0.5f, -0.5f, 0.0f}, {  0, 255,   0, 255} }, // Green Vertex
        { { 0.5f,  0.5f, 0.0f}, {  0,   0, 255, 255} }, // Blue Vertex
        { {-0.5f,  0.5f, 0.0f}, {255, 255,   0, 255} }, // Yellow Vertex
    };

    // The elements array defines how we use the vertices we defined above to construct 2D triangles
    uint16_t elements[] = {
        0, 1, 2, // The first triangle will use the vertices 0, 1 & 2
        2, 3, 0, // The second triangle will use the vertices 2, 3 & 0
    };

    // The data in the arrays "vertices" & "elements" above are on the RAM, so they are not visible to the GPU
    // So next, we create buffers for them on the GPU and copy the data there.
    // We will create two buffer objects: Vertex Buffer Object (VBO) and Element Buffer Object (EBO)
    GLuint vbo, ebo;
    // First we create a buffer for the vertices (VBO)
    glGenBuffers(1, &vbo);
    // Then we bind it to the be the ARRAY_BUFFER (which means that we will use it to store the vertex data)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // And finally, we copy the vertex data to the buffer.
    // The argument GL_STATIC_DRAW give a hint to the driver that we don't plan to change the data in this buffer often. 
    // This allows the driver to allocate the buffer in a way that it can be accessed quickly by the GPU.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Similarily, we define the EBO and fill it with the element data.
    // This time we bind it as the GL_ELEMENT_ARRAY_BUFFER since it will be used to store element data.
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    // Now, let's create a Vertex Array Object (VAO).
    // The VAO defines how we should read the data from the VBO and EBO to draw the geometry inside them.
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // After we bind the VAO, it will start recording the instructions that define which VBO and EBO will be used and how to read the data from the VBO.
    glBindVertexArray(vao);
    {
        // First, we bind the VBO (This is redundant here since we already have bound the VBO as the ARRAY_BUFFER above)
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // Now, we define how the VBO should be read and supplied to the vertex shader inputs.
        
        // Before we do that, go take a look at the vertex shader, specifically the two lines:
        //        layout(location = 0) in vec3 position;
        //        layout(location = 1) in vec4 color;
        // You will notices that we say that the position atttibute is located at 0, and the color attribute is located at 1.
        // This is important, since functions like glEnableVertexAttribArray and glVertexAttribPointer takes the attribute location as the first param.
        // So, we run glEnableVertexAttribArray(0), then we are enabling the attribute located at 0, which is the position attribute in our case.
        
        // Also, let us look at how the vertex data is laid out in the VBO.
        // |                                    Vertex 0 (16 byte)                                    | Vertex 1   ... 
        // |------------------------------------------------------------------------------------------|---------------
        // |    Position (3 x float, 12 byte)     |         Color (4 x unsigned byte, 4 byte)         | Position   ...
        // |--------------------------------------|---------------------------------------------------|---------------
        // | x (4 byte) | y (4 byte) | z (4 byte) | r (1 Byte) | g (1 byte) | b (1 byte) | a (1 byte) | x (4 byte) ...
        // |------------|------------|------------|------------|------------|------------|------------|---------------
        // In the VBO, the vertices are laid out one vertex after the other. Each vertex contains:
        // - A position vector containing 3 components (x, y, z) where each component is a float (4 bytes)
        // - A color vector containing 4 components (r, g, b, a) where each component is an unsigned byte (1 byte)
        // The total size of each vertex is therefore 16 bytes.
        // To read the position of the first vertex, we jump to the start of the VBO (pointer=offsetof(Vertex,position)=0) and read 3 floats.
        // Then to read the positions of subsequent vertices, we jump forward by the size of one vertex (stride=sizeof(vertex)=16), and read 3 floats again.
        // To read the color of the firsr vertex, we jump to 12 byes from the start of the VBO (pointer=offsetof(Vertex,color)=12) and read 4 unsigned bytes.
        // Then to read the color of subsequent vertices, we jump forward by the size of one vertex (stride=sizeof(vertex)=16), and read 4 unsigned bytes again.

        // Now, let us use what we learned above to configure the VAO

        // First, we enable vertex attribute array 0 (position).
        glEnableVertexAttribArray(0);
        // Next, we tell OpenGL how to read the position attribute from the VBO by setting the arguments as follows:
        // index:      0 which is the location of the position attribute in the vertex shader.
        // size:       3 which is the number of components we are reading for each position (x, y, z).
        // type:       GL_FLOAT which is the type of each component (x, y & z are all floats).
        // normalized: GL_FALSE which means that we do not want OpenGL to normalize each component.
        //             Note that we cannot normalize float components anyway.
        // stride:     sizeof(vertex)=16 which is how many bytes we should jump in the buffer after reading each position attribute to reach the position attribute in the next vertex.
        // pointer:    offsetof(Vertex, position)=0 which is how many byte we jump in the buffer at first to reach the position attribute of the first vertex.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        
        // Now, we do the same for the color attribute.
        // First, we enable vertex attribute array 1 (color).
        glEnableVertexAttribArray(1);
        // Next, we tell OpenGL how to read the color attribute from the VBO by setting the arguments as follows:
        // index:      1 which is the location of the color attribute in the vertex shader.
        // size:       4 which is the number of components we are reading for each color (r, g, b, a).
        // type:       GL_UNSIGNED_BYTE which is the type of each component (r, g, b & a are all unsigned bytes).
        // normalized: GL_TRUE which means that we do want OpenGL to normalize each component.
        //             For unsigned bytes, this means that OpenGL will divide each component by 255, so we store values in the range [0,255] and the shader recieves values in the range [0,1].
        // stride:     sizeof(vertex)=16 which is how many bytes we should jump in the buffer after reading each color attribute to reach the color attribute in the next vertex.
        // pointer:    offsetof(Vertex, color)=12 which is how many byte we jump in the buffer at first to reach the color attribute of the first vertex.
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

        // Finally, we bind the EBO here to tell the VAO which buffer to read the elements from.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    }
    // Finally, we unbind the VAO so that it stops recording instruction.
    glBindVertexArray(0);

    // No we enter the main game loop until the window is closed (the X button is pressed)
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // First, we set the clear color (Note that this function doesn't actually clear anything).
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // Then we clear the color buffer. We need to specify GL_COLOR_BUFFER_BIT because there are other buffers that we can clear. More on that later.
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the program we created earlier.
        glUseProgram(program);
        // Bind the VAO.
        glBindVertexArray(vao);
        // Draw a rectange. We need to specify the mode (GL_TRIANGLES), the number of vertices (6), the type of each element (Unsigned Short) and where in the EBO to start reading the elements (0).
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);

        // Finally, we swap the front and back buffers. This is what makes the triangle that we drew on the back buffer appear on the screen (the front buffer).
        glfwSwapBuffers(window);
    }

    // Do some clean up before exiting.
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
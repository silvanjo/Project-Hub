// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw3.h
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM for matrix operations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// For handling C++ strings and streams
#include <iostream>

GLFWwindow* window;

// Vertex Shader Source Code
const char* vertex_shader_src = "#version 330 core\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
"uniform mat4 MVP;\n"
"void main() {\n"
"    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);\n"
"}\n";

// Fragment Shader Source Code
const char* fragment_shader_src = "#version 330 core\n"
"out vec3 color;\n"
"void main() {\n"
"    color = vec3(1.0, 0.5, 0.0);\n" // Orange color
"}\n";

// Camera parameters
glm::vec3 cameraPos   = glm::vec3(2.0f, 2.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(-1.0f, -1.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// Timing
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f;

// Mouse input variables
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;
float yaw   = -135.0f; // Initialized to look toward the cube
float pitch = -35.0f;
bool firstMouse = true;

// Function prototypes
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

int main(void) {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Set GLFW window hints (optional)
    glfwWindowHint(GLFW_SAMPLES, 4);                      // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);        // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core Profile

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(800, 600, "Minecraft Clone - Basic Cube with Camera Movement", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set the mouse callback
    glfwSetCursorPosCallback(window, mouse_callback);

    // Capture the mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW (must be done after context creation)
    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Define the cube vertices
    GLfloat vertices[] = {
        // Positions          
        -0.5f, -0.5f, -0.5f, // Back face
         0.5f, -0.5f, -0.5f,         
         0.5f,  0.5f, -0.5f,         
         0.5f,  0.5f, -0.5f,         
        -0.5f,  0.5f, -0.5f,         
        -0.5f, -0.5f, -0.5f,         

        -0.5f, -0.5f,  0.5f, // Front face
         0.5f, -0.5f,  0.5f,         
         0.5f,  0.5f,  0.5f,         
         0.5f,  0.5f,  0.5f,         
        -0.5f,  0.5f,  0.5f,         
        -0.5f, -0.5f,  0.5f,         

        -0.5f,  0.5f,  0.5f, // Left face
        -0.5f,  0.5f, -0.5f,         
        -0.5f, -0.5f, -0.5f,         
        -0.5f, -0.5f, -0.5f,         
        -0.5f, -0.5f,  0.5f,         
        -0.5f,  0.5f,  0.5f,         

         0.5f,  0.5f,  0.5f, // Right face
         0.5f,  0.5f, -0.5f,         
         0.5f, -0.5f, -0.5f,         
         0.5f, -0.5f, -0.5f,         
         0.5f, -0.5f,  0.5f,         
         0.5f,  0.5f,  0.5f,         

        -0.5f, -0.5f, -0.5f, // Bottom face
         0.5f, -0.5f, -0.5f,         
         0.5f, -0.5f,  0.5f,         
         0.5f, -0.5f,  0.5f,         
        -0.5f, -0.5f,  0.5f,         
        -0.5f, -0.5f, -0.5f,         

        -0.5f,  0.5f, -0.5f, // Top face
         0.5f,  0.5f, -0.5f,         
         0.5f,  0.5f,  0.5f,         
         0.5f,  0.5f,  0.5f,         
        -0.5f,  0.5f,  0.5f,         
        -0.5f,  0.5f, -0.5f,         
    };

    // Generate and bind Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind Vertex Buffer Object (VBO)
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // Bind the VBO and upload the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Compile Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertexShader);
    // Check for compilation errors
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        // Error handling code here
    }

    // Compile Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragmentShader);
    // Check for compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        // Error handling code here
    }

    // Link shaders into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        // Error handling code here
    }

    // Use the shader program
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "vertexPosition_modelspace");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(
        posAttrib,    // Attribute index
        3,            // Number of components (x, y, z)
        GL_FLOAT,     // Data type
        GL_FALSE,     // Normalized?
        3 * sizeof(GLfloat), // Stride
        (void*)0      // Offset
    );

    // Get the location of the "MVP" uniform
    GLuint uniMVP = glGetUniformLocation(shaderProgram, "MVP");

    // Set up projection matrix
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),   // Field of view angle
        800.0f / 600.0f,       // Aspect ratio
        0.1f,                  // Near clipping plane
        100.0f                 // Far clipping plane
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 model = glm::mat4(1.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        processInput(window);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update view matrix based on camera position and orientation
        glm::mat4 view = glm::lookAt(
            cameraPos, 
            cameraPos + cameraFront, 
            cameraUp
        );

        // Combine the matrices
        glm::mat4 MVP = proj * view * model;

        // Send the MVP matrix to the shader
        glUniformMatrix4fv(uniMVP, 1, GL_FALSE, &MVP[0][0]);

        // Draw the cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

// Process keyboard input
void processInput(GLFWwindow *window)
{
    float cameraSpeed = 2.5f * deltaTime; // Adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

// Mouse callback function
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }    

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // Adjust this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, the screen doesn't get flipped
    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    // Update camera front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

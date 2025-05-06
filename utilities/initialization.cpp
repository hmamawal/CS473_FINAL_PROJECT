#include "initialization.hpp"
#include "input_handling.hpp"
#include "../classes/object_types.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

// External variables needed by initialization functions
extern Font arial_font;
extern Camera camera;
extern glm::vec3 point_light_color;
extern glm::vec4 light_direction; // Changed from light_position to light_direction
extern float last_x;
extern float last_y;
extern bool first_mouse;

void InitializeMouseSettings(GLFWwindow* window) {
    // Initialize mouse settings
    last_x = 800 / 2.0f; // Using default width, consider passing as parameters
    last_y = 800 / 2.0f; // Using default height, consider passing as parameters
    glfwSetCursorPosCallback(window, mouse_callback); 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    std::cout << "Mouse settings initialized" << std::endl;
}

void CreateShaders(Shader*& shader_program_ptr, Shader*& font_program_ptr) {
    // Create the shader programs for the shapes and the font
    shader_program_ptr = new Shader(".//shaders//vertex.glsl", ".//shaders//fragment.glsl");
    font_program_ptr = new Shader(".//shaders//vertex.glsl", ".//shaders//fontFragmentShader.glsl");
    
    std::cout << "Shaders created" << std::endl;
}

void CreateDepthShader(Shader*& depth_shader) {
    depth_shader = new Shader("shaders/depthVertexShader.glsl", "shaders/depthFragmentShader.glsl");
    if (depth_shader == nullptr) {
        std::cout << "Failed to create depth shader" << std::endl;
    } else {
        std::cout << "Depth shader created successfully" << std::endl;
    }
}

void SetupAvatars(Avatar& baseAvatar, AvatarHighBar*& high_bar_avatar, GameModels& models) {
    // baseAvatar is now already initialized in main.cpp, so we don't modify it here

    // Only initialize the high_bar_avatar
    high_bar_avatar = new AvatarHighBar(models.baseModel, 180.0f, glm::vec3(-10.0, 0.0, 0.0), IMPORTED_BASIC);
    high_bar_avatar->Scale(glm::vec3(0.5f, 0.5f, 0.5f));
    
    std::cout << "Avatars setup complete" << std::endl;
}

void SetupCameraAndProjection(Shader* shader_program_ptr, unsigned int scr_width, unsigned int scr_height) {
    // Set up projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (1.0f * scr_width) / (1.0f * scr_height), 0.1f, 100.0f);
    shader_program_ptr->setMat4("projection", projection);
    
    // Setup lighting - passing light_direction instead of light_position
    setupLighting(shader_program_ptr, point_light_color, light_direction, camera);
    
    std::cout << "Camera and projection setup complete" << std::endl;
}

void SetupFontShader(Shader* font_program_ptr) {
    // Font shader settings
    font_program_ptr->use();
    glm::mat4 identity(1.0);
    font_program_ptr->setMat4("local", identity);
    font_program_ptr->setMat4("model", identity);
    font_program_ptr->setMat4("view", identity);
    font_program_ptr->setMat4("projection", glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0));
    font_program_ptr->setVec4("transparentColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    font_program_ptr->setFloat("alpha", 0.3);
    font_program_ptr->setInt("texture1", 0);
    
    std::cout << "Font shader setup complete" << std::endl;
}

void SetupRendering() {
    // Set up the depth test and blending
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    std::cout << "Rendering setup complete" << std::endl;
}
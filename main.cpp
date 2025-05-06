#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utilities/environment.hpp"
#include "utilities/rendering.hpp"
#include "utilities/input_handling.hpp"
#include "utilities/initialization.hpp"
#include "classes/camera.hpp"
#include "classes/Font.hpp"
#include "classes/import_object.hpp"
#include "classes/avatar.hpp"
#include "classes/avatar_high_bar.hpp"
#include "classes/object_types.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// Global Variables
// ----------------
// Screen width and height
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// Clear color
glm::vec4 clear_color(0.0, 0.0, 0.0, 1.0);

// Mouse variables
bool first_mouse{true};
float last_x{0.0};
float last_y{0.0};

// Point light color (modified by R key)
glm::vec3 point_light_color(1.0f, 1.0f, 1.0f); // Default to white

// Camera Object
Camera camera(glm::vec3(0.0f, 1.0f, 25.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float delta_time{0.001};
float last_frame{0.0};

// Light position
glm::vec4 light_position(0.0, 5.0, 0.0, 1.0);

// Font for text display
Font arial_font("fonts/ArialBlackLarge.bmp", "fonts/ArialBlack.csv", 0.1, 0.15);

// High bar avatar
AvatarHighBar* high_bar_avatar = nullptr;

// Global variables to store the original camera state
glm::vec3 original_camera_position = camera.Position;
float original_camera_yaw = camera.Yaw;
float original_camera_pitch = camera.Pitch;

// Shader program pointers
Shader* shader_program_ptr = nullptr;
Shader* font_program_ptr = nullptr;
Shader* shadow_shader_ptr = nullptr;

int main() {
    std::cout << "Starting initialization..." << std::endl;
    // Initialize the environment
    GLFWwindow *window = InitializeEnvironment("CS473", SCR_WIDTH, SCR_HEIGHT);
    if (window == NULL) {
        std::cout << "Failed to initialize GLFWwindow" << std::endl;
        return -1;
    }

    std::cout << "Initializing environment..." << std::endl;

    // Add this line AFTER OpenGL is initialized 
    initializeShadowMap();

    std::cout << "Program starting..." << std::endl;

    // Initialize mouse, shaders, and setup rendering
    InitializeMouseSettings(window);
    
    // Initialize shaders
    CreateShaders(shader_program_ptr, font_program_ptr);
    shadow_shader_ptr = new Shader(".//shaders//shadow_mapping.vert", ".//shaders//shadow_mapping.frag");
    // After creating your shadow_shader_ptr
    if (shadow_shader_ptr->ID == 0) {
        std::cout << "Failed to compile shadow shader" << std::endl;
        return -1;
    }
    
    // Setup VAOs and models
    RenderingVAOs vaos = setupVAOs();
    ImportOBJ importer;
    GameModels models = loadModels(vaos, importer);
    
    // Setup avatars, camera, lighting, and font
    // Initialize baseAvatar with the correct model and parameters before passing to SetupAvatars
    Avatar baseAvatar(models.baseModel, 180.0f, glm::vec3(0.0, 0.4, 0.0), IMPORTED_BASIC);
    baseAvatar.Scale(glm::vec3(0.5f, 0.5f, 0.5f));
    
    // Now initialize the high_bar_avatar in SetupAvatars
    SetupAvatars(baseAvatar, high_bar_avatar, models);
    
    setupShaders(shader_program_ptr, arial_font, vaos);
    SetupCameraAndProjection(shader_program_ptr, SCR_WIDTH, SCR_HEIGHT);
    SetupFontShader(font_program_ptr);
    SetupRendering();

    std::cout << "Entering render loop..." << std::endl;
    static int frame_count = 0;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Adjust for real time
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame; 
        
        // Handle input
        ProcessInput(window);
        baseAvatar.ProcessInput(window, delta_time);
        high_bar_avatar->ProcessInput(window, delta_time);

        // Clear buffers
        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Render the scene
        renderScene(
            shader_program_ptr,
            shadow_shader_ptr,
            models,
            baseAvatar,
            high_bar_avatar,
            camera,
            point_light_color,
            light_position
        );
        
        // Enforce minimum camera height
        if (camera.Position.y < 0.5) {
            camera.Position.y = 0.5;
        }
        
        // Display text
        renderText(font_program_ptr, arial_font, camera);
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Limit frame rate to prevent excessive GPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // ~60 FPS
        
        // Debug frame count to help track potential issues
        if (frame_count % 100 == 0) {
            std::cout << "Frame: " << frame_count << std::endl;
        }
        frame_count++;
    }

    std::cout << "Render loop exited, starting cleanup..." << std::endl;

    // Cleanup resources
    cleanupResources(vaos, models);

    // Add this before terminating
    cleanupShadowMap();

    // Delete the shader programs
    if (shader_program_ptr != nullptr) {
        delete shader_program_ptr;
        shader_program_ptr = nullptr;
    }
    if (font_program_ptr != nullptr) {
        delete font_program_ptr;
        font_program_ptr = nullptr;
    }
    std::cout << "Shader programs deleted" << std::endl;
    if (shadow_shader_ptr != nullptr) {
        delete shadow_shader_ptr;
        shadow_shader_ptr = nullptr;
    }

    // Delete the high_bar_avatar object
    if (high_bar_avatar != nullptr) {
        delete high_bar_avatar;
        high_bar_avatar = nullptr;
    }
    std::cout << "High bar avatar deleted" << std::endl;

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
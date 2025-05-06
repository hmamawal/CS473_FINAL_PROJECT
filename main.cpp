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

// Light direction (replacing light position)
glm::vec4 light_direction(-1.0, -1.0, 0.0, 0.0);

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
Shader * depth_shader_ptr = nullptr;

int main() {
    // Initialize the environment
    GLFWwindow *window = InitializeEnvironment("CS473", SCR_WIDTH, SCR_HEIGHT);
    if (window == NULL) {
        std::cout << "Failed to initialize GLFWwindow" << std::endl;
        return -1;
    }

    std::cout << "Program starting..." << std::endl;

    // Initialize mouse, shaders, and setup rendering
    InitializeMouseSettings(window);
    
    // Initialize shaders
    CreateShaders(shader_program_ptr, font_program_ptr);
    CreateDepthShader(depth_shader_ptr);
    
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

    // Just before the render loop, after SetupRendering()
    // Define constants for the shadow map resolution
    const unsigned int SHADOW_WIDTH = 2048; 
    const unsigned int SHADOW_HEIGHT = 2048;

    // a. Define and generate the framebuffer
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    // b. Define and generate a 2D texture for depth information
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // c. Bind the framebuffer and attach the texture
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    // d. Set draw and read buffers to none since we only need depth
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is not complete!" << std::endl;
    }

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::cout << "Shadow mapping depth framebuffer setup complete" << std::endl;

    std::cout << "Entering render loop..." << std::endl;
    static int frame_count = 0;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Adjust for real time
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame; 

        // Create light space transformation matrix
        // 1. Create a position for the light source based on the negative direction vector
        glm::vec3 lightPos = -100.0f * glm::vec3(light_direction.x, light_direction.y, light_direction.z);

        // 2. Create the light view matrix - looking from light position toward the scene origin
        glm::mat4 lightView = glm::lookAt(
            lightPos,              // Light position
            glm::vec3(0.0f),       // Look at scene center
            glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
        );

        // 3. Create the orthographic projection matrix with your specified dimensions
        float near_plane = -10.0f;
        float far_plane = 150.0f;
        glm::mat4 lightProjection = glm::ortho(
            -100.0f, 100.0f,       // Left, right
            -100.0f, 100.0f,       // Bottom, top
            near_plane, far_plane  // Near, far
        );

        // 4. Create the combined light space matrix
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        // 5. Set the light space matrix uniform in your depth shader
        depth_shader_ptr->use();
        depth_shader_ptr->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        
        // FIRST PASS - Render to depth map
        // Set viewport to shadow map dimensions
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Use depth shader for shadow mapping
        depth_shader_ptr->use();
        depth_shader_ptr->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // Render the scene for depth map
        renderScene(
            depth_shader_ptr, // Use depth shader instead of regular shader
            models,
            baseAvatar,
            high_bar_avatar,
            camera,
            point_light_color,
            light_direction,
            true // Set is_depth_pass to true for depth rendering
        );

        // SECOND PASS - Regular rendering to screen
        // Reset viewport to screen dimensions
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Make sure you're using the main shader program
        shader_program_ptr->use();

        // Set the lightSpaceMatrix uniform in the shader
        shader_program_ptr->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // Assign the shadow map to texture unit 1
        // IMPORTANT: Don't use texture unit 0 as it's likely used by your model textures
        shader_program_ptr->setInt("shadow_map", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        // Your model textures will use GL_TEXTURE0 by default
        // No need to explicitly set that here if your renderScene function handles it properly

        // Handle input
        ProcessInput(window);
        baseAvatar.ProcessInput(window, delta_time);
        high_bar_avatar->ProcessInput(window, delta_time);

        // Render the scene normally
        renderScene(
            shader_program_ptr,
            models,
            baseAvatar,
            high_bar_avatar,
            camera,
            point_light_color,
            light_direction,
            false // Set is_depth_pass to false for normal rendering
        );

        // Reset active texture
        glActiveTexture(GL_TEXTURE0);

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

    if (depth_shader_ptr != nullptr) {
        delete depth_shader_ptr;
        depth_shader_ptr = nullptr;
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
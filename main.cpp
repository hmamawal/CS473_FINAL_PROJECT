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
Shader* depth_shader_ptr = nullptr;
Shader* post_processing_shader_ptr = nullptr;
Shader* skybox_shader_ptr = nullptr; 

// Post-processing effect selection
int current_effect = 0; // 0 = no effect
bool effect_key_pressed = false; // To prevent multiple toggles in one key press

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
    CreatePostProcessingShader(post_processing_shader_ptr);  
    CreateSkyboxShader(skybox_shader_ptr);  
    
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

    // Define constants for the shadow map resolution
    const unsigned int SHADOW_WIDTH = 2048; 
    const unsigned int SHADOW_HEIGHT = 2048;

    // Define and generate the framebuffer
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    // Define and generate a 2D texture for depth information
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

    // Bind the framebuffer and attach the texture
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    // Set draw and read buffers to none since we only need depth
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

    /////////////////////////
    // For post-processing //
    /////////////////////////
    // Create a framebuffer object for post-processing
    unsigned int postProcessingFBO;
    glGenFramebuffers(1, &postProcessingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

    // Create a texture attachment for the framebuffer
    unsigned int textureColorBuffer;
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    // Create a renderbuffer object for depth and stencil testing
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // 24 bits for depth, 8 for stencil
    
    // Attach the renderbuffer to the framebuffer's depth and stencil attachment
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Post-processing framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create a texture rectangle that fills the screen in NDC (-1,-1) to (1,1)
    float quadVertices[] = {
        // positions   // texture coords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    // Screen quad VAO/VBO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 *sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Get the time elapsed since the last frame
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame; 

        // Handle input 
        ProcessInput(window);
        baseAvatar.ProcessInput(window, delta_time);
        high_bar_avatar->ProcessInput(window, delta_time);

        // Light space transformation matrix

        // Create a position for the light source based on the negative direction vector
        glm::vec3 lightPos = -100.0f * glm::vec3(light_direction.x, light_direction.y, light_direction.z);

        // Create the light view matrix - looking from light position toward the scene origin
        glm::mat4 lightView = glm::lookAt(
            lightPos,              // Light position
            glm::vec3(0.0f),       // Look at scene center
            glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
        );

        // Create the orthographic projection matrix with your specified dimensions
        float near_plane = -10.0f;
        float far_plane = 150.0f;
        glm::mat4 lightProjection = glm::ortho(
            -100.0f, 100.0f,       // Left, right
            -100.0f, 100.0f,       // Bottom, top
            near_plane, far_plane  // Near, far
        );

        // Create the combined light space matrix
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        // Set the light space matrix uniform in your depth shader
        depth_shader_ptr->use();
        depth_shader_ptr->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        
        // FIRST PASS - Render to depth map for shadows
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Render the scene for depth map
        renderScene(
            depth_shader_ptr,
            models,
            baseAvatar,
            high_bar_avatar,
            camera,
            point_light_color,
            light_direction,
            true // is_depth_pass
        );

        // SECOND PASS - Render to post-processing framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw skybox first (before other scene objects but after clearing the buffer)
        renderSkybox(skybox_shader_ptr, models, camera, SCR_WIDTH, SCR_HEIGHT);
        
        // Set up the main shader
        shader_program_ptr->use();
        shader_program_ptr->setBool("debug_shadows", false);
        shader_program_ptr->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        
        // Apply post-processing effect
        shader_program_ptr->setInt("post_process_selection", current_effect);
        
        // Assign shadow map to texture unit 1
        shader_program_ptr->setInt("shadow_map", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        
        // Reset to texture unit 0 for other textures
        glActiveTexture(GL_TEXTURE0);

        // Render the scene normally
        renderScene(
            shader_program_ptr,
            models,
            baseAvatar,
            high_bar_avatar,
            camera,
            point_light_color,
            light_direction,
            false // is_depth_pass
        );

        // Enforce minimum camera height
        if (camera.Position.y < 0.5) {
            camera.Position.y = 0.5;
        }
        
        // Display text (HUD)
        renderText(font_program_ptr, arial_font, camera);

        // THIRD PASS - Render the framebuffer texture to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use the post-processing shader for the final pass
        post_processing_shader_ptr->use();
        post_processing_shader_ptr->setInt("screenTexture", 0);
        post_processing_shader_ptr->setInt("effect", current_effect);
        
        // Bind the texture from our framebuffer
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        
        // Render the quad
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Limit frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        
        // Debug frame count
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

    if (post_processing_shader_ptr != nullptr) {
        delete post_processing_shader_ptr;
        post_processing_shader_ptr = nullptr;
    }

    if (skybox_shader_ptr != nullptr) {
        delete skybox_shader_ptr;
        skybox_shader_ptr = nullptr;
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
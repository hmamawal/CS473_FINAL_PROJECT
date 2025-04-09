#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utilities/environment.hpp"
#include "utilities/rendering.hpp"
#include "classes/camera.hpp"
#include "classes/Font.hpp"
#include "classes/import_object.hpp"
#include "classes/avatar.hpp"
#include "classes/avatar_high_bar.hpp"
#include "classes/object_types.hpp"

// Function prototypes
void ProcessInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

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

// Shader program pointer
Shader* shader_program_ptr = nullptr;

int main() {
    // Initialize the environment
    GLFWwindow *window = InitializeEnvironment("CS473", SCR_WIDTH, SCR_HEIGHT);
    if (window == NULL) {
        std::cout << "Failed to initialize GLFWwindow" << std::endl;
        return -1;
    }

    std::cout << "Program starting..." << std::endl;

    // Initialize mouse settings
    last_x = SCR_WIDTH / 2.0f;
    last_y = SCR_HEIGHT / 2.0f;
    glfwSetCursorPosCallback(window, mouse_callback); 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    // Create the shader programs for the shapes and the font
    shader_program_ptr = new Shader(".//shaders//vertex.glsl", ".//shaders//fragment.glsl");
    Shader font_program(".//shaders//vertex.glsl", ".//shaders//fontFragmentShader.glsl");

    // Setup VAOs
    RenderingVAOs vaos = setupVAOs();
    
    // Create importer and load models
    ImportOBJ importer;
    GameModels models = loadModels(vaos, importer);
    
    // Create and setup avatars
    Avatar baseAvatar(models.baseModel, 180.0f, glm::vec3(0.0, 0.4, 0.0), IMPORTED_BASIC);
    baseAvatar.Scale(glm::vec3(0.5f, 0.5f, 0.5f)); 
    
    high_bar_avatar = new AvatarHighBar(models.baseModel, 180.0f, glm::vec3(-10.0, 0.0, 0.0), IMPORTED_BASIC);
    high_bar_avatar->Scale(glm::vec3(0.5f, 0.5f, 0.5f));
    
    // Setup shaders and lighting
    setupShaders(shader_program_ptr, arial_font, vaos);
    
    // Set up projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (1.0f * SCR_WIDTH) / (1.0f * SCR_HEIGHT), 0.1f, 100.0f);
    shader_program_ptr->setMat4("projection", projection);
    
    // Setup lighting
    setupLighting(shader_program_ptr, point_light_color, light_position, camera);
    
    // Font shader settings
    font_program.use();
    glm::mat4 identity(1.0);
    font_program.setMat4("local", identity);
    font_program.setMat4("model", identity);
    font_program.setMat4("view", identity);
    font_program.setMat4("projection", glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0));
    font_program.setVec4("transparentColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    font_program.setFloat("alpha", 0.3);
    font_program.setInt("texture1", 0);

    // Set up the depth test and blending
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "Entering render loop..." << std::endl;

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
        renderText(font_program, arial_font, camera);
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "Render loop exited, starting cleanup..." << std::endl;

    // Cleanup resources
    cleanupResources(vaos, models);

    // Delete the shader program
    if (shader_program_ptr != nullptr) {
        delete shader_program_ptr;
        shader_program_ptr = nullptr;
    }
    std::cout << "Shader program deleted" << std::endl;

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

// Process all input
void ProcessInput(GLFWwindow *window) {
    static bool c_key_pressed = false;
    static bool r_key_pressed = false;
    static bool l_key_pressed = false;
    static bool spotlight_on = true;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Process 'R' key to change point light color
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (!r_key_pressed) {
            r_key_pressed = true;
            
            // Toggle between red and white
            if (point_light_color.r == 1.0f && point_light_color.g == 0.0f && point_light_color.b == 0.0f) {
                // Change back to white
                point_light_color = glm::vec3(1.0f, 1.0f, 1.0f);
                std::cout << "Point light color changed to white" << std::endl;
            } else {
                // Change to red
                point_light_color = glm::vec3(1.0f, 0.0f, 0.0f);
                std::cout << "Point light color changed to red" << std::endl;
            }
            
            // Update the light color in the shader
            shader_program_ptr->setVec4("point_light.ambient", glm::vec4(0.5f * point_light_color, 1.0f));
            shader_program_ptr->setVec4("point_light.diffuse", glm::vec4(point_light_color, 1.0f));
            shader_program_ptr->setVec4("point_light.specular", glm::vec4(0.5f * point_light_color, 1.0f));
        }
    } else {
        r_key_pressed = false;
    }

    // Process 'C' key to toggle first-person view
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        if (!c_key_pressed) {
            c_key_pressed = true;

            if (!camera.first_person_view) {
                // Save the original camera state before entering first-person view
                original_camera_position = camera.Position;
                original_camera_yaw = camera.Yaw;
                original_camera_pitch = camera.Pitch;
            }

            // Toggle first-person view
            camera.first_person_view = !camera.first_person_view;

            if (!camera.first_person_view) {
                // Restore the original camera state when exiting first-person view
                camera.Position = original_camera_position;
                camera.Yaw = original_camera_yaw;
                camera.Pitch = original_camera_pitch;
                camera.updateCameraVectors();
            }
        }
    } else {
        c_key_pressed = false;
    }

    // Update camera position and orientation if in first-person view
    if (camera.first_person_view) {
        // Get the position and rotation angle from high_bar_avatar
        glm::vec3 avatar_pos;
        high_bar_avatar->GetPosition(avatar_pos);

        // Position the camera at the avatar's position but slightly higher (eye level)
        camera.Position = avatar_pos + glm::vec3(0.0f, 2.0f, 0.0f);

        // Get the X rotation angle (pitch) from avatar
        float x_rotation_angle;
        high_bar_avatar->GetXRotationAngle(x_rotation_angle);

        // Update camera pitch based on avatar's X rotation
        camera.Pitch = x_rotation_angle;

        // Update camera vectors with the new pitch
        camera.updateCameraVectors();

        return; // Skip regular camera controls when in first-person
    }

    // Regular camera controls (only when not in first-person view)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, delta_time);
    }

    // Process 'L' key to toggle spotlight
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (!l_key_pressed) {
            l_key_pressed = true;
            spotlight_on = !spotlight_on;
            shader_program_ptr->setBool("spot_light.on", spotlight_on);

            // Print feedback
            if (spotlight_on) {
                std::cout << "Spotlight ON" << std::endl;
            } else {
                std::cout << "Spotlight OFF" << std::endl;
            }
        }
    } else {
        l_key_pressed = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }
  
    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; 
    last_x = xpos;
    last_y = ypos;

    float sensitivity = 0.7f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
#include "rendering.hpp"
#include "build_shapes.hpp"
#include "input_handling.hpp" // Added to access spotlight_on variable
#include <iostream>
#include "../classes/object_types.hpp"

RenderingVAOs setupVAOs() {
    RenderingVAOs vaos;
    
    // Basic VAO setup
    glGenVertexArrays(1, &(vaos.basic_vao.id));
    vaos.basic_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, 6*sizeof(float), 0));
    vaos.basic_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, 6*sizeof(float), 3*sizeof(float)));
    
    // Texture VAO setup
    glGenVertexArrays(1, &(vaos.texture_vao.id));
    vaos.texture_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, 8*sizeof(float), 0));
    vaos.texture_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, 8*sizeof(float), 3*sizeof(float)));
    vaos.texture_vao.attributes.push_back(BuildAttribute(2, GL_FLOAT, false, 8*sizeof(float), 6*sizeof(float)));

    // Import VAO setup
    glGenVertexArrays(1, &(vaos.import_vao.id));
    int stride_size = 19*sizeof(float);
    vaos.import_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, stride_size, 0));
    vaos.import_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, stride_size, 3*sizeof(float)));
    vaos.import_vao.attributes.push_back(BuildAttribute(2, GL_FLOAT, false, stride_size, 6*sizeof(float)));
    vaos.import_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, stride_size, 8*sizeof(float)));
    vaos.import_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, stride_size, 11*sizeof(float)));
    vaos.import_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, stride_size, 14*sizeof(float)));
    vaos.import_vao.attributes.push_back(BuildAttribute(1, GL_FLOAT, false, stride_size, 17*sizeof(float)));
    vaos.import_vao.attributes.push_back(BuildAttribute(1, GL_FLOAT, false, stride_size, 18*sizeof(float)));
    
    // Skybox VAO setup
    glGenVertexArrays(1, &(vaos.skybox_vao.id));
    vaos.skybox_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, 3*sizeof(float), 0));
    
    std::cout << "VAOs created and attributes set up successfully." << std::endl;
    
    return vaos;
}

GameModels loadModels(RenderingVAOs& vaos, ImportOBJ& importer) {
    GameModels models;
    std::cout << "Loading models..." << std::endl;
    
    // Load base model
    models.baseModel = importer.loadFiles("models/baseModel", vaos.import_vao);
    std::cout << "BaseModel imported" << std::endl;
    
    // Load tumbling floor
    models.tumbling_floor = importer.loadFiles("models/tumbling_floor", vaos.import_vao);
    std::cout << "Tumbling floor imported" << std::endl;
    models.tumbling_floor_texture = importer.getTexture();
    std::cout << "Tumbling floor texture imported" << std::endl;

    // Load vault table
    models.vault_table = importer.loadFiles("models/VaultTable", vaos.import_vao);
    models.vault_table_textures = importer.getAllTextures();
    for (int i = 0; i < models.vault_table_textures.size(); i++) {
        std::cout << "Vault table texture " << i << ": " << models.vault_table_textures[i] << std::endl;
    }
    std::cout << "Vault table imported with " << models.vault_table_textures.size() << " textures." << std::endl;

    // Load Lou Gross building
    models.LouGrossBuilding = importer.loadFiles("models/ComplexBuilding", vaos.import_vao);
    std::cout << "Lou Gross Building imported" << std::endl;
    models.building_textures = importer.getAllTextures();
    for (int i = 0; i < models.building_textures.size(); i++) {
        std::cout << "Lou Gross Building texture " << i << ": " << models.building_textures[i] << std::endl;
    }
    std::cout << "Lou Gross Building imported with " << models.building_textures.size() << " textures." << std::endl;

    // Load high bar
    models.high_bar = importer.loadFiles("models/HighBar", vaos.import_vao);
    std::cout << "High Bar imported" << std::endl;

    // Load pommel horses
    models.pommel_horse = importer.loadFiles("models/PommelHorse", vaos.import_vao);
    std::cout << "PommelHorse imported" << std::endl;
    
    models.pommel_horse2 = importer.loadFiles("models/PommelHorse2", vaos.import_vao);
    std::cout << "PommelHorse2 imported" << std::endl;

    // Create floor
    models.floor_texture = GetTexture("./textures/hull_texture.png");
    models.floor = GetTexturedRectangle(vaos.texture_vao, glm::vec3(-25.0, -25.0, 0.0), 50.0, 50.0, 20.0, false);

    // Create skybox
    models.skybox = GetCube(vaos.skybox_vao);
    
    // Load skybox textures
    std::vector<std::string> skybox_faces = {
        "./textures/skybox/right.jpg",
        "./textures/skybox/left.jpg",
        "./textures/skybox/top.jpg",
        "./textures/skybox/bottom.jpg",
        "./textures/skybox/front.jpg",
        "./textures/skybox/back.jpg"
    };
    models.skybox_texture = GetCubeMap(skybox_faces, false);
    std::cout << "Skybox cube and textures loaded" << std::endl;
    
    return models;
}

void setupShaders(Shader* shader_program, Font& arial_font, const RenderingVAOs& vaos) {
    shader_program->use();
    
    // Setting up model, view and projection matrices
    glm::mat4 identity(1.0);
    glm::mat4 model = glm::rotate(identity, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    shader_program->setMat4("model", model);
    
    // Initialize font
    arial_font.initialize(vaos.texture_vao);
    std::cout << "Font initialized" << std::endl;
}

void setupLighting(Shader* shader_program, const glm::vec3& light_color, 
                  const glm::vec4& light_direction, const Camera& camera) {
    // Directional light setup (previously point light)
    shader_program->setVec4("directional_light.ambient", glm::vec4(0.5f * light_color, 1.0));
    shader_program->setVec4("directional_light.diffuse", glm::vec4(light_color, 1.0f));
    shader_program->setVec4("directional_light.specular", glm::vec4(0.5f * light_color, 1.0f));
    shader_program->setVec4("directional_light.direction", light_direction);
    shader_program->setBool("directional_light.on", true);

    shader_program->setVec4("view_position", glm::vec4(camera.Position, 1.0));

    // Spotlight setup - this could be a flashlight following the camera
    shader_program->setVec4("spot_light.position", glm::vec4(camera.Position, 1.0f));
    shader_program->setVec4("spot_light.direction", glm::vec4(camera.Front, 0.0f));

    // Light colors
    shader_program->setVec4("spot_light.ambient", glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));  
    shader_program->setVec4("spot_light.diffuse", glm::vec4(1.0f, 0.95f, 0.8f, 1.0f)); // Warm light
    shader_program->setVec4("spot_light.specular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Spotlight properties
    shader_program->setFloat("spot_light.cutOff", glm::cos(glm::radians(10.0f)));
    shader_program->setFloat("spot_light.outerCutOff", glm::cos(glm::radians(15.0f)));
    shader_program->setBool("spot_light.on", true);

    shader_program->setFloat("spot_light.constant", 1.0f);
    shader_program->setFloat("spot_light.linear", 0.09f);
    shader_program->setFloat("spot_light.quadratic", 0.032f);
}

void renderScene(Shader* shader_program, 
                GameModels& models, 
                Avatar& baseAvatar,
                AvatarHighBar* high_bar_avatar,
                const Camera& camera,
                const glm::vec3& light_color,
                const glm::vec4& light_direction,
                bool is_depth_pass) {
    
    shader_program->use();
    
    // Update view matrix based on camera
    // Create a non-const copy of the camera to call GetViewMatrix()
    Camera camera_copy = camera;
    glm::mat4 view = camera_copy.GetViewMatrix();
    shader_program->setMat4("view", view);
    shader_program->setVec4("view_position", glm::vec4(camera.Position, 1.0));
    
    // Update spotlight position and direction
    shader_program->setVec4("spot_light.position", glm::vec4(camera.Position, 1.0f));
    shader_program->setVec4("spot_light.direction", glm::vec4(camera.Front, 0.0f));
    
    // Ensure spotlight state is correctly set every frame
    shader_program->setBool("spot_light.on", spotlight_on);
    
    // Update directional light and ensure its state is correctly set every frame
    shader_program->setVec4("directional_light.ambient", glm::vec4(0.5f * light_color, 1.0));
    shader_program->setVec4("directional_light.diffuse", glm::vec4(light_color, 1.0f));
    shader_program->setVec4("directional_light.specular", glm::vec4(0.5f * light_color, 1.0f));
    shader_program->setVec4("directional_light.direction", light_direction);
    shader_program->setBool("directional_light.on", point_light_on); // Using existing point_light_on toggle
    
    // Draw base avatar
    baseAvatar.Draw(shader_program, false);
    
    // Draw floor
    glm::mat4 identity(1.0);
    glm::mat4 model = glm::rotate(identity, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    
    shader_program->setInt("shader_state", TEXTURED);
    shader_program->setMat4("model", model);
    
    glm::mat4 floor_local(1.0);
    floor_local = glm::translate(floor_local, glm::vec3(0.0, 0.0, -0.01));
    shader_program->setMat4("local", floor_local);
    
    // Add condition for floor texture
    if (!is_depth_pass) {
        glActiveTexture(GL_TEXTURE0);
        shader_program->setInt("textures[0]", 0);
        glBindTexture(GL_TEXTURE_2D, models.floor_texture);
    }
    models.floor.Draw();
    // After rendering each textured object:
    if (!is_depth_pass) {
        glActiveTexture(GL_TEXTURE0);
    }
    
    // Draw tumbling floor
    shader_program->setInt("shader_state", IMPORTED_TEXTURED);
    glm::mat4 tumbling_floor_local(1.0);
    tumbling_floor_local = glm::translate(tumbling_floor_local, glm::vec3(0.0, 0.4, 0.0));
    shader_program->setMat4("model", identity);
    shader_program->setMat4("local", tumbling_floor_local);
    
    // Add condition for tumbling floor texture
    if (!is_depth_pass) {
        glActiveTexture(GL_TEXTURE0);
        shader_program->setInt("textures[0]", 0);
        glBindTexture(GL_TEXTURE_2D, models.tumbling_floor_texture);
    }
    models.tumbling_floor.Draw();
    // After rendering each textured object:
    if (!is_depth_pass) {
        glActiveTexture(GL_TEXTURE0);
    }
    
    // Draw vault table
    shader_program->setInt("shader_state", IMPORTED_TEXTURED);
    glm::mat4 vault_table_local(1.0);
    vault_table_local = glm::translate(vault_table_local, glm::vec3(8.0, 0.0, 0.0));
    shader_program->setMat4("model", identity);
    shader_program->setMat4("local", vault_table_local);
    
    // When binding textures, add a condition:
    if (!is_depth_pass) {
        // For the vault table
        for (int i = 0; i < models.vault_table_textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string texture_string = "textures[" + std::to_string(i) + "]";
            shader_program->setInt(texture_string, i);
            glBindTexture(GL_TEXTURE_2D, models.vault_table_textures[i]);
        }
    }
    models.vault_table.Draw();
    if (!is_depth_pass) {
        glActiveTexture(GL_TEXTURE0); // Reset active texture
    }
    // After rendering each textured object:
    if (!is_depth_pass) {
        glActiveTexture(GL_TEXTURE0);
    }
    
    // Draw Lou Gross Building
    shader_program->setInt("shader_state", IMPORTED_TEXTURED);
    glm::mat4 building_local(1.0);
    building_local = glm::translate(building_local, glm::vec3(0.0, 0.0, 0.0));
    building_local = glm::scale(building_local, glm::vec3(2, 2, 2));
    shader_program->setMat4("model", identity);
    shader_program->setMat4("local", building_local);
    
    // Bind building textures
    if (!is_depth_pass) {
        for (int i = 0; i < models.building_textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string texture_string = "textures[" + std::to_string(i) + "]";
            shader_program->setInt(texture_string, i);
            glBindTexture(GL_TEXTURE_2D, models.building_textures[i]);
        }
    }
    models.LouGrossBuilding.Draw();
    if (!is_depth_pass) {
        glActiveTexture(GL_TEXTURE0); // Reset active texture
    }
    // After rendering each textured object:
    if (!is_depth_pass) {
        glActiveTexture(GL_TEXTURE0);
    }
    
    // Draw high bar
    shader_program->setInt("shader_state", IMPORTED_BASIC);
    glm::mat4 high_bar_local(1.0);
    high_bar_local = glm::translate(high_bar_local, glm::vec3(-10.0, 0.0, 0.0));
    high_bar_local = glm::scale(high_bar_local, glm::vec3(1.3f, 1.3f, 1.3f));
    shader_program->setMat4("model", identity);
    shader_program->setMat4("local", high_bar_local);
    models.high_bar.Draw();
    
    // Draw first pommel horse
    shader_program->setInt("shader_state", IMPORTED_BASIC);
    glm::mat4 pommel_horse_local(1.0);
    pommel_horse_local = glm::translate(pommel_horse_local, glm::vec3(-5.0, 0.0, -10.0));
    pommel_horse_local = glm::rotate(pommel_horse_local, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    shader_program->setMat4("model", identity);
    shader_program->setMat4("local", pommel_horse_local);
    models.pommel_horse.Draw();
    
    // Draw second pommel horse
    shader_program->setInt("shader_state", IMPORTED_BASIC);
    glm::mat4 pommel_horse2_local(1.0);
    pommel_horse2_local = glm::translate(pommel_horse2_local, glm::vec3(27.0, 0.0, -10.0));
    pommel_horse2_local = glm::rotate(pommel_horse2_local, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    shader_program->setMat4("model", identity);
    shader_program->setMat4("local", pommel_horse2_local);
    models.pommel_horse2.Draw();
    
    // Draw high bar avatar
    high_bar_avatar->Draw(shader_program, false);
}


void renderHUDBackground(Shader* shader_program, float x1, float y1, float x2, float y2, glm::vec4 color) {
    // Save current OpenGL state
    GLboolean depth_test_enabled;
    glGetBooleanv(GL_DEPTH_TEST, &depth_test_enabled);
    
    // Set up blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Disable depth testing temporarily
    glDisable(GL_DEPTH_TEST);
    
    // Use the shader
    shader_program->use();
    
    // Set up simple orthographic view
    glm::mat4 identity(1.0f);
    shader_program->setMat4("model", identity);
    shader_program->setMat4("view", identity);
    shader_program->setMat4("projection", glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
    
    // Set color - make sure to set both uniforms
    shader_program->setVec4("color", color);
    shader_program->setVec4("set_color", color);
    shader_program->setInt("shader_state", 0); // BASIC = 0, ensure this matches your enum
    
    // Create quad
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Define the rectangle vertices
    float vertices[] = {
        x1, y1, 0.0f,  // Bottom left
        x2, y1, 0.0f,  // Bottom right
        x2, y2, 0.0f,  // Top right
        x1, y2, 0.0f   // Top left
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Draw
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    // Clean up
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    // Restore previous state
    if (depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    }
}

void renderText(Shader* font_program, Font& arial_font, const Camera& camera) {
    // If HUD is toggled off, don't render any text
    if (!hud_visible) {
        return;
    }

    // Use more visible colors for the backgrounds
    glm::vec4 bgColor = glm::vec4(0.1f, 0.1f, 0.3f, 0.7f); // More blue, more visible
    glm::vec4 accentColor = glm::vec4(0.2f, 0.2f, 0.4f, 0.7f); // Slightly different for contrast
    
    // Background for camera position info (positioned precisely)
    renderHUDBackground(font_program, -0.15f, 0.70f, 0.4f, 0.85f, bgColor);
    
    // Background for light direction info
    renderHUDBackground(font_program, -0.15f, 0.60f, 0.4f, 0.70f, accentColor);
    
    // Background for HUD toggle hint
    renderHUDBackground(font_program, -0.99f, -0.99f, -0.65f, -0.90f, bgColor);
    
    // Now draw the text
    font_program->use();
    
    // Camera position
    std::string display_string = "Camera (";
    std::string cam_x = std::to_string(camera.Position.x);
    std::string cam_y = std::to_string(camera.Position.y);
    std::string cam_z = std::to_string(camera.Position.z);

    display_string += cam_x.substr(0, cam_x.find(".") + 3) + ",";
    display_string += cam_y.substr(0, cam_y.find(".") + 3) + ",";
    display_string += cam_z.substr(0, cam_z.find(".") + 3) + ")";
    
    arial_font.DrawText(display_string, glm::vec2(-0.1, 0.75), *font_program);

    // Light direction text
    std::string light_string = "Light Dir (";
    std::string light_x = std::to_string(light_direction.x);
    std::string light_y = std::to_string(light_direction.y);
    std::string light_z = std::to_string(light_direction.z);

    light_string += light_x.substr(0, light_x.find(".") + 3) + ",";
    light_string += light_y.substr(0, light_y.find(".") + 3) + ",";
    light_string += light_z.substr(0, light_z.find(".") + 3) + ")";
    
    arial_font.DrawText(light_string, glm::vec2(-0.1, 0.65), *font_program);

    // Add a hint about toggling the HUD
    arial_font.DrawText("Press 'H' to toggle HUD", glm::vec2(-0.95, -0.95), *font_program);
}

void renderSkybox(Shader* skybox_shader, GameModels& models, const Camera& camera, unsigned int scr_width, unsigned int scr_height) {
    if (skybox_shader == nullptr) {
        std::cerr << "Skybox shader is null!" << std::endl;
        return;
    }

    // Save the current depth function
    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);
    
    // Change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc(GL_LEQUAL);
    
    skybox_shader->use();
    
    // Create a view matrix that removes the translation (only rotation)
    // This ensures the skybox is always centered at the camera position
    Camera camera_copy = camera;
    glm::mat4 view = glm::mat4(glm::mat3(camera_copy.GetViewMatrix())); // Remove translation
    skybox_shader->setMat4("view", view);
    
    // Set projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
    skybox_shader->setMat4("projection", projection);
    
    // Bind the cubemap texture
    skybox_shader->setInt("skybox", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, models.skybox_texture);
    
    // Render the cube
    models.skybox.Draw();
    
    // Reset to the original depth function
    glDepthFunc(currentDepthFunc);
}

void cleanupResources(RenderingVAOs& vaos, GameModels& models) {
    // Cleanup VAOs
    glDeleteVertexArrays(1, &(vaos.basic_vao.id));
    glDeleteVertexArrays(1, &(vaos.import_vao.id));
    glDeleteVertexArrays(1, &(vaos.texture_vao.id));
    glDeleteVertexArrays(1, &(vaos.skybox_vao.id));
    
    // Cleanup models
    models.baseModel.DeallocateShape();
    models.LouGrossBuilding.DeallocateShape();
    models.tumbling_floor.DeallocateShape();
    models.vault_table.DeallocateShape();
    models.high_bar.DeallocateShape();
    models.pommel_horse.DeallocateShape();
    models.pommel_horse2.DeallocateShape();
    models.floor.DeallocateShape();
    models.skybox.DeallocateShape();
    
    std::cout << "All resources cleaned up successfully" << std::endl;
}
#include "rendering.hpp"
#include "build_shapes.hpp"
#include "input_handling.hpp" // Added to access spotlight_on variable
#include <iostream>
#include "../classes/object_types.hpp"

ShadowMap* shadowMap = nullptr;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

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

void setupLighting(Shader* shader_program, const glm::vec3& point_light_color, 
                  const glm::vec4& light_position, const Camera& camera) {
    // Point light setup
    shader_program->setVec4("point_light.ambient", glm::vec4(0.5f * point_light_color, 1.0));
    shader_program->setVec4("point_light.diffuse", glm::vec4(point_light_color, 1.0f));
    shader_program->setVec4("point_light.specular", glm::vec4(0.5f * point_light_color, 1.0f));
    shader_program->setVec4("point_light.position", light_position);
    shader_program->setBool("point_light.on", true);

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
                Shader* shadow_shader,
                GameModels& models, 
                Avatar& baseAvatar,
                AvatarHighBar* high_bar_avatar,
                const Camera& camera,
                const glm::vec3& point_light_color,
                const glm::vec4& light_position) {
    
    // 1. First render pass: render to depth map
    shadowMap->updateLightSpaceMatrix(light_position, glm::vec3(0.0f));
    
    shadow_shader->use();
    shadow_shader->setMat4("lightSpaceMatrix", shadowMap->lightSpaceMatrix);
    
    glViewport(0, 0, shadowMap->SHADOW_WIDTH, shadowMap->SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // In ShadowMap constructor after framebuffer setup
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete: " << status << std::endl;
    }
    
    // Enable front face culling for shadow mapping (optional - helps with peter panning)
    glCullFace(GL_FRONT);
    
    // Render scene geometry to the depth map
    renderSceneForShadowMap(shadow_shader, models, baseAvatar, high_bar_avatar);
    
    // Reset face culling to back faces
    glCullFace(GL_BACK);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // 2. Second render pass: regular rendering with shadow mapping
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Reset viewport
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader_program->use();
    
    // Update camera and lighting as before
    // Create a non-const copy of the camera to call GetViewMatrix()
    Camera camera_copy = camera;
    glm::mat4 view = camera_copy.GetViewMatrix();
    shader_program->setMat4("view", view);
    shader_program->setVec4("view_position", glm::vec4(camera.Position, 1.0));
    
    // Update spotlight and point light parameters as before
    shader_program->setVec4("spot_light.position", glm::vec4(camera.Position, 1.0f));
    shader_program->setVec4("spot_light.direction", glm::vec4(camera.Front, 0.0f));
    shader_program->setBool("spot_light.on", spotlight_on);
    
    shader_program->setVec4("point_light.ambient", glm::vec4(0.5f * point_light_color, 1.0));
    shader_program->setVec4("point_light.diffuse", glm::vec4(point_light_color, 1.0f));
    shader_program->setVec4("point_light.specular", glm::vec4(0.5f * point_light_color, 1.0f));
    shader_program->setVec4("point_light.position", light_position);
    shader_program->setBool("point_light.on", point_light_on);
    
    // Add shadow map and light space matrix
    shader_program->setMat4("lightSpaceMatrix", shadowMap->lightSpaceMatrix);
    glActiveTexture(GL_TEXTURE1); // Use a different texture unit than your diffuse textures
    glBindTexture(GL_TEXTURE_2D, shadowMap->depthMap);
    shader_program->setInt("shadowMap", 1);
    
    // Draw scene as before
    baseAvatar.Draw(shader_program, false);
    
    // Continue with your existing rendering code
    glm::mat4 identity(1.0);
    glm::mat4 model = glm::rotate(identity, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    
    shader_program->setInt("shader_state", TEXTURED);
    shader_program->setMat4("model", model);
    
    glm::mat4 floor_local(1.0);
    floor_local = glm::translate(floor_local, glm::vec3(0.0, 0.0, -0.01));
    shader_program->setMat4("local", floor_local);
    glBindTexture(GL_TEXTURE_2D, models.floor_texture);
    models.floor.Draw();
    
    // Draw tumbling floor
    shader_program->setInt("shader_state", IMPORTED_TEXTURED);
    glm::mat4 tumbling_floor_local(1.0);
    tumbling_floor_local = glm::translate(tumbling_floor_local, glm::vec3(0.0, 0.4, 0.0));
    shader_program->setMat4("model", identity);
    shader_program->setMat4("local", tumbling_floor_local);
    glBindTexture(GL_TEXTURE_2D, models.tumbling_floor_texture);
    models.tumbling_floor.Draw();
    
    // Draw vault table
    shader_program->setInt("shader_state", IMPORTED_TEXTURED);
    glm::mat4 vault_table_local(1.0);
    vault_table_local = glm::translate(vault_table_local, glm::vec3(8.0, 0.0, 0.0));
    shader_program->setMat4("model", identity);
    shader_program->setMat4("local", vault_table_local);
    
    // Bind and set all textures for the VaultTable
    for (int i = 0; i < models.vault_table_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string texture_string = "textures[" + std::to_string(i) + "]";
        shader_program->setInt(texture_string, i);
        glBindTexture(GL_TEXTURE_2D, models.vault_table_textures[i]);
    }
    models.vault_table.Draw();
    glActiveTexture(GL_TEXTURE0); // Reset active texture
    
    // Draw Lou Gross Building
    shader_program->setInt("shader_state", IMPORTED_TEXTURED);
    glm::mat4 building_local(1.0);
    building_local = glm::translate(building_local, glm::vec3(0.0, 0.0, 0.0));
    building_local = glm::scale(building_local, glm::vec3(2, 2, 2));
    shader_program->setMat4("model", identity);
    shader_program->setMat4("local", building_local);
    
    // Bind building textures
    for (int i = 0; i < models.building_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string texture_string = "textures[" + std::to_string(i) + "]";
        shader_program->setInt(texture_string, i);
        glBindTexture(GL_TEXTURE_2D, models.building_textures[i]);
    }
    models.LouGrossBuilding.Draw();
    glActiveTexture(GL_TEXTURE0); // Reset active texture
    
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
    pommel_horse2_local = glm::translate(pommel_horse2_local, glm::vec3(5.0, 0.0, -10.0));
    pommel_horse2_local = glm::rotate(pommel_horse2_local, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    shader_program->setMat4("model", identity);
    shader_program->setMat4("local", pommel_horse2_local);
    models.pommel_horse2.Draw();
    
    // Draw high bar avatar
    high_bar_avatar->Draw(shader_program, false);
}

void renderSceneForShadowMap(Shader* shadow_shader, 
    GameModels& models, 
    Avatar& baseAvatar,
    AvatarHighBar* high_bar_avatar) {
    // Identity matrix for model
    glm::mat4 identity(1.0);
    shadow_shader->setMat4("model", identity);

    // Render all objects that should cast shadows
    // Avatars
    baseAvatar.Draw(shadow_shader, false);
    high_bar_avatar->Draw(shadow_shader, false);

    // Models
    glm::mat4 local(1.0);

    // High bar
    shadow_shader->setMat4("local", local);
    models.high_bar.Draw();

    // Floor
    glm::mat4 floor_local(1.0);
    floor_local = glm::translate(floor_local, glm::vec3(0.0, 0.0, -0.01));
    shadow_shader->setMat4("local", floor_local);
    models.floor.Draw();

    // Tumbling floor
    glm::mat4 tumbling_floor_local(1.0);
    tumbling_floor_local = glm::translate(tumbling_floor_local, glm::vec3(0.0, 0.4, 0.0));
    shadow_shader->setMat4("local", tumbling_floor_local);
    models.tumbling_floor.Draw();

    // Vault table
    glm::mat4 vault_table_local(1.0);
    vault_table_local = glm::translate(vault_table_local, glm::vec3(8.0, 0.0, 0.0));
    shadow_shader->setMat4("local", vault_table_local);
    models.vault_table.Draw();

    // Lou Gross Building
    glm::mat4 building_local(1.0);
    building_local = glm::translate(building_local, glm::vec3(0.0, 0.0, 0.0));
    building_local = glm::scale(building_local, glm::vec3(2, 2, 2));
    shadow_shader->setMat4("local", building_local);
    models.LouGrossBuilding.Draw();

    // First pommel horse
    glm::mat4 pommel_horse_local(1.0);
    pommel_horse_local = glm::translate(pommel_horse_local, glm::vec3(-5.0, 0.0, -10.0));
    pommel_horse_local = glm::rotate(pommel_horse_local, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    shadow_shader->setMat4("local", pommel_horse_local);
    models.pommel_horse.Draw();

    // Second pommel horse
    glm::mat4 pommel_horse2_local(1.0);
    pommel_horse2_local = glm::translate(pommel_horse2_local, glm::vec3(5.0, 0.0, -10.0));
    pommel_horse2_local = glm::rotate(pommel_horse2_local, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    shadow_shader->setMat4("local", pommel_horse2_local);
    models.pommel_horse2.Draw();
}

void initializeShadowMap() {
    std::cout << "Initializing shadow map..." << std::endl;
    shadowMap = new ShadowMap();
    std::cout << "Shadow map initialized successfully" << std::endl;
}

void cleanupShadowMap() {
    if (shadowMap) {
        delete shadowMap;
        shadowMap = nullptr;
    }
}

void renderText(Shader* font_program, Font& arial_font, const Camera& camera) {
    font_program->use();
    
    std::string display_string = "Camera (";
    std::string cam_x = std::to_string(camera.Position.x);
    std::string cam_y = std::to_string(camera.Position.y);
    std::string cam_z = std::to_string(camera.Position.z);

    display_string += cam_x.substr(0, cam_x.find(".") + 3) + ",";
    display_string += cam_y.substr(0, cam_y.find(".") + 3) + ",";
    display_string += cam_z.substr(0, cam_z.find(".") + 3) + ")";
    
    arial_font.DrawText(display_string, glm::vec2(-0.1, 0.75), *font_program);
}

void cleanupResources(RenderingVAOs& vaos, GameModels& models) {
    // Cleanup VAOs
    glDeleteVertexArrays(1, &(vaos.basic_vao.id));
    glDeleteVertexArrays(1, &(vaos.import_vao.id));
    glDeleteVertexArrays(1, &(vaos.texture_vao.id));
    
    // Cleanup models
    models.baseModel.DeallocateShape();
    models.LouGrossBuilding.DeallocateShape();
    models.tumbling_floor.DeallocateShape();
    models.vault_table.DeallocateShape();
    models.high_bar.DeallocateShape();
    models.pommel_horse.DeallocateShape();
    models.pommel_horse2.DeallocateShape();
    models.floor.DeallocateShape();
    
    std::cout << "All resources cleaned up successfully" << std::endl;
}
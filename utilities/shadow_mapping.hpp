// utilities/shadow_mapping.hpp
#ifndef SHADOW_MAPPING_HPP
#define SHADOW_MAPPING_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShadowMap {
public:
    unsigned int depthMapFBO;
    unsigned int depthMap;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    glm::mat4 lightSpaceMatrix;

    ShadowMap() {
        // Create the FBO for the shadow map
        glGenFramebuffers(1, &depthMapFBO);
        
        // Create the depth texture
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
        
        // Attach the depth texture to the FBO
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void updateLightSpaceMatrix(const glm::vec4& lightPos, const glm::vec3& targetPos) {
        // For a directional light
        float near_plane = 1.0f, far_plane = 30.0f;
        float orthoSize = 15.0f;
        glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, near_plane, far_plane);
        
        // Use the light position as the view position
        glm::vec3 lightPosition = glm::vec3(lightPos);
        glm::mat4 lightView = glm::lookAt(
            lightPosition, 
            targetPos,  // Look at the center of the scene
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        
        // Calculate light space matrix
        lightSpaceMatrix = lightProjection * lightView;
    }
};

#endif // SHADOW_MAPPING_HPP
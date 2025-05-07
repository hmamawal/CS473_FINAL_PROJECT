#version 330 core
//Need to send the fragment color to the next step in the pipeline
out vec4 FragColor;

//bring in the texture image coordinates (if you have them)
in vec2 texture_coordinates;

//bring in the normal and the fragment position (world)
in vec3 norm;
in vec3 fragment_position;

//bring in the imported information
in vec3 ambient_color;
in vec3 diffuse_color;
in vec3 specular_color;
flat in float opacity;

//bring in the index for the texture
flat in int index_for_texture;

//bring in the state of the fragment shader
flat in int fragment_shader_state;

uniform vec4 set_color;

//if it -is- textured, assume this 2D texture is mapped.
#define NR_TEXTURES 19
uniform sampler2D textures[NR_TEXTURES];

uniform vec4 view_position;

struct DirectionalLight {
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    //add coefficients for attenuation here
    bool on;
};

uniform DirectionalLight directional_light;

struct SpotLight {
    vec4 position;
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float cutOff;      // Inner cutoff (cosine of angle)
    float outerCutOff; // Outer cutoff (cosine of angle)
    float constant;    // Attenuation constant term
    float linear;      // Attenuation linear term
    float quadratic;   // Attenuation quadratic term
    bool on;
};

uniform SpotLight spot_light;

in vec4 FragPosLightSpace;
uniform sampler2D shadow_map;

// Add post-processing uniform
uniform int post_process_selection;

uniform vec4 color;

vec4 CalcSpotLight(SpotLight light, vec3 norm, vec3 frag, vec3 eye);

vec4 CalcDirectionalLight (DirectionalLight light,vec3 norm,vec3 frag,vec3 eye);

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth value from light's perspective
    float closestDepth = texture(shadow_map, projCoords.xy).r; 
    
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // Calculate bias based on the angle between surface normal and light direction
    // The steeper the angle, the larger the bias needs to be
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    
    // Check whether current fragment is in shadow
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // Keep shadows within the light's far plane bounds
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

// Post-processing functions
vec4 inverseColor(vec4 color) {
    return vec4(1.0 - color.rgb, color.a);
}

vec4 grayscale(vec4 color) {
    float average = (color.r + color.g + color.b) / 3.0;
    return vec4(average, average, average, color.a);
}

vec4 redChannel(vec4 color) {
    return vec4(color.r, 0.0, 0.0, color.a);
}

vec4 greenChannel(vec4 color) {
    return vec4(0.0, color.g, 0.0, color.a);
}

vec4 blueChannel(vec4 color) {
    return vec4(0.0, 0.0, color.b, color.a);
}

vec4 sepiaEffect(vec4 color) {
    vec3 sepia;
    sepia.r = color.r * 0.393 + color.g * 0.769 + color.b * 0.189;
    sepia.g = color.r * 0.349 + color.g * 0.686 + color.b * 0.168;
    sepia.b = color.r * 0.272 + color.g * 0.534 + color.b * 0.131;
    return vec4(sepia, color.a);
}

// Kernel effects
vec4 blurEffect(vec4 color) {
    // Simple 3x3 box blur kernel
    float offset = 1.0 / 300.0;
    
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1.0/9.0, 1.0/9.0, 1.0/9.0,
        1.0/9.0, 1.0/9.0, 1.0/9.0,
        1.0/9.0, 1.0/9.0, 1.0/9.0
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        if (fragment_shader_state == 1 || fragment_shader_state == 3) {
            int tex_idx = index_for_texture;
            if(tex_idx == 0) sampleTex[i] = vec3(texture(textures[0], texture_coordinates + offsets[i]));
            else if(tex_idx == 1) sampleTex[i] = vec3(texture(textures[1], texture_coordinates + offsets[i]));
            else if(tex_idx == 2) sampleTex[i] = vec3(texture(textures[2], texture_coordinates + offsets[i]));
            // can add more texture indices as needed
            else sampleTex[i] = color.rgb; // Fallback
        } else {
            sampleTex[i] = color.rgb; // Use the original color for non-textured objects
        }
    }
    
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    return vec4(col, color.a);
}

vec4 sharpenEffect(vec4 color) {
    // Sharpen kernel
    float offset = 1.0 / 300.0;
    
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        if (fragment_shader_state == 1 || fragment_shader_state == 3) {
            int tex_idx = index_for_texture;
            if(tex_idx == 0) sampleTex[i] = vec3(texture(textures[0], texture_coordinates + offsets[i]));
            else if(tex_idx == 1) sampleTex[i] = vec3(texture(textures[1], texture_coordinates + offsets[i]));
            else if(tex_idx == 2) sampleTex[i] = vec3(texture(textures[2], texture_coordinates + offsets[i]));
            // can add more texture indices as needed
            else sampleTex[i] = color.rgb; // Fallback
        } else {
            sampleTex[i] = color.rgb; // Use the original color for non-textured objects
        }
    }
    
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    return vec4(col, color.a);
}

vec4 edgeDetectionEffect(vec4 color) {
    // Edge detection kernel
    float offset = 1.0 / 300.0;
    
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1,  1, 1,
        1, -8, 1,
        1,  1, 1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        if (fragment_shader_state == 1 || fragment_shader_state == 3) {
            int tex_idx = index_for_texture;
            if(tex_idx == 0) sampleTex[i] = vec3(texture(textures[0], texture_coordinates + offsets[i]));
            else if(tex_idx == 1) sampleTex[i] = vec3(texture(textures[1], texture_coordinates + offsets[i]));
            else if(tex_idx == 2) sampleTex[i] = vec3(texture(textures[2], texture_coordinates + offsets[i]));
            // can add more texture indices as needed
            else sampleTex[i] = color.rgb; // Fallback
        } else {
            sampleTex[i] = color.rgb; // Use the original color for non-textured objects
        }
    }
    
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    return vec4(col, color.a);
}

// Apply post-processing based on selection
vec4 applyPostProcessing(vec4 originalColor) {
    if (post_process_selection == 1) {
        return inverseColor(originalColor);
    } else if (post_process_selection == 2) {
        return grayscale(originalColor);
    } else if (post_process_selection == 3) {
        return redChannel(originalColor);
    } else if (post_process_selection == 4) {
        return greenChannel(originalColor);
    } else if (post_process_selection == 5) {
        return blueChannel(originalColor);
    } else if (post_process_selection == 6) {
        return sepiaEffect(originalColor);
    } else if (post_process_selection == 7) {
        return blurEffect(originalColor);
    } else if (post_process_selection == 8) {
        return sharpenEffect(originalColor);
    } else if (post_process_selection == 9) {
        return edgeDetectionEffect(originalColor);
    }
    
    // Default: no effect
    return originalColor;
}

//  0: BasicShape objects that just have a set color (basic)
//  1: BasicShape objects that have a texture
//  2: Imported BasicShape objects that use materials from Blender
//  3: Imported BasicShape objects that use materials and/or textures

void main()
{
    vec4 directional_light_color = CalcDirectionalLight(directional_light, norm,
                                            fragment_position, view_position.xyz);
    //Material colors are integrated into the directional light calculation
    if (fragment_shader_state == 2) {
        FragColor = directional_light_color;
        return;
    }

     // Calculate spotlight contribution
    vec4 spot_light_color = vec4(0.0);
    if (spot_light.on) {
        spot_light_color = CalcSpotLight(spot_light, norm, 
                                       fragment_position, view_position.xyz);
    }
    
    
    // Combine light contributions
    vec4 combined_light = directional_light_color + spot_light_color;

    //Object is textured
    if ((fragment_shader_state == 1) || (fragment_shader_state == 3)) {
        // can't index an array with a variable in GLSL
        //  so we have to use a set of if statements.
        if (index_for_texture == 0) {
            FragColor = texture(textures[0],texture_coordinates);
        } else if (index_for_texture == 1) {
            FragColor = texture(textures[1],texture_coordinates);
        } else if (index_for_texture == 2) {
            FragColor = texture(textures[2],texture_coordinates);
        } else if (index_for_texture == 3) {
            FragColor = texture(textures[3],texture_coordinates);
        } else if (index_for_texture == 4) {
            FragColor = texture(textures[4],texture_coordinates);
        } else if (index_for_texture == 5) {
            FragColor = texture(textures[5],texture_coordinates);
        } else if (index_for_texture == 6) {
            FragColor = texture(textures[6],texture_coordinates);
        } else if (index_for_texture == 7) {
            FragColor = texture(textures[7],texture_coordinates);
        } else if (index_for_texture == 8) {
            FragColor = texture(textures[8],texture_coordinates);
        } else if (index_for_texture == 9) {
            FragColor = texture(textures[9],texture_coordinates);
        } else if (index_for_texture == 10) {
            FragColor = texture(textures[10],texture_coordinates);
        } else if (index_for_texture == 11) {
            FragColor = texture(textures[11],texture_coordinates);
        } else if (index_for_texture == 12) {
            FragColor = texture(textures[12],texture_coordinates);
        } else if (index_for_texture == 13) {
            FragColor = texture(textures[13],texture_coordinates);
        } else if (index_for_texture == 14) {
            FragColor = texture(textures[14],texture_coordinates);
        } else if (index_for_texture == 15) {
            FragColor = texture(textures[15],texture_coordinates);
        } else if (index_for_texture == 16) {
            FragColor = texture(textures[16],texture_coordinates);
        } else if (index_for_texture == 17) {
            FragColor = texture(textures[17],texture_coordinates);
        } else if (index_for_texture == 18) {
            FragColor = texture(textures[18],texture_coordinates);
        }
        if (index_for_texture == 99) {
            FragColor = combined_light;
            return;
        }
        FragColor = combined_light * FragColor;
        FragColor = applyPostProcessing(FragColor);
        return;

    }

    //otherwise fragment state is 0 (use the set color)
    if (fragment_shader_state == 0) { // BASIC
        FragColor = color;  // Use the color directly for basic rendering
    } else {
        FragColor = combined_light * set_color;
    }
    FragColor = applyPostProcessing(FragColor);
    
};

vec4 CalcDirectionalLight (DirectionalLight light,vec3 norm,vec3 frag,vec3 eye) {
    if (!light.on) {
        return vec4(0.0,0.0,0.0,1.0);
    }
    
    // Calculate shadow
    float shadow = ShadowCalculation(FragPosLightSpace, norm, normalize(-light.direction.xyz));
    
    // Rest of directional light calculation 
    vec3 light_direction = normalize(-light.direction.xyz);
    vec3 normal = normalize(norm);
    float diffuse_coeff = max(dot(normal,light_direction),0.0);
    
    vec3 view_direction = normalize(view_position.xyz-frag);
    vec3 reflect_direction = reflect(-light_direction,normal);
    float spec_coeff = pow(max(dot(view_direction,reflect_direction),0.0),256.0);
    
    // Apply shadow - multiply diffuse and specular components (not ambient)
    if ((fragment_shader_state == 2) || (fragment_shader_state == 3)) {
        return (light.ambient * vec4(ambient_color,opacity) 
                + (1.0 - shadow) * diffuse_coeff * light.diffuse * vec4(diffuse_color,opacity)
                + (1.0 - shadow) * spec_coeff * light.specular * vec4(specular_color,opacity));
    } else {
        return (light.ambient 
            + (1.0 - shadow) * diffuse_coeff * light.diffuse 
            + (1.0 - shadow) * spec_coeff * light.specular);
    }
}

vec4 CalcSpotLight(SpotLight light, vec3 norm, vec3 frag, vec3 eye) {
    if (!light.on) {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }

    vec3 light_direction = normalize(light.position.xyz - frag);
    vec3 normal = normalize(norm);

    // Calculate distance and attenuation
    float distance = length(light.position.xyz - frag);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Calculate spotlight effect
    float theta = dot(light_direction, normalize(-light.direction.xyz));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // If outside the outer cone, return only ambient or nothing
    if (intensity <= 0.0) {
        return vec4(0.0, 0.0, 0.0, 1.0); // Or minimal ambient light
    }

    // Calculate diffuse lighting
    float diffuse_coeff = max(dot(normal, light_direction), 0.0);

    // Calculate specular lighting
    vec3 view_direction = normalize(eye - frag);
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec_coeff = pow(max(dot(view_direction, reflect_direction), 0.0), 256.0);

    // Apply intensity and attenuation to diffuse and specular (not ambient)
    vec4 ambient, diffuse, specular;

    // Handle materials
    if ((fragment_shader_state == 2) || (fragment_shader_state == 3)) {
        ambient = light.ambient * vec4(ambient_color, opacity);
        diffuse = attenuation * intensity * diffuse_coeff * light.diffuse * vec4(diffuse_color, opacity);
        specular = attenuation * intensity * spec_coeff * light.specular * vec4(specular_color, opacity);
    } else {
        ambient = light.ambient;
        diffuse = attenuation * intensity * diffuse_coeff * light.diffuse;
        specular = attenuation * intensity * spec_coeff * light.specular;
    }

    return (ambient + diffuse + specular);
}
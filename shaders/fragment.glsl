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

struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    //add coefficients for attenuation here
    bool on;
};

uniform PointLight point_light;

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

vec4 CalcSpotLight(SpotLight light, vec3 norm, vec3 frag, vec3 eye);

vec4 CalcPointLight (PointLight light,vec3 norm,vec3 frag,vec3 eye);

uniform sampler2D shadowMap;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace; 
} fs_in;

// MOVED HERE: Define ShadowCalculation function before it's used
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth value from light's perspective
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    
    // Get current depth of fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // Apply bias to avoid shadow acne
    float bias = 0.005;
    
    // Check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    // Keep fragments outside light frustum lit
    if(projCoords.z > 1.0)
        shadow = 0.0;
    
    return shadow;
}

//  0: BasicShape objects that just have a set color (basic)
//  1: BasicShape objects that have a texture
//  2: Imported BasicShape objects that use materials from Blender
//  3: Imported BasicShape objects that use materials and/or textures

void main()
{
    vec4 point_light_color = CalcPointLight(point_light, norm,
                                            fragment_position, view_position.xyz);
    // Material colors are integrated into the point light calculation
    if (fragment_shader_state == 2) {
        FragColor = point_light_color;
        return;
    }

    // Calculate spotlight contribution
    vec4 spot_light_color = vec4(0.0);
    if (spot_light.on) {
        spot_light_color = CalcSpotLight(spot_light, norm, 
                                         fragment_position, view_position.xyz);
    }
    
    // Combine light contributions
    vec4 combined_light = point_light_color + spot_light_color;

    // Calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    // Apply shadow to lighting
    combined_light.rgb *= (1.0 - shadow);

    // Object is textured
    if ((fragment_shader_state == 1) || (fragment_shader_state == 3)) {
        // Unfortunately you can't index an array with a variable in GLSL
        // so you have to use a set of if statements.
        if (index_for_texture == 0) {
            FragColor = texture(textures[0], texture_coordinates);
        } else if (index_for_texture == 1) {
            FragColor = texture(textures[1], texture_coordinates);
        } else if (index_for_texture == 2) {
            FragColor = texture(textures[2], texture_coordinates);
        } else if (index_for_texture == 3) {
            FragColor = texture(textures[3], texture_coordinates);
        } else if (index_for_texture == 4) {
            FragColor = texture(textures[4], texture_coordinates);
        } else if (index_for_texture == 5) {
            FragColor = texture(textures[5], texture_coordinates);
        } else if (index_for_texture == 6) {
            FragColor = texture(textures[6], texture_coordinates);
        } else if (index_for_texture == 7) {
            FragColor = texture(textures[7], texture_coordinates);
        } else if (index_for_texture == 8) {
            FragColor = texture(textures[8], texture_coordinates);
        } else if (index_for_texture == 9) {
            FragColor = texture(textures[9], texture_coordinates);
        } else if (index_for_texture == 10) {
            FragColor = texture(textures[10], texture_coordinates);
        } else if (index_for_texture == 11) {
            FragColor = texture(textures[11], texture_coordinates);
        } else if (index_for_texture == 12) {
            FragColor = texture(textures[12], texture_coordinates);
        } else if (index_for_texture == 13) {
            FragColor = texture(textures[13], texture_coordinates);
        } else if (index_for_texture == 14) {
            FragColor = texture(textures[14], texture_coordinates);
        } else if (index_for_texture == 15) {
            FragColor = texture(textures[15], texture_coordinates);
        } else if (index_for_texture == 16) {
            FragColor = texture(textures[16], texture_coordinates);
        } else if (index_for_texture == 17) {
            FragColor = texture(textures[17], texture_coordinates);
        } else if (index_for_texture == 18) {
            FragColor = texture(textures[18], texture_coordinates);
        }
        if (index_for_texture == 99) {
            FragColor = combined_light;
            return;
        }
        FragColor = combined_light * FragColor;
        return;
    }

    // Otherwise fragment state is 0 (use the set color)
    FragColor = combined_light * set_color;
}

vec4 CalcPointLight (PointLight light,vec3 norm,vec3 frag,vec3 eye) {

    if (!light.on) {
        return vec4(0.0,0.0,0.0,1.0);
    }
    vec3 light_direction = light.position.xyz - frag.xyz;
    light_direction = normalize(light_direction);
    vec3 normal = normalize(norm);
    float diffuse_coeff = max(dot(normal,light_direction),0.0);

    vec3 view_direction = normalize(view_position.xyz-frag);
    vec3 reflect_direction = reflect(-light_direction,normal);
    float spec_coeff = pow(max(dot(view_direction,reflect_direction),0.0),256.0);

    //handle materials
    if ((fragment_shader_state == 2) || (fragment_shader_state == 3)) {
        //need to use the different material components in the calculation
        return (light.ambient * vec4(ambient_color,opacity) 
                + diffuse_coeff * light.diffuse * vec4(diffuse_color,opacity)
                + spec_coeff * light.specular * vec4(specular_color,opacity));
    } else {
        return (light.ambient 
            + diffuse_coeff * light.diffuse 
            + spec_coeff * light.specular);
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
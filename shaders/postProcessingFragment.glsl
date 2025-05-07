#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int effect;

// Kernel for box blur
const float blurKernel[9] = float[](
    1.0/9.0, 1.0/9.0, 1.0/9.0,
    1.0/9.0, 1.0/9.0, 1.0/9.0,
    1.0/9.0, 1.0/9.0, 1.0/9.0
);

// Apply a kernel effect to the texture
vec3 applyKernel(float[9] kernelMatrix) {
    const float offset = 1.0 / 300.0;
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

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernelMatrix[i];
    
    return col;
}

void main()
{
    vec3 texColor = texture(screenTexture, TexCoords).rgb;
    vec3 result;
    
    // Different post-processing effects based on the uniform
    switch(effect) {
        case 1: // Grayscale
            // Use the weighted approach for accurate grayscale conversion
            float average = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;
            result = vec3(average);
            break;
            
        case 2: // Box blur
            result = applyKernel(blurKernel);
            break;
            
        case 3: // Lighten effect
            // Increase brightness by 30%
            result = texColor * 0.5;
            // Ensure we don't exceed 1.0
            result = min(result, vec3(1.0));
            break;
            
        default: // No effect
            result = texColor;
    }
    
    FragColor = vec4(result, 1.0);
}
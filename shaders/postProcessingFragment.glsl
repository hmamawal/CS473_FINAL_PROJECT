#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int effect;

// Kernel for edge detection
const float kernel[9] = float[](
    1.0,  1.0, 1.0,
    1.0, -8.0, 1.0,
    1.0,  1.0, 1.0
);

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
        case 1: // Invert colors
            result = 1.0 - texColor;
            break;
            
        case 2: // Grayscale
            float average = (texColor.r + texColor.g + texColor.b) / 3.0;
            result = vec3(average);
            break;
            
        case 3: // Edge detection
            result = applyKernel(kernel);
            break;
            
        case 4: // Box blur
            result = applyKernel(blurKernel);
            break;
            
        case 5: // Sharpen
            // High-pass filter (sharpen)
            const float sharpenKernel[9] = float[](
                -1, -1, -1,
                -1,  9, -1,
                -1, -1, -1
            );
            result = applyKernel(sharpenKernel);
            break;
            
        case 6: // Sepia tone
            result.r = texColor.r * 0.393 + texColor.g * 0.769 + texColor.b * 0.189;
            result.g = texColor.r * 0.349 + texColor.g * 0.686 + texColor.b * 0.168;
            result.b = texColor.r * 0.272 + texColor.g * 0.534 + texColor.b * 0.131;
            break;
            
        default: // No effect
            result = texColor;
    }
    
    FragColor = vec4(result, 1.0);
}
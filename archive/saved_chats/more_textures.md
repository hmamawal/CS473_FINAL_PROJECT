hmamawal: #codebase i was importing a model that had multiple textures (17) but not all of them were loading because in my fragment.glsl file, i was only handling up to 5 textures. i added a bunch more esle if cases for up to 18, but it broke my program to where it is 2d now. is there anything else i have to change for the fragment or elsewhere that will correspond with the changes i already made in fragment.glsl?

GitHub Copilot: It seems like the issue might be related to the `#define NR_TEXTURES` directive in your fragment.glsl file. You currently have:

```glsl
#define NR_TEXTURES 6
```

This defines the maximum number of textures your shader can handle. Since you added support for up to 18 textures, you need to update this value to match the number of textures you are handling. Change it to:

```glsl
#define NR_TEXTURES 19
```

This ensures that the textures array in your shader can accommodate all the textures you are trying to use.

Additionally, ensure that the corresponding OpenGL code in your application is updated to bind all 19 textures properly. For example, in your main.cpp or wherever you are binding textures, ensure that you are binding all textures to the correct texture units (e.g., `GL_TEXTURE0`, `GL_TEXTURE1`, ..., `GL_TEXTURE18`).

### Steps to Verify:
1. **Update `NR_TEXTURES`**: Update the `#define NR_TEXTURES` value in fragment.glsl to match the number of textures you are handling.
2. **Check Texture Binding**: Ensure that all 19 textures are being bound in your OpenGL code. For example:
   ```cpp
   for (int i = 0; i < 19; i++) {
       glActiveTexture(GL_TEXTURE0 + i);
       glBindTexture(GL_TEXTURE_2D, texture_ids[i]);
   }
   ```
3. **Update Shader Uniforms**: Ensure that the textures uniform array in your shader is being set correctly in your OpenGL code. For example:
   ```cpp
   glUniform1i(glGetUniformLocation(shaderProgram, "textures[0]"), 0);
   glUniform1i(glGetUniformLocation(shaderProgram, "textures[1]"), 1);
   // Repeat for all textures up to 18
   ```

4. **Verify VAO and VBO**: Ensure that your vertex attributes (e.g., texture coordinates) are being passed correctly to the shader. If the model appears 2D, it might be due to incorrect vertex data or transformations.

5. **Debugging**: If the issue persists, use tools like `glGetError()` or a graphics debugger (e.g., RenderDoc) to identify where the problem occurs.

Let me know if you need help with any specific part of this process!
hmamawal: implement these optimizations for shadows: <<<Improving shadow maps
We managed to get the basics of shadow mapping working, but as you can we're not there yet due to several (clearly visible) artifacts related to shadow mapping we need to fix. We'll focus on fixing these artifacts in the next sections.

Shadow acne
It is obvious something is wrong from the previous image. A closer zoom shows us a very obvious Moiré-like pattern:

Image of shadow acne as Moiré pattern with shadow mapping
We can see a large part of the floor quad rendered with obvious black lines in an alternating fashion. This shadow mapping artifact is called shadow acne and can be explained by the following image:

Shadow acne explained
Because the shadow map is limited by resolution, multiple fragments can sample the same value from the depth map when they're relatively far away from the light source. The image shows the floor where each yellow tilted panel represents a single texel of the depth map. As you can see, several fragments sample the same depth sample.

While this is generally okay, it becomes an issue when the light source looks at an angle towards the surface as in that case the depth map is also rendered from an angle. Several fragments then access the same tilted depth texel while some are above and some below the floor; we get a shadow discrepancy. Because of this, some fragments are considered to be in shadow and some are not, giving the striped pattern from the image.

We can solve this issue with a small little hack called a shadow bias where we simply offset the depth of the surface (or the shadow map) by a small bias amount such that the fragments are not incorrectly considered above the surface.

Shadow mapping, with shadow acne fixed using shadow bias.
With the bias applied, all the samples get a depth smaller than the surface's depth and thus the entire surface is correctly lit without any shadows. We can implement such a bias as follows:


float bias = 0.005;
float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
A shadow bias of 0.005 solves the issues of our scene by a large extent, but you can imagine the bias value is highly dependent on the angle between the light source and the surface. If the surface would have a steep angle to the light source, the shadows may still display shadow acne. A more solid approach would be to change the amount of bias based on the surface angle towards the light: something we can solve with the dot product:


float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
Here we have a maximum bias of 0.05 and a minimum of 0.005 based on the surface's normal and light direction. This way, surfaces like the floor that are almost perpendicular to the light source get a small bias, while surfaces like the cube's side-faces get a much larger bias. The following image shows the same scene but now with a shadow bias:

Shadow mapped images, with (sloped) shadow bias applied.
Choosing the correct bias value(s) requires some tweaking as this will be different for each scene, but most of the time it's simply a matter of slowly incrementing the bias until all acne is removed.

>>>

GitHub Copilot: I'll implement the shadow bias optimization to fix the shadow acne artifacts in your shadow mapping system. This will require modifying the shadow calculation function in your fragment shader.

Looking at your code, I can see that you already have a `ShadowCalculation` function in your fragment shader, but it's using a constant bias value. Let's update it to use a dynamic bias that adjusts based on the surface angle relative to the light direction.
````
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
````


Made changes.

I've implemented the dynamic shadow bias optimization to fix the shadow acne artifacts in your shadow mapping system. 

The key changes made to your `ShadowCalculation` function include:

1. Replacing the fixed bias with a dynamic one that adapts based on the angle between the surface normal and the light direction:
   ```glsl
   float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
   ```

2. This approach gives you:
   - A minimum bias of 0.005 for surfaces directly facing the light
   - A maximum bias of 0.05 for surfaces at steep angles to the light

This optimization will substantially reduce the shadow acne (the Moiré-like striped pattern) that was visible in your renders. Surfaces that are nearly perpendicular to the light direction (like floors) will receive just enough bias to prevent artifacts, while surfaces at steeper angles (like walls and vertical object faces) will receive proportionally more bias.

The shadow bias calculation takes into account:
- The surface normal vector
- The light direction vector
- Their dot product (which gives the cosine of the angle between them)

Your shadow rendering should now look much cleaner with these improvements. If you still notice any shadow acne artifacts in certain areas, you can adjust the minimum and maximum bias values (0.005 and 0.05) to find the optimal settings for your specific scene.
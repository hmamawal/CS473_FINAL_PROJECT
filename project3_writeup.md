# Writeup
## Summary
This project is a "gymanstics simulation" that aims to roughly recreate the physical gym that I did gymnastics at for three years here at West Point and add fun "animations" that gymnasts/avatars would do. Within this project, a user can walk around, make an avatar do a backflip or do "giants" around a high bar. They can take on the perspective of the gymnast on the high bar doing the "giants." Further, they can use post-processing effects, use a flash light. 

## Project Description and Process
I wanted to make this project because I thought it would be unique and fun to do. For project 3, I wanted to continue this simulation, implementing additional functionality and features to improve the feel of the simulator. Before I first wanted to decrease the size of the main file, because I always had to scroll so much to find certain points of my code. I separated large sections, like the initialization, the rendering, and the input handling into separate files. This made it easier to navigate the project when I went about implementing everything for project 3. I started this process almost immediately after I presented for project 2 because I was genuinly enjoying working on this project. Low and behold, other class work got in the way and I didn't end up working on this project again for a few weeks. When coming back, I went iteratively through the new project requirements to implement them. I first started by making the simulation more interactive by adding the ability to turn both the spotlight and the point light on and off. I did this by adding a variable in input_handling.cpp to track the state of the light (if 'l' key pressed, turn the light on or off) and change the shader file variable to on/off as well (via a Shader setBool method). Likewise for the point light, I pursued a similar approach. After this, I tried implementing shadows. After 1 failed attempt, I completely reverted the github to the last working commit then started over for shadows. I first started with converting the point light to a directional light, making the necessary changes in main, initialization, rendering, and the fragment.glsl shader (the key change was in the fragment.glsl, replacing the light position–based vector calculation with a constant normalized light direction to reflect the behavior of a directional light source). Next, to improve trouble shooting the shadows, I decided to add the ability to move the directional light with arrow keys, which simply took changes in the input handling files. When doing this, I realized the that the Avatar code still recognized the input from the arrow keys, causing my avatars to move around when i wanted them to stay in place, so i got rid of that functionality. Next, I started tackling the most significant parts of the shadow implementation: creating the rendering pipeline to perform a depth-only shadow pass using a dedicated shader, avoiding unintended color or texture operations. Here using the depth instructions from learnopengl, changes were made in main for the depth mapping using the frame buffer, the light space transformation, adn the 2 passes to render the scene for the depth map and for the screen. New separate fragment and vertex shaders were created. The regular fragment shader was updated to do the ShadowCalculation. For post-processing, two additional fragment and vertex glsl files were created. postProcessingFragment.glsl primarily handles the effects for this (calculations). Essentially, the main loop now has 4 main stages: depth pass (for shadows), scene render, text and HUD buffer, and post-processing (rendering the framebuffer texture onto the screen quad). 

As documented in my workspace chats (see archive/saved_chats/project3/), the implementation of certain features had specific challenges. The shadow implementation was particularly challenging, as evidenced in the "bad_shadows.md" and "dynamic_shadow_bias.md" files, where I had to adjust the shadow bias values to prevent shadow acne. The post-processing effects, as shown in "post-processing.md" and "only_3_post_processing_effects.md", required several iterations to get right. I ultimately implemented grayscale, blur, and lighten effects. The code modularization process, documented in "modularize1.md", "modularize2.md", and "modularize3.md", was helpful for making the project more navigatable. Finally, adding the cubemap for the skybox environment (see "working_cube_map.md") gave the simulation a more immersive feel, though it required careful attention to vertex attribute pointers and texture loading.

Using the instuctions for the cubemap as context, this was implemented fairly earily with the additions of new fragment and vertex shaders cube map additions to build_shapes, initialization, and rendering. Next, I went to implement the heads-up display features. First, I turned off the alpha variable for the red background. Next, I added the light direction to the heads-up display, which I was already calculating and outputting to the terminal. Finally, I added instructions for toggling the HUD on and off with the H key. For the red box backgrounds, I went back and forth with GitHub copilot to implement these. All these heads-up display steps were another iterative process with GitHub copilot to get to the result I wanted.

## After Action Review (AAR)

### What Was Planned
For Project 3, I planned to enhance my gymnastics simulation by adding:
- Interactive lighting controls (toggle lights on/off)
- Shadow mapping
- Post-processing effects
- Cubemap skybox
- Heads-up display with useful information
- Code modularization to improve maintainability

### What Actually Happened
I successfully implemented all planned features, though not without challenges:
- The shadow mapping required multiple attempts, with my first implementation failing completely
- Post-processing effects were initially overwhelming with too many options, so I scaled back to three well-implemented effects
- The cubemap implementation required careful attention to vertex attributes and texture loading
- Code modularization took more time than expected but paid off in the long run
- The spotlight and directional light toggles worked well from the beginning

### What Went Well
- Breaking the code into separate modules made debugging much easier
- Implementing the HUD with toggle functionality improved user experience
- The post-processing effects added visual variety to the simulation
- Adding camera controls for first-person view made the experience more immersive
- The cubemap skybox greatly enhanced the environment visuals

### What Could Have Been Better
- My initial shadow implementation failed because I didn't fully understand the shadow mapping process
- The avatar animations could be more sophisticated and fluid
- I should have addressed the camera issues when toggling to first-person view
- Better planning for the project timeline might have prevented the long gap in development

### Lessons Learned
- Pay attention to the full instructions for things. This is what caused me to miss the continuation to fix the shadow acne, making me think I implemented shadows wrong.
- Modularizing code early makes development and debugging much more manageable
- Starting with simpler implementations and then refining them is more effective than trying to implement complex features all at once
- Understanding the rendering pipeline thoroughly is useful, as I used it multiple times for different requirements for this project.
- Taking systematic notes during the development process (as in the saved_chats files) helps track progress and identify issues

These lessons will be valuable for future graphics programming projects and software development in general. The experience gained in this project has deepened my understanding of 3D graphics programming concepts and improved my approach to project planning and implementation.

## Works Cited

- **ChatGPT**  
    Chatgpt.com, 2025. [Link](https://chatgpt.com/c/67f2cacd-0c84-8003-95da-41b9f849ee84). Accessed 7 Apr. 2025.  
    *Blender script code for the gymnastics building. This chat also has images. See this pdf: archive\saved_chats\blender_scripts.pdf*

- **GitHub Copilot · Your AI Pair Programmer**  
    GitHub, 2024. [Link](https://github.com/features/copilot). Accessed 9 Apr. 2025.
    *I used this extensively throughout the project. I leveraged all 4 code pilot modes: completions, ask, edit, and "agent". I heavily relied on agent throughout, but I often had to go back after 3 attempts with agent, take what I learned from it's implementations, and complete it myself (or use the "ask" version of copilot to increase my efficiency and nudge it in the direction i wanted). Some concepts were pretty much entirely made by copilot (i beleive I understand them well though); namely: attenuation, color change, spotlight, flipping animation for floor person. the camera first person switch and high bar swing were those cases where I took what I learned and then didn't fully rely on AI. I learned from this to make very very specific and detailed prompts. I've combined all the saved chats here: archive\saved_chats\full_combined.txt. Note that I do not believe the "edit" and "agent" sessions were saved here (not included in the history). In any case, please see detialed prompts and answers in the aforementioned file.*





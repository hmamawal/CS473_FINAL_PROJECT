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

- **GitHub Copilot**  
  GitHub, 2025. [Link](https://github.com/features/copilot). Accessed 6 Mar. 2025.
  
  *GitHub Copilot was extensively used throughout this project:*
  - [Code Modularization](https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/32123fa96446acef87dfa2dbd4c78683d0a48625/archive/saved_chats/project3/modularize3.md): Restructuring the codebase into multiple files for better organization
  - [Spotlight Toggle Implementation](https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/32123fa96446acef87dfa2dbd4c78683d0a48625/archive/saved_chats/project3/spot_light_on_off.md): Adding functionality to toggle the spotlight with the "L" key and point light with the "P" key
  - [Shadow Implementation](https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/bad_shadows.md): Iterative development of shadow mapping
  - [Shadow Bias Optimization](https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/dynamic_shadow_bias.md): Improving shadow quality with dynamic bias
  - [Post-Processing Effects](https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/post-processing.md): Initial implementation of post-processing filters
  - [Refined Post-Processing](https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/only_3_post_processing_effects.md): Reducing to three core effects (grayscale, blur, and darken)
  - [Cube Map Implementation](https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/working%20cube%20map.md): Adding skybox functionality
  - [Heads-Up Display](https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/HUD.md): Implementing information display with toggle functionality
  - [Write-up Enhancement](https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/proofread%20and%20improve%20readme.md): Refining documentation

- **ChatGPT**  
  OpenAI, 2025.
  
  *ChatGPT was used for detailed explanations and understanding of specific implementations:*
  - [Cube Map Solution](https://chatgpt.com/canvas/shared/681a1de1ed5481918ece1bce945677c1): Detailed implementation of skybox functionality
  - [Directional Light Changes](https://chatgpt.com/share/681aad14-11a0-8003-9d21-9b85bc381d71): Explanation of point light to directional light conversion
  - [Shadow Depth Rendering](https://chatgpt.com/share/681ab0d8-fda0-8003-9fbc-2a0c07da7076): Clarification on shadow mapping depth techniques
  - [Post-Processing Pipeline](https://chatgpt.com/share/681abf9e-9364-8003-85bf-7a37d666fecc): Understanding of framebuffer effects implementation

Throughout the project development, I thoroughly reviewed all AI-generated code, made necessary adjustments to fix issues, and ensured I understood the underlying graphics concepts being implemented. The understanding demonstrated in this write-up reflects my comprehension of the techniques applied in the project.


## Works Cited Written Description: 
GitHub copilot. 
First, I started everything by using GitHub copilot to automatically modularize everything I had in main so that it was not so congested - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/32123fa96446acef87dfa2dbd4c78683d0a48625/archive/saved_chats/project3/modularize3.md. Copilot led to having multiple different files to do this. Further, I partnered with it to do troubleshooting and to automatically correct the errors it was facing. It also added some things that I was not looking for, like GPU and CPU use caps, which I realized it did when my high bar avatar was moving more slowly. Next, I had it fix the implementation that I was not able to get from before where pressing “L” would turn off the flashlight / spotlight. Next, I had copilot add functionality to turn off the point light with the “P” button - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/32123fa96446acef87dfa2dbd4c78683d0a48625/archive/saved_chats/project3/spot_light_on_off.md.  After this, I decided to tackle shadows. I tried multiple attempts at asking it to guide me through, but I was getting weird problems like the tumbling floor turning black and showing symbols as its texture. I then tried using copilot for suggested changes 2-3 times but to no avail (these chats are not included in the repository as I did not end up using them). I then iteratively asked it to guide me through the shadow implementation but giving it the instructions 1 by 1 for the lesson 24 shadows lesson - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/bad_shadows.md. After doing this, I had an implementation for the shadows, but without the optimizations. I went through the bias optimizations with copilot and slightly improved the shadows, but they were still a little awkward, so I moved on - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/dynamic_shadow_bias.md.  I then implemented the post-processing, which github copilot helped me with. It first created 6 effects and used its own reasoning, but I gave it better context to follow the ICE instructions better and made only 3 effects: grayscala, blur, and darken- https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/post-processing.md and https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/only_3_post_processing_effects.md.  I am now working on the cube map solution. I gave the instructions and the completed code solution to a chatgpt o4-mino-high chat - https://chatgpt.com/canvas/shared/681a1de1ed5481918ece1bce945677c1 - to make a  more detailed solution, which I gave back to GitHub copilot. After giving it back to GitHub Copilot, I obtained the complete working cube map - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/working%20cube%20map.md.  This was the least error-debugging I had to do after. Next, I went to implement the heads-up display features. First, I turned off the alpha variable for the red background. Next, I added the light direction to the heads-up display, which I was already calculating and outputting to the terminal. Finally, I added instructions for toggling the HUD on and off with the H key. For the red box backgrounds, I went back and forth with GitHub copilot to implement these - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/HUD.md. Lastly, github coplot helped me improve my write up after I wrote most of it - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/proofread%20and%20improve%20readme.md. All these heads-up display steps were another iterative process with GitHub copilot to get to the result I wanted. Github copilot also transformed this paragraph section into a markdown format.

Chatgpt.
When making the writeup, in order to better understand and explain the changes that were made for the point light to become a directional light, I gave the github commit change log to chatgpt and asked it to explain the changes to me: https://chatgpt.com/share/681aad14-11a0-8003-9d21-9b85bc381d71. I used part of the explanation in the writeup. For the depth part of the shadow explanation, I used this chat’s help: https://chatgpt.com/share/681ab0d8-fda0-8003-9fbc-2a0c07da7076. For explaining the post-processing changes, I used this chat: https://chatgpt.com/share/681abf9e-9364-8003-85bf-7a37d666fecc. 

Throughout making these changes, I made sure to review the changes made, fix any discrepancies, and ensure I understood what was happening. My understanding of what was accomplished is reflected in the write-up and in this citation.





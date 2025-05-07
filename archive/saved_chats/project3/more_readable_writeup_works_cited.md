hmamawal: change this works cited section to reflect the following works cited but formatted for markdown well: <<<GitHub copilot. 
First, I started everything by using GitHub copilot to automatically modularize everything I had in main so that it was not so congested - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/32123fa96446acef87dfa2dbd4c78683d0a48625/archive/saved_chats/project3/modularize3.md. Copilot led to having multiple different files to do this. Further, I partnered with it to do troubleshooting and to automatically correct the errors it was facing. It also added some things that I was not looking for, like GPU and CPU use caps, which I realized it did when my high bar avatar was moving more slowly. Next, I had it fix the implementation that I was not able to get from before where pressing “L” would turn off the flashlight / spotlight. Next, I had copilot add functionality to turn off the point light with the “P” button - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/32123fa96446acef87dfa2dbd4c78683d0a48625/archive/saved_chats/project3/spot_light_on_off.md.  After this, I decided to tackle shadows. I tried multiple attempts at asking it to guide me through, but I was getting weird problems like the tumbling floor turning black and showing symbols as its texture. I then tried using copilot for suggested changes 2-3 times but to no avail (these chats are not included in the repository as I did not end up using them). I then iteratively asked it to guide me through the shadow implementation but giving it the instructions 1 by 1 for the lesson 24 shadows lesson - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/bad_shadows.md. After doing this, I had an implementation for the shadows, but without the optimizations. I went through the bias optimizations with copilot and slightly improved the shadows, but they were still a little awkward, so I moved on - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/dynamic_shadow_bias.md.  I then implemented the post-processing, which github copilot helped me with. It first created 6 effects and used its own reasoning, but I gave it better context to follow the ICE instructions better and made only 3 effects: grayscala, blur, and darken- https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/post-processing.md and https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/only_3_post_processing_effects.md.  I am now working on the cube map solution. I gave the instructions and the completed code solution to a chatgpt o4-mino-high chat - https://chatgpt.com/canvas/shared/681a1de1ed5481918ece1bce945677c1 - to make a  more detailed solution, which I gave back to GitHub copilot. After giving it back to GitHub Copilot, I obtained the complete working cube map - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/working%20cube%20map.md.  This was the least error-debugging I had to do after. Next, I went to implement the heads-up display features. First, I turned off the alpha variable for the red background. Next, I added the light direction to the heads-up display, which I was already calculating and outputting to the terminal. Finally, I added instructions for toggling the HUD on and off with the H key. For the red box backgrounds, I went back and forth with GitHub copilot to implement these - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/HUD.md. Lastly, github coplot helped me improve my write up after I wrote most of it - https://github.com/hmamawal/CS473_FINAL_PROJECT/blob/main/archive/saved_chats/project3/proofread%20and%20improve%20readme.md. All these heads-up display steps were another iterative process with GitHub copilot to get to the result I wanted.

Chatgpt.
When making the writeup, in order to better understand and explain the changes that were made for the point light to become a directional light, I gave the github commit change log to chatgpt and asked it to explain the changes to me: https://chatgpt.com/share/681aad14-11a0-8003-9d21-9b85bc381d71. I used part of the explanation in the writeup. For the depth part of the shadow explanation, I used this chat’s help: https://chatgpt.com/share/681ab0d8-fda0-8003-9fbc-2a0c07da7076. For explaining the post-processing changes, I used this chat: https://chatgpt.com/share/681abf9e-9364-8003-85bf-7a37d666fecc. 

Throughout making these changes, I made sure to review the changes made, fix any discrepancies, and ensure I understood what was happening. My understanding of what was accomplished is reflected in the write-up and in this citation.
>>>

GitHub Copilot: I'll update the Works Cited section to reflect the provided information in a well-formatted markdown style. Here's the solution:

### [project3_writeup.md](file:///c%3A/Users/hunter.mamawal/Classes/CS473_Computer_Graphics/CS473_FINAL_PROJECT/project3_writeup.md)

I'll replace the current Works Cited section with a more comprehensive and well-formatted one that includes all the resources mentioned.

````markdown
// ...existing code...

## Works Cited

- **GitHub Copilot**  
  GitHub, 2024. [Link](https://github.com/features/copilot). Accessed 9 Apr. 2025.
  
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
  OpenAI, 2024.
  
  *ChatGPT was used for detailed explanations and understanding of specific implementations:*
  - [Cube Map Solution](https://chatgpt.com/canvas/shared/681a1de1ed5481918ece1bce945677c1): Detailed implementation of skybox functionality
  - [Directional Light Changes](https://chatgpt.com/share/681aad14-11a0-8003-9d21-9b85bc381d71): Explanation of point light to directional light conversion
  - [Shadow Depth Rendering](https://chatgpt.com/share/681ab0d8-fda0-8003-9fbc-2a0c07da7076): Clarification on shadow mapping depth techniques
  - [Post-Processing Pipeline](https://chatgpt.com/share/681abf9e-9364-8003-85bf-7a37d666fecc): Understanding of framebuffer effects implementation

Throughout the project development, I thoroughly reviewed all AI-generated code, made necessary adjustments to fix issues, and ensured I understood the underlying graphics concepts being implemented. The understanding demonstrated in this write-up reflects my comprehension of the techniques applied in the project.
````

Made changes.
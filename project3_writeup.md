Project 3 write up 

Write-up (20 pts): 
o A summary of your project 
o A short (2-3 paragraph) description of the development process you experienced (e.g. 
obstacles encountered, changes to the project scope during development, etc. – the story 
of your project) 
o A README.TXT file that provides details on how to interact with your program. 
o An AAR evaluating the lessons that you learned during the project 
o Works cited page. 

# Writeup
## Summary

## Project Description
This game is a "gymanstics simulation" that aims to roughly recreate the physical gym that I did gymnastics at for three years here at West Point and add fun "animations" that gymnasts/avatars would do. I wanted to make this project because I thought it would be unique and fun to do. For project 3, I wanted to continue this simulation, implementing additional functionality and features to improve the feel of the simulator. Before I first wanted to decrease the size of the main file, because I always had to scroll so much to find certain points of my code. I separated large sections, like the initialization, the rendering, and the input handling into separate files. This made it easier to navigate the project when I went about implementing everything for project 3. I started this process almost immediately after I presented for project 2 because I was genuinly enjoying working on this project. 

Low and behold, other class work got in the way and I didn't end up working on this project again for a few weeks. When coming back, I went systematically through the new project requirements to implement them. I first started by making the simulation more interactive by adding the ability to turn both the spotlight and the directional light on and off. I did this by adding a variable in input_handling.cpp to track the state of the light (if 'l' key pressed, turn the light on or off) and change the shader file variable to on/off as well (via a Shader setBool method). 

I started with importing the blender objects that I wanted, the first one being the floor avatar. I then implemented a backflip animation for that avatar when i pressed 'B'. Next I worked a very long time on an avatar that interracted with the highbar. After much back and forth with Github Copilot (and much struggle making the high bar object and the gymnast interract), I decided to make the gymnast go around the bar by "guestimating" values and visually inspecting the results. I settled on an approach that treated the gymnasts swings around the bar as 4 separate coordinate quadrants, performing movements until the next 90 degrees was reached. 

I then added a camera that followed the gymnast around the bar, and added a first person view. I then ensured the spotlight attenuated. I also added a point light that changed color when I pressed 'R'. I then added a vault table and pommel horse, and made the gymnast do a backflip off of the vault table. I also added a spotlight that followed the gymnast around. When trying to demonstrate the shininess of the materials, I created two new blender objects for the pommel horses that would demonstrate the shininess. After conversing with GitHub copilot and trying some solutions for showing the shininess more with solutions like ensuring the value of shininess from the .mtl file (Nx) was taken and used, but it still did not work. 

Lessons learned: 
- Making blender objects interract and animate and designing the math for these animations is not a fast process!
- Don't rely on GitHub copilot to conceptualize the math for you, even in pursuit of a more elegant solution.

## Works Cited

### Works Cited

- **Base Character Rigged Free 3D Model - .Blend - Free3D**  
    Free3d.com, 2020. [Link](https://free3d.com/3d-model/base-character-ready-to-animate-453899.html). Accessed 31 Mar. 2025.  
    *This is where I got the Avatar from.*

- **Black Metal Pattern Black Metal Texture, Black Metal, Black Metal Background, HD Wallpaper | Peakpx**  
    Peakpx.com, 2020. [Link](https://www.peakpx.com/en/hd-wallpaper-desktop-kdaok). Accessed 7 Apr. 2025.

- **CBKH/0008 - Couristan**  
    Couristan, 13 July 2022. [Link](https://www.couristan.com/product/cbkh-0008/). Accessed 31 Mar. 2025.  
    *This is where I got the blue carpet from.*

- **ChatGPT**  
    Chatgpt.com, 2025. [Link](https://chatgpt.com/c/67f2ff60-f1b4-8003-81b6-513eedcaf0ed). Accessed 7 Apr. 2025.  
    *I used this chat to generate the banner images for the walls. I gave it the photos from the Gross Center army west point page. Note that this chat will not work because the chat has images, which ChatGPT won't let me share. See this pdf: archive\saved_chats\picture_generation.pdf*

- **ChatGPT**  
    Chatgpt.com, 2025. [Link](https://chatgpt.com/c/67f2cacd-0c84-8003-95da-41b9f849ee84). Accessed 7 Apr. 2025.  
    *Blender script code for the gymnastics building. This chat also has images. See this pdf: archive\saved_chats\blender_scripts.pdf*

- **Gross Center - Army West Point**  
    Army West Point, 2015. [Link](https://goarmywestpoint.com/sports/2015/3/6/GEN_2014010117?id=17). Accessed 7 Apr. 2025.  
    *This is where I got the reference photo for the Lou Gross Sports center banners.*

- **White Brick Wall Images - Free Download on Freepik**  
    Freepik, 2025. [Link](https://www.freepik.com/free-photos-vectors/white-brick-wall). Accessed 7 Apr. 2025.

- **Metal Surfaces**  
    Istockphoto.com, 2024. [Link](https://www.istockphoto.com/photos/metal-surfaces). Accessed 7 Apr. 2025.

- **Basketball Leather Close-Up**  
    Istockphoto.com, 2024. [Link](https://www.istockphoto.com/photos/basketball-leather-close-up). Accessed 7 Apr. 2025.

- **MyBib citation Generator**  
    Mybib.com, 2025. [Link](https://www.mybib.com/). Accessed 7 Apr. 2025.

- **GitHub Copilot · Your AI Pair Programmer**  
    GitHub, 2024. [Link](https://github.com/features/copilot). Accessed 9 Apr. 2025.
    *I used this extensively throughout the project. I leveraged all 4 code pilot modes: completions, ask, edit, and "agent". I heavily relied on agent throughout, but I often had to got back after 3 attempts with agent, take what I learned from it's implementations, and complete it myself (or use the "ask" version of copilot to increase my efficiency and nudge it in the direction i wanted). Some concepts were pretty much entirely made by copilot (i beleive I understand them well though); namely: attenuation, color change, spotlight, flipping animation for floor person. the camera first person switch and high bar swing were those cases where I took what I learned and then didn't fully rely on AI. I learned from this to make very very specific and detailed prompts. I've combined all the saved chats here: archive\saved_chats\full_combined.txt. Note that I do not believe the "edit" and "agent" sessions were saved here (not included in the history). In any case, please see detialed prompts and answers in the aforementioned file.*





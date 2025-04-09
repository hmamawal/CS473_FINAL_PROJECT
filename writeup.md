# Writeup
## Summary

## Project Description
I wanted to make this project because I thought it would be unique and fun to do.

I went systematically through the project requirements to implement them. I started with importing the blender objects that I wanted, the first one being the floor avatar. I then implemented a backflip animation for that avatar when i pressed 'B'. Next I worked a very long time on an avatar that interracted with the highbar. After much back and forth with Github Copilot (and much struggle making the high bar object and the gymnast interract), I decided to make the gymnast go around the bar by "guestimating" values and visually inspecting the results. I settled on an approach that treated the gymnasts swings around the bar as 4 separate coordinate quadrants, performing movements until the next 90 degrees was reached. 

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




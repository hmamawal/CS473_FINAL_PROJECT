#!/usr/bin/env python3

from direct.showbase.ShowBase import ShowBase
from panda3d.core import CollisionTraverser, CollisionHandlerPusher
from panda3d.core import CollisionNode, CollisionSphere
from panda3d.core import Vec3, NodePath, BitMask32, WindowProperties
from direct.task import Task
import sys
import math
from models import create_box, create_sphere, create_cylinder

class GymnasticsGame(ShowBase):
    def __init__(self):
        ShowBase.__init__(self)
        
        # Display FPS counter
        self.setFrameRateMeter(True)
        
        # Initialize camera variables
        self.heading = 0
        self.pitch = 0
        self.camera_distance = 10
        self.mouse_x = 0
        self.mouse_y = 0
        self.last_mouse_x = 0
        self.last_mouse_y = 0
        self.mouse_sensitivity = 0.2
        
        # Stick man state variables
        self.is_on_bar = False
        self.is_rotating = False
        self.rotation_angle = 0
        self.original_stick_man_pos = Vec3(0, 0, 1.8)
        self.bar_stick_man_pos = Vec3(0, 0, 3.5)  # Position when hanging from bar
        
        # Hide the mouse cursor and set mouse in relative mode
        self.setup_mouse()
        
        # Setup camera
        self.disableMouse()  # Disable the default mouse control
        self.camera.setPos(0, -10, 3)
        self.camera.lookAt(0, 0, 0)
        
        # Setup collision detection
        self.cTrav = CollisionTraverser()
        self.pusher = CollisionHandlerPusher()
        
        # Create floor/mat
        self.create_gymnastics_mat()
        
        # Create high bar
        self.create_high_bar()
        
        # Setup player
        self.setup_player()
        
        # Create stick man character
        self.create_stick_man()
        
        # Setup key controls
        self.setup_controls()
        
        # Task to update the game
        self.taskMgr.add(self.update, "update")
        
        # Task to handle mouse movement
        self.taskMgr.add(self.mouse_task, "MouseTask")
    
    def setup_mouse(self):
        """Setup mouse properties for first-person view."""
        props = WindowProperties()
        props.setCursorHidden(True)
        props.setMouseMode(WindowProperties.M_relative)
        self.win.requestProperties(props)
        
        # Initialize mouse position in center of the window
        self.mouse_x = 0
        self.mouse_y = 0
        self.last_mouse_x = 0
        self.last_mouse_y = 0
    
    def mouse_task(self, task):
        """Task to handle mouse movement for camera control."""
        # Only update if window is in foreground
        if self.mouseWatcherNode.hasMouse():
            # Get mouse movement
            self.mouse_x = self.mouseWatcherNode.getMouseX()
            self.mouse_y = self.mouseWatcherNode.getMouseY()
            
            # Calculate mouse movement delta
            dx = self.mouse_x - self.last_mouse_x
            dy = self.mouse_y - self.last_mouse_y
            
            # Update camera orientation
            if dx != 0 or dy != 0:
                # Fix 1: Reversed the sign for dx to correct horizontal camera movement
                self.heading += dx * self.mouse_sensitivity * 100
                self.pitch -= dy * self.mouse_sensitivity * 100
                
                # Clamp pitch to prevent flipping
                self.pitch = max(-85, min(85, self.pitch))
            
            # Store current mouse position for next frame
            self.last_mouse_x = self.mouse_x
            self.last_mouse_y = self.mouse_y
        
        return Task.cont
    
    def create_gymnastics_mat(self):
        # Create a blue gymnastics mat (floor)
        self.mat = create_box()
        self.mat.setScale(10, 10, 0.5)
        self.mat.setPos(0, 0, -0.5)
        self.mat.setColor(0.2, 0.2, 0.8, 1)  # Blue color
        self.mat.reparentTo(self.render)
        
        # Add collision to the mat
        cs = CollisionNode('mat_collision')
        cs.addSolid(CollisionSphere(0, 0, 0, 1))
        cnp = self.mat.attachNewNode(cs)
        cnp.setScale(10, 10, 0.5)
        cnp.setPos(0, 0, 0)
    
    def create_high_bar(self):
        # Create posts (vertical bars)
        self.left_post = create_cylinder(radius=0.2, height=4)
        self.left_post.setPos(-3, 0, 2)
        self.left_post.setColor(0.6, 0.3, 0.1, 1)  # Brown color
        self.left_post.reparentTo(self.render)
        
        self.right_post = create_cylinder(radius=0.2, height=4)
        self.right_post.setPos(3, 0, 2)
        self.right_post.setColor(0.6, 0.3, 0.1, 1)  # Brown color
        self.right_post.reparentTo(self.render)
        
        # Create horizontal bar
        self.horizontal_bar = create_cylinder(radius=0.1, height=6)
        # Fix 2: Rotate the bar 90 degrees around the Z-axis (changing HPR values)
        self.horizontal_bar.setHpr(90, 90, 0)  # Correct orientation to connect to posts
        self.horizontal_bar.setPos(0, 0, 4)
        self.horizontal_bar.setColor(0.7, 0.7, 0.7, 1)  # Gray color
        self.horizontal_bar.reparentTo(self.render)
    
    def setup_player(self):
        # Create a simple player model (a sphere)
        self.player = create_sphere(radius=0.5)
        self.player.setPos(0, -5, 0.5)
        self.player.setColor(1, 0, 0, 1)  # Red color
        self.player.reparentTo(self.render)
        
        # Add collision detection to the player
        cs = CollisionNode('player')
        cs.addSolid(CollisionSphere(0, 0, 0, 0.5))
        cnp = self.player.attachNewNode(cs)
        cnp.setPythonTag('owner', self.player)
        
        self.pusher.addCollider(cnp, self.player)
        self.cTrav.addCollider(cnp, self.pusher)
    
    def setup_controls(self):
        # Set up keyboard controls
        self.keys = {}
        for key in ['arrow_up', 'arrow_down', 'arrow_left', 'arrow_right', 'w', 'a', 's', 'd']:
            self.keys[key] = 0  # Initialize keys as not pressed (0) instead of pressed (1)
            self.accept(key, self.set_key, [key, 1])
            self.accept(f'shift-{key}', self.set_key, [key, 1])
            self.accept(f'{key}-up', self.set_key, [key, 0])
        
        # Add arm control key for stick man
        self.accept('u', self.raise_stick_man_arms)
        self.accept('u-up', self.lower_stick_man_arms)
        
        # Add jump key to make the stick man jump to the high bar or drop from it
        self.accept('j', self.toggle_high_bar)
        
        # Add rotation key for when the stick man is on the bar
        self.accept('m', self.start_rotation)
        self.accept('m-up', self.stop_rotation)
        
        # Add ESC key to exit
        self.accept('escape', sys.exit)
        
        # Add key to reset mouse if it gets stuck
        self.accept('r', self.setup_mouse)
    
    def set_key(self, key, value):
        self.keys[key] = value
    
    def update(self, task):
        # Get elapsed time and calculate movement speed
        dt = globalClock.getDt()
        speed = 5.0 * dt
        
        # Handle stick man rotation around the bar if active
        if self.is_on_bar and self.is_rotating:
            self.update_stick_man_rotation(dt)
            
        # Calculate movement direction based on camera heading
        heading_rad = math.radians(self.heading)
        forward_dir = Vec3(math.sin(heading_rad), math.cos(heading_rad), 0)
        right_dir = Vec3(math.cos(heading_rad), -math.sin(heading_rad), 0)
        
        # Move player relative to camera direction
        move_dir = Vec3(0, 0, 0)
        if self.keys['arrow_up'] or self.keys['w']:
            move_dir += forward_dir
        if self.keys['arrow_down'] or self.keys['s']:
            move_dir -= forward_dir
        if self.keys['arrow_right'] or self.keys['d']:
            move_dir += right_dir
        if self.keys['arrow_left'] or self.keys['a']:
            move_dir -= right_dir
        
        # Normalize movement direction if needed
        if move_dir.length() > 0:
            move_dir.normalize()
            self.player.setX(self.player, move_dir.x * speed)
            self.player.setY(self.player, move_dir.y * speed)
        
        # Keep the player on the mat
        x, y, z = self.player.getPos()
        x = max(-9, min(9, x))
        y = max(-9, min(9, y))
        self.player.setPos(x, y, z)
        
        # Update camera based on player position and mouse orientation
        pitch_rad = math.radians(self.pitch)
        
        # Calculate camera position
        cam_x = x - self.camera_distance * math.sin(heading_rad) * math.cos(pitch_rad)
        cam_y = y - self.camera_distance * math.cos(heading_rad) * math.cos(pitch_rad)
        cam_z = z + self.camera_distance * math.sin(pitch_rad) + 1  # Add 1 for height offset
        
        self.camera.setPos(cam_x, cam_y, cam_z)
        self.camera.lookAt(x, y, z + 1)  # Look at player (with slight height offset)
        
        return Task.cont

    def create_stick_man(self):
        # Create a stick man using cylinders and a sphere
        
        # Head (sphere)
        self.head = create_sphere(radius=0.3)
        self.head.setColor(0.9, 0.75, 0.65, 1)  # Skin color
        
        # Torso (cylinder)
        self.torso = create_cylinder(radius=0.1, height=1.2)
        self.torso.setPos(0, 0, -0.6)  # Position below head
        self.torso.setColor(0.2, 0.2, 0.8, 1)  # Blue color
        self.torso.reparentTo(self.head)
        
        # Arms
        # Left arm
        self.left_arm = create_cylinder(radius=0.07, height=0.8)
        self.left_arm.setPos(0, 0, 0)
        self.left_arm.setHpr(0, 0, 90)  # Rotate to horizontal position
        self.left_arm.setColor(0.9, 0.75, 0.65, 1)  # Skin color
        self.left_arm.reparentTo(self.torso)
        self.left_arm.setPos(-0.4, 0, 0.4)  # Position on left side of torso
        
        # Right arm
        self.right_arm = create_cylinder(radius=0.07, height=0.8)
        self.right_arm.setPos(0, 0, 0)
        self.right_arm.setHpr(0, 0, -90)  # Rotate to horizontal position
        self.right_arm.setColor(0.9, 0.75, 0.65, 1)  # Skin color
        self.right_arm.reparentTo(self.torso)
        self.right_arm.setPos(0.4, 0, 0.4)  # Position on right side of torso
        
        # Legs
        # Left leg
        self.left_leg = create_cylinder(radius=0.07, height=1.0)
        self.left_leg.setPos(-0.2, 0, -1.2)  # Position at bottom of torso, left side
        self.left_leg.setColor(0.1, 0.1, 0.1, 1)  # Black color
        self.left_leg.reparentTo(self.torso)
        
        # Right leg
        self.right_leg = create_cylinder(radius=0.07, height=1.0)
        self.right_leg.setPos(0.2, 0, -1.2)  # Position at bottom of torso, right side
        self.right_leg.setColor(0.1, 0.1, 0.1, 1)  # Black color
        self.right_leg.reparentTo(self.torso)
        
        # Create a parent node to position the entire stick man
        self.stick_man = NodePath("stick_man")
        self.head.reparentTo(self.stick_man)
        
        # Position the stick man under the high bar
        self.stick_man.setPos(0, 0, 1.8)  # Position on the ground under the bar
        self.stick_man.reparentTo(self.render)
        
        return self.stick_man

    def raise_stick_man_arms(self):
        """Raise the stick man's arms to point upward when U is pressed"""
        # Set arms to point upward by changing their orientation to be perpendicular to the ground
        # For the left arm
        self.left_arm.setHpr(0, 0, 0)  # Reset rotation
        self.left_arm.setPos(-0.2, 0, 0.4)  # Position at the shoulder
        
        # For the right arm
        self.right_arm.setHpr(0, 0, 0)  # Reset rotation  
        self.right_arm.setPos(0.2, 0, 0.4)  # Position at the shoulder
    
    def lower_stick_man_arms(self):
        """Lower the stick man's arms back to the horizontal position"""
        # Return arms to original horizontal position
        self.left_arm.setHpr(0, 0, 90)  # Original horizontal position
        self.left_arm.setPos(-0.4, 0, 0.4)  # Original position
        
        self.right_arm.setHpr(0, 0, -90)  # Original horizontal position
        self.right_arm.setPos(0.4, 0, 0.4)  # Original position

    def jump_to_high_bar(self):
        """Make the stick man jump up to the high bar and grab it"""
        if not self.is_on_bar:
            # Move the stick man to the high bar position
            self.stick_man.setPos(self.bar_stick_man_pos)
            
            # Raise arms to grab the bar
            self.raise_stick_man_arms()
            
            # Update state
            self.is_on_bar = True
    
    def drop_from_high_bar(self):
        """Drop the stick man from the high bar back to the ground"""
        if self.is_on_bar:
            # Move the stick man back to the ground
            self.stick_man.setPos(self.original_stick_man_pos)
            
            # Lower arms back to normal position
            self.lower_stick_man_arms()
            
            # Update state
            self.is_on_bar = False

    def toggle_high_bar(self):
        """Toggle between attaching and detaching from the high bar"""
        if self.is_on_bar:
            self.drop_from_high_bar()
        else:
            self.jump_to_high_bar()

    def start_rotation(self):
        """Start rotating the stick man around the high bar when M is pressed"""
        if self.is_on_bar and not self.is_rotating:
            self.is_rotating = True
            # Store the current rotation angle (starting point)
            self.rotation_angle = 0
            
    def stop_rotation(self):
        """Stop rotating and return to hanging position when M is released"""
        if self.is_rotating:
            self.is_rotating = False
            # Reset the stick man to the hanging position
            self.reset_stick_man_position()
            
    def reset_stick_man_position(self):
        """Reset the stick man to the initial hanging position"""
        # Move back to the hanging position under the bar
        self.stick_man.setPos(self.bar_stick_man_pos)
        self.stick_man.setHpr(0, 0, 0)  # Reset rotation
        
        # Make sure arms are up to hold the bar
        self.raise_stick_man_arms()

    def update_stick_man_rotation(self, dt):
        """Update the stick man's rotation around the high bar"""
        if not self.is_rotating:
            return
            
        # Rotation speed in degrees per second
        rotation_speed = 180.0
        
        # Update the rotation angle
        self.rotation_angle += rotation_speed * dt
        
        # Calculate the new position based on rotation around the bar
        # The rotation center is at the high bar position (0, 0, 4)
        radius = 0.8  # Distance from the bar to the center of the stick man
        
        # Convert angle to radians
        angle_rad = math.radians(self.rotation_angle)
        
        # Calculate new position with the center of rotation at the high bar
        x = 0  # x-coordinate of the high bar
        y = 0  # y-coordinate of the high bar
        z = 4 - radius * math.cos(angle_rad)  # z-coordinate with offset for rotation
        
        # Also move slightly back and forth on the y-axis for a more realistic swinging motion
        y_offset = math.sin(angle_rad) * 0.5
        
        # Position the stick man at the calculated position
        self.stick_man.setPos(x, y_offset, z)
        
        # Rotate the stick man to follow the circular path
        # Subtract 90 degrees to make the head point outward from the center
        self.stick_man.setHpr(0, self.rotation_angle - 90, 0)

def main():
    # Create and run the game
    game = GymnasticsGame()
    game.run()

if __name__ == "__main__":
    main()
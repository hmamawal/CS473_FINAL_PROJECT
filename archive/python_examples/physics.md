# High Bar Game Physics Implementation

## Character Attachment to the High Bar

The gymnastics game implements a simple physics system to allow the stick man character to interact with the high bar. 

### Attaching to the Bar

When the player presses the jump key ('J'), the `toggle_high_bar()` function is called, which checks if the character is already on the bar:

```python
def toggle_high_bar(self):
    """Toggle between attaching and detaching from the high bar"""
    if self.is_on_bar:
        self.drop_from_high_bar()
    else:
        self.jump_to_high_bar()
```

The attachment mechanism involves:

1. **Position Change**: When the character jumps to the bar, their position is instantly moved to the `bar_stick_man_pos` (positioned below the bar).
   ```python
   self.stick_man.setPos(self.bar_stick_man_pos)
   ```

2. **Arm Positioning**: The character's arms are raised upward to simulate grabbing the bar.
   ```python
   self.raise_stick_man_arms()
   ```

3. **State Tracking**: The game maintains a boolean state variable `is_on_bar` to track whether the character is currently attached to the bar.

### Detaching from the Bar

When the character drops from the bar:

1. The position is reset to the original ground position (`original_stick_man_pos`).
2. The arms are lowered back to their default position.
3. The `is_on_bar` state is set to `False`.

## Rotation Around the Bar

### Physics Model

The rotation of the character around the bar implements a simplified physics model based on circular motion:

1. **Rotation Initiation**: Pressing the 'M' key triggers the `start_rotation()` method, which sets `is_rotating` to `True` and initializes the rotation angle to 0.

2. **Rotation Update Logic**: While rotating, the `update_stick_man_rotation(dt)` method is called each frame to:
   - Increase the rotation angle based on time and a constant rotation speed
   - Calculate the new position using circular motion equations
   - Position and orient the character accordingly

```python
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
```

### Key Components of the Rotation Physics

1. **Center of Rotation**: The high bar (located at position 0, 0, 4) acts as the center of rotation.

2. **Circular Path Calculation**:
   - The character moves in a circular path around the bar
   - Position is determined using trigonometric functions:
     - Z-position: `z = 4 - radius * math.cos(angle_rad)`
     - Y-position: Includes a small sinusoidal offset `y_offset = math.sin(angle_rad) * 0.5` for naturalistic swinging

3. **Character Orientation**: The character's orientation (heading, pitch, roll) is updated to always face outward from the center of rotation, creating a realistic rotation appearance.

4. **Stopping Rotation**: When the player releases the 'M' key, the rotation stops and the character returns to the hanging position.

## Implementation Notes

- The physics implementation is simplified and does not include realistic momentum or gravity calculations.
- A constant rotation speed is used rather than acceleration due to gravity.
- The character's position is directly set each frame rather than using velocity and force calculations.

This implementation prioritizes ease of control and visual appearance over physical accuracy, creating a game-like experience rather than a physics simulation.
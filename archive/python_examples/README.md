# Gymnastics High Bar Simulator

A simple 3D environment created with Python and Panda3D that simulates a gymnastics setting with a high bar apparatus.

## Overview

This application creates a 3D environment featuring a gymnastics mat and a high bar. You can navigate around the environment in first-person view, exploring the setup from different angles.

## Features

- 3D gymnastics environment with blue mat and regulation high bar
- First-person navigation with mouse look controls
- Procedurally generated 3D models (no external assets required)
- Collision detection to prevent walking through objects
- FPS counter for performance monitoring

## Requirements

- Python 3.x
- Panda3D
- NumPy

## Installation

1. Clone or download this repository
2. Install the required dependencies:
   ```
   pip install -r requirements.txt
   ```

## Usage

Run the game by executing:
```
python main.py
```

## Controls

- **W/A/S/D or Arrow Keys**: Move the player character
- **Mouse**: Look around
- **R**: Reset mouse tracking (if it gets stuck)
- **ESC**: Exit the game

## Project Structure

- `main.py`: Main game logic and setup
- `models.py`: Procedural 3D model generation
- `requirements.txt`: Required Python packages

## Future Enhancements

Currently, the game allows you to walk around and explore the environment but doesn't support interaction with the high bar. Planned future enhancements include:

- Ability to grab and swing on the high bar
- Physics-based gymnastics movements
- Additional gymnastics apparatus (parallel bars, vault, etc.)
- Character animations
- Scoring system for gymnastics routines

## Contributing

Contributions are welcome! Feel free to submit pull requests or suggest new features.

## License

This project is open source and available under the MIT License.

## Credits

Created by Hunter Mamawal (April 2025)
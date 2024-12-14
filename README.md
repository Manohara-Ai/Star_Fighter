# Star Fighter

**Star Fighter** is a Command-Line Interface (CLI) game where the player controls a spaceship to navigate through an asteroid field and avoid collisions. The game uses **escape sequences** for terminal-based animation, and it implements **multithreading** with **mutex synchronization** for handling concurrent tasks like movement and collision detection.

## Game Overview
In **Star Fighter**, the player must use their spaceship to dodge asteroids that fall across the screen. The game progresses with increasing difficulty as the number and speed of asteroids increase over time. The game ends when the spaceship collides with an asteroid 5 times. The player can control the spaceship using the keyboard, 'a' to move spaceship left and 'd' to move spaceship right, and the game offers a visual challenge with animated movement in the terminal.

## Features
- **Smooth CLI Animation:** Using escape sequences, the game renders moving objects, including the spaceship and asteroids.
- **Multithreading:** The game leverages threads to manage different aspects of the game concurrently, such as updating positions and checking for collisions.
- **Mutex Synchronization:** Mutexes are used to synchronize access to shared resources between threads, ensuring smooth gameplay and preventing race conditions.
- **Increasing Difficulty:** As the game progresses, the frequency and speed of the asteroids increase, making the game more challenging.
- **Terminal Control:** The game manipulates terminal output to animate objects, providing a more immersive experience in the command line.

## Files
- **`star_fighter.c`**: This file contains the main game logic, including the game loop, user input handling, and managing the overall flow of the game.
  - The spaceship's position is updated based on user input.
  - The asteroids' positions are dynamically updated.
  - Game state management, including collision detection, is handled here.
  
- **`helper.c`**: Contains utility functions used across the game.
  - Functions for cli animation
  - Functions to print game state
  - Functions to read / write current game state

## Installation and Setup

1. **Clone the repository** to your local machine:

   ```bash
   git clone https://github.com/yourusername/starfighter.git
   cd starfighter

2. **Compile the game** using following command:
   ````bash
   gcc star_fighter.c helper.c -o starfighter -pthread

3. **Run the game** using following command:
   ````bash
   ./starfighter

## Contributor
Manohara 

Happy coding and good luck with your high score!

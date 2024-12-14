/***
This file contains definitions of helper functions used in helper.c -- a shared library built specific for the game Star Fighter, the game shall use these functions repeatedly for better CLI Animations and other important tasks.
***/

// This function displays the intro page
void display_intro();

// This function reads game_state
int read_game_state();

// This function enables non blocking input
void enable_non_blocking_input();

// This function enables non blocking input
void disable_non_blocking_input();

// This function helps in generating the cli load screen whenever necessary
void load_screen_animation();

// This function helps to move the cursor to specified location from the current location
void move_cursor(int row, int col);

// This function helps in keeping track of progress of a process/ subprocess
void progress_bar();

// This function exits the terminal on resizing
void resize_kill();

// This function prints the score
void print_score(int score);

// This function prints the health of spaceship
void print_health(int power);

// This function compares previous high score and current score and update high score if necessary
void update_high_score(int new_high_score);



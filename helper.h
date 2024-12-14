/***
This file contains definitions of helper functions used in helper.c -- a shared library built specific for the game "", the game shall use these functions repeatedly for better CLI Animations and other important tasks.
***/

// This functions displays the intro page
void display_intro();

// This function helps in generating the cli load screen whenever necessary
void load_screen_animation();

// This function helps to move the cursor Left or Right from the current place
void move_cursor_LR();

// This function helps to move the cursor Up or Down from the current place
void move_cursor_UD();

// This function helps to move the cursor to specified place from the current place
void move_cursor(int row, int col);

// This function helps in keeping track of progress
void progress_bar();

// This function exits the terminal on resizing
void resize_kill();

// This functions prints the spaceship
void print_spaceship(int x, int y);

// This functions prints the asteroid
void print_asteroid(int x, int y);

// This function reads game_state
int read_game_state();

// This function handles the spaceship position
void* spaceship_thread(void* arg);

// This function handles asteroid positions
void* asteroid_thread(void* arg);

// This function handles score and health
void* score_health_thread(void* arg);

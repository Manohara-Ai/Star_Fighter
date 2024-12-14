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

// This functions prints the score
void print_score(int score);

// This function reads game_state
int read_game_state();

// This function enables non blocking input
void enable_non_blocking_input();

// This function enables non blocking input
void disable_non_blocking_input();

void update_high_score(int new_high_score);

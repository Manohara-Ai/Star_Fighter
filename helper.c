#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <termios.h>

#include "helper.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW  "\033[1;33m"
#define RESET "\033[0m"

const char* spaceship = "|-|";
const char* asteroid = "*";

// Function to display to display Intro Page
void display_intro() {
    printf("\n");
    printf("   ***********************************************\n");
    printf("       WELCOME TO STARFIGHTER: GALACTIC LAST HOPE\n");
    printf("   ***********************************************\n\n");

    printf("                 .---.\n");
    printf("                /  |  \\\n");
    printf("               /   |   \\\n");
    printf("              /   o|o   \\\n");
    printf("             /_____+_____\\		Dodge the Asteroids!\n");
    printf("             \\    / \\    /		Avoid them at all costs!\n");
    printf("              \\__/   \\__/\n");
    printf("             /  /\\   /\\  \\\n");
    printf("            /__/  '-'  \\__\\\n");
    printf("            \\_|         |_/\n");
    printf("            '-'         '-'\n\n");

    printf("\n");
    printf("Humanity is counting on you, Commander.\n\n");
    printf("Press ENTER key to launch your mission...\n");
    getchar();
    load_screen_animation();
}

// Function to check if a file exists
int file_exists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);  // Returns 1 if file exists, 0 otherwise
}

// Function to create a new game state
void create_game_state_file() {
    // Open the file in write mode ("w"). If the file does not exist, it will be created.
    FILE *file = fopen("game_state.txt", "w");

    if (file == NULL) {
        printf("Error: Unable to create game_state.txt\n");
        return;
    }

    fprintf(file, "Game state data...\n");
    fprintf(file, "Spaceship_x: 39\n");
    fprintf(file, "High score: 0\n");
      
    fclose(file);
}

// Function to display a loading screen animation
void load_screen_animation() {
    const char* filename = "game_state.txt";

    if (file_exists(filename)) {
        printf("Getting things up...\n");
    } else {
        printf("Setting things up...\n");
        create_game_state_file();
    }

    progress_bar();
    fflush(stdout);
}

// Function to move the cursor to specified position
// row -> specifies to what row and col -> specifies to what col the cursor should move to
void move_cursor(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

// Function to display a progress bar
// progress should be between 0 and 100
void progress_bar(int progress) {
    int total = 30;

    for (int i = 0; i <= total; i++) {
        printf("\rProgress: [");
        for (int j = 0; j < i; j++) printf(GREEN "=" RESET);
        for (int j = i; j < total; j++) printf(" ");
        printf("] %d%%", (i * 100) / total);
        fflush(stdout);
        usleep(100000); // 100ms delay
    }
	
    printf("\r" RED "Loading complete!" RESET "                          ");
}

// Signal handler for resizing the terminal
void handle_resize(int sig) {
    printf("Terminal resized! Exiting the game.\n");
    exit(0); // Exit the program
}

// Function to exit the program when the terminal is resized
void resize_kill() {
	while(1){
		signal(SIGWINCH, handle_resize);
	}
}

// Function to print the score
void print_score(int score) {
    move_cursor(24, 0);
    printf("Score: %d", score);
    fflush(stdout);
}

// Function that will run in a separate thread
int read_game_state() {
	int temp;
	FILE *file = fopen("game_state.txt", "r");
	if (file == NULL) {
		printf("Error: Unable to open game_state.txt\n");
		exit(0);
	}

	char line[100];
	while (fgets(line, sizeof(line), file)) {
		if (sscanf(line, "Spaceship_x: %d", &temp) == 1) {
		    break;  // Stop reading if we found the line with "Spaceship_x"
		}
	}

	fclose(file);
	return temp;
}

// Function to enable non-blocking input
void enable_non_blocking_input() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);  // Get current terminal attributes
    term.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);  // Apply changes immediately
}

// Function to restore terminal settings
void disable_non_blocking_input() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);  // Restore canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to update the high score if the current score is higher
void update_high_score(int current_score) {
    FILE *file = fopen("game_state.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open game state file for reading.\n");
        return;
    }

    char lines[10][100];
    int line_count = 0, existing_high_score = 0;

    // Read all lines into the array
    while (fgets(lines[line_count], sizeof(lines[line_count]), file)) {
        if (strstr(lines[line_count], "High score:")) {
            sscanf(lines[line_count], "High score: %d", &existing_high_score);
        }
        line_count++;
    }
    fclose(file);

    // Compare and update the high score if needed
    if (current_score > existing_high_score) {
        printf(GREEN "New high score: %d! (Previous: %d)" RESET "\n", current_score, existing_high_score);

        for (int i = 0; i < line_count; i++) {
            if (strstr(lines[i], "High score:")) {
                sprintf(lines[i], "High score: %d\n", current_score);
                break;
            }
        }

        // Write the updated content back to the file
        file = fopen("game_state.txt", "w");
        if (file == NULL) {
            printf("Error: Unable to open game state file for writing.\n");
            return;
        }

        for (int i = 0; i < line_count; i++) {
            fputs(lines[i], file);
        }
        fclose(file);
    } else {
        printf(RED "Current score (%d) did not beat the high score (%d)." RESET "\n", current_score, existing_high_score);
    }
}

void print_health(int power) {
    move_cursor(24, 68);
    
    if (power == 100){
    	printf(GREEN "Health: %d" RESET, power);
    }
    else if (power >= 80) {
        printf(GREEN "Health: %d" RESET " ", power);
    } else if (power >= 40 && power < 80) {
        printf(YELLOW "Health: %d" RESET " ", power);
    } else if (power > 0) {
        printf(RED "Health: %d" RESET " ", power);
    } else{
    	printf(RED "Health: %d" RESET "  ", power);
    }
    fflush(stdout);
}

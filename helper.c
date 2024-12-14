#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include "helper.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m" // Reset to default color

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
    printf("Press any key to launch your mission...\n");
    getchar(); // Wait for user input
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

    // Write some initial data to the file (optional)
    fprintf(file, "Game state data...\n");
    fprintf(file, "Spaceship_x: 39\n");
    fprintf(file, "Score: 0\n");
      
    // Close the file after writing
    fclose(file);
}

// Function to display a loading screen animation
void load_screen_animation() {
    const char* filename = "game_state.txt";  // File name to check for

    if (file_exists(filename)) {
        printf("Saved game state exists... Do you want to continue? (yes/no): ");
        
        char user_input[4];  // To store user input ("yes" or "no")
        fgets(user_input, sizeof(user_input), stdin);  // Use fgets to avoid overflow

        // Remove trailing newline character from fgets
        user_input[strcspn(user_input, "\n")] = '\0';

        if (strcmp(user_input, "yes") == 0) {
            printf("Fetching previous game state...\n");
        } else if (strcmp(user_input, "no") == 0) {
            printf("Making game state...\n");
            create_game_state_file();
        } else {
            printf("Invalid input. Exiting...\n");
            return;
        }
    } else {
        printf("Making game state...\n");
        create_game_state_file();
    }

    // Show loading animation
    for (int i = 0; i < 5; i++) {
        printf("Loading");
        for (int j = 0; j < i; j++) {
            printf(".");
            fflush(stdout);
            usleep(500000); // Sleep for 0.5 seconds
        }
        printf("\r");
    }
    printf("Loading complete!\n");
}

// Function to move the cursor left or right
// direction > 0 -> Move right, direction < 0 -> Move left
void move_cursor_LR(int direction) {
    if (direction > 0) {
        printf("\033[1C"); // Move cursor 1 step to the right
    } else if (direction < 0) {
        printf("\033[1D"); // Move cursor 1 step to the left
    }
}

// Function to move the cursor up or down
// direction > 0 -> Move down, direction < 0 -> Move up
void move_cursor_UD(int direction) {
    if (direction > 0) {
        printf("\033[1B"); // Move cursor 1 step down
    } else if (direction < 0) {
        printf("\033[1A"); // Move cursor 1 step up
    }
}

// Function to move the cursor to specified position
// row -> specifies to what row and col -> specifies to what col the cursor should move to
void move_cursor(int row, int col) {
    printf("\033[%d;%dH", row, col);  // Move cursor to (row, col)
}

// Function to display a progress bar
// progress should be between 0 and 100
void progress_bar(int progress) {
    int total = 30;

    for (int i = 0; i <= total; i++) {
        printf("\rProgress: [");
        for (int j = 0; j < i; j++) printf(GREEN "=" RESET); // Completed part in green
        for (int j = i; j < total; j++) printf(" "); // Remaining part
        printf("] %d%%", (i * 100) / total);
        fflush(stdout);
        usleep(100000); // 100ms delay
    }

    printf("\n" RED "Task Completed!\n" RESET);
}

// Signal handler for resizing the terminal
void handle_resize(int sig) {
    printf("Terminal resized! Exiting the program.\n");
    exit(0); // Exit the program
}

// Function to exit the program when the terminal is resized
void resize_kill() {
	while(1){
		signal(SIGWINCH, handle_resize);
	}
}

// Function to print the spaceship
void print_spaceship(int x, int y) {
    move_cursor(y, x);
    printf("%s", spaceship);
}

// Function to print the asteroid
void print_asteroid(int x, int y) {
    move_cursor(y, x);
    printf("%s", asteroid);
}

// Function that will run in a separate thread
int read_game_state() {
	int temp;
	FILE *file = fopen("game_state.txt", "r");  // Open the game_state.txt file in read mode
	if (file == NULL) {
		printf("Error: Unable to open game_state.txt\n");
		exit(0);
	}

	// Read each line of the file
	char line[100];  // A buffer to store each line from the file
	while (fgets(line, sizeof(line), file)) {
		// Try to match the format "Spaceship_x: <value>"
		if (sscanf(line, "Spaceship_x: %d", &temp) == 1) {
		    break;  // Stop reading if we found the line with "Spaceship_x"
		}
	}

	fclose(file);  // Close the file
	return temp;
}



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "helper.h"

#define SPACESHIP_Y 22
#define SCREEN_WIDTH 80
#define MAX_ASTEROIDS 100

int spaceship_x;  // Variable to store the parsed value
int score = 0;

typedef struct {
    int x;  // Asteroid x-coordinate
    int y;  // Asteroid y-coordinate
    int active;  // Is the asteroid active?
} Asteroid;

pthread_mutex_t print_mutex;

Asteroid asteroids[MAX_ASTEROIDS];  // Array to store asteroid positions
int current_asteroids = 1;  // Start with 1 asteroid

// Function declarations for threads
void* thread_spaceship(void* arg);
void* thread_asteroid(void* arg);
void* thread_score(void* arg);

// bounds for asteroid x -> [0, 80], y-> [0, 22]
// bounds for spaceship x -> [0, 78], y -> [0, 23]
// further more the asteroid should originate at x -> [0, 80] and decrement in y upto 22
// while spaceship is at y = 22 and can move in x -> [0,78]

int main() {
    system("clear");
    display_intro();
    sleep(3);
    spaceship_x = read_game_state();
    
    system("clear");
    
    sleep(2);
    
    pthread_mutex_init(&print_mutex, NULL);
    
    // Declare thread IDs
    pthread_t spaceship_thread, asteroid_thread, score_thread;
    
    // Create threads
    if (pthread_create(&spaceship_thread, NULL, thread_spaceship, NULL) != 0) {
        printf("Error: Unable to create spaceship thread.\n");
        return 1;
    }

    if (pthread_create(&asteroid_thread, NULL, thread_asteroid, NULL) != 0) {
        printf("Error: Unable to create asteroid thread.\n");
        return 1;
    }

    if (pthread_create(&score_thread, NULL, thread_score, NULL) != 0) {
        printf("Error: Unable to create score thread.\n");
        return 1;
    }

    // Wait for the threads to finish (they run indefinitely)
    pthread_join(spaceship_thread, NULL);
    pthread_join(asteroid_thread, NULL);
    pthread_join(score_thread, NULL);
    
    // Destroy the mutex
    pthread_mutex_destroy(&print_mutex);

    return 0;
}

// Function to simulate spaceship thread
void* thread_spaceship(void* arg) {
    char input;
    enable_non_blocking_input();  // Enable non-blocking input mode
    
    
    while (1) {
    	move_cursor(SPACESHIP_Y, spaceship_x);
	printf("|-|");
	fflush(stdout);
	
        // Check for user input
        if (read(STDIN_FILENO, &input, 1) > 0) {
            pthread_mutex_lock(&print_mutex);
            if (input == 'a' && spaceship_x > 0) {  // Move left
            	move_cursor(SPACESHIP_Y, spaceship_x+1);
            	printf("  ");
            	fflush(stdout);
                spaceship_x--;
            } else if (input == 'd' && spaceship_x < SCREEN_WIDTH - 2) {  // Move right
            	move_cursor(SPACESHIP_Y, spaceship_x);
            	printf("  ");
            	fflush(stdout);
                spaceship_x++;
            } else if (input == 'q') {  // Quit the game
                disable_non_blocking_input();
                printf("Quiting game...!\n");
                fflush(stdout);
                update_high_score(score);
                sleep(3);
                exit(0);
            }
            pthread_mutex_unlock(&print_mutex);
        }

        // Print spaceship at updated position
        move_cursor(SPACESHIP_Y, spaceship_x);
	printf("|-|");
	fflush(stdout);
        pthread_mutex_unlock(&print_mutex);

        usleep(20000);  // Control spaceship movement speed
    }

    disable_non_blocking_input();  // Restore terminal settings
    return NULL;
}

// Function to simulate asteroid generation and movement thread
void* thread_asteroid(void* arg) {
    // Initialize the asteroid positions
    srand(time(NULL));
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        asteroids[i].x = rand() % SCREEN_WIDTH;
        asteroids[i].y = 0;
        asteroids[i].active = 0;  // Initially inactive
    }

    int frame_counter = 0;  // Counter to manage difficulty increase

    while (1) {
        pthread_mutex_lock(&print_mutex);  // Lock for printing

        // Clear old asteroid positions
        for (int i = 0; i < current_asteroids; i++) {
            if (asteroids[i].active) {
                move_cursor(asteroids[i].y, asteroids[i].x);
                printf(" ");  // Clear the previous position
                fflush(stdout);
            }
        }

        // Update asteroid positions and check for collision
        for (int i = 0; i < current_asteroids; i++) {
            if (asteroids[i].active) {
            	if (frame_counter < 1000)
                	asteroids[i].y += 1;  // Move asteroid downward
                else if (frame_counter > 1000 && frame_counter < 2000)
                	asteroids[i].y += 2;
                else
                	asteroids[i].y += 4;

                // Check for collision with the spaceship
                if (asteroids[i].y == SPACESHIP_Y && asteroids[i].x == spaceship_x || asteroids[i].y == SPACESHIP_Y && asteroids[i].x == spaceship_x+1 || asteroids[i].y == SPACESHIP_Y && asteroids[i].x == spaceship_x+2) {
                    move_cursor(SPACESHIP_Y + 2, 0);  // Move cursor below game area
                    printf("Game Over! Asteroid hit the spaceship!\n");
                    fflush(stdout);
                    update_high_score(score);
                    sleep(3);
                    pthread_mutex_unlock(&print_mutex);  // Unlock before exiting
                    exit(0);  // Terminate the game
                }

                // If asteroid goes off the screen, reset its position
                if (asteroids[i].y > SPACESHIP_Y) {
                    asteroids[i].x = rand() % SCREEN_WIDTH;
                    asteroids[i].y = 0;
                }
            } else {
                // Activate asteroid if inactive
                asteroids[i].x = rand() % SCREEN_WIDTH;
                asteroids[i].y = 0;
                asteroids[i].active = 1;
            }
        }

        // Print updated asteroid positions
        for (int i = 0; i < current_asteroids; i++) {
            if (asteroids[i].active) {
                move_cursor(asteroids[i].y, asteroids[i].x);
                printf("*");  // Print asteroid
                fflush(stdout);
            }
        }

        fflush(stdout);
        pthread_mutex_unlock(&print_mutex);  // Unlock printing

        usleep(300000);  // Control speed (0.3 seconds)

        // Increase difficulty by adding more asteroids over time
        frame_counter++;
        if (frame_counter % 20 == 0 && current_asteroids < MAX_ASTEROIDS) { 
            current_asteroids+=5;  // Add a new asteroid every 20 frames
        }
    }
    return NULL;
}

// Function to simulate score update thread
void* thread_score(void* arg) {
    while(1) {
        score += 10;  // Increase score (simulating score update)
        pthread_mutex_lock(&print_mutex);
        print_score(score);
        pthread_mutex_unlock(&print_mutex);
        usleep(300000);
    }
    return NULL;
}


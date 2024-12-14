#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "helper.h"

#define SPACESHIP_Y 22
#define SCREEN_WIDTH 80
#define MAX_ASTEROIDS 100

// Global Variables
int spaceship_x;
int score = 0;
int power = 100;

// Monitor all 100 Asteroids
typedef struct {
    int x;
    int y;
    int active;
} Asteroid;

// Mutex for thread syncronization
pthread_mutex_t print_mutex;

// Array to store asteroid positions
Asteroid asteroids[MAX_ASTEROIDS];

// Initially start with 1 asteroid
int current_asteroids = 1;

// Function declarations for threads
void* thread_spaceship(void* arg);
void* thread_asteroid(void* arg);
void* thread_score(void* arg);
void* thread_power(void* arg);
void* thread_kill(void* arg);

int main() {
    system("clear");
    // Display Intro
    display_intro();
    sleep(3);
    
    //Read game state
    spaceship_x = read_game_state();
    system("clear");
    sleep(2);
    
    // Initialize mutex to NULL
    pthread_mutex_init(&print_mutex, NULL);
    
    // Declare thread IDs
    pthread_t spaceship_thread, asteroid_thread, score_thread, power_thread, enforcer_thread;
    
    // Create threads
    // Runs indefinitely, checks for change in terminal size, and kills the process if the terminal is tried to resize
    if (pthread_create(&enforcer_thread, NULL, thread_kill, NULL) != 0) {
        printf("Error: Unable to create score thread.\n");
        return 1;
    }
    
    // Runs indefinitely, handles spaceship position and related position updates
    if (pthread_create(&spaceship_thread, NULL, thread_spaceship, NULL) != 0) {
        printf("Error: Unable to create spaceship thread.\n");
        return 1;
    }
	
    // Runs indefinitely, handles asteroid count, positions, speed, collision with spaceship
    if (pthread_create(&asteroid_thread, NULL, thread_asteroid, NULL) != 0) {
        printf("Error: Unable to create asteroid thread.\n");
        return 1;
    }
    
    // Runs indefinitely, prints score
    if (pthread_create(&score_thread, NULL, thread_score, NULL) != 0) {
        printf("Error: Unable to create score thread.\n");
        return 1;
    }
    
    // Runs indefinitely, prints the health of the spaceship
    if (pthread_create(&power_thread, NULL, thread_power, NULL) != 0) {
        printf("Error: Unable to create score thread.\n");
        return 1;
    }

    // Wait for the threads to finish (they run indefinitely)
    pthread_join(spaceship_thread, NULL);
    pthread_join(asteroid_thread, NULL);
    pthread_join(score_thread, NULL);
    pthread_join(power_thread, NULL);
    pthread_join(enforcer_thread, NULL);
    
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
            // Move left
            if (input == 'a' && spaceship_x > 0) {  
            	move_cursor(SPACESHIP_Y, spaceship_x+1);
            	printf("  ");
            	fflush(stdout);
                spaceship_x--;
            }
            // Move right 
            else if (input == 'd' && spaceship_x < SCREEN_WIDTH - 2) {  
            	move_cursor(SPACESHIP_Y, spaceship_x);
            	printf("  ");
            	fflush(stdout);
                spaceship_x++;
            } 
            // Quit the game
            else if (input == 'q') {  
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

        usleep(20000);
    }
    
    // Restore terminal settings
    disable_non_blocking_input();  
    return NULL;
}

// Function to simulate asteroid generation and movement thread
void* thread_asteroid(void* arg) {
    // Initialize the asteroid positions
    srand(time(NULL));
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        asteroids[i].x = rand() % SCREEN_WIDTH;
        asteroids[i].y = 0;
        asteroids[i].active = 0;
    }
    
    // Counter to manage difficulty increase
    int frame_counter = 0;  

    while (1) {
        pthread_mutex_lock(&print_mutex);

        // Clear old asteroid positions
        for (int i = 0; i < current_asteroids; i++) {
            if (asteroids[i].active) {
                move_cursor(asteroids[i].y, asteroids[i].x);
                printf(" ");
                fflush(stdout);
            }
        }

        // Update asteroid positions and check for collision
        for (int i = 0; i < current_asteroids; i++) {
            // Speed of asteroid 
            if (asteroids[i].active) {
            	if (frame_counter < 1000)
                	asteroids[i].y += 1;
                else if (frame_counter > 1000 && frame_counter < 2000)
                	asteroids[i].y += 2;
                else
                	asteroids[i].y += 4;

                // Check for collision with the spaceship
                if (asteroids[i].y == SPACESHIP_Y && asteroids[i].x == spaceship_x || asteroids[i].y == SPACESHIP_Y && asteroids[i].x == spaceship_x+1 || asteroids[i].y == SPACESHIP_Y && asteroids[i].x == spaceship_x+2) {
                    if ((power-20)==0) {
                    	print_health(0);
                    	move_cursor(SPACESHIP_Y + 2, 0);
                    	printf("Game Over! Asteroid hit the spaceship!\n");
                    	fflush(stdout);
                    	disable_non_blocking_input(); 
                    	update_high_score(score);
                    	sleep(3);
                    	pthread_mutex_unlock(&print_mutex);
                    	exit(0);
                    }
                    else{
                    	power -= 20;
                    }
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
                printf("*");
                fflush(stdout);
            }
        }

        fflush(stdout);
        pthread_mutex_unlock(&print_mutex);

        usleep(300000);

        // Increase difficulty by adding more asteroids over time
        frame_counter++;
        if (frame_counter % 20 == 0 && current_asteroids < MAX_ASTEROIDS) { 
            current_asteroids+=5;
        }
    }
    return NULL;
}

// Function to score update thread
void* thread_score(void* arg) {
    while(1) {
        score += 10;
        pthread_mutex_lock(&print_mutex);
        print_score(score);
        pthread_mutex_unlock(&print_mutex);
        usleep(300000);
    }
    return NULL;
}

// Function to display health
void* thread_power(void* arg){
	while(1){
		pthread_mutex_lock(&print_mutex);
        	print_health(power);
        	pthread_mutex_unlock(&print_mutex);
        	usleep(300000);
	}
}

// Resize kill
void* thread_kill(void *arg) {
	while(1) {
		resize_kill();
	}
}

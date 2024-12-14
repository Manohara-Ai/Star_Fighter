#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "helper.h"

#define SPACESHIP_Y 24
int spaceship_x;  // Variable to store the parsed value

// bounds for asteroid x -> [0, 80], y-> [0, 24]
// bounds for spaceship x -> [0, 78], y -> [0, 24]
// further more the asteroid should originate at x -> [0, 80] and decrement in y upto 24
// while space ship is at y = 24 and can move in x -> [0,78]

int main() {
	system("clear");
	display_intro();
	sleep(3);
	spaceship_x = read_game_state();
	system("clear");
	
	while(1){
		print_spaceship(spaceship_x, SPACESHIP_Y);
		break;
	}
	return 0;
}


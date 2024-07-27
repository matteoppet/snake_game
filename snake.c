#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <conio.h>


// global var
#define ROWS 30
#define COLS 30
#define MAX_SNAKE_SIZE ROWS*COLS

char BOARD[ROWS][COLS];
int alive = true;

struct Player {
	int length;	
	int direction;
};


typedef struct {
	int x;
	int y;
} Position;


struct Apple {
	int x;
	int y;
};


// prototypes functions
void game();
void move_snake(struct Player *p, Position snake[]);
void display_board();
void update_board(Position snake[], int player_length, struct Apple apple);
void handle_movements(struct Player *p);
int spawn_apple(struct Apple *apple);
void eat_apple(struct Player *pPlayer);


int main() {
	game();
	
	return 0;
}


void game() {
	struct Player player;
	player.length = 1;
	player.direction = 1;
	
	Position snake[MAX_SNAKE_SIZE];
	snake[0].x = 1;
	snake[1].y = 1;

	struct Player *pPlayer = &player; // pointer of the struct to change its values
	
	struct Apple apple;
	apple.x = 1;
	apple.y = 1;
	struct Apple *pApple = &apple;
	
	system("cls"); // clean terminal	
	while (alive == true) {
		printf("\e[?25l"); // hide cursor
		
		move_snake(pPlayer, snake);
		
		// check eat apple
		if (BOARD[snake[0].y][snake[0].x] == 'O') {	
			spawn_apple(pApple);
			eat_apple(pPlayer);
		}
	
		// check death
		if (BOARD[snake[0].y][snake[0].x] == 'X') {
			alive = false;
		}

		update_board(snake, player.length, apple);
		display_board();
		
		printf("\nLength: %d\n", player.length);
		printf("\e[%iA", ROWS+2); // move cursor back to top
	
		sleep(0.7);
	}

	if (alive == false) {
		system("cls");
		printf("\e[%iA", ROWS+2); // move cursor back to top
		printf("\nYou lost, points: %d", player.length);	
	} 
}



void move_snake(struct Player *p, Position snake[]) {
	// move head
	handle_movements(p);

	// shifting array
	for (int i = p->length; i > 0; i--) {
		snake[i] = snake[i - 1];
	}
	
	Position newHead = snake[0];	
	// change head position
	if (p->direction == -1) {
		newHead.x -= 1;
		// snake[0].x -= 1;
	} else if (p->direction == 0) {
		newHead.y -= 1;
		// snake[0].y -= 1;
	} else if (p->direction == 1) {
		newHead.x += 1;
		// snake[0].x += 1;
	} else if (p->direction == 2) {
		newHead.y += 1;
		// snake[0].y += 1;
	}

	if (newHead.x < 0) {
		newHead.x = COLS-1;
	} else if (newHead.x > COLS-1) {
		newHead.x = 0;
	} else if (newHead.y < 0) {
		newHead.y = ROWS-1;
	} else if (newHead.y > ROWS-1) {
		newHead.y = 0;
	}

	// update head position on the array
	snake[0] = newHead;
}



void display_board(struct Player p) {
	for (int i=0; i < ROWS; i++) {
		for (int j=0; j < COLS; j++) {
			printf("%c ", BOARD[i][j]);
		}
		printf("\n");
	}
}


void update_board(Position snake[], int player_length, struct Apple apple) {
	for (int i=0; i < ROWS; i++) {
		for (int j=0; j < COLS; j++) {
			BOARD[i][j] = '.';
		}
	}

	// make the length of the player
	BOARD[apple.y][apple.x] = 'O';	
	
	for (int k = 0; k<player_length; k++) {
		BOARD[snake[k].y][snake[k].x] = 'X';
	}
}


void handle_movements(struct Player *p) {
	int c;
	
	if (_kbhit()) {
		c = getch();

		if (c == 119) {	
			p->direction = 0;
		} else if (c == 115) {
			p->direction = 2;
		} else if (c == 97) {
			p->direction = -1;
		} else if (c == 100) {
			p->direction = 1;
		}
	}
}


int spawn_apple(struct Apple *apple) {
	int x_apple;
	int y_apple;

	do {
		x_apple = rand() % COLS;
		y_apple = rand() % ROWS;
	
	} while (BOARD[x_apple][y_apple] == 'X'); 
		
	apple->x = x_apple;
	apple->y = y_apple;
}


void eat_apple(struct Player *p) {
	p->length += 1;	
}

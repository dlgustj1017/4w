#include <bangtal.h>
#include <time.h>
#include <stdlib.h>


SceneID scene;
ObjectID game_board[16], init_board[16];
ObjectID start;

const char* board_image[16] = {
	"1.png",
	"2.png",
	"3.png",
	"4.png",
	"5.png",
	"6.png",
	"7.png",
	"8.png",
	"9.png",
	"10.png",
	"11.png",
	"12.png",
	"13.png",
	"14.png",
	"15.png",
	"16.png",
};
const int board_x[16] = { 300, 450, 600, 750, 300, 450, 600, 750, 300, 450, 600, 750, 300, 450, 600, 750, };
const int board_y[16] = { 470, 470, 470, 470, 320, 320, 320, 320, 170, 170, 170, 170, 20, 20, 20, 20, };

int blank;

TimerID timer;
int mixCount;

int board_index(ObjectID object) {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] == object) return i;
	}
	return -1; 
}

void board_move(int index) {
	ObjectID t = game_board[blank];
	game_board[blank] = game_board[index];
	game_board[index] = t;

	locateObject(game_board[blank], scene, board_x[blank], board_y[blank]);
	locateObject(game_board[index], scene, board_x[index], board_y[index]);

	blank = index;
}

bool movable(int index) {
	if (blank < 0 or blank > 15) return false;

	if (blank % 4 != 0 and blank - 1 == index) return true;
	if (blank % 4 != 3 and blank + 1 == index) return true;
	if (blank / 4 != 0 and blank - 4 == index) return true;
	if (blank / 4 != 3 and blank + 4 == index) return true;

	return false;
}

void board_mix() {
	int index;


	do {
		switch (rand() % 4) {
		case 0: index = blank - 1; break;
		case 1: index = blank + 1; break;
		case 2: index = blank - 4; break;
		case 3: index = blank + 4; break;
		}
	} while (!movable(index));
	board_move(index);
}

bool completed() {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] != init_board[i]) return false;
	}

	return true;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == start) {
		mixCount = 10;
		setTimer(timer, 1.f);
		startTimer(timer);

	}
	else { 
		int index = board_index(object);
		if (movable(index)) {
			board_move(index);

			if (completed()) {
				showMessage("Completed!!");
			}
		}	
	}
}

void timerCallback(TimerID timer) {
	mixCount--;

	if (mixCount > 0) {
		board_mix();
		
		setTimer(timer, 1.f);
		startTimer(timer);
	}	 
}

int main() {
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene = createScene("겨울왕국", "배경.png");

	for (int i = 0; i < 16; i++) {
		game_board[i] = createObject(board_image[i]);
		init_board[i] = game_board[i];

		locateObject(game_board[i], scene, board_x[i], board_y[i]);
		showObject(game_board[i]);
	}

	blank = 15;

	hideObject(game_board[blank]);

	start = createObject("start.png");
	locateObject(start, scene, 540, 100);
	showObject(start);

	timer = createTimer(1.f);
	

	startGame(scene);

	return 0;
}
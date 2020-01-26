// jjj.cpp : This file contains the 'main' function. Program execution begins and ends there.// Quoridor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<raylib.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<stdio.h>
#pragma warning(disable:4996) 

struct wall_v
{
	int s;
	Rectangle wall;
};
struct wall_h
{
	int s;
	Rectangle wall;
};
struct Box
{
	int s;
	Rectangle box;
};
struct meet
{
	int s;
	Rectangle m;
};
struct moodPosition
{
	Rectangle oneMood;
	Rectangle twoMood;
	Rectangle saveMood;
};

enum terrain {
	empty,
	wall,
	goal,
	path
};

typedef enum {
	player1, player2, computer
}Turn;

enum coller {
	red = 2, black
};

typedef enum {
	ONE, TWO
}MOOD;

const float  size = 60;
const float w = 1080;
const float h = 720;
const float bx = 270;
const float by = 90;
int Xred, Xblack, Yred, Yblack;
int counter = 0;
int MAP[17][17] = { 0 };
int visited[17][17];
int rows = 17;
int cols = 17;
int winner = -2;
int NumberOfWallsP1 = 10;
int NumberOfWallsP2 = 10;
int NumberOfWallsCom = 10;

Turn turn;
MOOD mood;

Box map[9][9];
wall_h wallh[9][8];
wall_v wallv[8][9];
meet inter[8][8];
moodPosition MOODGAME;

void setConsts() {
	Xred = 0;
	Xblack = 8;
	Yred = 4;
	Yblack = 4;
	turn = player2;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			map[i][j].s = 0;
			map[i][j].box = Rectangle{ bx + j * size + 6,by + i * size + 6, size - 12, size - 12 };
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			wallh[i][j].s = 0;
			wallh[i][j].wall = Rectangle{ bx + (j + 1) * size - 6,by + i * size,12, size };
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			wallv[i][j].s = 0;
			wallv[i][j].wall = Rectangle{ bx + (j)* size ,by + (i + 1) * size - 6,size,12 };
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			inter[i][j].s = 0;
			inter[i][j].m = Rectangle{ bx + (j + 1) * size - 6,by + (i + 1) * size - 6, 12, 12 };
		}
	}
	MOODGAME.oneMood = { bx + (3 * size), by + (3 * size), 100, 100 };
	MOODGAME.twoMood = { bx + (3 * size), by + (6 * size), 100, 100 };
	MOODGAME.saveMood = { size,335,100,50 };
}

int setMOOD(Vector2 x) {
	if (CheckCollisionPointRec(x, MOODGAME.oneMood)) {
		mood = ONE;
		return 0;
	}
	if (CheckCollisionPointRec(x, MOODGAME.twoMood)) {
		mood = TWO;
		return 0;
	}

	return 1;
}
int checkWinner() {
	if (Xred == 8) {
		winner = red;
		return 0;
	}
	if (Xblack == 0) {
		winner = black;
		return 0;
	}
	return 1;
}
void changeTurn() {
	if (mood == TWO) {
		if (turn == player1) {
			turn = player2;
			return;
		}
		else {
			turn = player1;
			return;
		}
	}
	if (mood == ONE) {
		if (turn == player2) {
			turn = computer;
			return;
		}
		else {
			turn = player2;
			return;
		}
	}
}

int checkInter(Vector2 x) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (CheckCollisionPointRec(x, inter[i][j].m)) {
				return 0;
			}
		}
	}
	return 1;
}
void checkWall() {
	if (mood == TWO) {
		if (turn == player1) {
			NumberOfWallsP1--;
			return;
		}
		else {
			NumberOfWallsP2--;
			return;
		}
	}
	if (mood == ONE) {
		if (turn == player2) {
			NumberOfWallsP2--;
			return;
		}
		else {
			NumberOfWallsCom--;
			return;
		}
	}
}

void setMap(int a) {
	memset(MAP, 0, 4 * 17 * 17);

	//system("cls");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			if (wallh[i][j].s == 1)
				MAP[2 * i][2 * j + 1] = 1;
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			if (wallv[i][j].s == 1)
				MAP[2 * i + 1][2 * j] = 1;
		}
	}
	for (int i = 1; i < 17; i += 2) {
		for (int j = 1; j < 17; j += 2) {
			MAP[i][j] = 1;
		}
	}
	if (a == 0) {
		for (int i = 0; i < 17; i += 2) {
			MAP[0][i] = goal;
		}
	}
	else {
		for (int i = 0; i < 17; i += 2) {
			MAP[16][i] = goal;
		}
	}

	if (a == 0) {

		MAP[2 * Xblack][2 * Yblack] = 0;
	}
	else {
		MAP[2 * Xred][2 * Yred] = 0;
	}


}
int search(int row, int col) {
	if (row < 0 || col < 0 || row>16 || col >16) {

		return 0;
	}
	int* current = &MAP[row][col];
	if (*current == goal) {
		return 1;
	}
	if (*current == empty) {
		*current = path;
		if (search(row + 1, col) || search(row, col - 1) || search(row - 1, col) || search(row, col + 1)) {

			//*current = path;
			return 1;

		}
		*current = empty;
	}
	return 0;
}
int pathFind() {
	int result = -2;
	setMap(0);
	system("cls");

	for (int i = 0; i < 17; ++i) {
		for (int j = 0; j < 17; j++)
		{
			printf_s("%d\t", MAP[i][j]);
		}
		puts(" ");
	}
	printf("\n");
	result += search(2 * Xblack, 2 * Yblack);

	setMap(1);
	/*for (int i = 0; i < 17; ++i) {
		for (int j = 0; j < 17; j++)
		{
			printf_s("%d\t", MAP[i][j]);
		}
		puts(" ");
	}
	printf("\n");*/

	result += search(2 * Xred, 2 * Yred);

	printf("%d\n", result);
	return result;
}

void setBallPlayer(Vector2 x) {
	if (turn == player2) {
		if (CheckCollisionPointRec(x, map[Xred][Yred + 1].box) && map[Xred][Yred + 1].s == 0 && wallh[Xred][Yred].s != 1 && Yred < 8) {
			map[Xred][Yred].s = 0;
			map[Xred][Yred + 1].s = red;
			Yred++;
			changeTurn();
			return;
		}
		if (CheckCollisionPointRec(x, map[Xred][Yred - 1].box) && map[Xred][Yred - 1].s == 0 && wallh[Xred][Yred - 1].s != 1 && Yred > 0) {
			map[Xred][Yred].s = 0;
			map[Xred][Yred - 1].s = red;
			Yred--;
			changeTurn();
			return;
		}
		if (CheckCollisionPointRec(x, map[Xred + 1][Yred].box) && map[Xred + 1][Yred].s == 0 && wallv[Xred][Yred].s != 1 && Xred < 8) {
			map[Xred][Yred].s = 0;
			map[Xred + 1][Yred].s = red;
			Xred++;
			changeTurn();
			return;
		}
		if (CheckCollisionPointRec(x, map[Xred - 1][Yred].box) && map[Xred - 1][Yred].s == 0 && wallv[Xred - 1][Yred].s != 1 && Xred > 0) {
			map[Xred][Yred].s = 0;
			map[Xred - 1][Yred].s = red;
			Xred--;
			changeTurn();
			return;
		}
		if ((map[Xred + 1][Yred].s == black && wallv[Xred][Yred].s != 1) || (map[Xred - 1][Yred].s == black && wallv[Xred - 1][Yred].s != 1) || (map[Xred][Yred + 1].s == black && wallh[Xred][Yred].s != 1) || (map[Xred][Yred - 1].s == black && wallh[Xred][Yred - 1].s != 1)) {

			if (CheckCollisionPointRec(x, map[Xblack][Yblack + 1].box) && map[Xblack][Yblack + 1].s == 0 && wallh[Xblack][Yblack].s != 1 && Yblack < 8) {
				map[Xred][Yred].s = 0;
				map[Xblack][Yblack + 1].s = red;
				Yred = Yblack + 1;
				Xred = Xblack;
				changeTurn();
				return;

			}
			if (CheckCollisionPointRec(x, map[Xblack][Yblack - 1].box) && map[Xblack][Yblack - 1].s == 0 && wallh[Xblack][Yblack - 1].s != 1 && Yblack > 0) {
				map[Xred][Yred].s = 0;
				map[Xblack][Yblack - 1].s = red;
				Yred = Yblack - 1;
				Xred = Xblack;
				changeTurn();
				return;
			}
			if (CheckCollisionPointRec(x, map[Xblack + 1][Yblack].box) && map[Xblack + 1][Yblack].s == 0 && wallv[Xblack][Yblack].s != 1 && Xblack < 8) {
				map[Xred][Yred].s = 0;
				map[Xblack + 1][Yblack].s = red;
				Yred = Yblack;
				Xred = Xblack + 1;
				changeTurn();
				return;
			}
			if (CheckCollisionPointRec(x, map[Xblack - 1][Yblack].box) && map[Xblack - 1][Yblack].s == 0 && wallv[Xblack - 1][Yblack].s != 1 && Xblack > 0) {
				map[Xred][Yred].s = 0;
				map[Xblack - 1][Yblack].s = red;
				Yred = Yblack;
				Xred = Xblack - 1;
				changeTurn();
				return;
			}
		}

	}
	if (turn == player1) {
		if (CheckCollisionPointRec(x, map[Xblack][Yblack + 1].box) && map[Xblack][Yblack + 1].s != red && wallh[Xblack][Yblack].s != 1 && Yblack < 8) {
			map[Xblack][Yblack].s = 0;
			map[Xblack][Yblack + 1].s = black;
			Yblack++;
			changeTurn();
			return;

		}
		if (CheckCollisionPointRec(x, map[Xblack][Yblack - 1].box) && map[Xblack][Yblack - 1].s != red && wallh[Xblack][Yblack - 1].s != 1 && Yblack > 0) {
			map[Xblack][Yblack].s = 0;
			map[Xblack][Yblack - 1].s = black;
			Yblack--;
			changeTurn();
			return;
		}
		if (CheckCollisionPointRec(x, map[Xblack + 1][Yblack].box) && map[Xblack + 1][Yblack].s != red && wallv[Xblack][Yblack].s != 1 && Xblack < 8) {
			map[Xblack][Yblack].s = 0;
			map[Xblack + 1][Yblack].s = black;
			Xblack++;
			changeTurn();
			return;
		}
		if (CheckCollisionPointRec(x, map[Xblack - 1][Yblack].box) && map[Xblack - 1][Yblack].s != red && wallv[Xblack - 1][Yblack].s != 1 && Xblack >= 0) {
			map[Xblack][Yblack].s = 0;
			map[Xblack - 1][Yblack].s = black;
			Xblack--;
			changeTurn();
			return;
		}
		if ((map[Xblack + 1][Yblack].s == red && wallv[Xblack][Yblack].s != 1) || (map[Xblack - 1][Yblack].s == red && wallv[Xblack - 1][Yblack].s != 1) || (map[Xblack][Yblack + 1].s == red && wallh[Xblack][Yblack].s != 1) || (map[Xblack][Yblack - 1].s == red && wallh[Xblack][Yblack - 1].s != 1)) {

			//puts("JASJHDSA");
			if (CheckCollisionPointRec(x, map[Xred][Yred + 1].box) && map[Xred][Yred + 1].s == 0 && wallh[Xred][Yred].s != 1 && Yred < 8 /*&& (abs(Yblack - Yred) == 1 || abs(Xblack - Xred) == 1)*/) {

				//puts("UP RED");
				map[Xblack][Yblack].s = 0;
				map[Xred][Yred + 1].s = black;
				Yblack = Yred + 1;
				Xblack = Xred;
				changeTurn();
				return;
			}
			if (CheckCollisionPointRec(x, map[Xred][Yred - 1].box) && map[Xred][Yred - 1].s == 0 && wallh[Xred][Yred - 1].s != 1 && Yred > 0 /*&& (abs(Yblack - Yred) == 1 || abs(Xblack - Xred) == 1)*/) {
				puts("down RED");
				map[Xblack][Yblack].s = 0;
				map[Xred][Yred - 1].s = black;
				Yblack = Yred - 1;
				Xblack = Xred;
				changeTurn();
				return;
			}
			if (CheckCollisionPointRec(x, map[Xred + 1][Yred].box) && map[Xred + 1][Yred].s == 0 && wallv[Xred][Yred].s != 1 && Xred < 8/* && (abs(Yblack - Yred) == 1 || abs(Xblack - Xred) == 1)*/) {
				puts("right RED");
				map[Xblack][Yblack].s = 0;
				map[Xred + 1][Yred].s = black;
				Xblack = Xred + 1;
				Yblack = Yred;
				changeTurn();
				return;
			}
			if (CheckCollisionPointRec(x, map[Xred - 1][Yred].box) && map[Xred - 1][Yred].s == 0 && wallv[Xred - 1][Yred].s != 1 && Xred > 0 /*&& (abs(Yblack - Yred) == 1 || abs(Xblack - Xred) == 1)*/) {
				puts("left RED");
				map[Xblack][Yblack].s = 0;
				map[Xred - 1][Yred].s = black;
				Xblack = Xred - 1;
				Yblack = Yred;
				changeTurn();
				return;
			}
		}
	}
}
void setWallPlayer(Vector2 x) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (CheckCollisionPointRec(x, wallh[i][j].wall) && wallh[i][j].s != 1 && wallh[i + 1][j].s != 1 && inter[i][j].s != 1) {
				wallh[i][j].s = 1;
				wallh[i + 1][j].s = 1;
				inter[i][j].s = 1;
				if (pathFind()) {
					//printf("impoosible\n");
					wallh[i][j].s = 0;
					wallh[i + 1][j].s = 0;
					inter[i][j].s = 0;
					return;
				}
				//printf("found\n");
				checkWall();
				changeTurn();
				return;
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (CheckCollisionPointRec(x, wallv[i][j].wall) && wallv[i][j].s != 1 && wallv[i][j + 1].s != 1 && inter[i][j].s != 1) {
				wallv[i][j].s = 1;
				wallv[i][j + 1].s = 1;
				inter[i][j].s = 1;
				if (pathFind()) {
					//printf("impoosible\n");
					wallv[i][j].s = 0;
					wallv[i][j + 1].s = 0;
					inter[i][j].s = 0;
					return;
				}
				checkWall();
				changeTurn();
				//printf("found\n");
				return;
			}
		}
	}
}
void setWallcomputer() {
	int a = rand();
	for (int i = 0; i < 8; i++) {
		if (wallh[i][a % 8].s != 1 && wallh[i + 1][a % 8].s != 1 && inter[i][a % 8].s != 1) {
			wallh[i][a % 8].s = 1;
			wallh[i + 1][a % 8].s = 1;
			inter[i][a % 8].s = 1;
			if (pathFind()) {
				//printf("impoosible\n");
				wallh[i][a % 8].s = 0;
				wallh[i + 1][a % 8].s = 0;
				inter[i][a % 8].s = 0;
				return;
			}
			//printf("found\n");
			checkWall();
			changeTurn();
			return;
		}
	}
	for (int i = 0; i < 8; i++) {
		if (wallv[i][a % 8].s != 1 && wallv[i][a % 8 + 1].s != 1 && inter[i][a % 8].s != 1) {
			wallv[i][a % 8].s = 1;
			wallv[i][a % 8 + 1].s = 1;
			inter[i][a % 8].s = 1;
			if (pathFind()) {
				//printf("impoosible\n");
				wallv[i][a % 8].s = 0;
				wallv[i][a % 8 + 1].s = 0;
				inter[i][a % 8].s = 0;
				return;
			}
			checkWall();
			changeTurn();
			//printf("found\n");
			return;
		}
	}
}
void setBallcomputer() {
	int b = rand();
	if (b % 5 == 0 && map[Xblack][Yblack + 1].s == 0 && wallh[Xblack][Yblack].s != 1 && Yblack < 8) {
		map[Xblack][Yblack].s = 0;
		map[Xblack][Yblack + 1].s = black;
		Yblack++;
		changeTurn();
		return;
	}
	if (b % 5 == 1 && map[Xblack][Yblack - 1].s == 0 && wallh[Xblack][Yblack - 1].s != 1 && Yblack > 0) {
		map[Xblack][Yblack].s = 0;
		map[Xblack][Yblack - 1].s = black;
		Yblack--;
		changeTurn();
		return;
	}
	if (b % 5 == 2 && map[Xblack + 1][Yblack].s == 0 && wallv[Xblack][Yblack].s != 1 && Xblack < 8) {
		map[Xblack][Yblack].s = 0;
		map[Xblack + 1][Yblack].s = black;
		Xblack++;
		changeTurn();
		return;
	}
	if (b % 5 == 3 && map[Xblack - 1][Yblack].s == 0 && wallv[Xblack - 1][Yblack].s != 1 && Xblack > 0) {
		map[Xblack][Yblack].s = 0;
		map[Xblack - 1][Yblack].s = black;
		Xblack--;
		changeTurn();
		return;
	}
	if ((map[Xblack + 1][Yblack].s == red && wallv[Xblack][black].s != 1) || (map[Xblack - 1][Yblack].s == red && wallv[Xblack - 1][Yblack].s != 1) || (map[Xblack][Yblack + 1].s == red && wallh[Xblack][Yblack].s != 1) || (map[Xblack][Yblack - 1].s == red && wallh[Xblack][Yblack - 1].s != 1)) {
		if (b % 5 == 0 && map[Xred][Yred + 1].s == 0 && wallh[Xred][Yred].s != 1 && Yred < 8) {
			map[Xblack][Yblack].s = 0;
			map[Xred][Yred + 1].s = black;
			Yblack = Yred + 1;
			Xblack = Xred;
			changeTurn();
			return;
		}
		if (b % 5 == 1 && map[Xred][Yred - 1].s == 0 && wallh[Xred][Yred - 1].s != 1 && Yred > 0) {
			map[Xblack][Yblack].s = 0;
			map[Xred][Yred - 1].s = black;
			Yblack = Yred - 1;
			Xblack = Xred;
			changeTurn();
			return;
		}
		if (b % 5 == 2 && map[Xred + 1][Yred].s == 0 && wallv[Xred][Yred].s != 1 && Xred < 8) {
			map[Xblack][Yblack].s = 0;
			map[Xred + 1][Yred].s = black;
			Xblack = Xred + 1;
			Yblack = Yred;
			changeTurn();
			return;
		}
		if (b % 5 == 3 && map[Xred - 1][Yred].s == 0 && wallv[Xred - 1][Yred].s != 1 && Xred > 0) {
			map[Xblack][Yblack].s = 0;
			map[Xred - 1][Yred].s = black;
			Xblack = Xred - 1;
			Yblack = Yred;
			changeTurn();
			return;
		}
	}
}

void playGameMoodTwo() {
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Vector2 mousePosition = GetMousePosition();

		if ((turn == player1 && NumberOfWallsP1 != 0) || (turn == player2 && NumberOfWallsP2 != 0) && (checkInter(mousePosition) != 0)) {
			setWallPlayer(mousePosition);
		}
		setBallPlayer(mousePosition);
	}
	return;
}

void playGameMoodOne() {
	int a = rand();
	if (turn == computer) {
		if (a % 9 > 0)
			setBallcomputer();
		if (a % 9 == 0 && NumberOfWallsCom > 0)
			setWallcomputer();
	}

	if (turn == player2) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePosition = GetMousePosition();
			if (NumberOfWallsP2 != 0) {
				setWallPlayer(mousePosition);
			}
			setBallPlayer(mousePosition);
		}
	}
	return;
}

void drawMAP() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawRectangleRec(map[Xblack][Yblack].box, BLACK);
	DrawRectangleRec(map[Xred][Yred].box, RED);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			if (wallh[i][j].s == 0) {
				DrawRectangleRec(wallh[i][j].wall, BLUE);
			}
			else {
				DrawRectangleRec(wallh[i][j].wall, BLACK);
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			if (wallv[i][j].s == 0) {
				DrawRectangleRec(wallv[i][j].wall, BLUE);
			}

			else {
				DrawRectangleRec(wallv[i][j].wall, BLACK);
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (inter[i][j].s == 1) {
				DrawRectangleRec(inter[i][j].m, BLACK);
			}
		}
	}
	DrawRectangle(bx + 9 * size - 6, by, 12, 9 * size, BLUE);
	DrawRectangle(bx, by + 9 * size - 6, 9 * size, 12, BLUE);
	DrawRectangle(bx - 6, by, 12, 9 * size, BLUE);
	DrawRectangle(bx, by - 6, 9 * size, 12, BLUE);

	DrawRectangleRec(MOODGAME.saveMood, BLACK);
	DrawText("save", size + 20, 345, 27, YELLOW);

	DrawRectangle(810 + size, by + 6 * size, 2 * size, 12, RED);
	DrawRectangle(810 + size, by + 7 * size, 2 * size, 12, BLACK);
	char a[3], b[3], c[3];
	sprintf(a, "%i", NumberOfWallsP1);
	sprintf(b, "%i", NumberOfWallsP2);
	sprintf(c, "%i", NumberOfWallsCom);
	DrawText(b, 815 + 3 * size, by + 6 * size - 5, 27, BLACK);
	if (mood == ONE) {
		DrawText(c, 815 + 3 * size, by + 7 * size - 5, 27, BLACK);
	}
	if (mood == TWO) {
		DrawText(a, 815 + 3 * size, by + 7 * size - 5, 27, BLACK);
	}
	if (checkWinner() == 0) {
		if (winner == red) {
			DrawRectangle(810 + size + 30, by + 2 * size, size, size, RED);
		}
		else {
			DrawRectangle(810 + size + 30, by + 2 * size, size, size, BLACK);
		}
		DrawText("winner", 805 + size + 30, by + size, 30, BLACK);
	}
	else {
		if (turn == player2) {
			DrawRectangle(810 + size + 30, by + 2 * size, size, size, RED);
		}
		else {
			DrawRectangle(810 + size + 30, by + 2 * size, size, size, BLACK);
		}
	}
	EndDrawing();
}

void drawMOOD() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawText("CORIDOR GAME", bx, by, 60, BLACK);
	DrawRectangleRec(MOODGAME.oneMood, BLACK);
	DrawRectangleRec(MOODGAME.twoMood, BLACK);
	DrawCircle(bx + (3 * size) + 50, by + (3 * size) + 50, 15, YELLOW);
	DrawCircle(bx + (3 * size) + 25, by + (6 * size) + 50, 15, YELLOW);
	DrawCircle(bx + (3 * size) + 75, by + (6 * size) + 50, 15, YELLOW);

	EndDrawing();
}

//////////////////////////////////
int main() {
	srand(time(NULL));
	setConsts();
	InitWindow(w, h, "coridor");
	SetTargetFPS(60);
	FILE* saved = fopen("save.txt", "r");
	if (saved != NULL && !feof(saved)) {
		fscanf(saved, "%d%d%d%d", &mood, &NumberOfWallsP1, &NumberOfWallsP2, &NumberOfWallsCom);
		fscanf(saved, "%d%d%d%d", &Xblack, &Yblack, &Xred, &Yred);

		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 8; ++j) {
				fscanf(saved, "%d", &wallh[i][j].s);
			}
		}
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 9; ++j) {
				fscanf(saved, "%d", &wallv[i][j].s);
			}
		}
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				fscanf(saved, "%d", &inter[i][j].s);
			}
		}
	}
	while (!WindowShouldClose() && !(saved != NULL && !feof(saved)))
	{
		drawMOOD();
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePosition = GetMousePosition();
			if (!setMOOD(mousePosition))
				break;
		}
	}
	while (!WindowShouldClose())
	{
		drawMAP();
		if (mood == ONE && checkWinner()) {
			playGameMoodOne();
		}
		if (mood == TWO && checkWinner()) {
			playGameMoodTwo();
		}
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePosition = GetMousePosition();
			if (CheckCollisionPointRec(mousePosition, MOODGAME.saveMood)) {
				winner = 0;
				break;
			}
		}
	}
	if (saved != NULL) {
		fclose(saved);
	}
	FILE* saving = fopen("save.txt", "w");
	if (checkWinner() && winner == 0) {
		fprintf(saving, "%d %d %d %d\n", mood, NumberOfWallsP1, NumberOfWallsP2, NumberOfWallsCom);
		fprintf(saving, "%d %d %d %d\n", Xblack, Yblack, Xred, Yred);
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 8; ++j) {
				fprintf(saving, "%d ", wallh[i][j].s);
			}
			fprintf(saving, "\n");
		}
		fprintf(saving, "\n");
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 9; ++j) {
				fprintf(saving, "%d ", wallv[i][j].s);
			}
			fprintf(saving, "\n");
		}
		fprintf(saving, "\n");
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				fprintf(saving, "%d ", inter[i][j].s);
			}
			fprintf(saving, "\n");
		}
		fprintf(saving, "\n");
	}
	fclose(saving);
}
///////////////////////////////////////////////









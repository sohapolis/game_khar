// GAME TEST 6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<raylib.h>
#include<stdio.h>

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
const float  size = 60;
int w = 1080;
int h = 720;
int bx = 270;
int by = 90;
int NumberOfWallsP1 = 10;
int NumberOfWallsP2 = 10;
Box map[9][9];
wall_h wallh[9][8];
wall_v wallv[8][9];
meet inter[8][8];
int Xred, Xblack, Yred, Yblack;
int counter = 0;
int MAP[17][17] = { 0 };
int visited[17][17];
int rows = 17;
int cols = 17;
enum terrain {
	empty,
	wall,
	goal,
	path
};

typedef enum {
	player1,player2

}Turn;
enum coller {
	red,
	black
};
int search(int row, int col)
{
	if (row < 0 || col < 0 || row>17 || col >17) {

		return 0;
	}
	int* current = &MAP[row][col];//TODO PAK KARDAN GAND KARI
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
int checkPath(Turn turn) {
	
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
	if (turn == player1) {
		for (int i = 0; i < 17; i+=2) {
			MAP[0][i] = goal;
		}

		return search(Xblack, Yblack);
	}
	else {
		for (int i = 0; i < 17; i += 2) {
			MAP[16][i] = goal;
		}
		return search(Xred, Yred);
	}
	


	/*for (int i = 0; i < 17; ++i) {
		for (int j = 0; j < 17; j++)
		{
			printf_s("%d\t", MAP[i][j]);
		}
		puts(" ");
	}*/
	return 0;
}
void drowmap(){

	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawRectangleRec(map[Xred][Yred].box, RED);
	DrawRectangleRec(map[Xblack][Yblack].box, BLACK);
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
	EndDrawing();

}
void clickMouse(Vector2 x , Turn & turn){
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (CheckCollisionPointRec(x, inter[i][j].m)) {
				return;
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (CheckCollisionPointRec(x, wallh[i][j].wall) && wallh[i + 1][j].s != 1 && inter[i][j].s != 1 && ((turn == player1 && NumberOfWallsP1 > 0) || (turn == player2 && NumberOfWallsP2 > 0))){
				if (!checkPath(turn)) {
					printf("impoosible");
					return;
				}
				printf("found");
				wallh[i][j].s = 1;
				wallh[i + 1][j].s = 1;
				inter[i][j].s = 1;
				
				if (turn == player1) { 
					NumberOfWallsP1--;
					turn = player2;
				}
				else {

					NumberOfWallsP2--;
					turn = player1;
				}
				return;
			}
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			if (CheckCollisionPointRec(x, wallv[i][j].wall) && wallv[i][j].s != 1 && wallv[i][j + 1].s != 1 && inter[i][j].s != 1 && ((turn == player1 && NumberOfWallsP1 > 0) || (turn == player2 && NumberOfWallsP2 > 0))) {
				if (!checkPath(turn)) {
					printf("impoosible");
					return;
				}
				printf("found");
				wallv[i][j].s = 1;
				wallv[i][j + 1].s = 1;
				inter[i][j].s = 1;
				checkPath(turn);
				if (turn == player1) {
					NumberOfWallsP1--;
					turn = player2;
				}
				else {

					NumberOfWallsP2--;
					turn = player1;
				}
				return;
			}
		}
	}

	if (CheckCollisionPointRec(x, map[Xred][Yred + 1].box) && map[Xred][Yred + 1].s == 0 && wallh[Xred][Yred].s != 1 && turn == player2) {
		map[Xred][Yred].s = 0;
		map[Xred][Yred + 1].s = red;
		Yred++;
		turn = player1;
		return;
	}
	if (CheckCollisionPointRec(x, map[Xred][Yred - 1].box) && map[Xred][Yred - 1].s == 0 && wallh[Xred][Yred - 1].s != 1 && turn == player2) {
		map[Xred][Yred].s = 0;
		map[Xred][Yred - 1].s = red;
		Yred--;
		turn = player1;
		return;
	}
	if (CheckCollisionPointRec(x, map[Xred + 1][Yred].box) && map[Xred + 1][Yred].s == 0 && wallv[Xred][Yred].s != 1 && turn == player2) {
		map[Xred][Yred].s = 0;
		map[Xred + 1][Yred].s = red;
		Xred++;
		turn = player1;
		return;
	}
	if (CheckCollisionPointRec(x, map[Xred - 1][Yred].box) && map[Xred - 1][Yred].s == 0 && wallv[Xred - 1][Yred].s != 1 && turn == player2) {
		map[Xred][Yred].s = 0;
		map[Xred - 1][Yred].s = red;
		Xred--;
		turn = player1;
		return;
	}
	if (CheckCollisionPointRec(x, map[Xblack][Yblack + 1].box) && map[Xblack][Yblack + 1].s == 0 && wallh[Xblack][Yblack].s != 1 && turn == player1) {
		map[Xblack][Yblack].s = 0;
		map[Xblack][Yblack + 1].s = black;
		Yblack++;
		turn = player2;
		return;

	}
	if (CheckCollisionPointRec(x, map[Xblack][Yblack - 1].box) && map[Xblack][Yblack - 1].s == 0 && wallh[Xblack][Yblack - 1].s != 1 && turn == player1) {
		map[Xblack][Yblack].s = 0;
		map[Xblack][Yblack - 1].s = black;
		Yblack--;
		turn = player2;
		return;
	}
	if (CheckCollisionPointRec(x, map[Xblack+1][Yblack].box) && map[Xblack+1][Yblack].s == 0 && wallv[Xblack][Yblack].s != 1 && turn == player1) {
		map[Xblack][Yblack].s = 0;
		map[Xblack + 1][Yblack].s = black;
		Xblack++;
		turn = player2;
		return;
	}
	if (CheckCollisionPointRec(x, map[Xblack-1][Yblack].box) && map[Xblack-1][Yblack].s == 0 && wallv[Xblack - 1][Yblack].s != 1 && turn == player1) {
		map[Xblack][Yblack].s = 0;
		map[Xblack - 1][Yblack].s = black;
		Xblack--;
		turn = player2;
		return;
	}

}
int main() {
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

	Turn turn = player1;
	Xred = 0;
	Xblack = 8;
	Yred = 4;
	Yblack = 4;
	InitWindow(w, h, "game");

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		drowmap();
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePosition= GetMousePosition();
			clickMouse(mousePosition , turn);
		}
	}

}
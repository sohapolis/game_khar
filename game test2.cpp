// game test2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

int main() {
	const float  size = 60;
	int w = 1080;
	int h = 720;
	int bx = 270;
	int by = 90;
	const int red = 7;
	const int black= 8;
	Box a[81];
	wall_h b[81];
	wall_v c[81];
	meet d[81];
	int y, z, turn;
	z = 4;
	y = 76;
	a[z].s = red;
	a[y].s = black;
	for (int i = 0; i < 81; i++) {
		a[i].s = 0;
		a[i].box = Rectangle{ bx + (i % 9) * size + 6,by + (i / 9) * size + 6, size - 12, size - 12 };
	}
	for (int i = 0; i < 72; i++) {
		b[i].s = 0;
		b[i].wall = Rectangle{ bx + (i / 9 + 1) * size - 6,by + (i % 9) * size, 12, size };
	}

	for (int i = 0; i < 72; i++) {
		c[i].s = 0;
		c[i].wall = Rectangle{ bx + (i % 9) * size ,by + (i / 9 + 1) * size - 6, size,12 };
	}
	for (int i = 0; i < 64; i++) {
		d[i].s = 0;
		d[i].m = Rectangle{ bx + ((i) / 8 + 1) * size - 6,by + ((i % 8)+1) * size - 6, 12, 12 };
	}
	InitWindow(w, h, "game");

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawRectangleRec(a[z].box, RED);
		DrawRectangleRec(a[y].box, BLACK);

		for (int i = 0; i < 81; i++) {
			if (b[i].s == 0) {
				DrawRectangleRec(b[i].wall, BLUE);
			}
			if (c[i].s == 0) {
				DrawRectangleRec(c[i].wall, BLUE);
			}
		}
		for (int i = 0; i < 81; i++) {
			if (b[i].s == 1) {
				DrawRectangleRec(b[i].wall, BLACK);
			}

			if (c[i].s == 1) {
				DrawRectangleRec(c[i].wall, BLACK);
				//printf("\n enter!\n");
			}
		}
		
		DrawRectangle(bx + 9 * size - 6, by, 12, 9 * size, BLUE);
		DrawRectangle(bx, by + 9 * size - 6, 9 * size, 12, BLUE);
		DrawRectangle(bx - 6, by, 12, 9 * size, BLUE);
		DrawRectangle(bx, by - 6, 9 * size, 12, BLUE);
		/*for (int i = 0; i < 64; i++) {
			if (d[i].s ==1)
			DrawRectangleRec(d[i].m, RED);
		}*/
		//int sw = 1;
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
				Vector2 x = GetMousePosition();
				int sw = 1;
				for (int i = 0; i < 64; i++) {
					if (CheckCollisionPointRec(x, d[i].m)) {
						sw = 0;
					}
				}
				for (int i = 0; i < 72 && sw == 1; i++) {
					if (CheckCollisionPointRec(x, c[i].wall) && (i % 9 != 8 && c[i].s != 1 && c[i + 1].s != 1) && (d[(i % 9) * 8 + i / 9].s != 1)) {
						c[i].s = 1;
						c[i + 1].s = 1;

						if (i % 9 != 8) {
							d[(i % 9) * 8 + i / 9].s = 1;
						}
						sw = 0;
					}
					if (CheckCollisionPointRec(x, b[i].wall) && i % 9 != 8 && b[i].s != 1 && b[i + 1].s != 1 && (d[i - (i / 9)].s != 1)) {
						b[i].s = 1;
						b[i + 1].s = 1;
						if (i % 9 != 8) {
							d[i - (i / 9)].s = 1;
						}
						sw = 0;
					}

				}

				if (CheckCollisionPointRec(x, a[z + 1].box) && a[z + 1].s != black && b[(z / 9) + 9 * (z % 9)].s != 1) {
					a[z].s = 0;
					a[z + 1].s = red;
					z++;
					sw = 0;
				}
				if (CheckCollisionPointRec(x, a[z - 1].box) && a[z - 1].s != black && b[(z / 9) + 9 * (z % 9) - 9].s != 1) {
					a[z].s = 0;
					a[z - 1].s = red;
					z--;
					sw = 0;
				}
				if (CheckCollisionPointRec(x, a[z + 9].box) && a[z + 9].s != black && c[z].s != 1) {
					a[z].s = 0;
					a[z + 9].s = red;
					z += 9;
					sw = 0;
				}
				if (CheckCollisionPointRec(x, a[z - 9].box) && a[z - 9].s!=black && c[z - 9].s != 1) {
					a[z].s = 0;
					a[z - 9].s = red;
					z -= 9;
					sw = 0;
				}
				if (CheckCollisionPointRec(x, a[y + 1].box) && a[y +1].s != red && b[(y / 9) +9 * (y % 9)].s != 1) {
					a[y].s = 0;
					a[y + 1].s = black;
					y++;
					sw = 0;
				}
				if (CheckCollisionPointRec(x, a[y - 1].box) && a[y - 1].s != red && b[(y/9)+9*(y%9)-9].s != 1) {
					a[y].s = 0;
					a[y - 1].s = black;
					y--;
					sw = 0;
				}
				if (CheckCollisionPointRec(x, a[y + 9].box) && a[y + 9].s != red && c[y].s!=1) {
					a[y].s = 0;
					a[y + 9].s = black;
					y += 9;
					sw = 0;
				}
				if (CheckCollisionPointRec(x, a[y - 9].box)&& a[y - 9].s!=red && c[y-9].s != 1) {
					a[y].s = 0;
					a[y - 9].s = black;
					y -= 9;
					sw = 0;
				}


		}
		EndDrawing();
	}

}



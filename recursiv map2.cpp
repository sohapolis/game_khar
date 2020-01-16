// recursiv map2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <string.h>
#include<raylib.h>
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

void findPath(int cur, int dest, int& found, wall_h x[81], wall_v y[81], Box z[81]) {
	z[cur].s = 1;
	if ((cur / 9) == dest) {
		found = 1;
		return;
	}
	if (cur + 1 < 9 * ((cur / 9) + 1) - 1 && x[(cur / 9) + 9 * (cur % 9)].s == 0 && z[cur + 1].s != 1) {
		findPath(cur + 1, dest, found, x, y, z);
		
	}

	if (cur + 9 < 80 - (cur % 9) && y[cur].s == 0 && z[cur + 9].s != 1) {
		findPath(cur + 9, dest, found, x, y, z);
	}

	if (cur - 1 > 9 * (cur / 9) && x[(cur / 9) + 9 * (cur % 9) - 9].s == 0 && z[cur - 1].s != 1) {
		findPath(cur - 1, dest, found, x, y, z);
	}

	if (cur - 9 > cur % 9 && y[cur - 9].s == 0 && z[cur - 9].s != 1) {
		findPath(cur - 9, dest, found, x, y, z);
	}
}


int main()
{
	const float  size = 60;
	//int w = 1080;
	//int h = 720;
	int bx = 270;
	int by = 90;
	const int red = 7;
	const int black = 8;
	Box a[81];
	wall_h b[81];
	wall_v c[81];
	meet d[81];
	int y, z, turn;
	z = 4;
	y = 76;
	//a[z].s = red;
	//a[y].s = black;
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
		d[i].m = Rectangle{ bx + ((i) / 8 + 1) * size - 6,by + ((i % 8) + 1) * size - 6, 12, 12 };
	}
	int found = false;
	findPath(9, 0, found, b, c, a);
	for (int i = 0; i < 81; i++) {
		if (i % 9 == 0) {
			printf("\n")
		}
		printf("%d", a[i].s);
	}
	//printf("%d", found);
}


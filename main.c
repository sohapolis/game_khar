#pragma GCC optimize ("O3")
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

int counter = 0;
int pX[900];
int pY[900];
int maze[9][9];
int visited[9][9];
int rows = 9;
int cols = 9;
enum terrain {
    empty,
    wall,
    goal,
    path
};

void init_visited() {

    int i, j;
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            if (maze[i][j] == 1) {
                visited[i][j] = wall;

            } else {
                visited[i][j] = empty;
            }
        }


        visited[8][8] = goal;

    }
}

int search(int row, int col)
{
    if(row < 0 || col < 0 || row>9 || col >9){

        return 0;
    }
    int* current = &visited[row][col];
    if (*current == goal) {
        pX[counter] = 29;
        pY[counter] = 29;
        counter++;
        return 1;
    }


    if (*current == empty) {
        *current = path;


        if (search(row + 1, col) || search(row, col - 1) || search(row - 1, col) || search(row, col + 1)){
            pX[counter] = row;
            pY[counter] = col;
            counter++;
            //*current = path;
            return 1;
        }
        *current = empty;
    }

    return 0;
}

int main()
{
    init_visited();
    memset(maze,0, 4*9*9);
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {

            printf("%d  " , maze[i][j]);
        }
        puts("");
    }


    if (!search(0, 0)) {
        printf("Impossible !");
        return 0;
    } else {
        printf("found path!\n\n\n\n\n\n");

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if(visited[i][j] == path){
                    printf("#");
                    continue;
                } else{
                    printf("%d   " , visited[i][j]);
                }
            }
            puts(" ");
            puts(" ");
        }
    }
    return 0;
}
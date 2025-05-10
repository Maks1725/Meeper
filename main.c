#include <raylib.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int grid_width = 10;
    int grid_height = 10;
    int cell_size = 25;
    int font_size = 20;
    int border_out = 10;
    int border = 5;
    int mine_amount = 10;

    int posx;
    int posy;

    bool mines[grid_width][grid_height];
    int numbers[grid_width][grid_height];
    int board[grid_width][grid_height];
    memset(mines, 0, sizeof(mines));
    memset(numbers, 0, sizeof(numbers));
    memset(board, 0, sizeof(board));

    for (int i = 0; i < mine_amount; ++i) {
        posx = rand() % grid_width;
        posy = rand() % grid_height;
        if (mines[posx][posy]) {
            --i;
        }
        mines[posx][posy] = true;
    }

    for (int x = 0; x < grid_width; ++x) {
        for (int y = 0; y < grid_height; ++y) {
            for(posx = x - 1 < 0 ? 0 : x - 1; posx < x + 2 && posx < grid_width; ++posx) {
                for (posy = y - 1 < 0 ? 0 : y - 1; posy < y + 2 && posy < grid_height; ++posy) {
                    numbers[posx][posy] += mines[x][y] ? 1 : 0;
                }
            }
        }
    }

    int window_width = grid_width * cell_size + (grid_width - 1) * border + border_out * 2;
    int window_height = grid_height * cell_size + (grid_height - 1) * border + 100 + border_out * 2;
    
    InitWindow(window_width, window_height, "Meeper");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GRAY);

        for (int x = 0; x < grid_width; ++x) {
            for (int y = 0; y < grid_height; ++y) {
                posx = border_out + x * (cell_size + border);
                posy = (border_out + 100) + y * (cell_size + border);
                switch (board[x][y]) {
                    case 0:
                        DrawRectangle(posx, posy, cell_size, cell_size, LIGHTGRAY);
                        break;
                    default:
                        break;
                }
            }
        }
        
        EndDrawing();
    }

    return 0;
}


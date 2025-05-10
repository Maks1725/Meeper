#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Color colors[9] = {
    RAYWHITE,
    BLUE,
    GREEN,
    RED,
    DARKBLUE,
    PURPLE,
    DARKGREEN,
    GRAY,
    DARKGRAY
};

int grid_width = 10;
int grid_height = 10;
int cell_size = 30;
int font_size = 20;
int border_out = 10;
int top_space = 50;
int border = 2;
int mine_amount = 10;
int flags_placed = 0;
int cells_opened = 0;

void OpenCells(int x, int y, int board[grid_width][grid_height], int numbers[grid_width][grid_height]) {
    if (board[x][y] != 1) {
        board[x][y] = 1;
        cells_opened++;
    }
    for(int posx = x - 1 < 0 ? 0 : x - 1; posx < x + 2 && posx < grid_width; ++posx) {
        for (int posy = y - 1 < 0 ? 0 : y - 1; posy < y + 2 && posy < grid_height; ++posy) {
            if (board[posx][posy] != 1) {
                board[posx][posy] = 1;
                cells_opened++;
                if (numbers[posx][posy] == 0) {
                    OpenCells(posx, posy, board, numbers);
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int posx;
    int posy;
    int mousex;
    int mousey;
    int selectx;
    int selecty;
    char *text;

    bool mines[grid_width][grid_height];
    int numbers[grid_width][grid_height];
    int board[grid_width][grid_height];
    memset(mines, 0, sizeof(mines));
    memset(numbers, 0, sizeof(numbers));
    memset(board, 0, sizeof(board));

    srand(time(NULL));
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
    int window_height = grid_height * cell_size + (grid_height - 1) * border + top_space + border_out * 2;
    
    InitWindow(window_width, window_height, "Meeper");

    while (!WindowShouldClose()) {
        mousex = GetMouseX();
        mousey = GetMouseY();

        selectx = -1;
        selecty = -1;
        for (int x = 0; x < grid_width; ++x) {
            for (int y = 0; y < grid_height; ++y) {
                posx = border_out + x * (cell_size + border);
                posy = (border_out + top_space) + y * (cell_size + border);
                if (mousex >= posx && mousex <= posx + cell_size && mousey >= posy && mousey <= posy + cell_size) {
                    selectx = x;
                    selecty = y;
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (board[selectx][selecty] == 0) {
                if (mines[selectx][selecty]) {
                    for (int x = 0; x < grid_width; ++x) {
                        for (int y = 0; y < grid_height; ++y) {
                            if (mines[x][y]) {
                                board[x][y] = 1;
                            }
                        }
                    }
                } else {
                    if (numbers[selectx][selecty] == 0) {
                        OpenCells(selectx, selecty, board, numbers);
                    } else {
                        board[selectx][selecty] = 1;
                        cells_opened++;
                    }
                }
            }
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            if (board[selectx][selecty] == 0) {
                board[selectx][selecty] = 2;
                flags_placed++;
            } else if (board[selectx][selecty] == 2) {
                board[selectx][selecty] = 0;
                flags_placed--;
            }
        }

        BeginDrawing();

        ClearBackground(GRAY);

        for (int x = 0; x < grid_width; ++x) {
            for (int y = 0; y < grid_height; ++y) {
                posx = border_out + x * (cell_size + border);
                posy = (border_out + top_space) + y * (cell_size + border);
                switch (board[x][y]) {
                    // Hidden cell
                    case 0: 
                        if (x == selectx && y == selecty) {
                            DrawRectangle(posx, posy, cell_size, cell_size, DARKGRAY);
                        } else {
                            DrawRectangle(posx, posy, cell_size, cell_size, LIGHTGRAY);
                        }
                        break;
                    // Open cell
                    case 1:
                        DrawRectangle(posx, posy, cell_size, cell_size, RAYWHITE);
                        if (mines[x][y]) {
                            DrawRectangle(posx + cell_size * 0.2, posy + cell_size * 0.2, cell_size * 0.6, cell_size * 0.6, BLACK);
                            DrawRectangle(posx + cell_size * 0.4, posy + cell_size * 0.1, cell_size * 0.2, cell_size * 0.8, BLACK);
                            DrawRectangle(posx + cell_size * 0.1, posy + cell_size * 0.4, cell_size * 0.8, cell_size * 0.2, BLACK);
                        } else {
                            text = TextFormat("%d", numbers[x][y]);
                            DrawText(text, posx + (cell_size - MeasureText(text, font_size)) / 2, posy + (cell_size - font_size) / 2, font_size, colors[numbers[x][y]]);
                        }
                        break;
                    // Flag cell
                    case 2:
                        if (x == selectx && y == selecty) {
                            DrawRectangle(posx, posy, cell_size, cell_size, DARKGRAY);
                        } else {
                            DrawRectangle(posx, posy, cell_size, cell_size, LIGHTGRAY);
                        }
                        DrawRectangle(posx + cell_size * 0.25, posy + cell_size * 0.25, cell_size * 0.5, cell_size * 0.5, YELLOW);
                    default:
                        break;
                }
            }
        }
        
        EndDrawing();
    }

    return 0;
}


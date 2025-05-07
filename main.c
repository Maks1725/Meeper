#include <raylib.h>

int main(int argc, char **argv) {
    int grid_width = 10;
    int grid_height = 10;
    int cell_size = 40;
    int border_out = 10;
    int border = 5;
    int mine_amount = 10;

    int window_width = grid_width * cell_size + (grid_width - 1) * border + border_out * 2;
    int window_height = grid_height * cell_size + (grid_height - 1) * border + 100 + border_out * 2;
    
    InitWindow(window_width, window_height, "Meeper");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GRAY);

        for (int x = 0; x < grid_width; ++x) {
            for (int y = 0; y < grid_height; ++y) {
                DrawRectangle(border_out + x * (cell_size + border), (border_out + 100) + y * (cell_size + border),
                              cell_size, cell_size, LIGHTGRAY);
            }
        }
        
        EndDrawing();
    }

    return 0;
}

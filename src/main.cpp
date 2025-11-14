#include "raylib.h"

int main() {
    // Initialization
    const int screen_width = 800;
    const int screen_height = 600;

    InitWindow(screen_width, screen_height, "Yoshi's Wrath");
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        // TODO: Game logic will go here

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Yoshi's Wrath - Hello World!", 190, 280, 30, DARKGREEN);
        DrawFPS(10, 10);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}

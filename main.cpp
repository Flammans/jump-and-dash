#include "raylib.h"

int main() {
    // Initialization
    const int windowWidth = 800;
    const int windowHeight = 450;

    InitWindow(windowWidth, windowHeight, "Jump And Dash Game"); // Create window and OpenGL context

    // rectangle dimensions
    const int width{50}; 
    const int height{80};
    int posY{windowHeight - height};
    int velocity(0); // Initialize velocity
    int gravity(1); // Gravity effect

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        // Update
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        
        DrawRectangle(windowWidth / 2, posY, width, height, BLUE); // Draw rectangle
        DrawText("Press SPACE to jump", 10, 10, 20, DARKGRAY);
        DrawText("Press D to dash", 10, 30, 20, DARKGRAY);
        DrawText("Press ESC to exit", 10, 50, 20, DARKGRAY);
        if (IsKeyPressed(KEY_SPACE)) {
            velocity = -10; // Jump
            DrawText("Jump", 5, 25, 25, GREEN);
        }
        if (IsKeyPressed(KEY_D)) {
            velocity = -20; // Dash
        }

        if (posY < windowHeight - height) {
            velocity += gravity; // Apply gravity
        } else {
            velocity = 0; // Reset velocity when on the ground
            posY = windowHeight - height; // Reset position to ground level
        }
        if (posY > windowHeight - height) {
            posY = windowHeight - height; // Prevent going below ground
        }
        if (posY < 0) {
            posY = 0; // Prevent going above the window
        }
        if (posY > windowHeight - height) {
            posY = windowHeight - height; // Prevent going below ground
        } 

        posY += velocity; // Update position
        
 
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
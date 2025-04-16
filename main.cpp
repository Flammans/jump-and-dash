#include "raylib.h"

int main() {
    // Initialization
    const int windowWidth = 800;
    const int windowHeight = 450;

    InitWindow(windowWidth, windowHeight, "Jump And Dash Game"); // Create window and OpenGL context

    // Character dimensions
    const int characterWidth{50}; 
    const int characterHeight{80};
    int posY{windowHeight - characterHeight};
    int velocity{0}; // Initialize velocity
    const int gravity{1}; // Aceleration due to gravity (pixels/frame)/frame
    bool isInAir{false}; // Jumping state
    const int jumpVelocity{-22}; // Jump velocity
    const int dashVelocity{-40}; // Dash velocity

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        // Update
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawRectangle(windowWidth / 2, posY, characterWidth, characterHeight, BLUE); // Draw rectangle
        DrawText("Press SPACE to jump", 10, 10, 20, DARKGRAY);
        DrawText("Press D to dash", 10, 30, 20, DARKGRAY);
        DrawText("Press ESC to exit", 10, 50, 20, DARKGRAY);

        // jump and dash logic
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {
            velocity = jumpVelocity; // Jump
        }
        if (IsKeyPressed(KEY_D) && !isInAir) {
            velocity = dashVelocity; // Dash
        }

        posY += velocity; // Update position

        // Perform ground collision detection
        if (posY < windowHeight - characterHeight) {
            // Rectagle is oin air
            isInAir = true;
            velocity += gravity; // Apply gravity
        } else {
            // Rectagle is on the ground
            isInAir = false;
            velocity = 0; // Reset velocity when on the ground
            posY = windowHeight - characterHeight; // Reset position to ground level
        }
        if (posY > windowHeight - characterHeight) {
            posY = windowHeight - characterHeight; // Prevent going below ground
        }
        if (posY < 0) {
            posY = 0; // Prevent going above the window
        }
        if (posY > windowHeight - characterHeight) {
            posY = windowHeight - characterHeight; // Prevent going below ground
        } 

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
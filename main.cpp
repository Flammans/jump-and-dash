#include "raylib.h"

int main() {
    // Initialization
    const int windowWidth = 800;
    const int windowHeight = 450;

    InitWindow(windowWidth, windowHeight, "Jump And Dash Game"); // Create window and OpenGL context

    const int gravity{1}; // Aceleration due to gravity (pixels/frame)/frame
    Texture2D characterTexture = LoadTexture("textures/scarfy.png"); // Load character texture
    Rectangle characterRec; // Character rectangle
    characterRec.width = characterTexture.width/6; // Set width
    characterRec.height = characterTexture.height; // Set height
    characterRec.x = 0; // Set x position
    characterRec.y = 0; // Set y position
    Vector2 characterPos; // Character position
    characterPos.x = windowWidth / 2 - characterRec.width / 2; // Center the character
    characterPos.y = windowHeight - characterRec.height; // Set y position to the bottom of the window

    // Character dimensions

    int velocity{0}; // Initialize velocity
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
        
        DrawTextureRec(characterTexture, characterRec, characterPos, WHITE); // Draw character
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

        characterPos.y += velocity; // Update position

        // Perform ground collision detection
        if (characterPos.y < windowHeight - characterRec.height) {
            // Rectagle is oin air
            isInAir = true;
            velocity += gravity; // Apply gravity
        } else {
            // Rectagle is on the ground
            isInAir = false;
            velocity = 0; // Reset velocity when on the ground
            characterPos.y = windowHeight - characterRec.height; // Reset position to ground level
        }
        if (characterPos.y > windowHeight - characterRec.height) {
            characterPos.y = windowHeight - characterRec.height; // Prevent going below ground
        }
        if (characterPos.y < 0) {
            characterPos.y = 0; // Prevent going above the window
        }
        if (characterPos.y > windowHeight - characterRec.height) {
            characterPos.y = windowHeight - characterRec.height; // Prevent going below ground
        } 

        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(characterTexture); // Unload texture
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
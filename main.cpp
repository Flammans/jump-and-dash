#include "raylib.h"

int main() {
    // Initialization
    const int windowWidth = 800;
    const int windowHeight = 450;

    InitWindow(windowWidth, windowHeight, "Jump And Dash Game"); // Create window and OpenGL context

    const int gravity{1'000}; // Aceleration due to gravity (pixels/second)/second
    Texture2D characterTexture = LoadTexture("textures/scarfy.png"); // Load character texture
    Rectangle characterRec; // Character rectangle
    characterRec.width = characterTexture.width/6; // Set width
    characterRec.height = characterTexture.height; // Set height
    characterRec.x = 0; // Set x position
    characterRec.y = 0; // Set y position
    Vector2 characterPos; // Character position
    characterPos.x = windowWidth / 2 - characterRec.width / 2; // Center the character
    characterPos.y = windowHeight - characterRec.height; // Set y position to the bottom of the window

    // Animation frame
    int frame{0}; // Frame counter
    int velocity{0}; // Initialize velocity
    bool isInAir{false}; // Jumping state
    const int jumpVelocity{-600}; // Jump velocity (pixels/second)
    const int dashVelocity{-800}; // Dash velocity (pixels/second)

    // Amount of time to wait before updating the animation frame
    // This is used to control the speed of the animation
    // The value is set to 1/12 of a second, which means the animation will update every 12 frames
    // This is a common frame rate for animations, as it gives a smooth look without being too fast
    const float updateTime{1.0/12.0}; // Update time (seconds)
    float runningTime{0}; // Running time (seconds)

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        // Update
        float deltaTime{GetFrameTime()}; // Get time between frames
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawTextureRec(characterTexture, characterRec, characterPos, WHITE); // Draw character
        DrawText("Press SPACE to jump", 10, 10, 20, DARKGRAY);
        DrawText("Press D to dash", 10, 30, 20, DARKGRAY);
        DrawText("Press ESC to exit", 10, 50, 20, DARKGRAY);

        // Perform ground collision detection
        if (characterPos.y < windowHeight - characterRec.height) {
            // Rectagle is oin air
            isInAir = true;
            velocity += gravity * deltaTime; // Apply gravity
        } else {
            // Rectagle is on the ground
            isInAir = false;
            velocity = 0; // Reset velocity when on the ground
            characterPos.y = windowHeight - characterRec.height; // Reset position to ground level
        }

        // jump and dash logic
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {
            velocity = jumpVelocity; // Jump
        }
        if (IsKeyPressed(KEY_D) && !isInAir) {
            velocity = dashVelocity; // Dash
        }

        // Update the position
        characterPos.y += velocity * deltaTime; 

        runningTime += deltaTime; // Update running time
        if (runningTime >= updateTime) { // Check if it's time to update the animation frame
            runningTime = 0; // Reset running time
            
            //Update animation frame
            characterRec.x = frame * characterRec.width; // Set the x position of the texture
 
            frame++; // Update frame
            if (frame > 5) frame = 0; // Reset frame counter
        }

        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(characterTexture); // Unload texture
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
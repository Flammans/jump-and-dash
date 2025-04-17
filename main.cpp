#include "raylib.h"

struct AnimationData {
    Rectangle rec; // Rectangle for the frame
    Vector2 pos; // Position of the frame
    int frame; // Frame counter
    float runningTime{0.0f}; // Running time (seconds)
    float updateTime{1.0f/12.0f}; // Update time (seconds)
};

int main() {
    // Initialization
    const int windowWidth = 800;
    const int windowHeight = 450;

    InitWindow(windowWidth, windowHeight, "Jump And Dash Game"); // Create window and OpenGL context

    const int gravity{1'000}; // Aceleration due to gravity (pixels/second)/second
 
    // Nebula 
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png"); // Load nebula texture

    AnimationData nebulaData{ 
        {0.0f, 0.0f, nebula.width/8.0f, nebula.height/8.0f}, // Rectangle for the frame
        {windowWidth, windowHeight - nebula.height/8.0f}, // Position of the frame
        0, // Frame counter
        0.0f, // Running time (seconds)
        1.0f/12.0f // Update time (seconds)
    }; // Nebula animation data

    AnimationData nebula2Data{ 
        {0.0f, 0.0f, nebula.width/8.0f, nebula.height/8.0f}, // Rectangle for the frame
        {windowWidth + 400, windowHeight - nebula.height/8.0f}, // Position of the frame
        0, // Frame counter
        0.0f, // Running time (seconds)
        1.0f/16.0f // Update time (seconds)
    }; // Nebula 2 animation data

    // Set nebula X velocity (pixels/second) 
    int nebulaVelocity{-200}; // Nebula velocity 

    // Main Character  
    Texture2D character = LoadTexture("textures/scarfy.png"); // Load character texture
    AnimationData characterData; // Character animation data
    characterData.rec.width = character.width/6; // Set width
    characterData.rec.height = character.height; // Set height
    characterData.rec.x = 0; // Set x position
    characterData.rec.y = 0; // Set y position
    characterData.pos.x = windowWidth / 2 - characterData.rec.width / 2; // Center the character
    characterData.pos.y = windowHeight - characterData.rec.height; // Set y position to the bottom of the window
    characterData.frame = 0; // Initialize frame counter
    characterData.runningTime = 0.0f; // Initialize running time
    characterData.updateTime = 1.0f/12.0f; // Set update time (seconds)

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
        
        // Perform ground collision detection
        if (characterData.pos.y < windowHeight - characterData.rec.height) {
            // Rectagle is oin air
            isInAir = true;
            velocity += gravity * deltaTime; // Apply gravity
        } else {
            // Rectagle is on the ground
            isInAir = false;
            velocity = 0; // Reset velocity when on the ground
            characterData.pos.y = windowHeight - characterData.rec.height; // Reset position to ground level
        }

        // jump and dash logic
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {
            velocity = jumpVelocity; // Jump
        }
        if (IsKeyPressed(KEY_D) && !isInAir) {
            velocity = dashVelocity; // Dash
        }

        // Move nebula position
        nebulaData.pos.x += nebulaVelocity * deltaTime; // Update nebula position
        // Move nebula 2 position
        nebula2Data.pos.x += nebulaVelocity * deltaTime; // Update nebula 2 position
        
        // Update character the position
        characterData.pos.y += velocity * deltaTime; 

        if(!isInAir) {
            // Update running time 
            runningTime += deltaTime; 
            if (runningTime >= updateTime) { // Check if it's time to update the animation frame
                runningTime = 0; // Reset running time
                //Update animation frame
                characterData.rec.x = frame * characterData.rec.width; // Set the x position of the texture
                // Update frame
                frame++; 
                // Reset frame counter
                if (frame > 5) frame = 0; 
            }
        } 

        // Update nebula animation frame
        nebulaData.runningTime += deltaTime; // Update running time
        if (nebulaData.runningTime >= nebulaData.updateTime) { // Check if it's time to update the animation frame
            nebulaData.runningTime = 0; // Reset running time
            // Update nebula frame
            nebulaData.rec.x = nebulaData.frame * nebulaData.rec.width; // Set the x position of the texture
            // Update frame
            nebulaData.frame++; 
            // Reset frame counter
            if (nebulaData.frame > 7) nebulaData.frame = 0; 
        }


        // Update nebula animation frame
        nebula2Data.runningTime += deltaTime; // Update running time
        if (nebula2Data.runningTime >= nebula2Data.updateTime) { // Check if it's time to update the animation frame
            nebula2Data.runningTime = 0; // Reset running time
            // Update nebula frame
            nebula2Data.rec.x = nebula2Data.frame * nebula2Data.rec.width; // Set the x position of the texture
            // Update frame
            nebula2Data.frame++; 
            // Reset frame counter
            if (nebula2Data.frame > 7) nebula2Data.frame = 0; 
        }

        // Draw nebula
        DrawTextureRec(nebula, nebulaData.rec, nebulaData.pos, WHITE);
        // Draw nebula 2
        DrawTextureRec(nebula, nebula2Data.rec, nebula2Data.pos, RED);

        // Draw character
        DrawTextureRec(character, characterData.rec, characterData.pos, WHITE); 
        
        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(character); // Unload texture
    UnloadTexture(nebula); // Unload texture
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
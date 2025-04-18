#include "raylib.h"

struct AnimationData {
    Rectangle rec; // Rectangle for the frame
    Vector2 pos; // Position of the frame
    int frame; // Frame counter
    float runningTime{0.0f}; // Running time (seconds)
    float updateTime{1.0f/12.0f}; // Update time (seconds)
};

bool isOnGround(AnimationData data, int windowHeight) {
    // Check if the rectangle is on the ground
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimationData updateAnimationData(AnimationData data, float deltaTime, int maxFrames) {
    // Update the animation data
    data.runningTime += deltaTime; // Update running time
    if (data.runningTime >= data.updateTime) { // Check if it's time to update the animation frame
        data.runningTime = 0; // Reset running time
        // Update animation frame
        data.rec.x = data.frame * data.rec.width; // Set the x position of the texture
        // Update frame
        data.frame++; 
        // Reset frame counter
        if (data.frame > maxFrames) data.frame = 0; 
    }
    return data;
}
 
int main() {
    // Initialization
    // Window dimensions
    const int windowWidth = 800;
    const int windowHeight = 450;

    InitWindow(windowWidth, windowHeight, "Jump And Dash Game"); // Create window and OpenGL context

    const int gravity{1'000}; // Aceleration due to gravity (pixels/second)/second
 
    // Nebula 
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png"); // Load nebula texture

    const int sizeOfNebula{3}; // Number of frames in the nebula texture
    AnimationData nebulae[sizeOfNebula]{}; // Array of nebulae

    for (int i = 0; i < sizeOfNebula; i++)
    {
        nebulae[i].rec.x = 0.0f; // Set x position
        nebulae[i].rec.y = 0.0f; // Set y position
        nebulae[i].rec.width = nebula.width/8.0f; // Set width
        nebulae[i].rec.height = nebula.height/8.0f; // Set height
        nebulae[i].pos.x = windowWidth + GetRandomValue(0, 1200); // Set x position
        nebulae[i].pos.y = windowHeight - nebula.height/8.0f; // Set y position
        nebulae[i].frame = 0; // Initialize frame counter
        nebulae[i].runningTime = 0.0f; // Initialize running time
        nebulae[i].updateTime = 1.0f/16.0f; // Set update time (seconds)
    }

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
    int velocity{0}; // Initialize velocity
    bool isInAir{false}; // Jumping state
    const int jumpVelocity{-600}; // Jump velocity (pixels/second)
    const int dashVelocity{-800}; // Dash velocity (pixels/second)

    Texture2D background = LoadTexture("textures/far-buildings.png"); // Load background texture
    Texture2D midground = LoadTexture("textures/back-buildings.png"); // Load midground texture
    Texture2D foreground = LoadTexture("textures/foreground.png"); // Load foreground texture
    
    float bagroundPositionX{0.0f}; // Background position
    float migroundPositionX{0.0f}; // Midground position
    float foregroundPositionX{0.0f}; // Foreground position

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        // Update
        float deltaTime{GetFrameTime()}; // Get time between frames
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw background
        int countOfBackgrounds{3}; // Number of backgrounds
        bagroundPositionX -= 20 * deltaTime; // Update background position
        Vector2 backgroundPos{bagroundPositionX, 0.0f}; // Background position
        DrawTextureEx(background, backgroundPos, 0.0f, 3.0f, WHITE); // Draw background
        
        // loop background infinitely
        if (bagroundPositionX <= -background.width * 3) { // Check if background is out of the screen
            bagroundPositionX = 0.0f; // Reset background position
            countOfBackgrounds++; // Increment background count
        }
        for (int i = 0; i < countOfBackgrounds; i++)
        {
            DrawTextureEx(background, (Vector2){bagroundPositionX + background.width * (i * 3), 0.0f}, 0.0f, 3.0f, WHITE); // Draw background            
        }

        // Draw midground background
        int countOfMidground{3}; // Number of midgrounds
        migroundPositionX -= 40 * deltaTime; // Update midground position
        Vector2 midgroundPos{migroundPositionX, 0.0f}; // Midground position
        DrawTextureEx(midground, midgroundPos, 0.0f, 3.0f, WHITE); // Draw background
        // loop midground background infinitely
        if (migroundPositionX <= -midground.width * 3) { // Check if midground is out of the screen
            migroundPositionX = 0.0f; // Reset midground position
            countOfMidground++; // Increment midground count
        }
        for (int i = 0; i < countOfMidground; i++)
        {
            DrawTextureEx(midground, (Vector2){migroundPositionX + midground.width * (i * 3), 0.0f}, 0.0f, 3.0f, WHITE); // Draw midground            
        }

        // Draw foreground background
        int countOfForeground{3}; // Number of foregrounds
        foregroundPositionX -= 80 * deltaTime; // Update foreground position
        Vector2 foregroundPos{foregroundPositionX, 0.0f}; // Foreground position
        DrawTextureEx(foreground, foregroundPos, 0.0f, 3.0f, WHITE); // Draw background
        // loop foreground background infinitely
        if (foregroundPositionX <= -foreground.width * 3) { // Check if foreground is out of the screen
            foregroundPositionX = 0.0f; // Reset foreground position
            countOfForeground++; // Increment foreground count
        }
        for (int i = 0; i < countOfForeground; i++)
        {
            DrawTextureEx(foreground, (Vector2){foregroundPositionX + foreground.width * (i * 3), 0.0f}, 0.0f, 3.0f, WHITE); // Draw foreground            
        }

        // Perform ground collision detection
        if (isOnGround(characterData, windowHeight)) {
            // Rectagle is on the ground
            isInAir = false;
            velocity = 0; // Reset velocity when on the ground
            characterData.pos.y = windowHeight - characterData.rec.height; // Reset position to ground level
        } else {
            // Rectagle is in air
            isInAir = true;
            velocity += gravity * deltaTime; // Apply gravity
        }

        // jump and dash logic
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {
            velocity = jumpVelocity; // Jump
        }
        if (IsKeyPressed(KEY_D) && !isInAir) {
            velocity = dashVelocity; // Dash
        }

        for (int i = 0; i < sizeOfNebula; i++)
        {
            // Move nebula position
            nebulae[i].pos.x += nebulaVelocity * deltaTime; // Update nebula position
        }

        // Update character the position
        characterData.pos.y += velocity * deltaTime; 

        if(!isInAir) {
            characterData = updateAnimationData(characterData, deltaTime, 5); // Update character animation
        } 

        for (int i = 0; i < sizeOfNebula; i++)
        {
            // Check if nebula is out of the screen
            if (nebulae[i].pos.x < -nebulae[i].rec.width) {
                nebulae[i].pos.x = windowWidth + GetRandomValue(0, 400); // Reset position to the right side of the screen
            }
 
            nebulae[i] = updateAnimationData(nebulae[i], deltaTime, 7); // Update nebula animation

            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE); // Draw nebula
        }

        // Draw character
        DrawTextureRec(character, characterData.rec, characterData.pos, WHITE); 
        
        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(character); // Unload texture
    UnloadTexture(nebula); // Unload texture
    UnloadTexture(background); // Unload texture
    UnloadTexture(midground); // Unload texture
    UnloadTexture(foreground); // Unload texture
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
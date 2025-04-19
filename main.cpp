#include "raylib.h"

struct AnimationData {
    Rectangle rec; // Rectangle for the frame
    Vector2 pos; // Position of the frame
    int frame; // Frame counter
    float runningTime{0.0f}; // Running time (seconds)
    float updateTime{1.0f/12.0f}; // Update time (seconds)
};

bool IsOnGround(AnimationData data, int windowHeight) {
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

void DrawParallaxLayer(Texture2D texture, float &positionX, float speed, float deltaTime, float scale, int &count)
{
    // Draw the parallax layer
    // Update the position of the layer
    positionX -= speed * deltaTime;
    // Check if the layer is out of the screen
    if (positionX <= -texture.width * scale)
    {
        positionX = 0.0f;
        count++; 
    }
    // Draw the texture at the current position
    for (int i = 0; i < count; i++)
    {
        Vector2 position = { positionX + texture.width * scale * i, 0.0f };
        // Draw the texture
        DrawTextureEx(texture, position, 0.0f, scale, WHITE);
    }
}
 
int main() {
    // Initialization
    // Window dimensions
    const int windowWidth = 1200;
    const int windowHeight = 720;

    InitWindow(windowWidth, windowHeight, "Dnipro Roller"); // Create window and OpenGL context

    const int gravity{1'000}; // Aceleration due to gravity (pixels/second)/second
 
    // Czech-Hedgehog 
    Texture2D czechHedgehog = LoadTexture("textures/czech-hedgehog.png"); // Load czechHedgehog texture

    const int sizeOfczechHedgehog{3}; // Number of frames in the czechHedgehog texture
    AnimationData czechHedgehogs[sizeOfczechHedgehog]{}; // Array of czechHedgehoge

    for (int i = 0; i < sizeOfczechHedgehog; i++)
    {
        czechHedgehogs[i].rec.x = 0.0f; // Set x position
        czechHedgehogs[i].rec.y = 0.0f; // Set y position
        czechHedgehogs[i].rec.width = czechHedgehog.width; // Set width
        czechHedgehogs[i].rec.height = czechHedgehog.height; // Set height
        czechHedgehogs[i].pos.x = windowWidth + GetRandomValue(0, 1200); // Set x position
        czechHedgehogs[i].pos.y = windowHeight - czechHedgehog.height; // Set y position
        czechHedgehogs[i].frame = 0; // Initialize frame counter
        czechHedgehogs[i].runningTime = 0.0f; // Initialize running time
        czechHedgehogs[i].updateTime = 1.0f/16.0f; // Set update time (seconds)
    }

    
    // Set czechHedgehog X velocity (pixels/second) 
    int czechHedgehogVelocity{-180}; // czechHedgehog velocity 

    // Finish Line
    Texture2D finishLine = LoadTexture("textures/finish-line.png"); // Load finish line texture
    Rectangle finishLineRec; // Finish line rectangle
    Vector2 finishLinePos; // Finish line position
    finishLineRec.x = 0; // Set x position
    finishLineRec.y = 0; // Set y position
    finishLineRec.width = finishLine.width; // Set width
    finishLineRec.height = finishLine.height; // Set height

    // Main Character  
    Texture2D characterSpritesheet = LoadTexture("textures/rollfy.png"); // Load character texture
    Texture2D character = characterSpritesheet; // Set character texture
    Texture2D characterJump = LoadTexture("textures/rollfy-jump.png"); // Load character jump texture
    AnimationData characterData; // Character animation data
    characterData.rec.width = character.width/9; // Set width
    characterData.rec.height = character.height; // Set height
    characterData.rec.x = 0; // Set x position
    characterData.rec.y = 0; // Set y position
    characterData.pos.x = windowWidth / 2 - characterData.rec.width / 2; // Center the character
    characterData.pos.y = windowHeight - characterData.rec.height; // Set y position to the bottom of the window
    characterData.frame = 0; // Initialize frame counter
    characterData.runningTime = 0.0f; // Initialize running time
    characterData.updateTime = 10.0f/12.0f; // Set update time (seconds)

    // Animation frame
    int velocity{0}; // Initialize velocity
    bool isInAir{false}; // Jumping state
    const int jumpVelocity{-600}; // Jump velocity (pixels/second)
    const int dashVelocity{-800}; // Dash velocity (pixels/second)

    Texture2D background = LoadTexture("textures/dnipro-far-buildings.png"); // Load background texture
    Texture2D foreground = LoadTexture("textures/dnipro-street.png"); // Load foreground texture
    
    float bagroundPositionX{0.0f}; // Background position
    float foregroundPositionX{0.0f}; // Foreground position

    int backgroundCount{3}; // Background layer count
    int foregroundCount{3}; // Foreground layer count

    bool collisionDetected{false}; // Collision detection
    bool isGameOver{false}; // Game over state

    // Lose condition
    Texture2D loseCondition = LoadTexture("textures/rollfy-game-over.png"); // Load lose condition texture
    Rectangle loseConditionRec; // Lose condition rectangle
    Vector2 loseConditionPos; // Lose condition position
    loseConditionRec.x = 0; // Set x position
    loseConditionRec.y = 0; // Set y position
    loseConditionRec.width = loseCondition.width; // Set width
    loseConditionRec.height = loseCondition.height; // Set height
    loseConditionPos.x = windowWidth / 2 - loseConditionRec.width / 2; // Center the lose condition
    loseConditionPos.y = windowHeight / 2 - loseConditionRec.height / 2; // Center the lose condition



    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        // Update
        float deltaTime{GetFrameTime()}; // Get time between frames
        DrawParallaxLayer(background, bagroundPositionX, 10.0f, deltaTime, 0.75f, backgroundCount); // Draw background
        DrawParallaxLayer(foreground, foregroundPositionX, 180.0f, deltaTime, 0.75, foregroundCount); // Draw foreground
        
        // Perform ground collision detection
        if (IsOnGround(characterData, windowHeight)) {
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

        for (int i = 0; i < sizeOfczechHedgehog; i++)
        {
            // Move czechHedgehog position
            czechHedgehogs[i].pos.x += czechHedgehogVelocity * deltaTime; // Update czechHedgehog position
        }

        // Update character the position
        characterData.pos.y += velocity * deltaTime; 

        if(isInAir) {
            character = characterJump; // Change character texture to jump
            characterData.rec.width = characterJump.width; // Set width
            characterData.rec.height = characterJump.height; // Set height
            characterData.rec.x = 0; // Set x position
        } else {
            character = characterSpritesheet; // Change character texture to normal
            characterData.rec.width = character.width/9; // Set width
            characterData.rec.height = character.height; // Set height
            characterData = updateAnimationData(characterData, deltaTime, 5); // Update character animation
        }

        for (AnimationData czechHedgehog : czechHedgehogs) {
            // Check for collision with czechHedgehog
            float pad {20.0f}; // Padding for collision detection
            Rectangle czechHedgehogRec = {
            czechHedgehog.pos.x + pad, 
            czechHedgehog.pos.y + pad, 
            czechHedgehog.rec.width - 2* pad, 
            czechHedgehog.rec.height - 2* pad
            };
            
            Rectangle characterRec = {
                characterData.pos.x, 
                characterData.pos.y, 
                characterData.rec.width, 
                characterData.rec.height 
            };

            if (CheckCollisionRecs(czechHedgehogRec, characterRec)) {
                // Collision detected
                collisionDetected = true; // Set collision detected to true
                isGameOver = true; // Set game over state
                break; // Exit loop
            }
            
        };
    
        for (int i = 0; i < sizeOfczechHedgehog; i++)
        {
            // Check if czechHedgehog is out of the screen
            if (czechHedgehogs[i].pos.x < -czechHedgehogs[i].rec.width) {
                czechHedgehogs[i].pos.x = windowWidth + GetRandomValue(0, 400); // Reset position to the right side of the screen
            }
 
            czechHedgehogs[i] = updateAnimationData(czechHedgehogs[i], deltaTime, 7); // Update czechHedgehog animation

            DrawTextureRec(czechHedgehog, czechHedgehogs[i].rec, czechHedgehogs[i].pos, WHITE); // Draw czechHedgehog
  
        }

        // Draw character
        if (isGameOver) {
            // Draw character in white
            DrawTextureRec(loseCondition, loseConditionRec, loseConditionPos, WHITE); // Draw lose condition
        } else {
            // Draw character in white
            DrawTextureRec(character, characterData.rec, characterData.pos, WHITE); 
        }

        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(character); // Unload texture
    UnloadTexture(characterJump); // Unload texture
    UnloadTexture(czechHedgehog); // Unload texture
    UnloadTexture(background); // Unload texture
    UnloadTexture(foreground); // Unload texture
    UnloadTexture(loseCondition); // Unload texture
    UnloadTexture(finishLine); // Unload texture
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
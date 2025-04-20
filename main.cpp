#include "raylib.h"
#include <vector>

struct AnimationData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float runningTime;
    float updateTime;
};

struct ParallaxLayer {
    Texture2D texture;
    float positionX;
    float speed;
    float scale;
    int count;
};

enum GameState {
    GAME_RUNNING,
    GAME_OVER,
    GAME_WIN
};

bool IsOnGround(const AnimationData& data, int windowHeight) {
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimationData UpdateAnimationData(AnimationData data, float deltaTime, int maxFrames) {
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime) {
        data.runningTime = 0;
        data.frame++;
        if (data.frame > maxFrames) data.frame = 0;
        data.rec.x = data.frame * data.rec.width;
    }
    return data;
}

void DrawParallaxLayer(ParallaxLayer& layer, float deltaTime) {
    layer.positionX -= layer.speed * deltaTime;
    if (layer.positionX <= -layer.texture.width * layer.scale) layer.positionX = 0;

    for (int i = 0; i < layer.count; i++) {
        Vector2 pos = { layer.positionX + layer.texture.width * layer.scale * i, 0.0f };
        DrawTextureEx(layer.texture, pos, 0.0f, layer.scale, WHITE);
    }
}

Rectangle GetCollisionRect(const AnimationData& data) {
    float newWidth = data.rec.width * 0.5f;
    float newHeight = data.rec.height * 0.5f;
    float offsetX = (data.rec.width - newWidth) / 2.0f;
    float offsetY = (data.rec.height - newHeight) / 2.0f;
    return { data.pos.x + offsetX, data.pos.y + offsetY, newWidth, newHeight };
}

int main() {
    
    const int windowWidth = 1200;
    const int windowHeight = 720;
    const int gravity = 1000;
    const int jumpVelocity = -600;
    const int doubleJumpVelocity = -500;
    const int moveSpeed = 300;
    const int doubleJumpHorizontal = 250;
    const int czechHedgehogVelocity = -180;
    const float finishLineDistance = 4000.0f;

    InitWindow(windowWidth, windowHeight, "Dnipro Roller");
    InitAudioDevice();
    Sound jumpSound = LoadSound("sounds/jump.wav");
    Sound landSound = LoadSound("sounds/land.wav");
    Sound hitSound = LoadSound("sounds/hit.wav");
    Sound winSound = LoadSound("sounds/win.wav");
    Sound loseSound = LoadSound("sounds/lose.wav");
    Music bgMusic = LoadMusicStream("sounds/music.mp3");
    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic, 0.3f);
    SetSoundVolume(jumpSound, 0.3f);
    SetSoundVolume(landSound, 0.3f);
    SetSoundVolume(hitSound, 0.3f);

    Texture2D czechHedgehogTex = LoadTexture("textures/czech-hedgehog.png");
    Texture2D finishLineTex = LoadTexture("textures/shelter.png");
    Texture2D charRunTex = LoadTexture("textures/rollfy.png");
    Texture2D charJumpTex = LoadTexture("textures/rollfy-jump.png");
    Texture2D bgTex = LoadTexture("textures/dnipro-far-buildings.png");
    Texture2D fgTex = LoadTexture("textures/dnipro-street.png");
    Texture2D loseTex = LoadTexture("textures/rollfy-game-over.png");
    Texture2D winTex = LoadTexture("textures/rollfy-vin.png");

    ParallaxLayer background = { bgTex, 0.0f, 10.0f, 0.75f, 3 };
    ParallaxLayer foreground = { fgTex, 0.0f, 180.0f, 0.75f, 3 };

    const int numEnemies = 3;
    std::vector<AnimationData> enemies(numEnemies);
    for (auto& enemy : enemies) {
        enemy = {
            { 0.0f, 0.0f, (float)czechHedgehogTex.width, (float)czechHedgehogTex.height },
            { (float)(windowWidth + GetRandomValue(0, 1200)), (float)(windowHeight - czechHedgehogTex.height) },
            0, 0.0f, 1.0f / 16.0f
        };
    }

    AnimationData character = {
        { 0.0f, 0.0f, (float)(charRunTex.width / 9), (float)charRunTex.height },
        { (float)(windowWidth / 2 - charRunTex.width / 18), (float)(windowHeight - charRunTex.height) },
        0, 0.0f, 10.0f / 12.0f
    };

    float distanceTraveled = 0.0f;
    bool isInAir = false;
    bool finishLinePlaced = false;
    int velocityY = 0;
    int velocityX = 0;
    int dashDir = 1;
    int jumpCount = 0;
    GameState gameState = GAME_RUNNING;

    Vector2 finishLinePos = { 0.0f, 0.0f };
    Rectangle finishLineRec = { 0, 0, (float)finishLineTex.width, (float)finishLineTex.height };

    SetTargetFPS(60);
    float countdown = 20.0f;

    while (!WindowShouldClose()) {
        UpdateMusicStream(bgMusic);
        float dt = GetFrameTime();
        if (gameState == GAME_RUNNING && countdown > 0.0f) countdown -= dt;
        BeginDrawing();
        ClearBackground(WHITE);

        DrawParallaxLayer(background, dt);
        DrawParallaxLayer(foreground, dt);
        distanceTraveled += -czechHedgehogVelocity * dt;

        // Timer UI
        if (gameState == GAME_RUNNING) {
            if (countdown <= 0.0f) {
                PlaySound(loseSound);
                gameState = GAME_OVER;
            }
            int timeLeft = (int)countdown;
            int millis = (int)((countdown - timeLeft) * 1000);
            char timerText[64];
            sprintf(timerText, "Time left: %02d.%03d", timeLeft, millis);
            DrawText(timerText, windowWidth - 250, 20, 28, RED);
        }

        if (!finishLinePlaced && distanceTraveled >= finishLineDistance) {
            finishLinePos = { (float)windowWidth, (float)(windowHeight - finishLineRec.height) };
            finishLinePlaced = true;
        }

        if (gameState == GAME_RUNNING) {
            if (IsOnGround(character, windowHeight)) {
                if (isInAir) PlaySound(landSound);
                isInAir = false;
                jumpCount = 0;
                velocityY = 0;
                character.pos.y = windowHeight - character.rec.height;
            } else {
                isInAir = true;
                velocityY += gravity * dt;
                character.rec.x = 0;
            }

            velocityX = 0;
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                velocityX = moveSpeed;
                dashDir = 1;
            }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                velocityX = -moveSpeed;
                dashDir = -1;
            }

            if (IsKeyPressed(KEY_SPACE)) {
                PlaySound(jumpSound);
                if (jumpCount == 0) {
                    velocityY = jumpVelocity;
                    jumpCount++;
                } else if (jumpCount == 1) {
                    velocityY = doubleJumpVelocity;
                    velocityX = dashDir * doubleJumpHorizontal;
                    jumpCount++;
                }
            }

            character.pos.y += velocityY * dt;
            character.pos.x += velocityX * dt;

            if (character.pos.x < 0) character.pos.x = 0;
            if (character.pos.x > windowWidth - character.rec.width) character.pos.x = windowWidth - character.rec.width;
        }

        bool facingLeft = dashDir == -1;
        Texture2D currentTex = isInAir ? charJumpTex : charRunTex;
        Rectangle drawRec = character.rec;
        Vector2 drawPos = character.pos;

        if (facingLeft) {
            drawRec.x += drawRec.width;
            drawRec.width *= -1;
        }

        if (gameState == GAME_RUNNING)
        {
            if (!isInAir) character = UpdateAnimationData(character, dt, 5);
            DrawTextureRec(currentTex, drawRec, drawPos, WHITE);
        }

        Rectangle charRec = GetCollisionRect(character);
        for (auto& enemy : enemies) {

            enemy.pos.x += czechHedgehogVelocity * dt;

            if (finishLinePlaced) {
                float safeZone = 100.0f;
                float enemyRight = enemy.pos.x + enemy.rec.width;
                float finishLeft = finishLinePos.x;
                if (enemyRight > finishLeft - safeZone && enemy.pos.x < finishLeft + safeZone) {
                    enemy.pos.x = finishLeft + safeZone + GetRandomValue(100, 300);
                }
            }
            if (enemy.pos.x < -enemy.rec.width)
                enemy.pos.x = windowWidth + GetRandomValue(0, 400);


            enemy = UpdateAnimationData(enemy, dt, 7);

            DrawTextureRec(czechHedgehogTex, enemy.rec, enemy.pos, WHITE);

            if (gameState == GAME_RUNNING && CheckCollisionRecs(GetCollisionRect(enemy), charRec)) {
                PlaySound(hitSound);
                gameState = GAME_OVER;
            }
        }

        if (finishLinePlaced) {
            finishLinePos.x += czechHedgehogVelocity * dt;

            Rectangle finishRec = { finishLinePos.x, 0, finishLineRec.width, (float)windowHeight };
            if (gameState == GAME_RUNNING && CheckCollisionRecs(finishRec, charRec)) {
                PlaySound(winSound);
                gameState = GAME_WIN;
            }

            Vector2 origin = { 0, 0 };
            DrawTexturePro(finishLineTex, finishLineRec, { finishLinePos.x, finishLinePos.y - 40, finishLineRec.width, finishLineRec.height }, origin, 0.0f, WHITE);
        }

        static bool loseSoundPlayed = false;
        if (gameState == GAME_OVER && !loseSoundPlayed) {
            PlaySound(loseSound);
            loseSoundPlayed = true;
        }
        if (gameState == GAME_OVER) {
            DrawRectangle(0, 0, windowWidth, windowHeight, Fade(BLACK, 0.5f));
            DrawTextureRec(loseTex, { 0, 0, (float)loseTex.width, (float)loseTex.height }, { windowWidth / 2.0f - loseTex.width / 2.0f, windowHeight / 2.0f - loseTex.height / 2.0f }, WHITE);
            DrawText("Don't ignore air raid warnings!", windowWidth / 2 - 250 + 3, windowHeight / 2 + 100 + 2, 26, BLACK);
            DrawText("Don't ignore air raid warnings!", windowWidth / 2 - 250, windowHeight / 2 + 100, 26, RED);
            DrawText("If you want to survive, you must reach the shelter", windowWidth / 2 - 310 + 2, windowHeight / 2 + 140 + 2, 24, BLACK);
            DrawText("If you want to survive, you must reach the shelter", windowWidth / 2 - 310, windowHeight / 2 + 140, 24, DARKGRAY);
            DrawText("as fast as possible before another strike hits your city.", windowWidth / 2 - 340 + 2, windowHeight / 2 + 170 + 2, 24, BLACK);
            DrawText("as fast as possible before another strike hits your city.", windowWidth / 2 - 340, windowHeight / 2 + 170, 24, DARKGRAY);
        }
        static bool winSoundPlayed = false;
        if (gameState == GAME_WIN && !winSoundPlayed) {
            PlaySound(winSound);
            winSoundPlayed = true;
        }
        if (gameState == GAME_WIN) {
            DrawRectangle(0, 0, windowWidth, windowHeight, Fade(BLACK, 0.5f));
            DrawTextureRec(winTex, { 0, 0, (float)winTex.width, (float)winTex.height }, { windowWidth / 2.0f - winTex.width / 2.0f, windowHeight / 2.0f - winTex.height / 2.0f }, WHITE);
            DrawText("You made it to the shelter in time!", windowWidth / 2 - 240 + 2, windowHeight / 2 + 100 + 2, 26, BLACK);
            DrawText("You made it to the shelter in time!", windowWidth / 2 - 240, windowHeight / 2 + 100, 26, DARKGREEN);
            DrawText("This time you survived... but the danger is never far away.", windowWidth / 2 - 310 + 2, windowHeight / 2 + 140 + 2, 24, BLACK);
            DrawText("This time you survived... but the danger is never far away.", windowWidth / 2 - 310, windowHeight / 2 + 140, 24, DARKGRAY);
            DrawText("Stay alert and never ignore the warning signals.", windowWidth / 2 - 280 + 2, windowHeight / 2 + 170 + 2, 24, BLACK);
            DrawText("Stay alert and never ignore the warning signals.", windowWidth / 2 - 280, windowHeight / 2 + 170, 24, DARKGRAY);
        }

        if (gameState != GAME_RUNNING) {
            DrawText("Press [R] to Restart", windowWidth / 2 - 150 + 3, windowHeight / 2 + 300 + 3, 30, BLACK);
            DrawText("Press [R] to Restart", windowWidth / 2 - 150, windowHeight / 2 + 300, 30, DARKGRAY);

            if (IsKeyPressed(KEY_R)) {
                countdown = 20.0f;
                winSoundPlayed = false;
                loseSoundPlayed = false;
                character.pos = { windowWidth / 2.0f - character.rec.width / 2.0f, windowHeight - character.rec.height };
                character.frame = 0;
                character.runningTime = 0;
                velocityX = velocityY = 0;
                distanceTraveled = 0;
                finishLinePlaced = false;
                background.positionX = foreground.positionX = 0;
                gameState = GAME_RUNNING;
                jumpCount = 0;

                for (auto& enemy : enemies) {
                    enemy.pos.x = windowWidth + GetRandomValue(0, 1200);
                    enemy.frame = 0;
                    enemy.runningTime = 0;
                }
            }
        }

        EndDrawing();
    }

    UnloadTexture(charRunTex);
    UnloadTexture(charJumpTex);
    UnloadTexture(czechHedgehogTex);
    UnloadTexture(finishLineTex);
    UnloadTexture(bgTex);
    UnloadTexture(fgTex);
    UnloadTexture(loseTex);
    UnloadTexture(winTex);
    StopMusicStream(bgMusic);
    UnloadSound(jumpSound);
    UnloadSound(hitSound);
    UnloadSound(winSound);
    UnloadSound(landSound);
    UnloadSound(loseSound);
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

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

    Texture2D czechHedgehogTex = LoadTexture("textures/czech-hedgehog.png");
    Texture2D finishLineTex = LoadTexture("textures/finish-line.png");
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

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);

        DrawParallaxLayer(background, dt);
        DrawParallaxLayer(foreground, dt);
        distanceTraveled += -czechHedgehogVelocity * dt;

        if (!finishLinePlaced && distanceTraveled >= finishLineDistance) {
            finishLinePos = { (float)windowWidth, (float)(windowHeight - finishLineRec.height) };
            finishLinePlaced = true;
        }

        if (gameState == GAME_RUNNING) {
            if (IsOnGround(character, windowHeight)) {
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
            if (enemy.pos.x < -enemy.rec.width)
                enemy.pos.x = windowWidth + GetRandomValue(0, 400);

            enemy = UpdateAnimationData(enemy, dt, 7);
            DrawTextureRec(czechHedgehogTex, enemy.rec, enemy.pos, WHITE);

            if (gameState == GAME_RUNNING && CheckCollisionRecs(GetCollisionRect(enemy), charRec))
                gameState = GAME_OVER;
        }

        if (finishLinePlaced) {
            finishLinePos.x += czechHedgehogVelocity * dt;

            Rectangle finishRec = { finishLinePos.x, 0, finishLineRec.width, (float)windowHeight };
            if (gameState == GAME_RUNNING && CheckCollisionRecs(finishRec, charRec))
                gameState = GAME_WIN;

            Vector2 origin = { 0, 0 };
            DrawTexturePro(finishLineTex, finishLineRec, { finishLinePos.x, finishLinePos.y - 30, finishLineRec.width, finishLineRec.height }, origin, 16.0f, WHITE);
        }

        if (gameState == GAME_OVER)
            DrawTextureRec(loseTex, { 0, 0, (float)loseTex.width, (float)loseTex.height }, { windowWidth / 2.0f - loseTex.width / 2.0f, windowHeight / 2.0f - loseTex.height / 2.0f }, WHITE);
        else if (gameState == GAME_WIN)
            DrawTextureRec(winTex, { 0, 0, (float)winTex.width, (float)winTex.height }, { windowWidth / 2.0f - winTex.width / 2.0f, windowHeight / 2.0f - winTex.height / 2.0f }, WHITE);

        if (gameState != GAME_RUNNING) {
            DrawText("Press [R] to Restart", windowWidth / 2 - 150, windowHeight / 2 + 300, 30, DARKGRAY);

            if (IsKeyPressed(KEY_R)) {
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
    CloseWindow();

    return 0;
}

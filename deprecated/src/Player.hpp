#pragma once
#include "raylib.h"

class Player {
public:
    Player();
    ~Player();

    void Init(const char* assetPath, Vector2 startPos,
              KeyboardKey left, KeyboardKey right,
              KeyboardKey up, KeyboardKey attack);
    void Update(float delta);
    void Draw();
    void Unload();

    int health;

private:
    Texture2D texture;
    Rectangle frame;
    Vector2 position;
    float speed;
    KeyboardKey keyLeft, keyRight, keyUp, keyAttack;
};

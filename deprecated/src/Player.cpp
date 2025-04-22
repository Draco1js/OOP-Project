#include "Player.hpp"

Player::Player()
    : texture{}, frame{0, 0, 0, 0}, position{0, 0}, speed(200.0f),
      health(100), keyLeft(KEY_NULL), keyRight(KEY_NULL), keyUp(KEY_NULL), keyAttack(KEY_NULL) {}

Player::~Player() {}

void Player::Init(const char* assetPath, Vector2 startPos,
                  KeyboardKey left, KeyboardKey right,
                  KeyboardKey up, KeyboardKey attack) {
    texture = LoadTexture(assetPath);
    frame = { 0, 0, (float)texture.width, (float)texture.height };
    position = startPos;
    keyLeft = left;
    keyRight = right;
    keyUp = up;
    keyAttack = attack;
}

void Player::Update(float delta) {
    if (IsKeyDown(keyLeft)) position.x -= speed * delta;
    if (IsKeyDown(keyRight)) position.x += speed * delta;
    if (IsKeyPressed(keyUp)) position.y -= 50;
    if (position.x < 0) position.x = 0;
    if (position.x > 800 - frame.width) position.x = 800 - frame.width;
}

void Player::Draw() {
    DrawTexture(texture, position.x, position.y, WHITE);
}

void Player::Unload() {
    UnloadTexture(texture);
}

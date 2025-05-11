
#include "Player.h"
#include "raylib.h"
#include <algorithm> // Add this for std::min
#include <string>
#include <map>
#include <vector>
#include <filesystem>

// Animation frame data structure
struct AnimationData {
    std::vector<Texture2D> frames;
    float frameTime = 0.1f;  // Time per frame in seconds
};

// Character data structure
struct CharacterData {
    std::map<std::string, AnimationData> animations;
    std::string name;
    float scale = 2.0f;  // Scale factor for rendering
};

// Global character data
std::map<std::string, CharacterData> characters;
bool charactersLoaded = false;

// Load all character animations
void LoadCharacters() {
    if (charactersLoaded) return;
    
    // Define character names
    std::vector<std::string> characterNames = {"Iori", "Ryo"};
    
    // Define animation types and their corresponding file prefixes
    std::map<std::string, std::string> animationTypes = {
        {"IDLE", "Idle"},
        {"WALKING_FORWARD", "WalkFront"},
        {"WALKING_BACKWARD", "WalkBack"},
        {"JUMPING", "Jump"},
        {"DUCKING", "Duck"},
        {"BLOCKING", "Block"},
        {"PUNCHING", "Punch"},
        {"KICKING", "Kick"},
        {"SPECIAL_ATTACK", "Combo"},
        {"HURT", "Dead"},  // Using Dead animation for hurt state
        {"READY", "Ready"}
    };
    
    // Load each character's animations
    for (const auto& charName : characterNames) {
        CharacterData charData;
        charData.name = charName;
        
        // Load each animation type
        for (const auto& anim : animationTypes) {
            auto& animKey = anim.first;
            auto& animPrefix = anim.second;
            AnimationData animData;
            int frameCount = 1;
            
            // Try loading frames until we don't find any more
            while (true) {
                std::string framePath = "assets/" + charName + "/" + animPrefix + std::to_string(frameCount) + ".png";
                if (!FileExists(framePath.c_str())) break;
                
                Texture2D texture = LoadTexture(framePath.c_str());
                animData.frames.push_back(texture);
                frameCount++;
            }
            
            // Only add animation if we found at least one frame
            if (!animData.frames.empty()) {
                // Adjust frame time based on animation type
                if (animKey == "SPECIAL_ATTACK") animData.frameTime = 0.07f;
                else if (animKey == "PUNCHING" || animKey == "KICKING") animData.frameTime = 0.08f;
                
                charData.animations[animKey] = animData;
            }
        }
        
        characters[charName] = charData;
    }
    
    charactersLoaded = true;
}

// Unload all character animations to prevent memory leaks
void UnloadCharacters() {
    if (!charactersLoaded) return;
    
    for (auto& charD : characters) {
        auto& charNameta = charD.first;
        auto& charData = charD.second;
        for (auto& [animName, animData] : charData.animations) {
            for (auto& texture : animData.frames) {
                UnloadTexture(texture);
            }
        }
    }
    
    characters.clear();
    charactersLoaded = false;
}

Player::Player(float x, float y, bool isPlayer1)
    : x(x), y(y), velocityX(0), velocityY(0), width(50), height(100),
      health(1000), isPlayer1(isPlayer1), facingRight(isPlayer1),
      isJumping(false), isDucking(false), isBlocking(false),
      currentState(State::IDLE), attackTimer(0), specialMeter(0), stunTimer(0),
      characterName(isPlayer1 ? "Iori" : "Ryo"), currentFrame(0), frameTimer(0)
{
    originalHeight = height;
    
    // Ensure characters are loaded
    if (!charactersLoaded) {
        LoadCharacters();
    }
}

void Player::Update()
{
    // Update animation frame
    const auto& charData = characters[characterName];
    std::string animKey = GetAnimationKey();
    
    if (charData.animations.count(animKey) > 0) {
        const auto& anim = charData.animations.at(animKey);
        
        frameTimer += GetFrameTime();
        if (frameTimer >= anim.frameTime) {
            frameTimer = 0;
            currentFrame = (currentFrame + 1) % anim.frames.size();
        }
    }

    // Don't allow movement during attack animation or when stunned
    if (attackTimer <= 0 && stunTimer <= 0)
    {
        // Reset velocity
        velocityX = 0;

        // Reset blocking state - will be set if block button is pressed
        isBlocking = false;

        // Check for ducking first
        bool wantsToDuck = (IsKeyDown(KEY_S) && isPlayer1) ||
                           (IsKeyDown(KEY_DOWN) && !isPlayer1);

        Duck(wantsToDuck);

        // Dedicated blocking button (B)
        if (
            ((IsKeyDown(KEY_B) && isPlayer1) ||
             (IsKeyDown(KEY_M) && !isPlayer1)) &&
            !isDucking && !isJumping)
        {
            Block();
        }

        // Movement - only if not ducking and not blocking
        if (
            (
                (IsKeyDown(KEY_A) && isPlayer1) ||
                (IsKeyDown(KEY_LEFT) && !isPlayer1)) &&
            !isBlocking)
        {
            if (isDucking)
            {
                Move(-0.4);
            }
            else
            {
                Move(-1);
            }
            facingRight = false;
        }
        if (
            (
                (IsKeyDown(KEY_D) && isPlayer1) ||
                (IsKeyDown(KEY_RIGHT) && !isPlayer1)) &&
            !isBlocking)
        {
            if (isDucking)
            {
                Move(0.4);
            }
            else
            {
                Move(1);
            }
            facingRight = true;
        }

        // Jumping
        if (
            (
                (IsKeyDown(KEY_W) && isPlayer1) ||
                (IsKeyDown(KEY_UP) && !isPlayer1)) &&
            !isJumping && !isDucking &&
            !isBlocking)
        {
            Jump();
        }

        // Attacks
        if (
            (
                (IsKeyDown(KEY_Q) && isPlayer1) ||
                (IsKeyDown(KEY_COMMA) && !isPlayer1)) &&
            !isJumping && !isDucking && !isBlocking)
        {
            Punch();
        }

        if (
            (
                (IsKeyDown(KEY_E) && isPlayer1) ||
                (IsKeyDown(KEY_PERIOD) && !isPlayer1)) &&
            !isBlocking && !isDucking)
        {
            Kick();
        }
        if (
            ((IsKeyDown(KEY_F) && isPlayer1) ||
             (IsKeyDown(KEY_SLASH) && !isPlayer1)) &&
            specialMeter >= 100 && !isBlocking)
        {
            SpecialAttack();
        }
    }

    // Apply gravity
    velocityY += GRAVITY;

    // Update position
    x += velocityX;
    y += velocityY;

    // Ground collision
    if (y + height > GetScreenHeight() - 50)
    {
        y = GetScreenHeight() - 50 - height;
        velocityY = 0;
        isJumping = false;
    }

    // Screen boundaries
    if (x < 0)
        x = 0;
    if (x + width > GetScreenWidth())
        x = GetScreenWidth() - width;

    // Update attack timer
    if (attackTimer > 0)
    {
        attackTimer -= GetFrameTime();
        if (attackTimer <= 0)
        {
            currentState = State::IDLE;
        }
    }

    // Update stun timer
    if (stunTimer > 0)
    {
        stunTimer -= GetFrameTime();
    }

    // Gradually increase special meter
    if (specialMeter < 100)
    {
        specialMeter += 0.1f;
    }
}

std::string Player::GetAnimationKey() const {
    switch (currentState) {
        case State::IDLE: return "IDLE";
        case State::WALKING: 
            return facingRight ? "WALKING_FORWARD" : "WALKING_BACKWARD";
        case State::JUMPING: return "JUMPING";
        case State::DUCKING: return "DUCKING";
        case State::BLOCKING: return "BLOCKING";
        case State::PUNCHING: return "PUNCHING";
        case State::KICKING: return "KICKING";
        case State::SPECIAL_ATTACK: return "SPECIAL_ATTACK";
        case State::HURT: return "HURT";
        default: return "IDLE";
    }
}

void Player::Draw()
{
    // Get current animation frame
    const auto& charData = characters[characterName];
    std::string animKey = GetAnimationKey();
    
    if (charData.animations.count(animKey) > 0 && !charData.animations.at(animKey).frames.empty()) {
        const auto& anim = charData.animations.at(animKey);
        Texture2D texture = anim.frames[currentFrame];
        
        // // Calculate scale and position
        // float scale = charData.scale;
        
        // Flip texture based on facing direction
        Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
        if (!facingRight) {
            source.width *= -1;  // Flip horizontally
        }
        
        // Calculate destination rectangle
        Rectangle dest = { 
            x, 
            y, 
            width, 
            height 
        };
        
        // Draw the sprite
        DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);
        
        // Draw attack hitbox for debugging
        if (IsAttacking())
        {
            Rectangle hitbox = GetAttackHitbox();
            DrawRectangleLines(hitbox.x, hitbox.y, hitbox.width, hitbox.height, YELLOW);
        }
        
        // Draw special meter
        DrawRectangle(x, y - 10, width * (specialMeter / 100.0f), 5, YELLOW);
        
        // Draw health bar
        DrawRectangle(x, y - 20, width * (health / 200.0f), 5, GREEN);
        
        // Draw state text for debugging
        DrawText(animKey.c_str(), x, y - 35, 12, WHITE);
    }
    else {
        // Fallback to rectangle if animation not found
        Color playerColor = isPlayer1 ? RED : BLUE;
        
        if (isBlocking) {
            playerColor.a = 150;
        }
        
        if (stunTimer > 0 && (int)(stunTimer * 10) % 2 == 0) {
            playerColor = WHITE;
        }
        
        DrawRectangle(x, y, width, height, playerColor);
        
        // Draw facing direction indicator
        float eyeX = facingRight ? x + width * 0.7f : x + width * 0.3f;
        DrawCircle(eyeX, y + height * 0.2f, 5, WHITE);
        
        // Draw attack hitbox for debugging
        if (IsAttacking())
        {
            Rectangle hitbox = GetAttackHitbox();
            DrawRectangleLines(hitbox.x, hitbox.y, hitbox.width, hitbox.height, YELLOW);
        }
        
        // Draw special meter
        DrawRectangle(x, y - 10, width * (specialMeter / 100.0f), 5, YELLOW);
        
        // Draw state text for debugging
        const char *stateText = "";
        switch (currentState)
        {
        case State::IDLE: stateText = "IDLE"; break;
        case State::WALKING: stateText = "WALK"; break;
        case State::JUMPING: stateText = "JUMP"; break;
        case State::DUCKING: stateText = "DUCK"; break;
        case State::BLOCKING: stateText = "BLOCK"; break;
        case State::PUNCHING: stateText = "PUNCH"; break;
        case State::KICKING: stateText = "KICK"; break;
        case State::SPECIAL_ATTACK: stateText = "SPECIAL"; break;
        case State::HURT: stateText = "HURT"; break;
        }
        DrawText(stateText, x, y - 20, 12, WHITE);
    }
}

void Player::SetCharacter(const std::string& name) {
    if (characters.count(name) > 0) {
        characterName = name;
        currentFrame = 0;
        frameTimer = 0;
    }
}

std::string Player::GetCharacterName() const {
    return characterName;
}

void Player::Move(float direction)
{
    velocityX = direction * MOVE_SPEED;
    if (currentState != State::PUNCHING && currentState != State::KICKING)
    {
        currentState = State::WALKING;
    }
}

void Player::Jump()
{
    velocityY = JUMP_FORCE;
    isJumping = true;
    currentState = State::JUMPING;
    currentFrame = 0;  // Reset animation frame
}

void Player::Duck(bool shouldDuck)
{
    // Only process if there's a change in ducking state
    if (shouldDuck != isDucking)
    {
        isDucking = shouldDuck;

        if (shouldDuck)
        {
            height = originalHeight * DUCK_HEIGHT_RATIO;
            y += originalHeight - height; // Adjust y position
            currentState = State::DUCKING;
            currentFrame = 0;  // Reset animation frame
        }
        else
        {
            height = originalHeight;
            y -= originalHeight - height; // Restore y position

            // Only change state if we're currently ducking
            if (currentState == State::DUCKING)
            {
                currentState = State::IDLE;
                currentFrame = 0;  // Reset animation frame
            }
        }
    }
}

void Player::Block()
{
    isBlocking = true;
    currentState = State::BLOCKING;
    currentFrame = 0;  // Reset animation frame
}

void Player::Punch()
{
    attackTimer = ATTACK_DURATION;
    currentState = State::PUNCHING;
    currentFrame = 0;  // Reset animation frame
    // Add a small amount to special meter
    specialMeter = std::min(specialMeter + 5.0f, 100.0f);
}

void Player::Kick()
{
    attackTimer = ATTACK_DURATION * 1.5f; // Kicks last longer
    currentState = State::KICKING;
    currentFrame = 0;  // Reset animation frame
    // Add a small amount to special meter
    specialMeter = std::min(specialMeter + 7.0f, 100.0f);
}

void Player::SpecialAttack()
{
    attackTimer = ATTACK_DURATION * 2.0f; // Special attacks last even longer
    currentState = State::SPECIAL_ATTACK;
    currentFrame = 0;  // Reset animation frame
    specialMeter = 0; // Reset special meter
}

Rectangle Player::GetRect() const
{
    return {x, y, width, height};
}

Rectangle Player::GetAttackHitbox() const
{
    if (!IsAttacking())
    {
        return {0, 0, 0, 0}; // No hitbox if not attacking
    }

    float hitboxWidth = width;
    float hitboxHeight = height * 0.3f;
    float hitboxX = x;
    float hitboxY = y + height * 0.3f;

    // Adjust hitbox based on facing direction
    if (facingRight)
    {
        hitboxX = x + width; // Position hitbox to the right of player
    }
    else
    {
        hitboxX = x - hitboxWidth; // Position hitbox to the left of player
    }

    // Different hitbox for different attacks
    if (currentState == State::PUNCHING)
    {
        // Position punch hitbox higher so it can be ducked under
        hitboxY = y + height * 0.1f;  // Much higher position for punch
        hitboxHeight = height * 0.3f; // Shorter but higher hitbox
    }
    else if (currentState == State::KICKING)
    {
        hitboxWidth = width * 1.5f;
        hitboxHeight = height * 0.2f;
        hitboxY = y + height * 0.7f; // Lower for kick
    }
    else if (currentState == State::SPECIAL_ATTACK)
    {
        hitboxWidth = width * 2.0f;
        hitboxHeight = height * 0.8f;
    }

    return {hitboxX, hitboxY, hitboxWidth, hitboxHeight};
}

int Player::GetHealth() const
{
    return health;
}

float Player::GetWidth() const
{
    return width;
}

bool Player::IsAttacking() const
{
    return (currentState == State::PUNCHING ||
            currentState == State::KICKING ||
            currentState == State::SPECIAL_ATTACK) &&
           attackTimer > 0;
}

bool Player::IsBlocking() const
{
    return isBlocking;
}

void Player::TakeDamage(int amount, bool attackFromRight)
{
    // Reduce damage if blocking
    if (isBlocking)
    {
        amount = amount / 2;
    }

    health -= amount;
    if (health < 0)
        health = 0;

    // Apply hit stun
    stunTimer = 0.2f;
    currentState = State::HURT;
    currentFrame = 0;  // Reset animation frame

    // Knockback based on attack direction
    // If attack comes from right, knockback to the left and vice versa
    velocityX = attackFromRight ? 5.0f : -5.0f;

    // Add to opponent's special meter
    specialMeter = std::min(specialMeter + amount, 100.0f);
}

void Player::Reset(float newX, float newY, bool isFirstPlayer)
{
    // Reset position
    x = newX;
    y = newY;

    // Reset movement
    velocityX = 0;
    velocityY = 0;

    // Reset state
    health = 200;
    isPlayer1 = isFirstPlayer;
    facingRight = isFirstPlayer;
    isJumping = false;
    isDucking = false;
    isBlocking = false;
    currentState = State::IDLE;
    attackTimer = 0;
    specialMeter = 0;
    stunTimer = 0;

    // Reset height in case player was ducking
    height = originalHeight;
}

Player::State Player::GetCurrentState() const
{
    return currentState;
}

bool Player::IsFacingRight() const
{
    return facingRight;
}

#include "Fighter.hpp"
#include "Damage.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SpriteFilenames.hpp"

Fighter::Fighter(float x, float y, bool isPlayer)
    : health(100), defense(10), isPlayer(isPlayer), currentState(FighterState::Idle),
      facingRight(isPlayer), fallbackTexture(), sprite(fallbackTexture), currentAnimationName("idle")
{
    // Create a fallback texture with actual content
    if (!fallbackTexture.loadFromFile("assets/sprites/player1.png"))
    {
        std::cerr << "Failed to load fallback texture" << std::endl;
    }

    // Update sprite position
    sprite.setPosition({x, y});

    // Scale down the sprite to make it smaller on screen
    sprite.setScale({0.5f, 0.5f});

    // Load character animations
    loadAnimations(isPlayer ? "Iori" : "Ryo");

    // Set initial animation
    setAnimation(FighterState::Idle);

    std::cout << "Fighter created at position: " << x << ", " << y << std::endl;
}

void Fighter::update(float deltaTime)
{
    // Update current animation and get the current frame rect
    sf::IntRect frameRect = AnimationManager::update(currentAnimationName, sprite);

    // Apply the frame rect to crop the sprite
    sprite.setTextureRect(frameRect);

    // Store current position
    sf::Vector2f position = sprite.getPosition();

    // Apply flip if needed
    if (!facingRight)
    {
        sprite.setScale({-0.5f, 0.5f}); // Maintain the 0.5 scale while flipping
        // Adjust position for flipped sprite
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x, 0});
    }
    else
    {
        sprite.setScale({0.5f, 0.5f}); // Maintain the 0.5 scale
        sprite.setOrigin({0, 0});
    }

    // Restore position
    sprite.setPosition(position);
}

void Fighter::takeDamage(int rawDamage)
{
    // Convert simple integer damage to Damage object (backward compatibility)
    Damage damage(rawDamage);
    takeDamage(damage);
}

void Fighter::takeDamage(const Damage &damage)
{
    // Calculate actual damage based on this fighter's defense
    int actualDamage = damage.calculateActualDamage(defense);

    // Apply the damage
    health -= actualDamage;

    // Visual feedback with damage color
    if (damage.getType() == DamageType::Block)
    {
        // Show block animation
        setAnimation(FighterState::Block);
        // Flash with block color
        sprite.setColor(damage.getDamageColor());
    }
    else
    {
        // Flash the sprite with the damage color
        sprite.setColor(damage.getDamageColor());
    }

    // Reset color after a brief moment (would normally use a timer)
    sprite.setColor(sf::Color::White);

    if (health <= 0)
    {
        health = 0;
        setAnimation(FighterState::Dead);
    }
}

bool Fighter::attack(Fighter &target, FighterState attackType)
{
    // First set our animation state
    setAnimation(attackType);

    // Check if we're in range to hit (simplified)
    sf::FloatRect myBounds = sprite.getGlobalBounds();
    sf::FloatRect targetBounds = target.getSprite().getGlobalBounds();

    // In SFML 3.0, findIntersection returns std::optional<sf::FloatRect>
    std::optional<sf::FloatRect> intersection = myBounds.findIntersection(targetBounds);
    if (!intersection.has_value())
    {
        return false; // Out of range
    }

    // Determine damage based on attack type
    int baseDamage = 0;
    DamageType damageType = DamageType::Normal;

    switch (attackType)
    {
    case FighterState::Punch:
        baseDamage = 10;
        break;

    case FighterState::Kick:
        baseDamage = 15;
        // 20% chance of critical hit with kicks
        if (rand() % 100 < 20)
        {
            damageType = DamageType::Critical;
        }
        break;

    // Special moves would go here
    default:
        return false; // Not an attack move
    }

    // Check if target is blocking
    if (target.getCurrentState() == FighterState::Block)
    {
        damageType = DamageType::Block;
    }

    // Create and apply damage
    Damage damage(baseDamage, damageType);
    target.takeDamage(damage);

    return true;
}

bool Fighter::isAlive() const
{
    return health > 0;
}

int Fighter::getHealth() const
{
    return health;
}

sf::Sprite &Fighter::getSprite()
{
    return sprite;
}

void Fighter::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Fighter::setupAnimation(sf::Texture &texture, const std::string &animationName,
                             const std::string &filePath, sf::Vector2i frameCount,
                             sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency)
{
    if (!texture.loadFromFile(filePath))
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
    }
    else
    {
        // Set the texture rect to crop the sprite to the correct size
        sprite.setTextureRect(sf::IntRect({0, 0}, frameSize));

        // Add the animation to the manager
        AnimationManager::addAnimation(animationName, texture, frameCount, frameSize, startPosition, frequency);
    }
}

void Fighter::loadAnimations(const std::string &characterName)
{
    std::string spritesheetPath = "assets/sprites/" + characterName + ".png";

    // Define frame sizes and counts for each character
    sf::Vector2i frameSize = {SpriteFilenames::FRAME_WIDTH, SpriteFilenames::FRAME_HEIGHT};

    // Get animation info for each state using the SpriteFilenames helper functions
    auto [blockFrameCount, blockStartPos] = SpriteFilenames::getAnimationInfo(characterName, "Block");
    auto [deadFrameCount, deadStartPos] = SpriteFilenames::getAnimationInfo(characterName, "Dead");
    auto [idleFrameCount, idleStartPos] = SpriteFilenames::getAnimationInfo(characterName, "Idle");
    auto [jumpFrameCount, jumpStartPos] = SpriteFilenames::getAnimationInfo(characterName, "Jump");
    auto [kickFrameCount, kickStartPos] = SpriteFilenames::getAnimationInfo(characterName, "Kick");
    auto [punchFrameCount, punchStartPos] = SpriteFilenames::getAnimationInfo(characterName, "Punch");
    auto [walkBackFrameCount, walkBackStartPos] = SpriteFilenames::getAnimationInfo(characterName, "WalkBack");
    auto [walkFrontFrameCount, walkFrontStartPos] = SpriteFilenames::getAnimationInfo(characterName, "WalkFront");

    // Setup animations for each state with appropriate frequencies (higher values = slower animations)
    setupAnimation(idleTexture, "idle", spritesheetPath, idleFrameCount, frameSize, idleStartPos, 20);

    setupAnimation(walkForwardTexture, "walkForward", spritesheetPath, walkFrontFrameCount, frameSize, walkFrontStartPos, 20);

    setupAnimation(walkBackwardTexture, "walkBackward", spritesheetPath, walkBackFrameCount, frameSize, walkBackStartPos, 20);

    setupAnimation(jumpTexture, "jump", spritesheetPath, jumpFrameCount, frameSize, jumpStartPos, 20);

    setupAnimation(punchTexture, "punch", spritesheetPath, punchFrameCount, frameSize, punchStartPos, 15);

    setupAnimation(kickTexture, "kick", spritesheetPath, kickFrameCount, frameSize, kickStartPos, 15);

    setupAnimation(blockTexture, "block", spritesheetPath, blockFrameCount, frameSize, blockStartPos, 20);

    setupAnimation(deadTexture, "dead", spritesheetPath, deadFrameCount, frameSize, deadStartPos, 25);

    // Set initial texture
    sprite.setTexture(idleTexture);
}

std::string Fighter::getAnimationName(FighterState state) const
{
    switch (state)
    {
    case FighterState::Idle:
        return "idle";
    case FighterState::WalkForward:
        return "walkForward";
    case FighterState::WalkBackward:
        return "walkBackward";
    case FighterState::Jump:
        return "jump";
    case FighterState::Punch:
        return "punch";
    case FighterState::Kick:
        return "kick";
    case FighterState::Block:
        return "block";
    case FighterState::Dead:
        return "dead";
    default:
        return "idle";
    }
}

void Fighter::setAnimation(FighterState state)
{
    if (currentState != state)
    {
        currentState = state;
        currentAnimationName = getAnimationName(state);

        // Set the appropriate texture based on the state
        switch (state)
        {
        case FighterState::Idle:
            sprite.setTexture(idleTexture);
            break;
        case FighterState::WalkForward:
            sprite.setTexture(walkForwardTexture);
            break;
        case FighterState::WalkBackward:
            sprite.setTexture(walkBackwardTexture);
            break;
        case FighterState::Jump:
            sprite.setTexture(jumpTexture);
            break;
        case FighterState::Punch:
            sprite.setTexture(punchTexture);
            break;
        case FighterState::Kick:
            sprite.setTexture(kickTexture);
            break;
        case FighterState::Block:
            sprite.setTexture(blockTexture);
            break;
        case FighterState::Dead:
            sprite.setTexture(deadTexture);
            break;
        default:
            sprite.setTexture(idleTexture);
            break;
        }

        // Reset the animation
        AnimationManager::resetAnimationIndex(currentAnimationName);
    }
}

FighterState Fighter::getCurrentState() const
{
    return currentState;
}

#include "Fighter.hpp"
#include "Damage.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Fighter::Fighter(float x, float y, bool isPlayer) 
    : health(100), defense(10), isPlayer(isPlayer), currentState(FighterState::Idle), 
      facingRight(isPlayer), fallbackTexture(), sprite(fallbackTexture), currentAnimationName("idle")
{
    // Create a fallback texture with actual content
    if (!fallbackTexture.loadFromFile("assets/sprites/player1.png")) {
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
    if (!facingRight) {
        sprite.setScale({-0.5f, 0.5f});  // Maintain the 0.5 scale while flipping
        // Adjust position for flipped sprite
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x, 0});
    } else {
        sprite.setScale({0.5f, 0.5f});  // Maintain the 0.5 scale
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

void Fighter::takeDamage(const Damage& damage)
{
    // Calculate actual damage based on this fighter's defense
    int actualDamage = damage.calculateActualDamage(defense);
    
    // Apply the damage
    health -= actualDamage;
    
    // Visual feedback with damage color
    if (damage.getType() == DamageType::Block) {
        // Show block animation
        setAnimation(FighterState::Block);
        // Flash with block color
        sprite.setColor(damage.getDamageColor());
    } else {
        // Flash the sprite with the damage color
        sprite.setColor(damage.getDamageColor());
    }
    
    // Reset color after a brief moment (would normally use a timer)
    sprite.setColor(sf::Color::White);
    
    if (health <= 0) {
        health = 0;
        setAnimation(FighterState::Dead);
    }
}

bool Fighter::attack(Fighter& target, FighterState attackType)
{
    // First set our animation state
    setAnimation(attackType);
    
    // Check if we're in range to hit (simplified)
    sf::FloatRect myBounds = sprite.getGlobalBounds();
    sf::FloatRect targetBounds = target.getSprite().getGlobalBounds();
    
    // In SFML 3.0, findIntersection returns std::optional<sf::FloatRect>
    std::optional<sf::FloatRect> intersection = myBounds.findIntersection(targetBounds);
    if (!intersection.has_value()) {
        return false; // Out of range
    }
    
    // Determine damage based on attack type
    int baseDamage = 0;
    DamageType damageType = DamageType::Normal;
    
    switch (attackType) {
        case FighterState::Punch:
            baseDamage = 10;
            break;
            
        case FighterState::Kick:
            baseDamage = 15;
            // 20% chance of critical hit with kicks
            if (rand() % 100 < 20) {
                damageType = DamageType::Critical;
            }
            break;
            
        // Special moves would go here
        default:
            return false; // Not an attack move
    }
    
    // Check if target is blocking
    if (target.getCurrentState() == FighterState::Block) {
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

sf::Sprite& Fighter::getSprite()
{
    return sprite;
}

void Fighter::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Fighter::setupAnimation(sf::Texture& texture, const std::string& animationName,
                            const std::string& filePath, sf::Vector2i frameCount,
                            sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency)
{
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
    } else {
        // Set the texture rect to crop the sprite to the correct size
        sprite.setTextureRect(sf::IntRect({0, 0}, frameSize));
        
        // Add the animation to the manager
        AnimationManager::addAnimation(animationName, texture, frameCount, frameSize, startPosition, frequency);
    }
}

void Fighter::loadAnimations(const std::string& characterName)
{
    std::string spritesheetPath = "assets/sprites/" + characterName + ".png";
    
    // Define frame sizes and counts for each character
    sf::Vector2i frameSize;
    std::map<FighterState, sf::Vector2i> frameCounts;
    std::map<FighterState, sf::Vector2i> startPositions;
    
    if (characterName == "Iori") {
        frameSize = {300, 300}; // Adjust based on actual spritesheet
        
        // Define frame counts for each animation
        frameCounts[FighterState::Idle] = {9, 1};
        frameCounts[FighterState::WalkForward] = {10, 1};
        frameCounts[FighterState::WalkBackward] = {9, 1};
        frameCounts[FighterState::Jump] = {9, 1};
        frameCounts[FighterState::Punch] = {9, 1};
        frameCounts[FighterState::Kick] = {10, 1};
        frameCounts[FighterState::Block] = {7, 1};
        frameCounts[FighterState::Dead] = {10, 1};
        
        // Define starting positions in the spritesheet
        startPositions[FighterState::Idle] = {0, 0};
        startPositions[FighterState::WalkForward] = {0, 300};
        startPositions[FighterState::WalkBackward] = {0, 600};
        startPositions[FighterState::Jump] = {0, 900};
        startPositions[FighterState::Punch] = {0, 1200};
        startPositions[FighterState::Kick] = {0, 1500};
        startPositions[FighterState::Block] = {0, 1800};
        startPositions[FighterState::Dead] = {0, 2100};
    } else { // Ryo
        frameSize = {300, 300}; // Adjust based on actual spritesheet
        
        // Define frame counts for each animation
        frameCounts[FighterState::Idle] = {4, 1};
        frameCounts[FighterState::WalkForward] = {6, 1};
        frameCounts[FighterState::WalkBackward] = {5, 1};
        frameCounts[FighterState::Jump] = {3, 1};
        frameCounts[FighterState::Punch] = {9, 1};
        frameCounts[FighterState::Kick] = {8, 1};
        frameCounts[FighterState::Block] = {3, 1};
        frameCounts[FighterState::Dead] = {4, 1};
        
        // Define starting positions in the spritesheet
        startPositions[FighterState::Idle] = {0, 0};
        startPositions[FighterState::WalkForward] = {0, 300};
        startPositions[FighterState::WalkBackward] = {0, 600};
        startPositions[FighterState::Jump] = {0, 900};
        startPositions[FighterState::Punch] = {0, 1200};
        startPositions[FighterState::Kick] = {0, 1500};
        startPositions[FighterState::Block] = {0, 1800};
        startPositions[FighterState::Dead] = {0, 2100};
    }
    
    // Setup animations for each state with slower frequencies (higher values = slower animations)
    setupAnimation(idleTexture, "idle", spritesheetPath, 
                  frameCounts[FighterState::Idle], frameSize, 
                  startPositions[FighterState::Idle], 20); // Increased from 10 to 20
                  
    setupAnimation(walkForwardTexture, "walkForward", spritesheetPath, 
                  frameCounts[FighterState::WalkForward], frameSize, 
                  startPositions[FighterState::WalkForward], 20); // Increased from 10 to 20
                  
    setupAnimation(walkBackwardTexture, "walkBackward", spritesheetPath, 
                  frameCounts[FighterState::WalkBackward], frameSize, 
                  startPositions[FighterState::WalkBackward], 20); // Increased from 10 to 20
                  
    setupAnimation(jumpTexture, "jump", spritesheetPath, 
                  frameCounts[FighterState::Jump], frameSize, 
                  startPositions[FighterState::Jump], 20); // Increased from 10 to 20
                  
    setupAnimation(punchTexture, "punch", spritesheetPath, 
                  frameCounts[FighterState::Punch], frameSize, 
                  startPositions[FighterState::Punch], 15); // Increased from 7 to 15
                  
    setupAnimation(kickTexture, "kick", spritesheetPath, 
                  frameCounts[FighterState::Kick], frameSize, 
                  startPositions[FighterState::Kick], 15); // Increased from 7 to 15
                  
    setupAnimation(blockTexture, "block", spritesheetPath, 
                  frameCounts[FighterState::Block], frameSize, 
                  startPositions[FighterState::Block], 20); // Increased from 10 to 20
                  
    setupAnimation(deadTexture, "dead", spritesheetPath, 
                  frameCounts[FighterState::Dead], frameSize, 
                  startPositions[FighterState::Dead], 25); // Increased from 15 to 25
                  
    // Set initial texture
    sprite.setTexture(idleTexture);
}

std::string Fighter::getAnimationName(FighterState state) const
{
    switch (state) {
        case FighterState::Idle: return "idle";
        case FighterState::WalkForward: return "walkForward";
        case FighterState::WalkBackward: return "walkBackward";
        case FighterState::Jump: return "jump";
        case FighterState::Punch: return "punch";
        case FighterState::Kick: return "kick";
        case FighterState::Block: return "block";
        case FighterState::Dead: return "dead";
        default: return "idle";
    }
}

void Fighter::setAnimation(FighterState state)
{
    if (currentState != state) {
        currentState = state;
        currentAnimationName = getAnimationName(state);
        
        // Set the appropriate texture based on the state
        switch (state) {
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

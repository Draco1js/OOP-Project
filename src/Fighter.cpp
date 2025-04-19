#include "Fighter.hpp"
#include "Damage.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>


Fighter::Fighter(float x, float y, bool isPlayer) 
    : health(100), defense(10), isPlayer(isPlayer), currentState(FighterState::Idle), 
      facingRight(isPlayer), fallbackTexture(), sprite(fallbackTexture)
{
    // Create a fallback texture with actual content
    if (!fallbackTexture.loadFromFile("assets/sprites/player1.png")) {
        std::cerr << "Failed to load fallback texture" << std::endl;
    }
    
    // Update sprite position
    sprite.setPosition({x, y});
    
    // Load character animations
    loadAnimations(isPlayer ? "Iori" : "Ryo");
    
    // Set initial animation
    setAnimation(FighterState::Idle);
    
    std::cout << "Fighter created at position: " << x << ", " << y << std::endl;
}

void Fighter::update(float deltaTime)
{
    // Update current animation
    if (animations.count(currentState) > 0) {
        animations[currentState].update(deltaTime);
        
        // Store current position and scale before updating sprite
        sf::Vector2f position = sprite.getPosition();
        
        // Get current sprite from animation
        sf::Sprite& animSprite = animations[currentState].getCurrentSprite();
        
        // Copy texture and rectangle from animation sprite
        const sf::Texture& texture = animSprite.getTexture();
        sprite.setTexture(texture, true);
        sprite.setTextureRect(animSprite.getTextureRect());
        
        // Apply flip if needed
        if (!facingRight) {
            sprite.setScale({-1.0f, 1.0f});
            // Adjust position for flipped sprite
            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin({bounds.size.x, 0});
        } else {
            sprite.setScale({1.0f, 1.0f});
            sprite.setOrigin({0, 0});
        }
        
        // Restore position
        sprite.setPosition(position);
        
        // Check if non-looping animation is finished
        if (animations[currentState].isFinished()) {
            setAnimation(FighterState::Idle);
        }
    }
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

void Fighter::loadAnimations(const std::string& characterName)
{
    std::string basePath = "assets/sprites/Assests/" + characterName + "/";
    
    // Load idle animation
    Animation idleAnim(0.1f);
    idleAnim.loadFromFiles(basePath + "Idle*.png", 
        characterName == "Iori" ? 9 : 4);
    animations[FighterState::Idle] = idleAnim;
    
    // Load walk forward animation
    Animation walkForwardAnim(0.1f);
    walkForwardAnim.loadFromFiles(basePath + "WalkFront*.png", 
        characterName == "Iori" ? 10 : 6);
    animations[FighterState::WalkForward] = walkForwardAnim;
    
    // Load walk backward animation
    Animation walkBackwardAnim(0.1f);
    walkBackwardAnim.loadFromFiles(basePath + "WalkBack*.png", 
        characterName == "Iori" ? 9 : 5);
    animations[FighterState::WalkBackward] = walkBackwardAnim;
    
    // Load jump animation
    Animation jumpAnim(0.1f);
    jumpAnim.loadFromFiles(basePath + "Jump*.png", 
        characterName == "Iori" ? 9 : 3);
    jumpAnim.setLooping(false);
    animations[FighterState::Jump] = jumpAnim;
    
    // Load punch animation
    Animation punchAnim(0.07f);
    punchAnim.loadFromFiles(basePath + "Punch*.png", 
        characterName == "Iori" ? 9 : 9);
    punchAnim.setLooping(false);
    animations[FighterState::Punch] = punchAnim;
    
    // Load kick animation
    Animation kickAnim(0.07f);
    kickAnim.loadFromFiles(basePath + "Kick*.png", 
        characterName == "Iori" ? 10 : 8);
    kickAnim.setLooping(false);
    animations[FighterState::Kick] = kickAnim;
    
    // Load block animation
    Animation blockAnim(0.1f);
    blockAnim.loadFromFiles(basePath + "Block*.png", 
        characterName == "Iori" ? 7 : 3);
    blockAnim.setLooping(false);
    animations[FighterState::Block] = blockAnim;
    
    // Load dead animation
    Animation deadAnim(0.15f);
    deadAnim.loadFromFiles(basePath + "Dead*.png", 
        characterName == "Iori" ? 10 : 4);
    deadAnim.setLooping(false);
    animations[FighterState::Dead] = deadAnim;
}

void Fighter::setAnimation(FighterState state)
{
    if (currentState != state && animations.count(state) > 0) {
        currentState = state;
        animations[currentState].reset();
    }
}

FighterState Fighter::getCurrentState() const
{
    return currentState;
}
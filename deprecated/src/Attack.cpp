#include "Attack.hpp"
#include "Damage.hpp"
#include <iostream>
#include <cstdlib> // For rand()

Attack::Attack(FighterState attackType, AttackStrength strength)
    : attackType(attackType), strength(strength)
{
    // Set base properties depending on attack type
    switch (attackType) {
        case FighterState::Punch:
            range = 50.0f;
            hitboxWidth = 40.0f;
            hitboxHeight = 20.0f;
            hitboxOffsetX = 30.0f;  // In front of the fighter
            hitboxOffsetY = -40.0f; // At chest/head height
            
            // Adjust based on strength
            if (strength == AttackStrength::Light) {
                baseDamage = 8;
                startupFrames = 3;
                recoveryFrames = 5;
            } else if (strength == AttackStrength::Medium) {
                baseDamage = 12;
                startupFrames = 5;
                recoveryFrames = 8;
            } else { // Heavy
                baseDamage = 18;
                startupFrames = 8;
                recoveryFrames = 12;
            }
            break;
            
        case FighterState::Kick:
            range = 70.0f;
            hitboxWidth = 50.0f;
            hitboxHeight = 25.0f;
            hitboxOffsetX = 40.0f;  // Further reach than punch
            hitboxOffsetY = -20.0f; // Lower than punch (leg height)
            
            // Adjust based on strength
            if (strength == AttackStrength::Light) {
                baseDamage = 10;
                startupFrames = 4;
                recoveryFrames = 6;
            } else if (strength == AttackStrength::Medium) {
                baseDamage = 15;
                startupFrames = 7;
                recoveryFrames = 10;
            } else { // Heavy
                baseDamage = 22;
                startupFrames = 10;
                recoveryFrames = 15;
            }
            break;
            
        default:
            // Default values for other attack types
            baseDamage = 5;
            range = 30.0f;
            startupFrames = 2;
            recoveryFrames = 5;
            hitboxWidth = 30.0f;
            hitboxHeight = 20.0f;
            hitboxOffsetX = 20.0f;
            hitboxOffsetY = -30.0f;
            break;
    }
}

bool Attack::execute(Fighter& attacker, Fighter& target)
{
    // Set attacker's animation
    attacker.setAnimation(attackType);
    
    // Get attacker's position and facing direction
    sf::Vector2f attackerPos = attacker.getSprite().getPosition();
    bool facingRight = attacker.getSprite().getScale().x > 0; // Assuming scale.x < 0 means facing left
    
    // Calculate attack hitbox
    sf::FloatRect hitbox = calculateHitbox(attackerPos, facingRight);
    
    // Check if target is in hitbox
    sf::FloatRect targetBounds = target.getSprite().getGlobalBounds();
    
    std::optional<sf::FloatRect> intersection = hitbox.findIntersection(targetBounds);
    if (!intersection.has_value()) {
        return false; // Target not in range
    }
    
    // Determine damage type
    DamageType damageType = DamageType::Normal;
    
    // 15% chance of critical hit for medium strength, 25% for heavy
    int critChance = 0;
    if (strength == AttackStrength::Medium)
        critChance = 15;
    else if (strength == AttackStrength::Heavy)
        critChance = 25;
        
    if (rand() % 100 < critChance) {
        damageType = DamageType::Critical;
    }
    
    // Check if target is blocking
    if (target.getCurrentState() == FighterState::Block) {
        damageType = DamageType::Block;
    }
    
    // Create damage object and apply to target
    Damage damage(baseDamage, damageType);
    target.takeDamage(damage);
    
    std::cout << "Attack hit! Dealt " << baseDamage 
              << " base damage of type " << static_cast<int>(damageType) << std::endl;
              
    return true;
}

sf::FloatRect Attack::calculateHitbox(const sf::Vector2f& position, bool facingRight) const
{
    float offsetX = facingRight ? hitboxOffsetX : -hitboxOffsetX - hitboxWidth;
    
    return sf::FloatRect(
        {position.x + offsetX, position.y + hitboxOffsetY},  // position vector
        {hitboxWidth, hitboxHeight}                          // size vector
    );
}

int Attack::getDamage() const
{
    return baseDamage;
}

float Attack::getRange() const
{
    return range;
}

float Attack::getStartupFrames() const
{
    return startupFrames;
}

float Attack::getRecoveryFrames() const
{
    return recoveryFrames;
}

void Attack::drawHitbox(sf::RenderWindow& window, const sf::Vector2f& position, bool facingRight)
{
    // Create a shape to represent the hitbox
    sf::RectangleShape hitboxShape;
    sf::FloatRect hitbox = calculateHitbox(position, facingRight);
    
    hitboxShape.setPosition(hitbox.position);
    hitboxShape.setSize(hitbox.size);
    hitboxShape.setFillColor(sf::Color(255, 0, 0, 128)); // Semi-transparent red
    hitboxShape.setOutlineColor(sf::Color::Red);
    hitboxShape.setOutlineThickness(1.0f);
    
    window.draw(hitboxShape);
}

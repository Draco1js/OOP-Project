#pragma once
#include <SFML/Graphics.hpp>
#include "Fighter.hpp"

// Forward declarations
class Damage;

enum class AttackStrength
{
    Light,   // Fast but low damage
    Medium,  // Balanced speed/damage
    Heavy    // Slow but high damage
};

class Attack
{
public:
    // Constructors
    Attack(FighterState attackType, AttackStrength strength);
    
    // Execute the attack from attacker to potential target
    bool execute(Fighter& attacker, Fighter& target);
    
    // Get attack properties
    int getDamage() const;
    float getRange() const;
    float getStartupFrames() const;
    float getRecoveryFrames() const;
    
    // Debug visualization
    void drawHitbox(sf::RenderWindow& window, const sf::Vector2f& position, bool facingRight);
    
private:
    // Calculate hitbox based on attacker position and facing
    sf::FloatRect calculateHitbox(const sf::Vector2f& position, bool facingRight) const;
    
    // Attack properties
    FighterState attackType;
    AttackStrength strength;
    int baseDamage;
    float range;
    float startupFrames;  // Frames before attack becomes active
    float recoveryFrames; // Frames after attack where fighter is vulnerable
    
    // Hitbox dimensions relative to the attacker's position
    float hitboxOffsetX;
    float hitboxOffsetY;
    float hitboxWidth;
    float hitboxHeight;
};

#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Animation.hpp"

// Forward declaration
class Damage;

enum class FighterState
{
    Idle,
    WalkForward,
    WalkBackward,
    Jump,
    Fall,
    Punch,
    Kick,
    Block,
    Dead
};

class Fighter
{
public:
    Fighter(float x, float y, bool isPlayer);
    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow &window);

    // Damage methods
    void takeDamage(int damage);           // For backward compatibility
    void takeDamage(const Damage &damage); // New method
    bool attack(Fighter &target, FighterState attackType);

    // State control
    void setAnimation(FighterState state);
    FighterState getCurrentState() const;

    // Getters
    bool isAlive() const;
    int getHealth() const;
    sf::Sprite &getSprite();

protected:
    int health;
    int defense;
    sf::Texture fallbackTexture; // Fallback texture
    sf::Sprite sprite;
    bool isPlayer;
    bool facingRight;

    FighterState currentState;
    std::map<FighterState, Animation> animations;

private:
    void loadAnimations(const std::string &characterName);
};

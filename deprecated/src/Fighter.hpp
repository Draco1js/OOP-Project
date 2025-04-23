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
    std::string currentAnimationName;
    
    // Textures for each animation state
    sf::Texture idleTexture;
    sf::Texture walkForwardTexture;
    sf::Texture walkBackwardTexture;
    sf::Texture jumpTexture;
    sf::Texture punchTexture;
    sf::Texture kickTexture;
    sf::Texture blockTexture;
    sf::Texture deadTexture;
    
    // Helper method to set up animations
    void setupAnimation(sf::Texture& texture, const std::string& animationName,
                        const std::string& filePath, sf::Vector2i frameCount,
                        sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency);
    
    // Helper to get animation name from state
    std::string getAnimationName(FighterState state) const;

private:
    void loadAnimations(const std::string &characterName);
};

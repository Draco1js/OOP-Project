#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Animation
{
public:
    Animation(float frameTime = 0.1f);

    // Add a frame to the animation
    void addFrame(const sf::Texture &texture);

    // Load frames from a directory pattern (e.g., "assets/sprites/Assests/Iori/Idle*.png")
    bool loadFromFiles(const std::string &pattern, int frameCount);

    // Update the animation
    void update(float deltaTime);

    // Get the current sprite
    sf::Sprite &getCurrentSprite();

    // Get the current texture
    const sf::Texture *getCurrentTexture() const;

    // Reset animation to first frame
    void reset();

    // Set animation speed
    void setFrameTime(float time);

    // Set if animation should loop
    void setLooping(bool loop);

    // Check if animation is finished
    bool isFinished() const;

private:
    std::vector<sf::Sprite> frames;
    std::vector<sf::Texture> textures; // Keep textures in memory
    float frameTime;
    float currentTime;
    int currentFrame;
    bool looping;
    bool finished;
};

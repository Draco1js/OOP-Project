#include "Animation.hpp"
#include <iostream>
#include <filesystem>
#include <regex>

Animation::Animation(float frameTime)
    : frameTime(frameTime), currentTime(0.0f), currentFrame(0), looping(true), finished(false)
{
}

void Animation::addFrame(const sf::Texture &texture)
{
    textures.push_back(texture);
    frames.emplace_back(textures.back());
}

bool Animation::loadFromFiles(const std::string &pattern, int frameCount)
{
    // Extract the base path and filename pattern
    std::string basePath = pattern.substr(0, pattern.find_last_of('*'));
    std::string extension = pattern.substr(pattern.find_last_of('.'));

    // Clear existing frames
    frames.clear();
    textures.clear();

    bool success = true;

    // Load each frame
    for (int i = 1; i <= frameCount; ++i)
    {
        std::string filename = basePath + std::to_string(i) + extension;
        sf::Texture texture;
        
        // Try to load the texture
        if (texture.loadFromFile(filename))
        {
            // Store the texture in our vector
            textures.push_back(texture);  // Use copy instead of move
            
            // Create a sprite using the texture we just added
            frames.emplace_back(textures.back());
        }
        else
        {
            std::cerr << "Failed to load texture: " << filename << std::endl;
            success = false;
        }
    }

    // Reset animation state
    currentFrame = 0;
    currentTime = 0;
    finished = false;

    return success && !frames.empty();
}

void Animation::update(float deltaTime)
{
    if (finished && !looping)
    {
        return;
    }

    currentTime += deltaTime;

    if (currentTime >= frameTime)
    {
        currentTime = 0;
        currentFrame++;

        if (currentFrame >= frames.size())
        {
            if (looping)
            {
                currentFrame = 0;
            }
            else
            {
                currentFrame = frames.size() - 1;
                finished = true;
            }
        }
    }
}

sf::Sprite& Animation::getCurrentSprite()
{
    if (frames.empty()) {
        static sf::Texture emptyTexture;
        static bool textureLoaded = false;
        static sf::Sprite emptySprite(emptyTexture);
        
        if (!textureLoaded) {
            auto res = emptyTexture.loadFromFile("assets/sprites/player1.png");
            if (!res) {
                std::cerr << "Failed to load fallback texture: " << std::endl;
            } else {
                std::cout << "Fallback texture loaded successfully" << std::endl;
            }
            emptySprite.setTexture(emptyTexture);
            textureLoaded = true;
        }
        return emptySprite;
    }
    
    return frames[currentFrame];
}

void Animation::reset()
{
    currentFrame = 0;
    currentTime = 0;
    finished = false;
}

void Animation::setFrameTime(float time)
{
    frameTime = time;
}

void Animation::setLooping(bool loop)
{
    looping = loop;
}

bool Animation::isFinished() const
{
    return finished;
}

const sf::Texture *Animation::getCurrentTexture() const
{
    if (frames.empty() || currentFrame >= frames.size())
    {
        return nullptr;
    }
    return &frames[currentFrame].getTexture();
}

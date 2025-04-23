#pragma once
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace SpriteFilenames {

// Iori sprite filenames without extensions
const std::vector<std::string> ioriFiles = {
    "Block1", "Block2", "Block3", "Block4", "Block5", "Block6", "Block7",
    "Combo1", "Combo2", "Combo3", "Combo4", "Combo5", "Combo6", "Combo7", 
    "Combo8", "Combo9", "Combo10", "Combo11", "Combo12", "Combo13", "Combo14", 
    "Combo15", "Combo16", "Combo17", "Combo18", "Combo19", "Combo20", "Combo21", 
    "Combo22", "Combo23", "Combo24", "Combo25", "Combo26", "Combo27",
    "Dead1", "Dead2", "Dead3", "Dead4", "Dead5", "Dead6", "Dead7", 
    "Dead8", "Dead9", "Dead10",
    "Duck1", "Duck2", "Duck3", "Duck4", "Duck5", "Duck6", "Duck7", 
    "Duck8", "Duck9",
    "Idle1", "Idle2", "Idle3", "Idle4", "Idle5", "Idle6", "Idle7", 
    "Idle8", "Idle9",
    "Iori",
    "Jump1", "Jump2", "Jump3", "Jump4", "Jump5", "Jump6", "Jump7", 
    "Jump8", "Jump9",
    "Kick1", "Kick2", "Kick3", "Kick4", "Kick5", "Kick6", "Kick7", 
    "Kick8", "Kick9", "Kick10",
    "Punch1", "Punch2", "Punch3", "Punch4", "Punch5", "Punch6", "Punch7", 
    "Punch8", "Punch9",
    "Ready1", "Ready2", "Ready3", "Ready4", "Ready5", "Ready6", "Ready7",
    "WalkBack1", "WalkBack2", "WalkBack3", "WalkBack4", "WalkBack5", "WalkBack6", 
    "WalkBack7", "WalkBack8", "WalkBack9",
    "WalkFront1", "WalkFront2", "WalkFront3", "WalkFront4", "WalkFront5", "WalkFront6", 
    "WalkFront7", "WalkFront8", "WalkFront9", "WalkFront10"
};

// Ryo sprite filenames without extensions
const std::vector<std::string> ryoFiles = {
    "Block1", "Block2", "Block3",
    "Combo1", "Combo2", "Combo3", "Combo4", "Combo5", "Combo6", "Combo7", 
    "Combo8", "Combo9", "Combo10", "Combo11", "Combo12", "Combo13", "Combo14", 
    "Combo15",
    "Dead1", "Dead2", "Dead3", "Dead4",
    "Duck1", "Duck2",
    "Idle1", "Idle2", "Idle3", "Idle4",
    "Jump1", "Jump2", "Jump3",
    "Kick1", "Kick2", "Kick3", "Kick4", "Kick5", "Kick6", "Kick7", "Kick8",
    "Punch1", "Punch2", "Punch3", "Punch4", "Punch5", "Punch6", "Punch7", 
    "Punch8", "Punch9",
    "Ready1", "Ready2", "Ready3", "Ready4",
    "WalkBack1", "WalkBack2", "WalkBack3", "WalkBack4", "WalkBack5",
    "WalkFront1", "WalkFront2", "WalkFront3", "WalkFront4", "WalkFront5", "WalkFront6",
    "Win1", "Win2", "Win3"
};

// Map to get files by character name
const std::map<std::string, const std::vector<std::string>&> characterFiles = {
    {"Iori", ioriFiles},
    {"Ryo", ryoFiles}
};

// Standard frame size for all sprites
const int FRAME_WIDTH = 300;
const int FRAME_HEIGHT = 300;

// Animation sequence order for calculating offsets
const std::vector<std::string> animationSequence = {
    "Block", "Combo", "Dead", "Duck", "Idle", "Jump", "Kick", "Punch", "Ready", "WalkBack", "WalkFront", "Win"
};

// Helper function to get file path (now adds .png extension)
inline std::string getFilePath(const std::string& character, const std::string& filename) {
    return "assets/sprites/" + character + "/" + filename + ".png";
}

// Helper function to get all files for a specific animation type
inline std::vector<std::string> getAnimationFiles(const std::string& character, const std::string& animationType) {
    std::vector<std::string> result;
    
    try {
        const auto& files = characterFiles.at(character);
        
        for (const auto& file : files) {
            if (file.find(animationType) == 0) {
                result.push_back(file);
            }
        }
    } catch (const std::out_of_range&) {
        // Handle case where character doesn't exist
    }
    
    return result;
}

// Helper function to count frames for a specific animation type
inline int getFrameCount(const std::string& character, const std::string& animationType) {
    return getAnimationFiles(character, animationType).size();
}

// Helper function to calculate the offset for an animation in the spritesheet
inline int getAnimationOffset(const std::string& character, const std::string& animationType) {
    int offset = 0;
    
    // Count frames of all animations that come before this one in the sequence
    for (const auto& anim : animationSequence) {
        if (anim == animationType) {
            break;
        }
        
        // Add the number of frames for this animation type to the offset
        offset += getFrameCount(character, anim);
    }
    
    return offset;
}

// Helper function to get the spritesheet position for a specific animation
inline sf::Vector2i getSpritePosition(const std::string& character, const std::string& animationType) {
    int offset = getAnimationOffset(character, animationType);
    return {offset * FRAME_WIDTH, 0}; // All animations are in a single row (y=0)
}

// Helper function to get frame count and position for an animation
inline std::pair<sf::Vector2i, sf::Vector2i> getAnimationInfo(const std::string& character, const std::string& animationType) {
    int count = getFrameCount(character, animationType);
    sf::Vector2i position = getSpritePosition(character, animationType);
    
    return {{count, 1}, position}; // frameCount is {width, height} in sheet coordinates
}

} // namespace SpriteFilenames

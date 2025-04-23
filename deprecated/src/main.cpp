#include "Game.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdio>
#include <exception>
#include <stdexcept>

int main() {
    std::cout << "=== Program starting ===" << std::endl;
    
    // Add a pause at the beginning to ensure we can see output
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    try {
        std::cout << "About to create Game object" << std::endl;
        
        // Try to create the game object
        Game game;
        
        std::cout << "Game object created successfully" << std::endl;
        std::cout << "About to run game" << std::endl;
        
        // Add another pause
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Run the game
        game.run();
        
        std::cout << "Game run completed" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "UNKNOWN EXCEPTION CAUGHT" << std::endl;
        return 1;
    }
    
    std::cout << "=== Program ending normally ===" << std::endl;
    
    // Final pause to ensure we can see the output
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    return 0;
}

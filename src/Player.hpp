#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Fighter.hpp"
#include<iostream>

class Player : public Fighter {
public:
    Player(float x, float y, bool isPlayer);
    void update() override;
    void handleInput();
};

#endif // PLAYER_HPP
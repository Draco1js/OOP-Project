#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// Types of damage that can be dealt in the game
enum class DamageType
{
	Normal,	  // Standard damage
	Critical, // Extra damage (critical hits)
	Special,  // Special move damage
	Block	  // Reduced damage when blocking
};

class Damage
{
public:
	// Constructor with default damage type
	Damage(int amount, DamageType type = DamageType::Normal);

	// Calculate final damage based on attacker and defender stats
	int calculateActualDamage(int defenderDefense = 0) const;

	// Get a color representing this damage type (for visual effects)
	sf::Color getDamageColor() const;

	// Getters
	int getAmount() const;
	DamageType getType() const;
	std::string getTypeAsString() const;

	// Static helper to get multiplier for each damage type
	static float getMultiplierForType(DamageType type);

private:
	int amount;
	DamageType type;
};

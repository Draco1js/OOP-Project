#include "Damage.hpp"
#include <cmath>

Damage::Damage(int amount, DamageType type)
	: amount(amount), type(type)
{
}

int Damage::calculateActualDamage(int defenderDefense) const
{
	// Apply type multiplier to base damage
	float multiplier = getMultiplierForType(type);
	float calculatedDamage = amount * multiplier;

	// Apply defense reduction (simple formula)
	calculatedDamage = calculatedDamage * (100.0f / (100.0f + defenderDefense));

	// Round to nearest integer and ensure minimum damage of 1
	int finalDamage = static_cast<int>(std::round(calculatedDamage));
	return (finalDamage < 1 && amount > 0) ? 1 : finalDamage;
}

int Damage::getAmount() const
{
	return amount;
}

DamageType Damage::getType() const
{
	return type;
}

std::string Damage::getTypeAsString() const
{
	switch (type)
	{
	case DamageType::Normal:
		return "Normal";
	case DamageType::Critical:
		return "Critical";
	case DamageType::Special:
		return "Special";
	case DamageType::Block:
		return "Block";
	default:
		return "Unknown";
	}
}

sf::Color Damage::getDamageColor() const
{
	switch (type)
	{
	case DamageType::Normal:
		return sf::Color::White;
	case DamageType::Critical:
		return sf::Color::Red;
	case DamageType::Special:
		return sf::Color(64, 224, 208); // Turquoise
	case DamageType::Block:
		return sf::Color(150, 150, 150); // Gray
	default:
		return sf::Color::White;
	}
}

float Damage::getMultiplierForType(DamageType type)
{
	switch (type)
	{
	case DamageType::Normal:
		return 1.0f;
	case DamageType::Critical:
		return 1.5f;
	case DamageType::Special:
		return 1.3f;
	case DamageType::Block:
		return 0.3f; // Reduced damage when blocking
	default:
		return 1.0f;
	}
}

#include "Weapon.h"

Weapon::Weapon(const std::string& name, int baseDamage, int strengthModifier, int durability)
    : name(name), baseDamage(baseDamage), strengthModifier(strengthModifier), durability(durability) {}

int Weapon::getTotalDamage(int heroStrength) const {
    return baseDamage + (strengthModifier * heroStrength);
}

void Weapon::use() {
    if (durability > 0) {
        durability--;
    }
}

bool Weapon::isBroken() const {
    return durability <= 0;
}

void Weapon::display() const {
    std::cout << "Weapon: " << name
              << " | Base Damage: " << baseDamage
              << " | Strength Modifier: " << strengthModifier
              << " | Durability: " << durability
              << " | Enemies Defeated: " << enemiesDefeated << std::endl;
}

std::string Weapon::getName() const {
    return name;
}

int Weapon::getDurability() const {
    return durability;
}

#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <iostream>

class Weapon {
private:
    std::string name;
    int baseDamage;
    int strengthModifier;
    int durability;
    int enemiesDefeated = 0; // Add this line

public:
    Weapon(const std::string& name, int baseDamage, int strengthModifier, int durability);

    int getTotalDamage(int heroStrength) const;
    void use();
    bool isBroken() const;
    void display() const;

    std::string getName() const;
    int getDurability() const;
    void incrementEnemiesDefeated() { enemiesDefeated++; }
    int getEnemiesDefeated() const { return enemiesDefeated; }
};

#endif

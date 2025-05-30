#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <iostream>
using namespace std;

class Weapon {
private:
    std::string name;
    int baseDamage;
    int strengthModifier;
    int durability;

public:
    Weapon(const std::string& name, int baseDamage, int strengthModifier, int durability);

    int getTotalDamage(int heroStrength) const;
    void use();
    bool isBroken() const;
    void display() const;
    std::string getName() const;
    int getDurability() const;
    int getStrengthModifier() const { return strengthModifier; }
};

#endif

#include "Hero.h"
#include "Enemy.h"
#include <iostream>
#include <string>
using namespace std;

Hero::Hero(const string& name, int level, int exp, int health, int strength)
    : name(name), level(level), exp(exp), health(health), strength(strength) {}

void Hero::displayStats() const {
    cout << "Name: " << name
         << ", Level: " << level
         << ", Experience: " << exp
         << ", Health: " << health
         << ", Strength: " << strength
         << ", Gold: " << gold // Add this line
         << endl;
}

void Hero::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Hero::attack(Enemy& target) {
    cout << name << " attacks " << target.getName() << " for " << strength << " damage!" << endl;
    target.takeDamage(strength);
}

int Hero::getHealth() const {
    return health;
}

int Hero::getMaxHealth() const {
    return 100 + (level - 1) * 20;
}

void Hero::setMaxHealth() {
    health = getMaxHealth();
}

int Hero::getStrength() const {
    return strength;
}

int Hero::getLevel() const {
    return level;
}

int Hero::getExp() const {
    return exp;
}

void Hero::gainExp(int amount) {
    exp += amount;
    cout << name << " gained " << amount << " EXP!" << endl;
    checkLevelUp(); // Automatically check after gaining Exp
}

void Hero::checkLevelUp() {
    const int expPerLevel = 100 * level;
    while (exp >= expPerLevel) {
        exp -= expPerLevel;
        level++;
        strength += 5;
        health += 20;
        cout << name << " leveled up to level " << level << "!" << endl;
        cout << "Stats increased! +" << 5 << " Strength, +" << 20 << " Health!" << endl;
        cout << endl;
    }
}

string Hero::getName() const {
    return name;
}

Hero::~Hero() {}

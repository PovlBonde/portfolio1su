#include "Hero.h"
#include "Enemy.h"
#include <iostream>
#include <string>
using namespace std;

Hero::Hero(const string& name, int level, int exp, int health, int strength)
    : name(name), level(level), exp(exp), health(health), strength(strength), weapon(nullptr) {}

void Hero::displayStats() const {
    cout << "Name: " << name
         << ", Level: " << level
         << ", Experience: " << exp
         << ", Health: " << health
         << ", Strength: " << strength
         << ", Gold: " << gold
         << ", Enemies Defeated: " << enemiesDefeated << endl;
    if (weapon) {
        weapon->display();
        if (weapon->isBroken()) cout << " (Broken)";
        cout << endl;
    }
}

void Hero::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Hero::equipWeapon(Weapon* newWeapon) {
    if (weapon) delete weapon; // Clean up old weapon
    weapon = newWeapon;
    cout << name << " equipped " << weapon->getName() << "!" << endl;
    cout << endl;
}

int Hero::attackDamage() const {
    if (weapon && !weapon->isBroken())
        return weapon->getTotalDamage(strength);
    return strength;
}

void Hero::attack(Enemy& target) {
    int dmg = attackDamage();
    cout << name << " attacks " << target.getName() << " for " << dmg << " damage!" << endl;
    target.takeDamage(dmg);
    if (weapon && !weapon->isBroken()) weapon->use();

    if (target.getHealth() <= 0) {
        enemiesDefeated++;
        if (weapon && !weapon->isBroken()) {
            weaponKills[weapon->getName()]++;
        }
        // ...other code for enemy defeat...
    }
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
    cout << name << " gained " << amount << " EXP!" << endl << endl;
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

Hero::~Hero() {
    if (weapon) delete weapon;
}

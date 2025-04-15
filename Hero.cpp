#include "Hero.h"
#include "Enemy.h"
#include <iostream>
#include <string>
using namespace std;

Hero::Hero(const string& name, int level, int exp, int health, int strength)
    : name(name), level(level), exp(exp), health(health), strength(strength) {}

void Hero::displayStats() const {
    cout << "Name: " << name << ", Level: " << level << ", Experience: " << exp << ", Health: " << health << ", Strength: " << strength << endl;
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

void Hero::gainExp(int amount) {
    exp += amount;
    cout << name << " gained " << amount << " experience points!" << endl;
}

string Hero::getName() const {
    return name;
}

Hero::~Hero() {}

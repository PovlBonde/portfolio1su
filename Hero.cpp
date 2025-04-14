#include "Hero.h"
#include <iostream>
#include <string>
using namespace std;

Hero::Hero(const string& name, int level, int exp, int health, int strength) // Fix constructor implementation
    : name(name), level(level), exp(exp), health(health), strength(strength) {}

void Hero::displayStats() const {
    cout << "Name: " << name << ", Level: " << level << ", Experience: " << exp << ", Health: " << health << ", Strength: "<< strength << endl;
}

void Hero::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Hero::attack(Hero& target) {
    cout << name << " attacks " << target.name << " for " << strength << " damage!" << endl;
    target.takeDamage(strength);
}

Hero::~Hero() {}

#include "Enemy.h"
#include "Hero.h"
#include <iostream>
#include <string>
using namespace std;

Enemy::Enemy(const string& name, int health, int strength, int dropExp)
    : name(name), health(health), strength(strength), dropExp(dropExp) {}

void Enemy::displayStats() const {
    cout << "Name: " << name << ", Health: " << health << ", Strength: " << strength << ", Exp Dropped when slain: " << dropExp << endl;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Enemy::attack(Hero& target) {
    cout << name << " attacks " << target.getName() << " for " << strength << " damage!" << endl; // Use getName to access name of the hero
    target.takeDamage(strength);
}

int Enemy::getHealth() const {
    return health;
}

string Enemy::getName() const {
    return name;
}

int Enemy::getDropExp() const {
    return dropExp;
}

Enemy::~Enemy() {}
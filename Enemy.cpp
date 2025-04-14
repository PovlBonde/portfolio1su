#include "Enemy.h"
#include <iostream>
#include <string>
using namespace std;

Enemy::Enemy(const string& name, int level, int health, int strength)
    : name(name), level(level), health(health), strength(strength) {}

void Enemy::displayStats() const {
    cout << "Name: " << name << ", Level: " << level << ", Health: " << health << ", Strength: " << strength << endl;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Enemy::attack(Enemy& target) {
    cout << name << " attacks " << target.name << " for " << strength << " damage!" << endl;
    target.takeDamage(strength);
}

Enemy::~Enemy() {}
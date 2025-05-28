#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <iostream>
using namespace std;

// Forward declaration of Hero class
class Hero;

class Enemy {
private:
    string name;
    int health;
    int strength;
    int dropExp;

public:
    Enemy(const string& name, int health, int strength, int dropExp);
    ~Enemy();
    void displayStats() const;
    void takeDamage(int damage);
    void attack(Hero& target);
    int getHealth() const;
    string getName() const;
    int getDropExp() const;
    int getStrength() const;
};

#endif // ENEMY_H
#include <string>
#include <iostream>
using namespace std;

class Enemy {
private:
    string name;
    int level;
    int health;
    int strength;

public:
    Enemy(const string& name, int level, int health, int strength);
    ~Enemy(); // Destructor declaration
    void displayStats() const;
    void takeDamage(int damage);
    void attack(Enemy& target);
};
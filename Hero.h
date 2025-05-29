#include <string>
#include "Weapon.h"
using namespace std;

// Forward declaration of Enemy class
class Enemy;

class Hero {
private:
    string name;
    int level;
    int exp;
    int health;
    int strength;
    int gold = 0;
    int enemiesDefeated = 0; // Add this line
    Weapon* weapon = nullptr; // Add this line

public:
    Hero(const string& name, int level, int exp, int health, int strength);
    ~Hero();
    void displayStats() const;
    void takeDamage(int damage);
    void attack(Enemy& target);
    int getHealth() const;
    int getMaxHealth() const;
    void setMaxHealth();
    int getStrength() const;
    void gainExp(int amount);
    void checkLevelUp();
    string getName() const;
    int getLevel() const;
    int getExp() const;
    int getGold() const { return gold; } // Add this getter
    void addGold(int amount) { gold += amount; } // Add this setter
    Weapon* getWeapon() const { return weapon; }
    void equipWeapon(Weapon* newWeapon);
    int attackDamage() const; // Returns damage with weapon if equipped
    int getEnemiesDefeated() const { return enemiesDefeated; } // Add getter
};

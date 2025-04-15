#include <string>
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
    int dropExp;

public:
    Hero(const string& name, int level, int exp, int health, int strength);
    ~Hero();
    void displayStats() const;
    void takeDamage(int damage);
    void attack(Enemy& target);
    int getHealth() const;
    int getStrength() const;
    void gainExp(int amount);
    void checkLevelUp();
    string getName() const;
};

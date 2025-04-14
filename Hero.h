#include <string>
using namespace std;

class Hero {
private:
    string name;
    int level;
    int exp;
    int health;
    int strength;

public:
    Hero(const string& name, int level, int exp, int health, int strength); // Fix constructor signature
    ~Hero(); // Add destructor declaration
    void displayStats() const;
    void takeDamage(int damage);
    void attack(Hero& target);
};

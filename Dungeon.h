#include <string>
#include <vector>
#include "Enemy.h"
#include "Weapon.h"
using namespace std;

class Dungeon {
public:
    Dungeon(const string& name, int level);
    ~Dungeon();

    string getName() const;
    int getLevel() const;
    const vector<Enemy>& getEnemies() const;
    int getDropGold() const;
    Weapon* getDroppedWeapon() const;

private:
    string name;
    int level;
    vector<Enemy> enemies;
    int dropGold;
    Weapon* droppedWeapon = nullptr;

    vector<Enemy> generateEnemies(int level);
};
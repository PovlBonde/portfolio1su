#include <string>
#include <vector>
#include "Enemy.h"
using namespace std;

class Dungeon {
public:
    Dungeon(const string& name, int level);

    string getName() const;
    int getLevel() const;
    const vector<Enemy>& getEnemies() const;
    int getDropGold() const;

private:
    string name;
    int level;
    vector<Enemy> enemies;
    int dropGold;

    vector<Enemy> generateEnemies(int level);
};
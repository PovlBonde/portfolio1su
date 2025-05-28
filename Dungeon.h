#include <string>
#include <vector>
#include "Enemy.h"

class Dungeon {
public:
    Dungeon(const std::string& name, int level);

    std::string getName() const;
    int getLevel() const;
    const std::vector<Enemy>& getEnemies() const;

private:
    std::string name;
    int level;
    std::vector<Enemy> enemies;

    std::vector<Enemy> generateEnemies(int level);
};
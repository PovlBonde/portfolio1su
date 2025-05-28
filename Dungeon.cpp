#include "Dungeon.h"
#include <iostream>
#include <cstdlib> // for rand, srand
#include <ctime>
using namespace std;

Dungeon::Dungeon(const string& name, int level)
    : name(name), level(level)
{
    if (level <= 0) {
        throw invalid_argument("Dungeon level must be > 0");
    }
    enemies = generateEnemies(level);
    dropGold = rand() % (level * 10 + 10) + 5; // Store drop gold once
}

string Dungeon::getName() const {
    return name;
}

int Dungeon::getLevel() const {
    return level;
}

const vector<Enemy>& Dungeon::getEnemies() const {
    return enemies;
}

int Dungeon::getDropGold() const {
    return dropGold; // Return stored value
}

vector<Enemy> Dungeon::generateEnemies(int level) {
    vector<Enemy> enemies;
    int numEnemies = rand() % 3 + level; // 1–(level+2) enemies

    for (int i = 0; i < numEnemies; ++i) {
        string enemyName;
        int health, strength, dropExp;

        if (level <= 0) {
            throw invalid_argument("Dungeon level must be > 0");
        }
        else if (level == 1) {
            enemyName = "Lesser Goblin L" + to_string(level) + "-" + to_string(i + 1);
            health = 20 + level * 10 + rand() % 10;
            strength = 5 + level * 2 + rand() % 5;
            dropExp = 30 + level * 10;
        }
        else if (level == 2) {
            enemyName = "Greater Goblin L" + to_string(level) + "-" + to_string(i + 1);
            health = 30 + level * 10 + rand() % 15;
            strength = 8 + level * 2 + rand() % 6;
            dropExp = 50 + level * 15;
        }
        else if (level == 3) {
            enemyName = "Troll L" + to_string(level) + "-" + to_string(i + 1);
            health = 40 + level * 12 + rand() % 20;
            strength = 12 + level * 3 + rand() % 8;
            dropExp = 80 + level * 20;
        }
        else if (level == 4) {
            enemyName = "Dark Elf L" + to_string(level) + "-" + to_string(i + 1);
            health = 60 + level * 15 + rand() % 30;
            strength = 15 + level * 4 + rand() % 10;
            dropExp = 120 + level * 25;
        }

        enemies.emplace_back(enemyName, health, strength, dropExp);
    }

    return enemies;
}
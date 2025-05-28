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

    // Seed rand once, ideally in main
    enemies = generateEnemies(level);
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
        else if (level >= 3) {
            enemyName = "Troll L" + to_string(level) + "-" + to_string(i + 1);
            health = 40 + level * 12 + rand() % 20;
            strength = 12 + level * 3 + rand() % 8;
            dropExp = 80 + level * 20;
        }

        enemies.emplace_back(enemyName, health, strength, dropExp);
    }

    return enemies;
}
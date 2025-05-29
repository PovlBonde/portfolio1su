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
    int numEnemies = rand() % 2 + level; // 1–(level+1) enemies, less overwhelming

    for (int i = 0; i < numEnemies; ++i) {
        string enemyName;
        int health, strength, dropExp;

        if (level == 1) {
            enemyName = "Lesser Goblin L" + to_string(level) + "-" + to_string(i + 1);
            health = 18 + rand() % 6;         // 18–23 HP
            strength = 4 + rand() % 3;        // 4–6 STR
            dropExp = 30 + rand() % 11;       // 30–40 EXP
        }
        else if (level == 2) {
            enemyName = "Greater Goblin L" + to_string(level) + "-" + to_string(i + 1);
            health = 28 + rand() % 8;         // 28–35 HP
            strength = 7 + rand() % 3;        // 7–9 STR
            dropExp = 45 + rand() % 16;       // 45–60 EXP
        }
        else if (level == 3) {
            enemyName = "Troll L" + to_string(level) + "-" + to_string(i + 1);
            health = 40 + rand() % 11;        // 40–50 HP
            strength = 10 + rand() % 4;       // 10–13 STR
            dropExp = 70 + rand() % 21;       // 70–90 EXP
        }
        else if (level == 4) {
            enemyName = "Dark Elf L" + to_string(level) + "-" + to_string(i + 1);
            health = 55 + rand() % 16;        // 55–70 HP
            strength = 13 + rand() % 5;       // 13–17 STR
            dropExp = 110 + rand() % 21;      // 110–130 EXP
        }

        enemies.emplace_back(enemyName, health, strength, dropExp);
    }

    return enemies;
}
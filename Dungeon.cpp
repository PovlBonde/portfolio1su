#include "Dungeon.h"
#include <iostream>
#include <cstdlib> // for rand, srand
#include <ctime>

Dungeon::Dungeon(const std::string& name, int level)
    : name(name), level(level)
{
    if (level <= 0) {
        throw std::invalid_argument("Dungeon level must be > 0");
    }

    // Seed rand once, ideally in main
    enemies = generateEnemies(level);
}

std::string Dungeon::getName() const {
    return name;
}

int Dungeon::getLevel() const {
    return level;
}

const std::vector<Enemy>& Dungeon::getEnemies() const {
    return enemies;
}

std::vector<Enemy> Dungeon::generateEnemies(int level) {
    std::vector<Enemy> enemies;
    int numEnemies = rand() % 3 + level; // 1–(level+2) enemies

    for (int i = 0; i < numEnemies; ++i) {
        std::string enemyName = "Enemy L" + std::to_string(level) + "-" + std::to_string(i + 1);
        int health = 20 + level * 10 + rand() % 10;
        int strength = 5 + level * 2 + rand() % 5;
        int dropExp = 30 + level * 10;

        enemies.emplace_back(enemyName, health, strength, dropExp);
    }

    return enemies;
}
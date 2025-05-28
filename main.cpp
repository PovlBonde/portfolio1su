#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include "Hero.h"
#include "Enemy.h"
#include "Dungeon.h"
using namespace std;

void saveGame(const Hero& hero, const vector<Enemy>& enemies) {
    ofstream saveFile("savegame.txt");
    if (!saveFile.is_open()) {
        cout << "Error: Could not open file to save game!" << endl;
        return;
    }

    saveFile << hero.getName() << " "
             << hero.getLevel() << " "
             << hero.getExp() << " "
             << hero.getHealth() << " "
             << hero.getStrength() << endl;

    saveFile << enemies.size() << endl;

    for (const auto& enemy : enemies) {
        saveFile << enemy.getName() << " "
                 << enemy.getHealth() << " "
                 << enemy.getStrength() << " "
                 << enemy.getDropExp() << endl;
    }

    saveFile.close();
    cout << "Game saved successfully!" << endl << endl;
}

bool loadGame(Hero& hero, vector<Enemy>& enemies) {
    ifstream loadFile("savegame.txt");
    if (!loadFile.is_open()) {
        cout << "No saved game found!" << endl;
        return false;
    }

    string heroName;
    int heroLevel, heroExp, heroHealth, heroStrength;

    if (!(loadFile >> heroName >> heroLevel >> heroExp >> heroHealth >> heroStrength)) {
        cout << "Error: Failed to load hero data!" << endl;
        return false;
    }

    hero = Hero(heroName, heroLevel, heroExp, heroHealth, heroStrength);

    int enemyCount;
    if (!(loadFile >> enemyCount)) {
        cout << "Error: Failed to load enemy count!" << endl;
        return false;
    }
    loadFile.ignore(); // consume the leftover newline

    enemies.clear();
    for (int i = 0; i < enemyCount; ++i) {
        string line;
        getline(loadFile, line);

        if (line.empty()) {
            cout << "Error: Empty line for enemy #" << i + 1 << endl;
            return false;
        }

        // Match enemy line with format: name (any words) + 3 integers
        regex pattern(R"(^(.*)\s+(\d+)\s+(\d+)\s+(\d+)$)");
        smatch match;

        if (regex_match(line, match, pattern)) {
            string name = match[1];
            int health = stoi(match[2]);
            int strength = stoi(match[3]);
            int dropExp = stoi(match[4]);

            enemies.emplace_back(name, health, strength, dropExp);
        } else {
            cout << "Error: Failed to parse enemy stats for enemy #" << i + 1 << endl;
            return false;
        }
    }

    loadFile.close();
    cout << "Game loaded successfully!" << endl;
    return true;
}

int main() {
    cout << "Welcome to my game!" << endl;

    int choice;
    Hero selectedHero("", 0, 0, 0, 0);
    vector<Enemy> enemies;
    bool gameLoaded = false;

    while (true) {
        cout << "Select options:" << endl;
        cout << "0. Create Game" << endl;
        cout << "1. Load Game" << endl;
        cout << "2. Exit" << endl << endl;

        cin >> choice;
        cout << endl;

        switch (choice) {
            case 0:
                cout << "Creating a new game..." << endl;
                break;
            case 1:
                cout << "Loading game..." << endl;
                gameLoaded = loadGame(selectedHero, enemies);
                if (!gameLoaded) {
                    cout << "Starting a new game instead..." << endl;
                }
                break;
            case 2:
                cout << "Exiting game..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Try again." << endl;
                continue;
        }
        break;
    }

    cout << endl;

    if (!gameLoaded) {
        while (true) {
            cout << "Select your character:" << endl;
            cout << "0. Warrior - A strong human with high resilience, but lower damage" << endl;
            cout << "1. Mage - A powerful magic being with high damage potential, but easy to kill, if struck" << endl;
            cout << "2. Archer - A fast and adept elf, capable of high damage, while still able to take a couple of hits" << endl << endl;

            cin >> choice;
            cout << endl;

            switch (choice) {
                case 0:
                    cout << "You have selected Warrior!" << endl;
                    selectedHero = Hero("Warrior", 1, 0, 100, 10);
                    break;
                case 1:
                    cout << "You have selected Mage!" << endl;
                    selectedHero = Hero("Mage", 1, 0, 50, 20);
                    break;
                case 2:
                    cout << "You have selected Archer!" << endl;
                    selectedHero = Hero("Archer", 1, 0, 75, 15);
                    break;
                default:
                    cout << "Invalid choice. Try again." << endl;
                    continue;
            }
            break;
        }
    }

    cout << endl;
    selectedHero.displayStats();
    cout << endl;

    //Create dungeon and enemies
    Dungeon dungeon("Goblin Forest", 1);
    Dungeon dungeon2("Dark Cave", 2);
    Dungeon dungeon3("Troll Lair", 3);

    cout << "Ahead of you lies three dungeons:" << endl;
    cout << "0. " << dungeon.getName() << " (Level " << dungeon.getLevel() << ")" << endl;
    cout << "1. " << dungeon2.getName() << " (Level " << dungeon2.getLevel() << ")" << endl;
    cout << "2. " << dungeon3.getName() << " (Level " << dungeon3.getLevel() << ")" << endl;

    cout << "Select a dungeon to enter (0-2):" << endl;
    cin >> choice;
    cout << endl;

    switch (choice) {
        case 0:
            cout << "You have entered " << dungeon.getName() << "!" << endl;
            enemies = dungeon.getEnemies();
            break;
        case 1:
            cout << "You have entered " << dungeon2.getName() << "!" << endl;
            enemies = dungeon2.getEnemies();
            break;
        case 2:
            cout << "You have entered " << dungeon3.getName() << "!" << endl;
            enemies = dungeon3.getEnemies();
            break;
        default:
            cout << "Invalid choice. Defaulting to " << dungeon.getName() << "." << endl;
            enemies = dungeon.getEnemies();
    }

    cout << endl;
    cout << "Enemies in this dungeon:" << endl;
    for (const auto& enemy : enemies) {
        enemy.displayStats();
    }
    cout << endl;

    if (enemies.empty()) {
        enemies = {
            Enemy("Horse", 40, 5, 50),
            Enemy("Lesser Goblin", 30, 8, 100),
            Enemy("Greater Goblin", 50, 10, 150),
            Enemy("Troll", 60, 13, 200),
            Enemy("Dark Elf", 50, 16, 250)
        };
    }

    

    return 0;
}
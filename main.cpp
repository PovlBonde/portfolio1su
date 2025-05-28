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

    // Main dungeon loop
    while (true) {
        cout << "Ahead of you lies three dungeons:" << endl;
        cout << "0. " << dungeon.getName() << " (Level " << dungeon.getLevel() << ", Gold when defeated: " << dungeon.getDropGold() << ")" << endl;
        cout << "1. " << dungeon2.getName() << " (Level " << dungeon2.getLevel() << ", Gold when defeated: " << dungeon2.getDropGold() << ")" << endl;
        cout << "2. " << dungeon3.getName() << " (Level " << dungeon3.getLevel() << ", Gold when defeated: " << dungeon3.getDropGold() << ")" << endl;
        cout << "3. Exit Game" << endl;
        cout << "Select a dungeon to enter (0-2) or 3 to exit:" << endl;
        cin >> choice;
        cout << endl;

        if (choice == 3) {
            cout << "Exiting game..." << endl;
            break;
        }

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

        // Battle all enemies in the dungeon
        while (!enemies.empty() && selectedHero.getHealth() > 0) {
            cout << "Select an enemy to fight: " << endl;
            cin >> choice;
            cout << endl;
            if (choice < 0 || choice >= enemies.size()) {
                cout << "Invalid choice. Defaulting to the first enemy." << endl;
                choice = 0;
            }
            Enemy& selectedEnemy = enemies[choice];
            cout << "You have selected to fight " << selectedEnemy.getName() << "!" << endl;

            // Fighting sequence
            cout << "The fight begins!" << endl;
            while (selectedHero.getHealth() > 0 && selectedEnemy.getHealth() > 0) {
                // Hero attacks Enemy
                selectedHero.attack(selectedEnemy);
                cout << endl;
                cout << selectedEnemy.getName() << " has " << selectedEnemy.getHealth() << " health left." << endl;
                cout << selectedHero.getName() << " has " << selectedHero.getHealth() << " health left." << endl;
                cout << endl;

                std::this_thread::sleep_for(std::chrono::seconds(1)); // Pause for 1 second

                if (selectedEnemy.getHealth() <= 0) {
                    cout << "You defeated the " << selectedEnemy.getName() << "!" << endl;
                    selectedHero.gainExp(selectedEnemy.getDropExp());
                    enemies.erase(enemies.begin() + choice); // Remove enemy from the list
                    break;
                }
                // Enemy attacks Hero
                selectedEnemy.attack(selectedHero);
                cout << endl;
                cout << selectedHero.getName() << " has " << selectedHero.getHealth() << " health left." << endl;
                cout << selectedEnemy.getName() << " has " << selectedEnemy.getHealth() << " health left." << endl;
                cout << endl;
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Pause for 1 second
                if (selectedHero.getHealth() <= 0) {
                    cout << "You have been defeated by the " << selectedEnemy.getName() << "!" << endl;
                    cout << "Game Over!" << endl;
                    return 0;
                }
            }

            if (!enemies.empty() && selectedHero.getHealth() > 0) {
                cout << "Enemies remaining in this dungeon:" << endl;
                for (size_t i = 0; i < enemies.size(); ++i) {
                    cout << i << ". ";
                    enemies[i].displayStats();
                }
                cout << endl;
            }
        }

        if (selectedHero.getHealth() > 0) {
            cout << "Congratulations! You have defeated all enemies in the dungeon!" << endl;
            int goldEarned = 0;
            if (choice == 0) goldEarned = dungeon.getDropGold();
            else if (choice == 1) goldEarned = dungeon2.getDropGold();
            else if (choice == 2) goldEarned = dungeon3.getDropGold();
            cout << "You earned " << goldEarned << " gold!" << endl;
        } else {
            // Hero died, exit game
            break;
        }

        continue; // Continue to the next dungeon or exit
    }

    return 0;
}
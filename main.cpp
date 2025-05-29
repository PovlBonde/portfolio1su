#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <sqlite3.h>
#include "Hero.h"
#include "Enemy.h"
#include "Dungeon.h"
using namespace std;

void saveGame(const Hero& hero, const vector<Enemy>& enemies, const vector<bool>& dungeonDefeated) {
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

    // Save dungeonDefeated vector
    for (bool defeated : dungeonDefeated) {
        saveFile << defeated << " ";
    }
    saveFile << endl;

    saveFile.close();
    cout << "Game saved successfully!" << endl << endl;
}

bool loadGame(Hero& hero, vector<Enemy>& enemies, vector<bool>& dungeonDefeated) {
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

    // Load dungeonDefeated vector
    dungeonDefeated.clear();
    string line;
    getline(loadFile, line); // Read the line with dungeonDefeated
    if (line.empty()) getline(loadFile, line); // In case of leftover newline
    istringstream iss(line);
    bool val;
    while (iss >> val) {
        dungeonDefeated.push_back(val);
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
    vector<bool> dungeonDefeated(4, false); // Track if each dungeon is defeated
    bool bossUnlocked = false; // Track if boss is unlocked
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
                // When loading
                gameLoaded = loadGame(selectedHero, enemies, dungeonDefeated);
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
    Dungeon dungeon0("Goblin Forest", 1);
    Dungeon dungeon1("Goblin Cave", 2);
    Dungeon dungeon2("Troll Lair", 3);
    Dungeon dungeon3("Dark Castle", 4);

    // Main dungeon loop
    while (true) {
        cout << "Ahead of you lies three dungeons:" << endl;
        cout << "0. " << dungeon0.getName() << " (Level " << dungeon0.getLevel() << ", Gold when defeated: " << dungeon0.getDropGold() << ")";
        if (dungeonDefeated[0]) cout << " [DEFEATED]";
        cout << endl;
        cout << "1. " << dungeon1.getName() << " (Level " << dungeon1.getLevel() << ", Gold when defeated: " << dungeon1.getDropGold() << ")";
        if (dungeonDefeated[1]) cout << " [DEFEATED]";
        cout << endl;
        cout << "2. " << dungeon2.getName() << " (Level " << dungeon2.getLevel() << ", Gold when defeated: " << dungeon2.getDropGold() << ")";
        if (dungeonDefeated[2]) cout << " [DEFEATED]";
        cout << endl;
        cout << "3. " << dungeon3.getName() << " (Level " << dungeon3.getLevel() << ", Gold when defeated: " << dungeon3.getDropGold() << ")";
        if (dungeonDefeated[3]) cout << " [DEFEATED]";
        cout << endl;
        if (bossUnlocked) {
            cout << "7. Boss Fight (UNLOCKED!)" << endl;
        }
        cout << "4. View Hero Stats" << endl;
        cout << "5. Exit Game" << endl;
        cout << "6. Save Game" << endl;
        cout << endl;
        cout << "Select an action:" << endl;
        cin >> choice;
        cout << endl;

        if (choice == 4) {
            selectedHero.displayStats();
            cout << endl;
            continue;
        }
        if (choice == 5) {
            cout << "Exiting game..." << endl;
            break;
        }
        if (choice == 6) { // <-- Add this block
            // When saving
            saveGame(selectedHero, enemies, dungeonDefeated);
            continue;
        }

        switch (choice) {
            case 0:
                cout << "You have entered " << dungeon0.getName() << "!" << endl;
                enemies = dungeon0.getEnemies();
                break;
            case 1:
                cout << "You have entered " << dungeon1.getName() << "!" << endl;
                enemies = dungeon1.getEnemies();
                break;
            case 2:
                cout << "You have entered " << dungeon2.getName() << "!" << endl;
                enemies = dungeon2.getEnemies();
                break;
            case 3:
                cout << "You have entered " << dungeon3.getName() << "!" << endl;
                enemies = dungeon3.getEnemies();
                break;
            default:
                cout << "Invalid choice. Defaulting to " << dungeon0.getName() << "." << endl;
                enemies = dungeon0.getEnemies();
        }

        cout << endl;
        cout << "Enemies in this dungeon:" << endl;
        for (size_t i = 0; i < enemies.size(); ++i) {
            cout << i << ". ";
            enemies[i].displayStats();
        }
        cout << enemies.size() << ". View Hero stats" << endl;
        cout << enemies.size() + 1 << ". Exit Dungeon" << endl;
        cout << endl;

        // Battle all enemies in the dungeon
        while (!enemies.empty() && selectedHero.getHealth() > 0) {
            choice = -1;
            cout << "Select an action: " << endl;
            cin >> choice;
            cout << endl;
            if (choice < 0) {
                cout << "Invalid choice. Try again" << endl;
                continue;
            }
            if (choice == static_cast<int>(enemies.size())) {
                selectedHero.displayStats();
                cout << endl;
                continue; // Go back to the enemy selection
            }
            if (choice == static_cast<int>(enemies.size() + 1)) {
                cout << "Exiting dungeon..." << endl;
                break;
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
            Weapon* droppedWeapon = nullptr;
            if (choice == 0) {
                goldEarned = dungeon0.getDropGold();
                dungeonDefeated[0] = true;
                droppedWeapon = dungeon0.getDroppedWeapon();
            }
            else if (choice == 1) {
                goldEarned = dungeon1.getDropGold();
                dungeonDefeated[1] = true;
                droppedWeapon = dungeon1.getDroppedWeapon();
            }
            else if (choice == 2) {
                goldEarned = dungeon2.getDropGold();
                dungeonDefeated[2] = true;
                droppedWeapon = dungeon2.getDroppedWeapon();
            }
            else if (choice == 3) {
                goldEarned = dungeon3.getDropGold();
                dungeonDefeated[3] = true;
                droppedWeapon = dungeon3.getDroppedWeapon();
            }
            cout << "You earned " << goldEarned << " gold!" << endl;
            selectedHero.addGold(goldEarned);

            // Weapon drop
            if (droppedWeapon) {
                cout << "You found a weapon: ";
                droppedWeapon->display();
                cout << "Do you want to equip it? (1 = Yes, 0 = No): ";
                int equipChoice;
                cin >> equipChoice;
                cout << endl;
                if (equipChoice == 1) {
                    selectedHero.equipWeapon(droppedWeapon);
                } else {
                    delete droppedWeapon;
                }
            }

            // Unlock boss if all dungeons are defeated
            bossUnlocked = dungeonDefeated[0] && dungeonDefeated[1] && dungeonDefeated[2] && dungeonDefeated[3];
        } else {
            // Hero died, exit game
            break;
        }

        if (bossUnlocked && choice == 7) {
            cout << "You have unlocked the BOSS FIGHT!" << endl;
            // You can define a special boss here
            Enemy boss("Ancient Dragon", 150, 25, 500);
            cout << "Boss: ";
            boss.displayStats();
            cout << endl;

            // Boss fight loop
            while (selectedHero.getHealth() > 0 && boss.getHealth() > 0) {
                selectedHero.attack(boss);
                cout << boss.getName() << " has " << boss.getHealth() << " health left." << endl;
                if (boss.getHealth() <= 0) {
                    cout << "You have defeated the " << boss.getName() << "! YOU WIN THE GAME!" << endl;
                    return 0;
                }
                boss.attack(selectedHero);
                cout << selectedHero.getName() << " has " << selectedHero.getHealth() << " health left." << endl;
                if (selectedHero.getHealth() <= 0) {
                    cout << "You have been defeated by the " << boss.getName() << "!" << endl;
                    cout << "Game Over!" << endl;
                    return 0;
                }
                cout << endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            continue;
        }

        continue; // Continue to the next dungeon or exit
    }

    return 0;
}
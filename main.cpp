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

    if (enemies.empty()) {
        enemies = {
            Enemy("Horse", 40, 5, 50),
            Enemy("Lesser Goblin", 30, 8, 100),
            Enemy("Greater Goblin", 50, 10, 150),
            Enemy("Troll", 60, 13, 200),
            Enemy("Dark Elf", 50, 16, 250)
        };
    }

    while (true) {
        cout << "Ahead of you are a group of enemies, blocking your way to the dragon" << endl;
        cout << "Choose an option:" << endl;
        cout << "0. Fight" << endl;
        cout << "1. Save Game" << endl;
        cout << "2. Exit Game" << endl << endl;

        cin >> choice;
        cout << endl;

        switch (choice) {
            case 0:
                cout << "You have chosen to fight!" << endl;
                break;
            case 1:
                saveGame(selectedHero, enemies);
                continue;
            case 2:
                cout << "Exiting game..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Try again." << endl << endl;
                continue;
        }
        break;
    }

    while (!enemies.empty() && selectedHero.getHealth() > 0) {
        cout << "Select an enemy to fight:" << endl;
        for (int i = 0; i < enemies.size(); ++i) {
            cout << i << ". " << enemies[i].getName()
                 << " - Health: " << enemies[i].getHealth()
                 << ", Strength: " << enemies[i].getStrength()
                 << " - Exp when slain: " << enemies[i].getDropExp() << endl;
        }

        cout << endl;
        cout << "-1. View Hero Stats" << endl;
        cout << "-2. to save the game." << endl;
        cout << "-3. to exit the game." << endl << endl;
      

        cin >> choice;
        cout << endl;

        if (choice == -1) {
            selectedHero.displayStats();
            cout << endl;
            continue;
        }

        if (choice == -2) {
            saveGame(selectedHero, enemies);
            continue;
        }

        if (choice == -3) {
            cout << "Exiting game..." << endl;
            break;
        }

        if (choice >= enemies.size()) {
            cout << "Invalid choice. Try again." << endl << endl;
            continue;
        }

        Enemy& selectedEnemy = enemies[choice];

        cout << "The fight begins!" << endl << endl;

        while (selectedHero.getHealth() > 0 && selectedEnemy.getHealth() > 0) {
            selectedHero.attack(selectedEnemy);
            this_thread::sleep_for(chrono::seconds(1));
            cout << endl << selectedEnemy.getName() << " has " << selectedEnemy.getHealth() << " health left." << endl << endl;
            this_thread::sleep_for(chrono::seconds(1));

            if (selectedEnemy.getHealth() <= 0) {
                cout << "You defeated the " << selectedEnemy.getName() << "!" << endl;
                selectedHero.gainExp(selectedEnemy.getDropExp());
                enemies.erase(enemies.begin() + choice);
                cout << endl;
                break;
            }

            selectedEnemy.attack(selectedHero);
            this_thread::sleep_for(chrono::seconds(1));
            cout << endl << selectedHero.getName() << " has " << selectedHero.getHealth() << " health left." << endl << endl;
            this_thread::sleep_for(chrono::seconds(1));

            if (selectedHero.getHealth() <= 0) {
                cout << "You were defeated by the " << selectedEnemy.getName() << "!" << endl;
                cout << "Game Over" << endl;
                return 0;
            }
        }

        this_thread::sleep_for(chrono::seconds(1));
    }

    if (enemies.empty() && selectedHero.getHealth() > 0) {
        selectedHero.setMaxHealth(); // Heal the hero to their maximum health

        cout << "You have defeated all the enemies blocking your way" << endl;
        cout << "The final challenge awaits: The Dragon!" << endl << endl;

        Enemy dragon("Dragon", 150, 30, 500);

        cout << "You heal up, before your final fight" << endl;
        cout << endl;

        while (true) {
            cout << "Select an action: " << endl;
            cout << "0. Fight the Dragon" << endl;
            cout << "1. Save Game" << endl;
            cout << "2. Exit Game" << endl << endl;

            cin >> choice;
            cout << endl;

            switch (choice) {
                case 0:
                    cout << "You have chosen to fight the Dragon!" << endl << endl;
                    break;
                case 1:
                    saveGame(selectedHero, enemies);
                    continue;
                case 2:
                    cout << "Exiting game..." << endl;
                    return 0;
                default:
                    cout << "Invalid choice. Try again." << endl;
                    continue;
            }

            while (selectedHero.getHealth() > 0 && dragon.getHealth() > 0) {
                selectedHero.attack(dragon);
                this_thread::sleep_for(chrono::seconds(2));
                cout << endl << dragon.getName() << " has " << dragon.getHealth() << " health left." << endl << endl;
                this_thread::sleep_for(chrono::seconds(2));

                if (dragon.getHealth() <= 0) {
                    cout << "You have defeated the Dragon!" << endl;
                    selectedHero.gainExp(dragon.getDropExp());
                    cout << "Congratulations, you have completed the game!" << endl;
                    return 0;
                }

                dragon.attack(selectedHero);
                this_thread::sleep_for(chrono::seconds(2));
                cout << endl << selectedHero.getName() << " has " << selectedHero.getHealth() << " health left." << endl << endl;
                this_thread::sleep_for(chrono::seconds(2));

                if (selectedHero.getHealth() <= 0) {
                    cout << "You were defeated by the Dragon!" << endl;
                    cout << "Game Over!" << endl;
                    return 0;
                }
            }
        }
    }

    return 0;
}
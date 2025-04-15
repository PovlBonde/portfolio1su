#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include <thread> // Add this include for std::this_thread::sleep_for
#include <chrono> // Add this include for std::chrono::seconds
#include "Hero.h"
#include "Enemy.h"
using namespace std;

int main() {
    cout << "Welcome to my game!" << endl;

    int choice;
    while (true) {
        cout << "Select options:" << endl;
        cout << "0. Create Game" << endl;
        cout << "1. Load Game" << endl;
        cout << "2. Exit" << endl;
        cout << endl;

        cin >> choice;

        cout << endl;

        switch (choice) {
            case 0:
                cout << "Creating a new game..." << endl;
                break;
            case 1:
                cout << "Loading game..." << endl;
                break;
            case 2:
                cout << "Exiting game..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Try again." << endl;
                continue; // Restart the loop on invalid input
        }
        break; // Exit the loop if a valid option is selected
    }

    Hero selectedHero("", 0, 0, 0, 0); // Declare the Hero object outside the switch block

    cout << endl;

    while (true) {
        cout << "Select your character:" << endl;
        cout << "0. Warrior - A strong human with high resilience, but lower damage" << endl;
        cout << "1. Mage - A powerful magic being with high damage potential, but easy to kill, if struck" << endl;
        cout << "2. Archer - A fast and adept elf, capable of high damage, while still able to take a couple of hits" << endl;

        cout << endl;

        cin >> choice;

        cout << endl;

        switch (choice) {
            case 0:
                cout << "You have selected Warrior!" << endl;
                selectedHero = Hero("Warrior", 1, 0, 100, 10); // Initialize the Hero object
                break;
            case 1:
                cout << "You have selected Mage!" << endl;
                selectedHero = Hero("Mage", 1, 0, 50, 20); // Initialize the Hero object
                break;
            case 2:
                cout << "You have selected Archer!" << endl;
                selectedHero = Hero("Archer", 1, 0, 75, 15); // Initialize the Hero object
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
                continue; // Restart the loop on invalid input
        }
        break; // Exit the loop if a valid option is selected
    }

    cout << endl;

    selectedHero.displayStats();

    cout << endl;

    vector<Enemy> enemies;
    enemies.push_back(Enemy("Horse", 50, 5, 100));
    enemies.push_back(Enemy("Lesser Goblin", 40, 8, 200));
    enemies.push_back(Enemy("Greater Goblin", 60, 10, 300));
    enemies.push_back(Enemy("Troll", 80, 13, 400));
    enemies.push_back(Enemy("Dark Elf", 60, 16, 500));

    cout << "Ahead of you are a group of enemies, blocking your way to the dragon" << endl;
    cout << "Choose an option:" << endl;
    cout << "0. Fight" << endl;
    cout << "1. Save Game" << endl;
    cout << "2. Exit Game" << endl;
    cout << endl;

    while (true) {
        cin >> choice;

        cout << endl;

        switch (choice) {
            case 0:
                cout << "You have chosen to fight!" << endl;
                break;
            case 1:
                cout << "Game saved!" << endl;
                break;
            case 2:
                cout << "Exiting game..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Exiting game..." << endl;
                continue; // Restart the loop on invalid input
        }
        break;
    }

    cout << endl;    

    while (!enemies.empty() && selectedHero.getHealth() > 0) {
        cout << "Select an enemy to fight:" << endl;
    
        for (int i = 0; i < enemies.size(); ++i) {
            cout << i << ". " << enemies[i].getName()
                 << " - Health: " << enemies[i].getHealth()
                 << ", Strength: " << enemies[i].getStrength()
                 << " - Exp when slain: " << enemies[i].getDropExp() << endl;
        }

        cout << enemies.size() << ". View Hero Stats" << endl;
    
        cout << "-1. to exit the game." << endl;

        cout << endl;
        cin >> choice;
        cout << endl;

        if (choice == enemies.size()){
            selectedHero.displayStats();
            cout << endl;
            continue;
        }
    
        if (choice == -1) {
            cout << "Exiting game..." << endl;
            break;
        }
    
        if (choice < 0 || choice >= enemies.size()+1) {
            cout << "Invalid choice. Try again." << endl;
            cout << endl;
            continue;
        }
    
        Enemy& selectedEnemy = enemies[choice];

        cout << "The fight begins!" << endl;
        cout << endl;

        while (selectedHero.getHealth() > 0 && selectedEnemy.getHealth() > 0) {
            
            //Hero attacks enemy
            selectedHero.attack(selectedEnemy);

            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 1 second

            cout << endl;
            cout << selectedEnemy.getName() << " has " << selectedEnemy.getHealth() << " health left." << endl;
            cout << endl;

            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 1 second

            if (selectedEnemy.getHealth() <= 0) {
                cout << "You defeated the " << selectedEnemy.getName() << "!" << endl;
                selectedHero.gainExp(selectedEnemy.getDropExp());
                cout << endl;
    
                enemies.erase(enemies.begin() + choice); // Remove enemy from the list
                break;
            }

            // Enemy attacks Hero
            selectedEnemy.attack(selectedHero);
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 1 second

            cout << endl;
            cout << selectedHero.getName() << " has " << selectedHero.getHealth() << " health left." << endl;
            cout << endl;

            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 1 second

            if (selectedHero.getHealth() <= 0) {
                cout << "You were defeated by the " << selectedEnemy.getName() << "!" << endl;
                cout << "Game Over!" << endl;
                return 0;
            }

        }
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 1 second
    }

    return 0;
}
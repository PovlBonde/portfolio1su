#include <iostream>
#include <string>
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

        cin >> choice;

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

    while (true) {
        cout << "Select your character:" << endl;
        cout << "0. Warrior - A strong human with high resilience, but lower damage" << endl;
        cout << "1. Mage - A powerful magic being with high damage potential, but easy to kill, if struck" << endl;
        cout << "2. Archer - A fast and adept elf, capable of high damage, while still able to take a couple of hits" << endl;

        cin >> choice;

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

    selectedHero.displayStats(); // Use the selected Hero object

    cout << "Ahead of you are a group of enemies, blocking your way to the dragon" << endl;
    cout << "Choose an option:" << endl;
    cout << "0. Fight" << endl;
    cout << "1. Save Game" << endl;
    cout << "2. Exit Game" << endl;

    while (true) {
        cin >> choice;

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
                return 0; // Exit the game on invalid input
        }
        break;
    }
    cout << "Select an enemy to fight:" << endl;

    cout << "0. Horse - Health: 50, Strength: 5 - Exp when slain: 100" << endl;
    cout << "1. Lesser Goblin - Health: 40, Strength: 8 - Exp when slain: 200" << endl;
    cout << "2. Greater Goblin - Health: 60, Strength: 10 - Exp when slain: 300" << endl;
    cout << "3. Troll - Health: 80, Strength: 13 - Exp when slain: 400" << endl;
    cout << "4. Dark Elf - Health: 60, Strength: 16 - Exp when slain: 500" << endl;

    Enemy selectedEnemy("", 0, 0, 0); // Initialize the Enemy object

    while (true){
        cin >> choice;

        switch (choice) {
            case 0:
                cout << "You have chosen to fight the Horse!" << endl;
                selectedEnemy = Enemy("Horse", 50, 5, 100); 
                break;
            case 1:
                cout << "You have chosen to fight the Lesser Goblin!" << endl;
                selectedEnemy = Enemy("Lesser Goblin", 40, 8, 200); 
                break;
            case 2:
                cout << "You have chosen to fight the Greater Goblin!" << endl;
                selectedEnemy = Enemy("Greater Goblin", 60, 10, 300); 
                break;
            case 3:
                cout << "You have chosen to fight the Troll!" << endl;
                selectedEnemy = Enemy("Troll", 80, 13, 400); 
                break;
            case 4:
                cout << "You have chosen to fight the Dark Elf!" << endl;
                selectedEnemy = Enemy("Dark Elf", 60, 16, 500);
                break;
            default:
                cout << "Invalid choice. Exiting game..." << endl;
                return 0; // Exit the game on invalid input
        }
        break; // Exit the loop after selecting an enemy
    }

    // Fighting sequence
    cout << "The fight begins!" << endl;
    while (selectedHero.getHealth() > 0 && selectedEnemy.getHealth() > 0) {
        // Hero attacks Enemy
        selectedHero.attack(selectedEnemy);
        if (selectedEnemy.getHealth() <= 0) {
            cout << "You defeated the " << selectedEnemy.getName() << "!" << endl;
            selectedHero.gainExp(selectedEnemy.getDropExp());
            break;
        }

        // Enemy attacks Hero
        selectedEnemy.attack(selectedHero);
        if (selectedHero.getHealth() <= 0) {
            cout << "You were defeated by the " << selectedEnemy.getName() << "!" << endl;
            cout << "Game Over!" << endl;
            return 0;
        }
    }

    return 0;
}
#include <iostream>
#include <string>
#include "Hero.h"
#include "Enemy.h"
using namespace std;

int main() {
    cout << "Welcome to the RPG Game!" << endl;

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

    return 0;
}
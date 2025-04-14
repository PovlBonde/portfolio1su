#include <iostream>
#include <string>
#include "Hero.h"
using namespace std;

int main() {
    Hero hero1("Warrior", 1, 0, 100, 10);

    hero1.displayStats();

    return 0;
}
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

sqlite3* openDatabase() {
    sqlite3* db;
    if (sqlite3_open("game.db", &db)) {
        cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
}

void createTables(sqlite3* db) {
    const char* heroTable = R"(
        CREATE TABLE IF NOT EXISTS hero (
            name TEXT PRIMARY KEY,
            level INTEGER,
            exp INTEGER,
            health INTEGER,
            strength INTEGER,
            gold INTEGER,
            enemiesDefeated INTEGER
        );
    )";
    const char* weaponTable = R"(
        CREATE TABLE IF NOT EXISTS weapon (
            hero_name TEXT PRIMARY KEY,
            name TEXT,
            baseDamage INTEGER,
            strengthModifier INTEGER,
            durability INTEGER,
            enemiesDefeated INTEGER,
            FOREIGN KEY(hero_name) REFERENCES hero(name)
        );
    )";
    const char* dungeonTable = R"(
        CREATE TABLE IF NOT EXISTS dungeon_status (
            hero_name TEXT,
            dungeon_index INTEGER,
            defeated INTEGER,
            PRIMARY KEY(hero_name, dungeon_index),
            FOREIGN KEY(hero_name) REFERENCES hero(name)
        );
    )";
    const char* weaponTypeKillsTable = R"(
        CREATE TABLE IF NOT EXISTS weapon_type_kills (
            weapon_name TEXT PRIMARY KEY,
            kills INTEGER
        );
    )";
    const char* heroWeaponTypeKillsTable = R"(
        CREATE TABLE IF NOT EXISTS hero_weapon_type_kills (
            hero_name TEXT,
            weapon_name TEXT,
            kills INTEGER,
            PRIMARY KEY(hero_name, weapon_name),
            FOREIGN KEY(hero_name) REFERENCES hero(name)
        );
    )";
    char* errMsg = nullptr;
    sqlite3_exec(db, heroTable, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, weaponTable, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, dungeonTable, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, weaponTypeKillsTable, nullptr, nullptr, &errMsg);
    sqlite3_exec(db, heroWeaponTypeKillsTable, nullptr, nullptr, &errMsg);
}

void saveGameSqlite(const Hero& hero, const vector<bool>& dungeonDefeated) {
    sqlite3* db = openDatabase();
    if (!db) return;
    createTables(db);

    // Upsert hero
    string sqlHero = R"(
        INSERT INTO hero (name, level, exp, health, strength, gold, enemiesDefeated)
        VALUES (?, ?, ?, ?, ?, ?, ?)
        ON CONFLICT(name) DO UPDATE SET
            level=excluded.level,
            exp=excluded.exp,
            health=excluded.health,
            strength=excluded.strength,
            gold=excluded.gold,
            enemiesDefeated=excluded.enemiesDefeated;
    )";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sqlHero.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, hero.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, hero.getLevel());
    sqlite3_bind_int(stmt, 3, hero.getExp());
    sqlite3_bind_int(stmt, 4, hero.getHealth());
    sqlite3_bind_int(stmt, 5, hero.getStrength());
    sqlite3_bind_int(stmt, 6, hero.getGold());
    sqlite3_bind_int(stmt, 7, hero.getEnemiesDefeated());
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Upsert weapon if equipped
    Weapon* w = hero.getWeapon();
    if (w) {
        string sqlWeapon = R"(
            INSERT INTO weapon (hero_name, name, baseDamage, strengthModifier, durability, enemiesDefeated)
            VALUES (?, ?, ?, ?, ?, ?)
            ON CONFLICT(hero_name) DO UPDATE SET
                name=excluded.name,
                baseDamage=excluded.baseDamage,
                strengthModifier=excluded.strengthModifier,
                durability=excluded.durability,
                enemiesDefeated=excluded.enemiesDefeated;
        )";
        sqlite3_prepare_v2(db, sqlWeapon.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, hero.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, w->getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, w->getTotalDamage(0) - w->getStrengthModifier() * 0); // baseDamage
        sqlite3_bind_int(stmt, 4, w->getStrengthModifier());
        sqlite3_bind_int(stmt, 5, w->getDurability());
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // Save dungeon status
    string sqlDungeon = R"(
        INSERT INTO dungeon_status (hero_name, dungeon_index, defeated)
        VALUES (?, ?, ?)
        ON CONFLICT(hero_name, dungeon_index) DO UPDATE SET
            defeated=excluded.defeated;
    )";
    for (int i = 0; i < (int)dungeonDefeated.size(); ++i) {
        sqlite3_prepare_v2(db, sqlDungeon.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, hero.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, i);
        sqlite3_bind_int(stmt, 3, dungeonDefeated[i] ? 1 : 0);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    cout << "Game saved to database for hero: " << hero.getName() << endl;
    sqlite3_close(db);
    cout << endl;
}

bool loadGameSqlite(Hero& hero, vector<bool>& dungeonDefeated, const std::string& heroName) {
    sqlite3* db = openDatabase();
    if (!db) return false;
    createTables(db);

    // Load hero
    string sqlHero = "SELECT level, exp, health, strength, gold, enemiesDefeated FROM hero WHERE name = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sqlHero.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, heroName.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int level = sqlite3_column_int(stmt, 0);
        int exp = sqlite3_column_int(stmt, 1);
        int health = sqlite3_column_int(stmt, 2);
        int strength = sqlite3_column_int(stmt, 3);
        int gold = sqlite3_column_int(stmt, 4);
        int enemiesDefeated = sqlite3_column_int(stmt, 5);
        hero = Hero(heroName, level, exp, health, strength);
        hero.addGold(gold);
        hero.setEnemiesDefeated(enemiesDefeated); // Use setter
    } else {
        cout << "No hero found with name: " << heroName << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(stmt);

    // Load weapon
    string sqlWeapon = "SELECT name, baseDamage, strengthModifier, durability, enemiesDefeated FROM weapon WHERE hero_name = ?";
    sqlite3_prepare_v2(db, sqlWeapon.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, heroName.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        string wname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int baseDamage = sqlite3_column_int(stmt, 1);
        int strengthModifier = sqlite3_column_int(stmt, 2);
        int durability = sqlite3_column_int(stmt, 3);
        int wEnemiesDefeated = sqlite3_column_int(stmt, 4);
        Weapon* w = new Weapon(wname, baseDamage, strengthModifier, durability);
        hero.equipWeapon(w);
    }
    sqlite3_finalize(stmt);

    // Load dungeon status
    string sqlDungeon = "SELECT dungeon_index, defeated FROM dungeon_status WHERE hero_name = ?";
    sqlite3_prepare_v2(db, sqlDungeon.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, heroName.c_str(), -1, SQLITE_TRANSIENT);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int idx = sqlite3_column_int(stmt, 0);
        int def = sqlite3_column_int(stmt, 1);
        if (idx >= 0 && idx < (int)dungeonDefeated.size())
            dungeonDefeated[idx] = (def != 0);
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
    cout << "Game loaded for hero: " << heroName << endl;
    return true;
}

void incrementWeaponTypeKills(const std::string& heroName, const std::string& weaponName) {
    sqlite3* db = openDatabase();
    if (!db) return;
    createTables(db);

    // Update global weapon type kills
    std::string sqlGlobal = R"(
        INSERT INTO weapon_type_kills (weapon_name, kills)
        VALUES (?, 1)
        ON CONFLICT(weapon_name) DO UPDATE SET
            kills = kills + 1;
    )";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sqlGlobal.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, weaponName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Update hero-weapon type kills
    std::string sqlHero = R"(
        INSERT INTO hero_weapon_type_kills (hero_name, weapon_name, kills)
        VALUES (?, ?, 1)
        ON CONFLICT(hero_name, weapon_name) DO UPDATE SET
            kills = kills + 1;
    )";
    sqlite3_prepare_v2(db, sqlHero.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, heroName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, weaponName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void displayWeaponTypeKills() {
    sqlite3* db = openDatabase();
    if (!db) return;
    createTables(db);

    std::string sql = "SELECT weapon_name, kills FROM weapon_type_kills";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    cout << "Global Weapon Type Kills:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string weapon = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int kills = sqlite3_column_int(stmt, 1);
        cout << weapon << ": " << kills << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void displayHeroWeaponTypeKills(const std::string& heroName) {
    sqlite3* db = openDatabase();
    if (!db) return;
    createTables(db);

    std::string sql = "SELECT weapon_name, kills FROM hero_weapon_type_kills WHERE hero_name = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, heroName.c_str(), -1, SQLITE_TRANSIENT);
    cout << "Weapon Type Kills for " << heroName << ":" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string weapon = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int kills = sqlite3_column_int(stmt, 1);
        cout << weapon << ": " << kills << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
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
        choice = -1; // Reset choice at the start of each main menu loop
        cout << "Select options:" << endl;
        cout << "0. Create Game" << endl;
        cout << "1. Load Game" << endl;
        cout << "2. See stats" << endl;
        cout << "3. Exit" << endl << endl;

        cin >> choice;
        cout << endl;

        switch (choice) {
            case 0: {
                // New: Prompt for hero name
                string heroName;
                cout << "Enter a name for your hero: ";
                cin >> ws;
                getline(cin, heroName);

                // Always create as Warrior (level 1, exp 0, health 100, strength 10)
                selectedHero = Hero(heroName, 1, 0, 100, 10);
                cout << "Hero '" << heroName << "' created with Warrior stats!" << endl;
                gameLoaded = true;
                break;
            }
            case 1: {
                cout << "Enter the hero name to load: ";
                string loadName;
                cin >> ws;
                getline(cin, loadName);
                cout << "[DEBUG] Attempting to load hero: '" << loadName << "'" << endl;
                gameLoaded = loadGameSqlite(selectedHero, dungeonDefeated, loadName);
                if (!gameLoaded) {
                    cout << "No save found. Starting a new game instead..." << endl;
                }
                break;
            }
            case 2: {
                // See stats submenu
                int statsChoice = -1;
                while (true) {
                    cout << "Stats Menu:" << endl;

                    // Display saved heroes in alphabetical order, with their kills
                    sqlite3* db = openDatabase();
                    if (!db) return 1;
                    createTables(db);
                    string sql = "SELECT DISTINCT hero.name, hero.enemiesDefeated "
                                 "FROM hero "
                                 "ORDER BY hero.name ASC";
                    sqlite3_stmt* stmt;
                    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                    cout << "Saved Heroes:" << endl;
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        string heroName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                        int enemiesDefeated = sqlite3_column_int(stmt, 1);
                        cout << heroName << " - Enemies Defeated: " << enemiesDefeated << endl;

                        // --- Show weapon kills for this hero, in allWeaponTypes order ---
                        bool hasWeapons = false;
                        vector<string> allWeaponTypes = {
                            "Large Stick",
                            "Machette",
                            "Warrior Sword",
                            "Mace"
                        };
                        for (const auto& weaponType : allWeaponTypes) {
                            string weaponSql = "SELECT kills FROM hero_weapon_type_kills WHERE hero_name = ? AND weapon_name = ?";
                            sqlite3_stmt* weaponStmt;
                            sqlite3_prepare_v2(db, weaponSql.c_str(), -1, &weaponStmt, nullptr);
                            sqlite3_bind_text(weaponStmt, 1, heroName.c_str(), -1, SQLITE_TRANSIENT);
                            sqlite3_bind_text(weaponStmt, 2, weaponType.c_str(), -1, SQLITE_TRANSIENT);
                            if (sqlite3_step(weaponStmt) == SQLITE_ROW) {
                                hasWeapons = true;
                                int kills = sqlite3_column_int(weaponStmt, 0);
                                cout << "    - " << weaponType << ": " << kills << " kills" << endl;
                            }
                            sqlite3_finalize(weaponStmt);
                        }
                        if (!hasWeapons) {
                            cout << "    (No weapon kills recorded)" << endl;
                        }
                    }
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    cout << endl;

                    // List all possible weapon types (add your actual weapon names here)
                    vector<string> allWeaponTypes = {
                        "Large Stick",
                        "Machette",
                        "Warrior Sword",
                        "Mace"
                    };

                    sqlite3* db2 = openDatabase();
                    if (db2) {
                        cout << "Weapon Types and Top Hero for Each:" << endl;
                        for (const auto& weaponName : allWeaponTypes) {
                            // Find top hero for this weapon
                            string topHeroSql = "SELECT hero_name, kills FROM hero_weapon_type_kills WHERE weapon_name = ? ORDER BY kills DESC LIMIT 1";
                            sqlite3_stmt* topHeroStmt;
                            sqlite3_prepare_v2(db2, topHeroSql.c_str(), -1, &topHeroStmt, nullptr);
                            sqlite3_bind_text(topHeroStmt, 1, weaponName.c_str(), -1, SQLITE_TRANSIENT);
                            string topHero = "None";
                            int topKills = 0;
                            if (sqlite3_step(topHeroStmt) == SQLITE_ROW) {
                                topHero = reinterpret_cast<const char*>(sqlite3_column_text(topHeroStmt, 0));
                                topKills = sqlite3_column_int(topHeroStmt, 1);
                            }
                            sqlite3_finalize(topHeroStmt);
                            cout << weaponName << " | Top Hero: " << topHero << " | Kills: " << topKills << endl;
                        }
                        sqlite3_close(db2);
                        cout << endl;
                    }

                    // Only show option to return to main menu
                    cout << "0. Return to Main Menu" << endl << endl;
                    cout << "Select an option: ";
                    cin >> statsChoice;
                    cout << endl;
                    if (statsChoice == 0) {
                        break;
                    } else {
                        cout << "Invalid choice. Try again." << endl << endl;
                    }
                }
                continue;
            }
            case 3:
                cout << "Exiting game..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Try again." << endl;
                continue;
        }
        break;
    }

    cout << endl;

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
        int menuIndex = 0;
        vector<int> dungeonMenuIndexes;
        cout << "Ahead of you lies three dungeons:" << endl;
        cout << menuIndex << ". " << dungeon0.getName() << " (Level " << dungeon0.getLevel() << ", Gold when defeated: " << dungeon0.getDropGold() << ")";
        if (dungeonDefeated[0]) cout << " [DEFEATED]";
        cout << endl; dungeonMenuIndexes.push_back(menuIndex++);

        cout << menuIndex << ". " << dungeon1.getName() << " (Level " << dungeon1.getLevel() << ", Gold when defeated: " << dungeon1.getDropGold() << ")";
        if (dungeonDefeated[1]) cout << " [DEFEATED]";
        cout << endl; dungeonMenuIndexes.push_back(menuIndex++);

        cout << menuIndex << ". " << dungeon2.getName() << " (Level " << dungeon2.getLevel() << ", Gold when defeated: " << dungeon2.getDropGold() << ")";
        if (dungeonDefeated[2]) cout << " [DEFEATED]";
        cout << endl; dungeonMenuIndexes.push_back(menuIndex++);

        cout << menuIndex << ". " << dungeon3.getName() << " (Level " << dungeon3.getLevel() << ", Gold when defeated: " << dungeon3.getDropGold() << ")";
        if (dungeonDefeated[3]) cout << " [DEFEATED]";
        cout << endl; dungeonMenuIndexes.push_back(menuIndex++);

        int bossIndex = -1;
        if (bossUnlocked) {
            bossIndex = menuIndex;
            cout << bossIndex << ". Boss Fight (UNLOCKED!)" << endl;
            ++menuIndex;
        }

        int statsIndex = menuIndex++;
        int exitIndex = menuIndex++;
        int saveIndex = menuIndex++;

        cout << statsIndex << ". View Hero Stats" << endl;
        cout << exitIndex << ". Exit Game" << endl;
        cout << saveIndex << ". Save Game" << endl;
        cout << endl;
        cout << "Select an action:" << endl;
        cin >> choice;
        cout << endl;

        if (choice == statsIndex) {
            selectedHero.displayStats();
            cout << endl;
            continue;
        }
        if (choice == exitIndex) {
            cout << "Exiting game..." << endl;
            break;
        }
        if (choice == saveIndex) {
            cout << "Saving game as: " << selectedHero.getName() << endl;
            saveGameSqlite(selectedHero, dungeonDefeated);
            continue;
        }
        if (bossUnlocked && choice == bossIndex) {
            cout << "You have cleared the passage to the Dragon!" << endl;
            // You can define a special boss here
            Enemy boss("Ancient Dragon", 150, 25, 500);
            boss.displayStats();
            cout << endl;

            // Boss fight loop
            while (selectedHero.getHealth() > 0 && boss.getHealth() > 0) {
                selectedHero.attack(boss);
                cout << endl;
                cout << boss.getName() << " has " << boss.getHealth() << " health left." << endl;
                cout << selectedHero.getName() << " has " << selectedHero.getHealth() << " health left." << endl;
                cout << endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));

                if (boss.getHealth() <= 0) {
                    cout << "You have defeated the " << boss.getName() << "! YOU WIN THE GAME!" << endl;
                    // Increment weapon type kills for boss kill
                    Weapon* w = selectedHero.getWeapon();
                    if (w) {
                        incrementWeaponTypeKills(selectedHero.getName(), w->getName());
                    }
                    saveGameSqlite(selectedHero, dungeonDefeated);
                    return 0;
                }
                boss.attack(selectedHero);
                cout << endl;
                cout << selectedHero.getName() << " has " << selectedHero.getHealth() << " health left." << endl;
                cout << boss.getName() << " has " << boss.getHealth() << " health left." << endl;
                cout << endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                if (selectedHero.getHealth() <= 0) {
                    cout << "You have been defeated by the " << boss.getName() << "!" << endl;
                    cout << "Game Over!" << endl;
                    return 0;
                }
            }
            continue;
        }

        // Dungeons
        int currentDungeonIndex = -1;
        for (size_t i = 0; i < dungeonMenuIndexes.size(); ++i) {
            if (choice == dungeonMenuIndexes[i]) {
                currentDungeonIndex = i;
                break;
            }
        }
        if (currentDungeonIndex == -1) {
            cout << "Invalid choice. Try again." << endl;
            continue;
        }

        cout << endl;

        // --- FIX: Populate enemies for the selected dungeon ---
        enemies.clear();
        if (currentDungeonIndex == 0) {
            enemies = dungeon0.getEnemies();
        } else if (currentDungeonIndex == 1) {
            enemies = dungeon1.getEnemies();
        } else if (currentDungeonIndex == 2) {
            enemies = dungeon2.getEnemies();
        } else if (currentDungeonIndex == 3) {
            enemies = dungeon3.getEnemies();
        }

        // Battle all enemies in the dungeon
        while (!enemies.empty() && selectedHero.getHealth() > 0) {
            // Moved this block inside the loop:
            cout << "Enemies in this dungeon:" << endl;
            for (size_t i = 0; i < enemies.size(); i++) {
                cout << i << ". ";
                enemies[i].displayStats();
            }
            cout << enemies.size() << ". View Hero stats" << endl;
            cout << enemies.size() + 1 << ". Exit Dungeon" << endl;
            cout << endl;

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
            if (choice >= static_cast<int>(enemies.size())) {
                cout << "Invalid choice. Try again." << endl;
                continue;
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
                    selectedHero.setEnemiesDefeated(selectedHero.getEnemiesDefeated() + 1); // <-- Add this line
                    Weapon* w = selectedHero.getWeapon();
                    if (w) {
                        incrementWeaponTypeKills(selectedHero.getName(), w->getName());
                    }
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
        }

        if (selectedHero.getHealth() > 0 && enemies.empty()) {
            cout << "Congratulations! You have defeated all enemies in the dungeon!" << endl;
            int goldEarned = 0;
            Weapon* droppedWeapon = nullptr;
            if (currentDungeonIndex == 0) {
                goldEarned = dungeon0.getDropGold();
                dungeonDefeated[0] = true;
                droppedWeapon = dungeon0.getDroppedWeapon();
            }
            else if (currentDungeonIndex == 1) {
                goldEarned = dungeon1.getDropGold();
                dungeonDefeated[1] = true;
                droppedWeapon = dungeon1.getDroppedWeapon();
            }
            else if (currentDungeonIndex == 2) {
                goldEarned = dungeon2.getDropGold();
                dungeonDefeated[2] = true;
                droppedWeapon = dungeon2.getDroppedWeapon();
            }
            else if (currentDungeonIndex == 3) {
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

            // --- AUTOSAVE after dungeon clear ---
            saveGameSqlite(selectedHero, dungeonDefeated);
        } else if (selectedHero.getHealth() <= 0) {
            // Hero died, exit game
            break;
        }
    }

    return 0;
}
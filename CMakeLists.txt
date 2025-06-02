cmake_minimum_required(VERSION 3.10)

# Project name
project(PortfolioGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
set(SOURCES
    main.cpp
    Dungeon.cpp
    Enemy.cpp
    Hero.cpp
    Weapon.cpp
)

# Add executable
add_executable(PortfolioGame ${SOURCES})

# Find and link SQLite3
find_package(SQLite3 REQUIRED)
target_link_libraries(PortfolioGame PRIVATE SQLite::SQLite3)


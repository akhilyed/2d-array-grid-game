#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>
#include <queue>
#include <utility>

using namespace std;

const char PLAYER = 'P';
const char TRESURE = 'T';
const char MONSTER = 'M';
const char GROUND = 'G';
const char WATER = 'W';
const char ROCKS = 'R';
const int MAX_HEALTH = 3;

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void printDungeon(char** dungeon, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << dungeon[i][j] << ' ';
        }
        cout << endl;
    }
}

bool isValidMove(char tile, bool hasBoat, bool hasGear, string& message) {
    if (tile == WATER && !hasBoat) {
        message = "You cannot cross water without a boat!";
        return false;
    }
    if (tile == ROCKS && !hasGear) {
        message = "You cannot cross rocks without climbing gear!";
        return false;
    }
    return true;
}

bool isReachable(char** dungeon, int size, int startX, int startY, int targetX, int targetY, bool hasBoat, bool hasGear) {
    bool visited[size][size] = {false};
    queue<pair<int, int>> q;
    q.push({startX, startY});
    visited[startX][startY] = true;

    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        if (x == targetX && y == targetY) {
            return true;
        }

        for (int dir = 0; dir < 4; ++dir) {
            int newX = x + directions[dir][0];
            int newY = y + directions[dir][1];

            if (newX >= 0 && newX < size && newY >= 0 && newY < size && !visited[newX][newY]) {
                if (dungeon[newX][newY] == GROUND || dungeon[newX][newY] == TRESURE ||
                    (dungeon[newX][newY] == WATER && hasBoat) ||
                    (dungeon[newX][newY] == ROCKS && hasGear)) {
                    visited[newX][newY] = true;
                    q.push({newX, newY});
                }
            }
        }
    }
    return false;
}

void initializeDungeon(char** dungeon, char** originalTiles, int size, int chest_amt) {
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            dungeon[i][j] = GROUND;
            originalTiles[i][j] = GROUND;
        }
    }

    dungeon[0][0] = GROUND;
    originalTiles[0][0] = GROUND;

    int treasuresPlaced = 0;
    while (treasuresPlaced < chest_amt) {
        int x = rand() % size;
        int y = rand() % size;

        if ((x != 0 || y != 0) && dungeon[x][y] == GROUND) {
            dungeon[x][y] = TRESURE;
            originalTiles[x][y] = TRESURE;
            treasuresPlaced++;
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (dungeon[i][j] == GROUND && (i != 0 || j != 0)) {
                int randTile = rand() % 4;
                switch (randTile) {
                    case 0: dungeon[i][j] = WATER; break;
                    case 1: dungeon[i][j] = ROCKS; break;
                    case 2: dungeon[i][j] = MONSTER; break;
                    default: break;
                }
                originalTiles[i][j] = dungeon[i][j];
            }
        }
    }
}

void handleTreasure(char** dungeon, char** originalTiles, int playerX, int playerY, int size, int& treasuresCollected, bool& hasBoat, bool& hasGear, string& message) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int dir = 0; dir < 4; ++dir) {
        int newX = playerX + directions[dir][0];
        int newY = playerY + directions[dir][1];
        if (newX >= 0 && newX < size && newY >= 0 && newY < size) {
            if (dungeon[newX][newY] == TRESURE) {
                message = "You found a treasure!";
                dungeon[newX][newY] = GROUND;
                originalTiles[newX][newY] = GROUND;
                treasuresCollected++;
                if (rand() % 2 == 0) {
                    hasBoat = true;
                    message += " You found a boat!";
                } else {
                    hasGear = true;
                    message += " You found climbing gear!";
                }
            }
        }
    }
}

int main() {
    char rules;
    int chest_amt;
    bool boat = false;
    bool gear = false;
    string cont;
    int playerX = 0, playerY = 0;
    cout << "Welcome dungeon explorer! Would you like to read the rules? (y/n): ";
    cin >> rules;

    if (rules == 'y' || rules == 'Y') {
        do {
            cout << endl;
            cout << "When you enter a dungeon, all obstacles and treasures will be randomly placed.\n\n"
                 << "The 'P' space is the player (you) and shows your location.\n\n"
                 << "'T' spaces are treasure spots. Whenever the player is adjacent to a treasure spot, it will disappear and be collected, containing weapons, cash, or utilities. You must collect all of the treasure for the exit to spawn.\n\n"
                 << "'G' spaces are ground spots in which the player can move to.\n\n"
                 << "'W' spaces are water spots, which cannot be crossed until the player gets a boat, which can be claimed randomly from a treasure chest. If the player does not have a boat and touches a water spot, they will respawn to the closest place they can and lose one health.\n\n"
                 << "'M' spaces are monsters which will make you fight with them if they are ever in the cardinal directions of the player. Players must kill them before they kill the player or they will lose.\n\n"
                 << "'R' spaces are rocky spots in which you need climbing gear to cross over. Trying to cross over without it causes you to lose one health.\n\n"
                 << "Enter 'understood' to start playing: ";
            cin.ignore();
            getline(cin, cont);
        } while (cont != "understood");
    } else {
        cout << "Ok then meanie >:(" << endl;
    }

    int diff;
    cout << endl;
    cout << "Please select your dungeon by entering a number:\n"
         << "1. The Abyssal Crypt (easy)\n"
         << "2. The Darkreach Catacombs (medium)\n"
         << "3. The Forsaken Vaults (hard)\n"
         << "Dungeon Choice: ";
    cin >> diff;

    char** dungeon = nullptr;
    char** originalTiles = nullptr;
    int size = 0;

    switch (diff) {
        case 1:
            cout << "Welcome to the Abyssal Crypt, a dungeon buried deep beneath an ancient ruin. Though no one has ever seen it, rumors say there lies a deep curse that seems to enchant everything that lies there, for the good or bad." << endl;
            size = 6;
            chest_amt = 4;
            break;
        case 2:
            cout << "Welcome to the Darkreach Catacombs where life is taken and remade into something more ... sinister." << endl;
            size = 9;
            chest_amt = 5;
            break;
        case 3:
            cout << "Welcome to the Forsaken Vaults is supposedly a place for those deemed too dangerous and volatile for the real world to see. However, the real reason behind the creation of the vaults remains a mystery." << endl;
            size = 12;
            chest_amt = 6;
            break;
        default:
            cout << "You accidentally selected an invalid option and died of boredom, bye! :)" << endl;
            return 0;
    }

    // Add a delay to let the player read the description
    this_thread::sleep_for(chrono::seconds(3)); // 3-second delay

    dungeon = new char*[size];
    originalTiles = new char*[size];
    for (int i = 0; i < size; ++i) {
        dungeon[i] = new char[size];
        originalTiles[i] = new char[size];
    }

    bool validDungeon = false;
    while (!validDungeon) {
        initializeDungeon(dungeon, originalTiles, size, chest_amt);
        dungeon[playerX][playerY] = PLAYER;

        validDungeon = true;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (dungeon[i][j] == TRESURE && !isReachable(dungeon, size, playerX, playerY, i, j, boat, gear)) {
                    validDungeon = false;
                    break;
                }
            }
            if (!validDungeon) break;
        }
    }

    int treasuresCollected = 0;
    char move;
    string message;
    while (true) {
        clearScreen();
        printDungeon(dungeon, size);

        if (!message.empty()) {
            cout << message << endl;
            message.clear();
            this_thread::sleep_for(chrono::milliseconds(1000));
        }

        cout << "Enter your move (w/a/s/d to move up/left/down/right, q to quit): ";
        cin >> move;

        int newX = playerX;
        int newY = playerY;

        if (move == 'w') newX--; 
        else if (move == 'a') newY--; 
        else if (move == 's') newX++; 
        else if (move == 'd') newY++; 
        else if (move == 'q') break; 
        else {
            message = "Invalid move! Please use w/a/s/d or q.";
            continue;
        }

        if (newX >= 0 && newX < size && newY >= 0 && newY < size) {
            if (isValidMove(dungeon[newX][newY], boat, gear, message)) {
                dungeon[playerX][playerY] = originalTiles[playerX][playerY];
                playerX = newX;
                playerY = newY;
                dungeon[playerX][playerY] = PLAYER;
                handleTreasure(dungeon, originalTiles, playerX, playerY, size, treasuresCollected, boat, gear, message);
            }
        } else {
            message = "You can't move outside the dungeon!";
        }
    }

    for (int i = 0; i < size; ++i) {
        delete[] dungeon[i];
        delete[] originalTiles[i];
    }
    delete[] dungeon;
    delete[] originalTiles;

    return 0;
}

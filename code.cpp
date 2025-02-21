//Ansh Patel,Akhil Yeduvaka
//2/19/25
//2d arrays console game
//Software design
//Extra:Different types of levels
#include <iostream>
#include <string>

using namespace std;
int main() {
    char rules;
    string cont;
    int chest_amt;
    bool boat = false;
    bool gear = false;
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
                 << "'H' spaces are rocky spots in which you need climbing gear to cross over. Trying to cross over without it causes you to lose one health.\n\n"
                 << "Enter 'understood' to start playing: ";
            cin.ignore(); // Clear the newline character from the input buffer
            getline(cin, cont);
        } while (cont != "understood");
    } else {
        cout << "Ok then meanie >:(" << endl;
    }


    int diff;
    // Options for game
    cout << endl;
    cout << "Please select your dungeon by entering a number:\n"
         << "1. The Abyssal Crypt (easy)\n"
         << "2. The Darkreach Catacombs (medium)\n"
         << "3. The Forsaken Vaults (hard)\n"
         << "Dungeon Choice: ";
    cin >> diff;


    // Switch statement
    switch (diff) {
        case 1:
            cout << "Welcome to the Abyssal Crypt, a dungeon buried deep beneath an ancient ruin. Though no one has ever seen it, rumors say there lies a deep curse that seems to enchant everything that lies there, for the good or bad." << endl;
            char dungeon[6][6];
            chest_amt = 4;
            break;
        case 2:
            cout << "Welcome to the Darkreach Catacombs where life is taken and remade into something more ... sinister." << endl;
            char dungeon[9][9];
            chest_amt = 5;
            boat = true;
            break;
        case 3:
            cout << "Welcome to the Forsaken Vaults is supposedly a place for those deemed too dangerous and volatile for the real world to see. However, the real reason behind the creation of the vaults remains a mystery." << endl;
            char dungeon[12][12];
            chest_amt = 6;
            boat = true;
            gear = true;
            break;
        default:
            cout << "Invalid choice. Please select a valid dungeon." << endl;
            break;
    }




    return 0;
}

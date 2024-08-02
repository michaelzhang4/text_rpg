#include "helpers.hpp"

using namespace std;

void HUD(Player* p) {
    system("cls");
    p->display_stats();
    choices(p);
}

int main() {
    Player* player = new Player(start());
    while(1) {
        HUD(player);
    }

    // Clean up dynamically allocated memory
    delete player;
    for (auto item : item_hashes) {
        delete all_items[item];
    }
    
    return 0;
}
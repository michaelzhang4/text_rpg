#include "helpers.hpp"

int main() {
    set_up();
    Player* player = create_player(start());
    current_area->print_description();
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
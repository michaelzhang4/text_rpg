#include "helpers.hpp"

int main() {
    enableEmoji();
    set_up();
    Player* player = create_player(start());
    intro();
    while(1) {
        HUD(player);
    }

    // Clean up dynamically allocated memory
    delete player;
    cleanup_memory();
    
    return 0;
}
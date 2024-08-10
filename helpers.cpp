#include "helpers.hpp"

using namespace std;

void death_screen() {
    ClearScreen();
    cout << "You died!\n\nGame over - reload from previous save or start a new game\n";
    exit(0);
}

void HUD(Player* p) {
    ClearScreen();
    string floor_color = "🟧";
    if(current_area->color==Color::Green) {
        floor_color = "🟩";
    } else if (current_area->color==Color::Red) {
        floor_color = "🟥";
    } else if (current_area->color==Color::Black) {
        floor_color = "⬛";
    } else if (current_area->color==Color::White) {
        floor_color = "⬜";
    }
    cout << "Floor "<< current_area->index+1 << " - " << current_area->name << " " << floor_color << endl;
    p->display_stats();
    choices(p);
}

void slow_print(string s) {
    int time=20;
    for(char c: s) {
        cout << c;
        FLUSH();
        SleepMs(time);
    }
}

void combatHUD(Enemy *enemy, Player *player) {
    player->display_stats();
    cout << "\nVS\n\n";
    enemy->display_stats();
}

int player_turn(Player *p, Enemy *enemy) {
    ClearScreen();
    combatHUD(enemy,p);
    string choice;
    cout << "\n1. Attack 2. Run\n";
    cin >> choice;
    lower(choice);
    if(choice=="1" || choice=="attack") {
        int effective_damage = p->damage();
        if (rand()%100 <= p->totalCritChance()) {
            cout << "You landed a critical strike!\n";
            effective_damage=ceil((double)effective_damage*p->totalCritDmg());
        };
        int died = enemy->take_damage(effective_damage);
        if(died==1) {
            p->gain(enemy->exp, enemy->gold);
            unlock_stages(enemy);
            cleared(p,enemy);
            for(auto pair:enemy->drops) {
                if(!pair.second) {
                    break;
                }
                Item* item = all_items[pair.second->hash];
                if(pair.second->owned==false) {
                    int rng = rand()%100;
                    if(rng<=pair.first) {
                        string choice;
                        cout << endl << enemy->name << " dropped a " << item->name << "!\n";
                        cout << "\nEnter any key to continue...\n";
                        item->owned=true;
                        owned_items.push_back(item);
                        cin >> choice;
                        break;
                    }
                }

            }
            return 0;
        }
    } else if(choice=="2" || choice=="run") {
        ClearScreen();
        combatHUD(enemy,p);
        p->take_damage(enemy);
        cout << "\nYou ran away after taking a hit\n";
        SleepMs(SLEEP);
        return 1;
    } else {
        cout << "Enter a valid action\n";
        SleepMs(SLEEP);
        return 2;
    }
    return 3;
}

void enemy_turn(Player *p, Enemy *enemy, int surprised) {
    ClearScreen();
    combatHUD(enemy,p);
    if(surprised==1) {
        cout << "\nThe " << enemy->name << " caught you by surprise\n";
        SleepMs(1000);
    }
    SleepMs(1000);
    p->take_damage(enemy);
}

int combat(Player *p, Enemy *arena_enemy) {
    int decision;
    Enemy *enemy;
    if(!arena_enemy) {
        enemy_template e;
        int rng;
        do {
            rng = rand()%100 + current_area->enemy_list.size();
            rng%=current_area->enemy_list.size();
            e = current_area->enemy_list[rng];
        } while(e.name == previous_encounter);
        enemy = new Enemy(current_area->enemy_list[rng]);
        previous_encounter = enemy->name;
    } else {
        enemy = arena_enemy;
        enemy->enemyStats.health=enemy->enemyStats.maxHealth;
    }
    int rng = rand()%100;
    if(rng <= 35) {
        enemy_turn(p,enemy,1);
    }
    while(1) {
        start_combat:
        decision = player_turn(p,enemy);
        if(decision==1) {
            if(current_area->index==0 && !areas[1]->unlocked){
                rest=-1;
            } else if(current_area->index==1){
                rest=0;
            } else {
                rest=1;
            }
            return decision;
        } else if (decision==2) {
            goto start_combat;
        } else if (decision==0) {
            break;
        }
        enemy_turn(p,enemy,0);
    }
    if(current_area->index==0 && !areas[1]->unlocked){
        rest=-1;
    } else if(current_area->index==1){
        rest=-1;
    } else {
        rest=0;
    }
    return 0;
}

bool pattern_match(string& input, regex& pattern) {
    return regex_search(input, pattern);
}

void cleared(Player *p, Enemy *e) {
    if(e->name == "Dungeon Keeper" && arena_bosses[0].second==false) {
        arena_bosses[0].second=true;
        p->gold+=1000*p->level;
        cout << "Dungeon Keeper dropped " << 1000*p->level << " 🪙\n";
    }
}

void unlock_stages(Enemy* e) {
    bool unlocked=false;
    if(e->name=="Goblin Chieftain" && areas[1]->unlocked==false) {
        cout << "\nYou have slain the strongest foe in Goblin Village\n\n";
        cout << areas[1]->name << " has been unlocked for travel!\n";
        areas[1]->unlocked=true;
        areas[2]->unlocked=true;
        unlocked=true;
    } else if(e->name=="Isolated Frost Demon" && areas[3]->unlocked==false) {
        cout << "\nYou have slain the strongest foe in Rocky Mountains\n\n";
        cout << areas[3]->name << " has been unlocked for travel!\n";
        areas[3]->unlocked=true;
        unlocked=true;
    }
    if(unlocked) {
        cout << "\nEnter any key to continue...\n";
        string choice;
        cin >> choice;
    }
}

int start() {
    srand(time(NULL));
    cout << fixed << setprecision(2);
    while(1) {
        ClearScreen();
        cout << "Welcome to the Tower of God\n\n";
        cout << "1. New Game\n2. Load Save\n";
        string choice; cin >> choice;lower(choice);
        if(choice == "1" | choice == "new" | choice == "new game") {
            return 1;
        } else if (choice=="2" | choice == "load" | choice == "load save") {
            return 2;
        }
    }
}

bool isNumber(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

void lower(string &s) {
    transform(s.begin(), s.end(), s.begin(),[](unsigned char c){ return std::tolower(c); });
}

void print_explore() {
    int sleep_time=250;
    ClearScreen();
    cout << "Exploring\n";
    SleepMs(sleep_time);
    ClearScreen();
    cout << "Exploring.\n";
    SleepMs(sleep_time);
    ClearScreen();
    cout << "Exploring..\n";
    SleepMs(sleep_time);
    ClearScreen();
    cout << "Exploring...\n";
    SleepMs(sleep_time);
}

void event(Player *p) {
    vector<Event*> e = current_area->event_list;
    int rng = rand() % e.size();
    while(e[rng]==previous_event) {
        rng = rand() % e.size();
    }
    e[rng]->execute_event(p);
    previous_event=e[rng];
}

void explore(Player *p) {
    for(int i=0;i<3;++i) {
        print_explore();
    }
    int temp = rand() % 100;
    if (temp>=current_area->chance_split[0]) {
        cout << "\nMonster encountered! Get ready for combat!\n";
        SleepMs(SLEEP);
        ClearScreen();
        combat(p,NULL);
    } else if (temp>=current_area->chance_split[1]) {
        event(p);
    } else {
        cout << "\nNothing of interest happened\n";
        SleepMs(SLEEP);
        ClearScreen();
    }
}

void __rest(Player *p) {
    if(rest<2) {
        if (p->playerStats.health==p->totalHealth()) {
            cout << "\nYou had a nice rest\n";
        } else {
            int heal=ceil((double)p->totalHealth()*p->recoveryRate());
            if (heal<=0) {
                heal=1;
            }
            cout << "\nYou rested, healing " << heal << " health\n";
            p->playerStats.health+=heal;
            if (p->playerStats.health>p->totalHealth()) {
                p->playerStats.health=p->totalHealth();
            }
        }
        rest+=1;
    } else {
        cout << "\nYou may rest again after the next combat\n";
    }
    SleepMs(1000);
}

void items(Player *p) {
    while(1) {
        ClearScreen();
        cout << "Items ⚔️\n";
        int i,index=1;
        for(i=0;i<owned_items.size();++i) {
            if(i%3==0) {
                cout << endl;
            }
            if(owned_items[i]->owned) {
                if(owned_items[i]==p->primary_equipped) {
                    cout << index << ". " << owned_items[i]->name << "<primary>  ";
                } else if(owned_items[i]==p->secondary_equipped) {
                    cout << index << ". " << owned_items[i]->name << "<secondary>  ";
                } else if(owned_items[i]==p->armor_equipped) {
                    cout << index << ". " << owned_items[i]->name << "<armor>  ";
                } else {
                    cout << index << ". " << owned_items[i]->name << "  ";
                }
                ++index;
            }
        }
        cout << "\ne. Unequip primary f. Unequip secondary g. Unequip armor\nb. Exit items\n";
        string choice;cin >> choice;lower(choice);
        try {
            if (choice == "b" || choice == "exit" || choice=="back") {
                break;
            } else if (choice=="e" || choice == "unequip primary") {
                p->unequip(NULL,0);
            } else if (choice=="f" || choice == "unequip secondary") {
                p->unequip(NULL,1);
            } else if (choice=="g" || choice == "unequip armor") {
                p->unequip(NULL,2);
            } else if(1<= stoi(choice) && stoi(choice)<=i) {
                Item* item = owned_items[stoi(choice)-1];
                if (item->owned) {
                    item->inspect_item(p, 0);
                }
            }
        } catch (...) {
            cout << "Enter a valid input...";
            SleepMs(SLEEP);
        }
    }
}

void delete_save() {
    const char* filename = "save.txt";
    remove(filename);
}

void intro() {
    ClearScreen();
    slow_print("You find yourself transported to a strange tower.\n\n");
    slow_print("Looking up you see a vast number of floors to the top.\n\n");
    slow_print("You see a glimpse of many eyes observing you from above.\n\n");
    if(areas[2]->unlocked) {
        slow_print("A lone figure ponders life on the peak of a volcano.\n\n");
    }
    slow_print("Will you find an escape on the highest floor?\n\n\n");
    cout << "Enter any key to continue...\n";
    string choice;cin >> choice;
};

void choices(Player *p) {
    cout << "\n1. Explore 2. Shop   3. Rest\n4. Items   5. Save";
    cout << "   6. Travel\n7. Arena   8. Chance 9. Exit\n";
    string choice;cin >> choice;lower(choice);
    if(choice=="1" || choice=="explore") {
        explore(p);
    } else if (choice=="2" || choice=="shop") {
        shop(p);
    } else if(choice=="3" || choice=="rest") {
        __rest(p);
    } else if(choice=="4" || choice=="items") {
        items(p);
    } else if(choice=="5" || choice=="save") {
        save_game(p,false);
    } else if(choice=="6" || choice=="travel") {
        travel(p);
    } else if(choice=="7" || choice=="arena") {
        arena(p);
    } else if(choice=="8" || choice=="chance") {
        chance(p);
    } else if(choice=="9" || choice=="exit") {
        exit(0);
    } 
}

void chance(Player *p) {
    int cost = 500 * p->level;
    cout << "Pay " << cost << " to try your luck? (y/n)"
    " <Careful this overwrites your save>\n";
    string choice;cin >> choice;lower(choice);
    if(choice=="y" || choice=="yes") {
        if(p->gold < cost) {
            cout << "\nYou do not have enough gold\n";
        } else {
            p->gold-=cost;
            int sleep_time=400;
            print_chance();
            int rng = rand()%100;
            cout << "\n\n";
            int size = gamba.size();
            if (size==0) {
                cout << "You have won all the chance items already!\n";
            }
            int random_ind = rand()%gamba.size();
            if(!gamba[random_ind]->owned && rng <= 10) {
                cout << "7 ";
                FLUSH();
                SleepMs(sleep_time);
                cout << "7 ";
                FLUSH();
                SleepMs(sleep_time*2);
                cout << "7 ";
                FLUSH();
                SleepMs(1000);
                cout << "\nCongratulations you won a " << gamba[random_ind]->name << "!\n";
                gamba[random_ind]->owned = true;
                owned_items.push_back(gamba[random_ind]);
            } else {
                if(rng>=70){
                    cout << "7 ";
                    FLUSH();
                    SleepMs(sleep_time);
                    cout << "7 ";
                    FLUSH();
                    SleepMs(sleep_time*2);
                    cout << rand()%7 << " ";
                    FLUSH();
                    SleepMs(1000);
                } else if(rng>=40){
                    cout << rand()%10 << " ";
                    FLUSH();
                    SleepMs(sleep_time);
                    cout << rand()%10 << " ";
                    FLUSH();
                    SleepMs(sleep_time*2);
                    cout << rand()%7 << " ";
                    FLUSH();
                    SleepMs(1000);
                }else {
                    cout << "7 ";
                    FLUSH();
                    SleepMs(sleep_time);
                    cout << rand()%7 << " ";
                    FLUSH();
                    SleepMs(sleep_time*2);
                    cout << "7 ";
                    FLUSH();
                    SleepMs(1000);
                }
                cout << "\n\nBetter luck next time\n";
            }
        }
        save_game(p,true);
        cout << "\nEnter any key to continue...\n";
        string choice;cin>>choice;
    }
}

void print_chance() {
    int sleep_time=400;
    ClearScreen();
    cout << "1 ";
    FLUSH();
    SleepMs(sleep_time);
    cout << "2 ";
    FLUSH();
    SleepMs(sleep_time);
    cout << "3 ";
    FLUSH();
    SleepMs(sleep_time);
    cout << "4 ";
    FLUSH();
    SleepMs(sleep_time);
    cout << "5 ";
    FLUSH();
    SleepMs(sleep_time);
    cout << "6 ";
    FLUSH();
    SleepMs(sleep_time);
}

void arena(Player *p) {
    ClearScreen();
    cout << "Arena Bosses: \n" << endl;
    int i;
    for(i=0;i<arena_bosses.size();++i) {
        cout << i+1 << ". " << arena_bosses[i].first->name << " - level " << arena_bosses[i].first->level;
        if (arena_bosses[i].second) {
            cout << " <cleared>";
        }
        cout << endl;
    }
    cout << "b. Exit arena" << endl;
    string choice;cin >> choice;lower(choice);
    if (choice == "b" || choice == "exit" || choice=="back") {
    } else {
        try { 
            if (1<=stoi(choice) && stoi(choice)<=i) {
            combat(p, arena_bosses[stoi(choice)-1].first);
            }
        }
        catch (...) {
            cout << "Enter a valid input...";
            SleepMs(SLEEP);
        }
    }
}

void travel(Player* p) {
    while(1) {
        ClearScreen();
        cout << "Floors: \n" << endl;
        int i;
        for(i=0;i<areas.size();++i) {
            if (areas[i]->unlocked) {
                if (current_area==areas[i]) {
                    cout << i+1 << ". " << areas[i]->name << " - Current location" << endl;
                } else {
                    cout << i+1 << ". " << areas[i]->name << endl;
                }
            } else {
                cout << i+1 << ". ?" << endl;
            }
        }
        cout << "b. Exit travel" << endl;
        string choice;cin >> choice;lower(choice);
        if (choice == "b" || choice == "exit" || choice=="back") {
            break;
        } else {
            try {
                if (1<=stoi(choice) && stoi(choice)<=i) {
                    if (areas[stoi(choice)-1]->unlocked) {
                        if(current_area==areas[stoi(choice)-1]) {
                            cout << "\nYou are already here!" << endl;
                        } else {
                            current_area = areas[stoi(choice)-1];
                            cout << "\nYou travelled to " << current_area->name << endl;
                            current_area->print_description();
                            break;
                        }
                    } else {
                        cout << "\nArea not unlocked yet!" << endl;
                    }
                    SleepMs(SLEEP);
                }
            }
            catch (...) {
            cout << "Enter a valid input...";
            SleepMs(SLEEP);
            }
        }
    }
}

void title(Player* p) {
    p->print_name();
}

void print_item(Item* item) {
    if(item->owned) {
        cout << item->name << " - Owned" << endl;
    } else if(!item->owned) {
        cout << item->name << "  " << item->price << "g" << endl;
    }
}

void set_up() {
    create_items();
    create_areas();
}

void shop(Player *p) {
    while(1) {
        ClearScreen();
        cout << "Shop 🛒\n";
        cout << "🪙  : " << p->gold << "g\n\n";
        int i;
        for(i=0;i<current_area->shop_list.size();++i) {
            cout << i+1 << ". ";
            print_item(current_area->shop_list[i]);
        }
        cout <<"b. Exit shop\n";
        string choice;cin >> choice;lower(choice);
        if (choice == "b" || choice == "exit" || choice=="back") {
            break;
        } else {
            try {
                if(1<=stoi(choice) && stoi(choice)<=i) {
                    current_area->shop_list[stoi(choice)-1]->inspect_item(p,1);
                }
            }
            catch (...) {
            cout << "Enter a valid input...";
            SleepMs(SLEEP);
            }
        }
    }
}

void load_game(Player *p) {
    ifstream inFile("save.txt");
    if(inFile.fail()) {
        string name;
        while(1) {
            ClearScreen();
            cout << "No previous save. A new game has started.\n\n";
            cout << "Your journey awaits...\n\n";
            cout << "Enter the name of your character:\n";
            cin >> name;
            break;
        }
        p->name = name;
    } else {
        string key = "a8b65fbd5d6b4d2be18be1d55f5b0b73ff7b578a0e8727d79c8f9bb6e227fbdb";
        string encrypted_data((istreambuf_iterator<char>(inFile)),
                                   istreambuf_iterator<char>());
        inFile.close();

        string data = xor_encrypt_decrypt(encrypted_data, key);
        istringstream dataStream(data);

        dataStream >> p->name;
        dataStream >> p->playerStats.health;
        dataStream >> p->playerStats.maxHealth;
        dataStream >> p->playerStats.armor;
        dataStream >> p->playerStats.damage;
        dataStream >> p->exp;
        dataStream >> p->expLevel;
        dataStream >> p->level;
        dataStream >> p->gold;
        dataStream >> rest;
        string primary_equipped_item;
        dataStream >> primary_equipped_item;
        if (primary_equipped_item != "none") {
            p->primary_equipped = all_items[primary_equipped_item];
        }
        string secondary_equipped_item;
        dataStream >> secondary_equipped_item;
        if (secondary_equipped_item != "none") {
            p->secondary_equipped = all_items[secondary_equipped_item];
        }
        string armor_equipped_item;
        dataStream >> armor_equipped_item;
        if (armor_equipped_item != "none") {
            p->armor_equipped = all_items[armor_equipped_item];
        }
        int index;
        dataStream >> index;
        for (int i = 0; i <= index; i++) {
            areas[i]->unlocked = true;
            unlocked_areas.push_back(areas[i]);
        }
        dataStream >> index;
        current_area = areas[index];

        int arena_encoding, pos = 0;
        dataStream >> arena_encoding;
        while (arena_encoding > 0) {
            if (arena_encoding & 1) {
                arena_bosses[pos].second = true;
                arena_bosses[pos].first->gold = 0;
            }
            ++pos;
            arena_encoding >>= 1;
        }
        string item;
        while (dataStream >> item) {
            all_items[item]->owned = true;
            owned_items.push_back(all_items[item]);
        }
    }
    inFile.close();

}

void save_game(Player *p, bool force) {
    string choice;
    if(force==false) {
        cout << "This will overwrite any previous saves\nAre you sure you want to save? (y/n)\n";
        cin >> choice;lower(choice);
    } else if(force==true) {
        choice="y";
    }
    if (choice=="y" | choice=="yes") {
        ofstream outFile("save.txt");
        
        string key = "a8b65fbd5d6b4d2be18be1d55f5b0b73ff7b578a0e8727d79c8f9bb6e227fbdb";

        string data;

        data += p->name + " ";
        data += std::to_string(p->playerStats.health) + " ";
        data += std::to_string(p->playerStats.maxHealth) + " ";
        data += std::to_string(p->playerStats.armor) + " ";
        data += std::to_string(p->playerStats.damage) + " ";
        data += std::to_string(p->exp) + " ";
        data += std::to_string(p->expLevel) + " ";
        data += std::to_string(p->level) + " ";
        data += std::to_string(p->gold) + " ";
        data += std::to_string(rest) + " ";
        data += p->primary_equipped->hash + " ";
        data += p->secondary_equipped->hash + " ";
        data += p->armor_equipped->hash + " ";


        int total=0;
        for(auto a:areas) {
            if(a->unlocked) {
                if(a->index>total) {
                    total=a->index;
                }
            }
        }
        data += to_string(total) + " ";

        data += to_string(current_area->index) + " ";

        int encoding = 0;
        for(int i=0;i<arena_bosses.size();++i) {
            if(arena_bosses[i].second) {
                encoding+=pow(2,i);
            }
        }

        data += to_string(encoding);

        for(string item:item_hashes) {
            if (all_items[item]->owned) {
                data += item + " ";
            }
        }

        string encrypted_data = xor_encrypt_decrypt(data, key);

        outFile << encrypted_data;

        outFile.close();
        if(!force) {
            cout << "Game saved successfully!\n";
        }
        SleepMs(SLEEP);
    }
}

string xor_encrypt_decrypt(const string &data, const string &key) {
    if (key.empty()) {
        return data;
    }
    string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] ^= key[i % key.size()];
    }
    return result;
}

Player *create_player(int option) {
    Player *p = new Player("",10,0,1,1,0);
    if(option==1) {
        string name;
        while(1) {
            ClearScreen();
            cout << "Your journey awaits...\n\n";
            cout << "Enter the name of your character:\n";
            cin >> name;
            break;
        }
        p->name = name;
    } else if (option==2) {
        load_game(p);
    }
    return p;
}

void add_item(int hp, int arm, int dmg, int c,
            double cdmg, double rr, int price, int sell_price, req_stats h,
            string name,string hash, int type, bool owned) {
    all_items[hash] = new Item(hp, arm, dmg, c, cdmg, rr, price,
    sell_price, h, name, hash, type, owned);
    item_hashes.push_back(hash);
}

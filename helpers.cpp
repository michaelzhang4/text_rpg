#include "helpers.hpp"

using namespace std;

void death_screen() {
    system("cls");
    cout << "You died!\n\nGame over - reload from previous save or start a new game";
    exit(0);
}

void HUD(Player* p) {
    system("cls");
    cout << "Area - " << current_area->name << endl;
    p->display_stats();
    choices(p);
}

void combatHUD(Enemy *enemy, Player *player) {
    player->display_stats();
    cout << "\nVS\n\n";
    enemy->display_stats();
}

int player_turn(Player *p, Enemy *enemy) {
    system("cls");
    combatHUD(enemy,p);
    string choice;
    cout << "\n\n1. Attack 2. Run\n";
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
            return 1;
        }
    } else if(choice=="2" || choice=="run") {
        system("cls");
        combatHUD(enemy,p);
        p->take_damage(enemy);
        cout << "\nYou ran away after taking a hit";
        Sleep(SLEEP);
        return 1;
    } else {
        cout << "Enter a valid action";
        Sleep(SLEEP);
        return 2;
    }
    return 0;
}

void enemy_turn(Player *p, Enemy *enemy, int surprised) {
    system("cls");
    combatHUD(enemy,p);
    if(surprised==1) {
        cout << "\n\nThe " << enemy->name << " caught you by surprise";
        Sleep(SLEEP);
    }
    Sleep(1000);
    p->take_damage(enemy);
}

void combat(Player *p, Enemy *arena_enemy) {
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
    }
    int rng = rand()%100;
    if(rng <= 35) {
        enemy_turn(p,enemy,1);
    }
    while(1) {
        start_combat:
        decision = player_turn(p,enemy);
        if(decision==1) {
            break;
        } else if (decision==2) {
            goto start_combat;
        }
        enemy_turn(p,enemy,0);
    }
}

void unlock_stages(Enemy* e) {
    bool unlocked=false;
    if(e->name=="Goblin Chieftain" && areas[1]->unlocked==false) {
        cout << "\nYou have slain the strongest foe in Goblin Village\n\n";
        cout << areas[1]->name << " has been unlocked for travel!\n";
        areas[1]->unlocked=true;
        unlocked=true;
    } else if(e->name=="Isolated Frost Demon" && areas[2]->unlocked==false) {
        cout << "\nYou have slain the strongest foe in Rocky Mountains\n\n";
        cout << areas[2]->name << " has been unlocked for travel!\n";
        areas[2]->unlocked=true;
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
        system("cls");
        cout << "Welcome to my text-based RPG\n\n";
        cout << "1. New Game\n2. Load Save\n";
        string choice; cin >> choice;lower(choice);
        if(choice == "1" | choice == "new" | choice == "new game") {
            return 1;
        } else if (choice=="2" | choice == "load" | choice == "load save") {
            return 2;
        }
    }
}

void lower(string &s) {
    transform(s.begin(), s.end(), s.begin(),[](unsigned char c){ return std::tolower(c); });
}

void print_explore() {
    int sleep_time=250;
    system("cls");
    cout << "Exploring\n";
    Sleep(sleep_time);
    system("cls");
    cout << "Exploring.\n";
    Sleep(sleep_time);
    system("cls");
    cout << "Exploring..\n";
    Sleep(sleep_time);
    system("cls");
    cout << "Exploring...\n";
    Sleep(sleep_time);
}

void explore(Player *p) {
    for(int i=0;i<3;++i) {
        print_explore();
    }
    int temp = rand() % 100;
    if (temp>=0) {
        cout << "\nMonster encountered! Get ready for combat!";
        Sleep(SLEEP);
        system("cls");
        combat(p,NULL);
    } else {
        cout << "\nNothing of interest happened";
        Sleep(1000);
        system("cls");
    }
}

void __rest(Player *p) {
    if(rest<2) {
        if (p->playerStats.health==p->totalHealth()) {
            cout << "\nYou had a nice rest";
        } else {
            int heal=ceil((double)p->totalHealth()*p->recoveryRate());
            if (heal<=0) {
                heal=1;
            }
            cout << "\nYou rested, healing " << heal << " health";
            p->playerStats.health+=heal;
            if (p->playerStats.health>p->totalHealth()) {
                p->playerStats.health=p->totalHealth();
            }
        }
        rest+=1;
    } else {
        cout << "\nYou have rested too long, it's time to move";
    }
    Sleep(SLEEP);
}

void items(Player *p) {
    while(1) {
        system("cls");
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
            Sleep(SLEEP);
        }
    }
}

void choices(Player *p) {
    cout << "\n1. Explore 2. Shop   3. Rest\n4. Items   5. Save";
    cout << "   6. Travel\n7. Arena   8. Chance 9. Exit\n";
    string choice;cin >> choice;lower(choice);
    if(choice=="1" || choice=="explore") {
        explore(p);
        rest=0;
    } else if (choice=="2" || choice=="shop") {
        shop(p);
    } else if(choice=="3" || choice=="rest") {
        __rest(p);
    } else if(choice=="4" || choice=="items") {
        items(p);
    } else if(choice=="5" || choice=="save") {
        save_game(p);
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
    int cost = 1000; //* p->level;
    cout << "It costs " << cost << " to try your luck"
    "\nDo you accept(y/n)?";
    string choice;cin >> choice;lower(choice);
    if(choice=="y" || choice=="yes") {
        if(p->gold < cost) {
            cout << "\nYou do not have enough gold";
        } else {
            p->gold-=cost;
            int sleep_time=400;
            print_chance();
            int rng = rand()%100;
            cout << "\n\n";
            int size = gamba.size();
            if (size==0) {
                cout << "You have won all the chance items already!";
            }
            int random_ind = rand()%gamba.size();
            if(!gamba[random_ind]->owned && rng <= 10) {
                cout << "7 ";
                Sleep(sleep_time);
                cout << "7 ";
                Sleep(sleep_time*2);
                cout << "7 ";
                cout << "\nCongrats you won a " << gamba[random_ind]->name;
                gamba[random_ind]->owned = true;
                owned_items.push_back(gamba[random_ind]);
            } else {
                if(rng>=45){
                    cout << "7 ";
                    Sleep(sleep_time);
                    cout << "7 ";
                    Sleep(sleep_time*2);
                    cout << rand()%7 << " ";
                } else {
                    cout << "7 ";
                    Sleep(sleep_time);
                    cout << rand()%7 << " ";
                    Sleep(sleep_time);
                    cout << "7 ";
                }
                cout << "\nBetter luck next time";
            }
        }
        Sleep(SLEEP);
    }
}

void print_chance() {
    int sleep_time=400;
    system("cls");
    cout << "1 ";
    Sleep(sleep_time);
    cout << "2 ";
    Sleep(sleep_time);
    cout << "3 ";
    Sleep(sleep_time);
    cout << "4 ";
    Sleep(sleep_time);
    cout << "5 ";
    Sleep(sleep_time);
    cout << "6 ";
    Sleep(sleep_time);
}

void arena(Player *p) {
    system("cls");
    cout << "Arena Bosses: \n" << endl;
    int i;
    for(i=0;i<arena_bosses.size();++i) {
        cout << i+1 << ". " << arena_bosses[i]->name << " - level " << arena_bosses[i]->level << endl;
    }
    cout << "b. Exit arena" << endl;
    string choice;cin >> choice;lower(choice);
    if (choice == "b" || choice == "exit" || choice=="back") {
    } else if(1<=stoi(choice) && stoi(choice)<=i) {
        combat(p, arena_bosses[stoi(choice)-1]);
    }
}

void travel(Player* p) {
    while(1) {
        system("cls");
        cout << "Areas to travel: \n" << endl;
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
        } else if(1<=stoi(choice) && stoi(choice)<=i) {
            if (areas[stoi(choice)-1]->unlocked) {
                if(current_area==areas[stoi(choice)-1]) {
                    cout << "\nYou are already here!" << endl;
                } else {
                    current_area = areas[stoi(choice)-1];
                    cout << "\nYou travelled to " << current_area->name << endl;
                }
            } else {
                cout << "\nArea not unlocked yet!" << endl;
            }
            Sleep(SLEEP);
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
        system("cls");
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
        } else if(1<=stoi(choice) && stoi(choice)<=i) {
            current_area->shop_list[stoi(choice)-1]->inspect_item(p,1);
        }
    }
}

void load_game(Player *p) {
    ifstream inFile("save.txt");
    if(inFile.fail()) {
        string name;
        while(1) {
            system("cls");
            cout << "No previous save. A new game has started.\n\n";
            cout << "Your journey awaits...\n\n";
            cout << "Enter the name of your character:\n";
            cin >> name;
            break;
        }
        p->name = name;
    } else {
        inFile >> p->name;
        inFile >> p->playerStats.health;
        inFile >> p->playerStats.maxHealth;
        inFile >> p->playerStats.armor;
        inFile >> p->playerStats.damage;
        inFile >> p->exp;
        inFile >> p->expLevel;
        inFile >> p->level;
        inFile >> p->gold;
        inFile >> rest;
        string primary_equipped_item;
        inFile >> primary_equipped_item;
        if(primary_equipped_item!="none") {
            p->primary_equipped=all_items[primary_equipped_item];
        }
        string secondary_equipped_item;
        inFile >> secondary_equipped_item;
        if(secondary_equipped_item!="none") {
            p->secondary_equipped=all_items[secondary_equipped_item];
        }
        string armor_equipped_item;
        inFile >> armor_equipped_item;
        if(armor_equipped_item!="none") {
            p->armor_equipped=all_items[armor_equipped_item];
        }
        int index;
        inFile >> index;
        for(int i=0;i<=index;i++) {
            areas[i]->unlocked=true;
            unlocked_areas.push_back(areas[i]);
        }
        inFile >> index;
        current_area = areas[index];
        
        string item;
        while(inFile >> item) {
            all_items[item]->owned=true;
            owned_items.push_back(all_items[item]);
        }
    }
    inFile.close();

}

void save_game(Player *p) {
    cout << "This will overwrite any previous saves\nAre you sure you want to save? (y/n)\n";
    string choice;cin >> choice;lower(choice);
    if (choice=="y" | choice=="yes") {
        ofstream outFile("save.txt");

        outFile << p->name << " ";
        outFile << p->playerStats.health << " ";
        outFile << p->playerStats.maxHealth << " ";
        outFile << p->playerStats.armor << " ";
        outFile << p->playerStats.damage << " ";
        outFile << p->exp << " ";
        outFile << p->expLevel << " ";
        outFile << p->level << " ";
        outFile << p->gold << " ";
        outFile << rest << " ";
        outFile << p->primary_equipped->hash << " ";
        outFile << p->secondary_equipped->hash << " ";
        outFile << p->armor_equipped->hash << " ";

        int total=0;
        for(auto a:areas) {
            if(a->unlocked) {
                if(a->index>total) {
                    total=a->index;
                }
            }
        }
        outFile << total << " ";

        outFile << current_area->index << " ";

        for(string item:item_hashes) {
            if (all_items[item]->owned) {
                outFile << item << " ";
            }
        }

        

        outFile.close();
        cout << "Game saved successfully";
        Sleep(SLEEP);
    }
}

Player *create_player(int option) {
    Player *p = new Player("",10,0,1,1,0);
    if(option==1) {
        string name;
        while(1) {
            system("cls");
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
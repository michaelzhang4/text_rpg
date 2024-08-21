#include "helpers.hpp"

using namespace std;

void death_screen() {
    ClearScreen();
    cout << "You died!\n\nGame over - reload from previous save or start a new game\n";
    cleanup_memory();
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
    cout << "Floor "<< current_area->index+1 << " - " << current_area->name << " " << floor_color << "\n\n";
    p->display_stats();
    choices(p);
}

int SkillHUD(Player *p, Enemy *e, int &effective_damage) {
    while(1) {
        ClearScreen();
        cout << "Skills 🃏\n";
        int i=0,index=1;
        vector<Skill*> skills;
        if (p->primary_equipped->skill) {
            cout << index << ". ";
            p->primary_equipped->skill->print_info();
            cout << "\n";
            skills.push_back(p->primary_equipped->skill);
            ++index;
            ++i;
        }
        if (p->secondary_equipped->skill) {
            cout << index << ". ";
            p->secondary_equipped->skill->print_info();
            cout << "\n";
            skills.push_back(p->secondary_equipped->skill);
            ++index;
            ++i;
        }
        if (p->armor_equipped->skill) {
            cout << index << ". ";
            p->armor_equipped->skill->print_info();
            cout << "\n";
            skills.push_back(p->armor_equipped->skill);
            ++index;
            ++i;
        }
        for(;i<owned_skills.size();++i) {
            if(owned_skills[i]->owned) {
                skills.push_back(owned_skills[i]);
                cout << index << ". " << owned_skills[i]->name << "\n";
                ++index;
            }
        }
        cout << "\nb. Exit skills\n";
        string choice;cin >> choice;lower(choice);
        try {
            if (choice == "b" || choice == "exit" || choice=="back") {
                return 0;
            } else if(1<= stoi(choice) && stoi(choice)<=i) {
                Skill* skill = skills[stoi(choice)-1];
                if (skill->owned) {
                    skill->execute_skill(p,e,effective_damage);
                    return 1;
                }
            }
        } catch (...) {
            cout << "Enter a valid input...";
            SleepMs(SLEEP);
        }
    }
}

void slow_print(string s) {
    int time=10;
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
    cout << "\n1. Attack 2. Skill 3. Run\n";
    cin >> choice;
    lower(choice);
    int effective_damage=0;
    if(choice=="1" || choice=="attack") {
        effective_damage = p->damage();
        if (rand()%100 <= p->totalCritChance()) {
            cout << "You landed a critical strike!\n";
            effective_damage=ceil((double)effective_damage*p->totalCritDmg());
        };
    } else if(choice=="2" || choice=="skill") {
        int skill_choice = SkillHUD(p,enemy,effective_damage);
        if (skill_choice == 0) {
            return 3;
        }
    } else if(choice=="3" || choice=="run") {
        ClearScreen();
        combatHUD(enemy,p);
        p->take_damage(enemy, enemy->enemyStats.damage);
        cout << "\nYou ran away after taking a hit\n";
        SleepMs(SLEEP);
        return 2;
    } else {
        cout << "Enter a valid action\n";
        SleepMs(1000);
        return 3;
    }
    if(effective_damage>0) {
        int died = enemy->take_damage(p,effective_damage);
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
    }
    return 4;
}

void enemy_turn(Player *p, Enemy *enemy, int surprised) {
    ClearScreen();
    combatHUD(enemy,p);
    if(surprised==1) {
        cout << "\nThe " << enemy->name << " caught you by surprise\n";
        SleepMs(1000);
    }
    cout << "\n";
    SleepMs(700);
    int effective_damage = enemy->enemyStats.damage;
    if (rand()%100 <= enemy->enemyStats.critChance) {
        cout << enemy->name << " landed a critical strike!\n";
        effective_damage=ceil((double)effective_damage*p->totalCritDmg());
    };
    p->take_damage(enemy,effective_damage);
}

int combat(Player *p, Enemy *arena_enemy) {
    stats before_combat = p->playerStats;
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
        if(enemy->enemyStats.health<=0)
            enemy->enemyStats.health=enemy->enemyStats.maxHealth;
    }
    if(p->totalSpeed() == enemy->enemyStats.speed) {
        int rng = rand()%100;
        if(rng <= 35) {
            enemy_turn(p,enemy,1);
        }
    } else if (p->totalSpeed() < enemy->enemyStats.speed) {
        enemy_turn(p,enemy,0);
    }
    while(1) {
        start_combat:
        decision = player_turn(p,enemy);
        // 0 killed enemy
        if(decision==0) {
            break;
        // 2 run away
        } else if (decision==2) {
            if(current_area->index==0 && !areas[1]->unlocked){
                rest=-1;
            } else if(current_area->index==1){
                rest=0;
            } else {
                rest=1;
            }
            if(arena_enemy==nullptr) { 
                delete enemy;
            }
            p->playerStats.armor = before_combat.armor;
            p->playerStats.critChance = before_combat.critChance;
            p->playerStats.critDamage = before_combat.critDamage;
            p->playerStats.mana = before_combat.mana;
            p->playerStats.speed = before_combat.speed;
            p->playerStats.pen = before_combat.pen;
            p->playerStats.damage = before_combat.damage;
            return decision;
        // 3 invalid decision
        } else if (decision==3) {
            goto start_combat;
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
    if(arena_enemy==nullptr) { 
        delete enemy;
    }
    p->playerStats.armor = before_combat.armor;
    p->playerStats.critChance = before_combat.critChance;
    p->playerStats.critDamage = before_combat.critDamage;
    p->playerStats.mana = before_combat.mana;
    p->playerStats.speed = before_combat.speed;
    p->playerStats.pen = before_combat.pen;
    p->playerStats.damage = before_combat.damage;
    return 0;
}

bool pattern_match(string& input, regex pattern) {
    return regex_search(input, pattern);
}

void cleared(Player *p, Enemy *e) {
    if(e->name == "Eris" && arena_bosses[0].second==false) {
        arena_bosses[0].second=true;
        p->gold+=1000*p->level;
        cout << "Eris dropped " << 1000*p->level << " 🪙\n";
    } else if(e->name == "Nemesis" && arena_bosses[0].second==false) {
        arena_bosses[0].second=true;
        p->gold+=1000*p->level;
        cout << "Nemesis dropped " << 1000*p->level << " 🪙\n";
    } else if(e->name == "Hecate" && arena_bosses[0].second==false) {
        arena_bosses[0].second=true;
        p->gold+=1000*p->level;
        cout << "Hecate dropped " << 1000*p->level << " 🪙\n";
    } else if(e->name == "Erebus" && arena_bosses[0].second==false) {
        arena_bosses[0].second=true;
        p->gold+=1000*p->level;
        cout << "Erebus dropped " << 1000*p->level << " 🪙\n";
    } else if(e->name == "Hypnos" && arena_bosses[0].second==false) {
        arena_bosses[0].second=true;
        p->gold+=1000*p->level;
        cout << "Hypnos dropped " << 1000*p->level << " 🪙\n";
    } else if(e->name == "Nyx" && arena_bosses[0].second==false) {
        arena_bosses[0].second=true;
        p->gold+=1000*p->level;
        cout << "Nyx dropped " << 1000*p->level << " 🪙\n";
    } else if(e->name == "Thanatos" && arena_bosses[0].second==false) {
        arena_bosses[0].second=true;
        p->gold+=1000*p->level;
        cout << "Thanatos dropped " << 1000*p->level << " 🪙\n";
    }
}

void unlock_stages(Enemy* e) {
    bool unlocked=false;
    if(e->name=="Goblin Chieftain <BOSS>" && areas[1]->unlocked==false) {
        cout << "\nYou have slain the strongest foe in Goblin Village\n\n";
        cout << areas[1]->name << " has been unlocked for travel!\n";
        areas[1]->unlocked=true;
        areas[2]->unlocked=true;
        unlocked=true;
    } else if(e->name=="Dragon <BOSS>" && areas[3]->unlocked==false) {
        cout << "\nYou have slain the strongest foe in the Rocky Mountain\n\n";
        cout << areas[3]->name << " has been unlocked for travel!\n";
        areas[3]->unlocked=true;
        unlocked=true;
    } else if(e->name=="Sand Elemental <BOSS>" && areas[4]->unlocked==false) {
        cout << "\nYou have slain the strongest foe in the Searing Desert\n\n";
        cout << areas[4]->name << " has been unlocked for travel!\n";
        areas[4]->unlocked=true;
        unlocked=true;
    } else if(e->name=="Ice Queen <BOSS>" && areas[5]->unlocked==false) {
        cout << "\nYou have slain the strongest foe in the Frozen Tundra\n\n";
        cout << areas[5]->name << " has been unlocked for travel!\n";
        areas[5]->unlocked=true;
        unlocked=true;
    } else if(e->name=="Poseidon <BOSS>" && areas[6]->unlocked==false) {
        cout << "\nYou have slain the ruler of Atlantis\n\n";
        cout << areas[6]->name << " has been unlocked for travel!\n";
        areas[6]->unlocked=true;
        unlocked=true;
    } else if(e->name=="Hades <BOSS>") {
        cout << "\nYou have slain the Hades!\n\n";
        cout << "A portal to the outside world opens...\n";
        cout << "You have ascended from the tower!\n";
        cout << "Check back for new updates in the future!\n";
        cleanup_memory();
        exit(0);
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
    int sleep_time=200;
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

void mining() {
    int sleep_time=250;
    for(int i=0;i<3;++i) {
        ClearScreen();
        cout << "Mining\n";
        SleepMs(sleep_time);
        ClearScreen();
        cout << "Mining.\n";
        SleepMs(sleep_time);
        ClearScreen();
        cout << "Mining..\n";
        SleepMs(sleep_time);
        ClearScreen();
        cout << "Mining...\n";
        SleepMs(sleep_time);
    }
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
        SleepMs(1000);
        ClearScreen();
        combat(p,NULL);
    } else if (temp>=current_area->chance_split[1]) {
        event(p);
    } else {
        cout << "\nNothing of interest happened\n";
        SleepMs(1000);
        ClearScreen();
    }
    while (p->exp >= p->expLevel) {
        p->level+=1;
        cout << 
        "\nYou have leveled up!\nYou are now level "
        << p->level << endl;
        p->exp -=p->expLevel;
        p->expLevel+=(4+p->expLevel/6);
        SleepMs(SLEEP);
        lvl:
        ClearScreen();
        p->display_stats();

        cout << "\nChoose a stat to increase:"
        << "\n1. +4❤️   2. +1🛡️   3. +1🗡️   \n4. +1💧  5. +1⚡\n";
        string choice;
        cin >> choice;
        try {
            int dec = stoi(choice);
            if (dec==1) {
                p->playerStats.maxHealth+=5;
            } else if (dec==2) {
                p->playerStats.armor+=1;
            } else if (dec==3) {
                p->playerStats.damage+=1;
            } else if (dec==4) {
                p->playerStats.mana+=1;
            } else if (dec==5) {
                p->playerStats.speed+=1;
            } else {
                cout << "\nEnter a valid input";
                SleepMs(SLEEP);
                goto lvl;
            }
        } catch(...){
            cout << "\nEnter a valid input";
            SleepMs(SLEEP);
            goto lvl;
        }
        p->playerStats.health=p->playerStats.maxHealth+
        p->primary_equipped->itemStats.health+
        p->secondary_equipped->itemStats.health+
        p->armor_equipped->itemStats.health;
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
    SleepMs(500);
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
    cout << "🗡️  - Base hit damage\n"
    "🛡️  - Incoming damage reduction (minimum hit is 1)\n"
    "❤️  - Health (When this drops to 0 you die)\n"
    "🎯 - Armor penetration (how much enemy armor ignored when attacking)\n"
    "💥 - Critical strike chance (percentage chance to strike harder)\n"
    "🔥 - Critical strike multiplier (when critically striking damage becomes 🔥 *🗡️  )\n"
    "💧 - Mana for casting skills\n"
    "⚡ - Speed (being faster means you strike first and have a chance of dodging enemy attacks)\n"
    "🌿 - Recovery amount when resting (🌿 *❤️  )\n\n";
    cout << "Enter any key to continue...\n";
    string choice;cin>>choice;
    ClearScreen();
    slow_print("You find yourself transported to a strange tower.\n\n");
    slow_print("Looking up you see a vast number of floors to the top.\n\n");
    if(areas[6]->unlocked) {
        slow_print("The judge contemplates your existance in the tower\n\n");
    } else if(areas[5]->unlocked) {
        slow_print("You hear sounds from the ocean, it sounds like a party?\n\n");
    } else if(areas[4]->unlocked) {
        slow_print("Some believe braving the cold is better than rushing to an inevitable doom\n\n");
    } else if(areas[3]->unlocked) {
        slow_print("The sand in the tower is restless, hungry to end another journey\n\n");
    } else if(areas[2]->unlocked) {
        slow_print("A lone silhouette gazes at you from the deepest of caverns.\n\n");
    }
    slow_print("Will you find an escape on the highest floor?\n\n");
    cout << "Enter any key to continue...\n";
    cin >> choice;
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
        cleanup_memory();
        exit(0);
    } 
}

void cleanup_memory() {
    for (auto& area : areas) {
        delete area;
    }


    for (auto& pair : arena_bosses) {
        delete pair.first;
    }

    for (auto& item : all_items) {
        delete item.second; 
    }
}


void chance(Player *p) {
    int cost = 100 * p->level;
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
        cout << current_area->shop_descript;
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
    ifstream inFile("save.txt", ios::binary);
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
        vector<char> encrypted_data((istreambuf_iterator<char>(inFile)),
                                   istreambuf_iterator<char>());
        inFile.close();

        string data = xor_encrypt_decrypt(string(encrypted_data.begin(),encrypted_data.end()), key);
        istringstream dataStream(data);

        dataStream >> p->name;
        dataStream >> p->playerStats.health;
        dataStream >> p->playerStats.maxHealth;
        dataStream >> p->playerStats.armor;
        dataStream >> p->playerStats.damage;
        dataStream >> p->playerStats.mana;
        dataStream >> p->playerStats.speed;
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
        ofstream outFile("save.txt",ios::binary);
        
        string key = "a8b65fbd5d6b4d2be18be1d55f5b0b73ff7b578a0e8727d79c8f9bb6e227fbdb";

        string data;

        data += p->name + " ";
        data += std::to_string(p->playerStats.health) + " ";
        data += std::to_string(p->playerStats.maxHealth) + " ";
        data += std::to_string(p->playerStats.armor) + " ";
        data += std::to_string(p->playerStats.damage) + " ";
        data += std::to_string(p->playerStats.mana) + " ";
        data += std::to_string(p->playerStats.speed) + " ";
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

        data += to_string(encoding) + " ";

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
    int size = key.size();
    string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] ^= key[i % size];
    }
    return result;
}

Player *create_player(int option) {
    Player *p = new Player("",10,0,1,0,0,1,0);
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
            double cdmg, double rr, int _pen, int _mana, int _speed, int price, int sell_price, req_stats h,
            string name,string hash, int type, Skill* skill, bool owned) {
    all_items[hash] = new Item(hp, arm, dmg, c, cdmg, rr, _pen, _mana, _speed, price,
    sell_price, h, name, hash, type, skill, owned);
    item_hashes.push_back(hash);
}

void add_skill(std::string name, std::string hash, skillType type, int value, 
            int hpCost, int manaCost, bool owned) {
    all_skills[hash] = new Skill(name,hash,type,value,hpCost,manaCost,false);
}

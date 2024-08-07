#include "helpers.hpp"

using namespace std;

int rest=0;
vector<string> item_hashes = {"dagger",
"short_sword","sword","knife","goblin_spear",
"bow", "staff", "cloth_armor", "steel_sword","emerald_sword","diamond_sword"};
vector<Item*> owned_items;
unordered_map<string,Item*> all_items = create_items();
array<Area*,AREAS> areas = create_areas();
Area *current_area = areas[0];
vector<Area*> unlocked_areas;
string previous_encounter = "None";


void death_screen() {
    system("cls");
    cout << "You died!\n\nGAME OVER";
    exit(0);
}

void HUD(Player* p) {
    system("cls");
    cout << "Area - " << current_area->name << endl;
    p->display_stats();
    choices(p);
}

Item::Item(int hp, int arm, int dmg, int p, int sell_p, req_stats required, string n, string h, int t, bool oo) {
    itemStats.health=hp;
    itemStats.maxHealth=hp;
    itemStats.armor=arm;
    itemStats.damage=dmg;
    price=p;
    sell_price=sell_p;
    req=required;
    name=n;
    hash=h;
    type=t;
    owned=oo;
}

void Item::inspect_item(Player *p, int from_shop) {
    while(1) {
        system("cls");
        if(from_shop==1) {
            if(this->type==0) {
                cout << name << " - Weapon\n" << "🪙  :" << price << "g" << endl;
            } else if(this->type==1) {
                cout << name << " - Armor\n" << "🪙  :" << price << "g" << endl;
            }
        } else if (from_shop==0) {
            if(this->type==0) {
                cout << name << " - Weapon"  << endl;
            } else if(this->type==1) {
                cout << name << " - Armor" << endl;
            }
        }
        if(itemStats.damage!=0) {
            cout << "🗡️  :" << itemStats.damage << " ";
        }
        if(itemStats.armor!=0) {
            cout << "🛡️  :" << itemStats.armor << " ";
        }
        if(itemStats.health!=0) {
            cout << "❤️  :" << itemStats.health << " ";
        }
        cout << "\nRequirements:\n";
        if(req.dmg>0) {
            cout << req.dmg <<"🗡️ ";
        }
        if(req.arm>0) {
            cout << req.arm <<"🛡️ ";
        }
        if(req.hp>0) {
            cout << req.hp <<"❤️ ";
        }
        cout << "level "<< req.lvl << endl;
        if(from_shop==1) {
            cout << "\n🪙  : " << p->gold << "g" << endl;
        }
        if (this->owned==false) {
            cout << "\n1. Buy\nb. Back\n";
            string choice;
            cin >> choice;
            lower(choice);
            if(choice=="1" | choice=="buy") {
                if(p->gold >= price) {
                    owned_items.push_back(this);
                    this->owned=true;
                    p->gold -= price;
                } else {
                    cout << "You can't afford this item";
                    Sleep(SLEEP);
                }
            } else if (choice=="b" | choice=="back") {
                break;
            }
        } else if (this->owned==true) {
            if (p->primary_equipped==this) {
                cout << "\n1. Unequip primary 2. Equip secondary 3. Sell - " << this->sell_price << "g\nb. Back\n";
                string choice;
                cin >> choice;
                lower(choice);
                if(choice=="1" | choice=="unequip primary") {
                    p->unequip(this,0);
                } else if(choice=="2" | choice=="equip secondary") {
                    p->equip(this,1);
                }
                else if (choice=="3" | choice=="sell") {
                    cout << "\nAre you sure you want to sell "<< this->name << "? (y/n)\n";
                    cin >> choice;lower(choice);
                    if (choice=="y" | choice=="yes") {
                        auto it = std::find(owned_items.begin(), owned_items.end(), this);
                        if (it != owned_items.end()) {
                            owned_items.erase(it);
                        }
                        p->gold+=this->sell_price;
                        this->owned=false;
                        p->unequip(this,-1);
                        if(from_shop==0) {
                            return;
                        } 
                    }
                } else if (choice=="b" | choice=="back") {
                    break;
                }
            } else if (p->secondary_equipped==this) {
                cout << "\n1. Equip primary 2. Unequip secondary 3. Sell - " << this->sell_price << "g\nb. Back\n";
                string choice;
                cin >> choice;
                lower(choice);
                if(choice=="1" | choice=="equip primary") {
                    p->equip(this,0);
                } else if(choice=="2" | choice=="unequip secondary") {
                    p->unequip(this,1);
                }
                
                else if (choice=="3" | choice=="sell") {
                    cout << "\nAre you sure you want to sell "<< this->name << "? (y/n)\n";
                    cin >> choice;lower(choice);
                    if (choice=="y" | choice=="yes") {
                        auto it = std::find(owned_items.begin(), owned_items.end(), this);
                        if (it != owned_items.end()) {
                            owned_items.erase(it);
                        }
                        p->gold+=this->sell_price;
                        this->owned=false;
                        p->unequip(this,-1);
                        if(from_shop==0) {
                            return;
                        } 
                    }
                } else if (choice=="b" | choice=="back") {
                    break;
                }
            } else if (p->armor_equipped==this) {
                cout << "\n1. Unequip 2. Sell - " << this->sell_price << "g\nb. Back\n";
                string choice;
                cin >> choice;
                lower(choice);
                if(choice=="1" | choice=="unequip") {
                    p->unequip(this,2);
                } else if (choice=="2" | choice=="sell") {
                    cout << "\nAre you sure you want to sell "<< this->name << "? (y/n)\n";
                    cin >> choice;lower(choice);
                    if (choice=="y" | choice=="yes") {
                        auto it = std::find(owned_items.begin(), owned_items.end(), this);
                        if (it != owned_items.end()) {
                            owned_items.erase(it);
                        }
                        p->gold+=this->sell_price;
                        this->owned=false;
                        p->unequip(this,-1);
                        if(from_shop==0) {
                            return;
                        } 
                    }
                } else if (choice=="b" | choice=="back") {
                    break;
                }
            } else if (type==0) {
                cout << "\n1. Equip primary 2. Equip secondary 3. Sell - " << this->sell_price << "g\nb. Back\n";
                string choice;
                cin >> choice;
                lower(choice);
                if(choice=="1" | choice=="equip primary") {
                    p->equip(this,0);
                } else if(choice=="2" | choice=="equip secondary") {
                    p->equip(this,1);
                }
                else if (choice=="3" | choice=="sell") {
                    cout << "\nAre you sure you want to sell "<< this->name << "? (y/n)\n";
                    cin >> choice;lower(choice);
                    if (choice=="y" | choice=="yes") {
                        auto it = std::find(owned_items.begin(), owned_items.end(), this);
                        if (it != owned_items.end()) {
                            owned_items.erase(it);
                        }
                        p->gold+=this->sell_price;
                        this->owned=false;
                        if(from_shop==0) {
                            return;
                        } 
                    }
                } else if (choice=="b" | choice=="back") {
                    break;
                }
            } else if(type==1) {
                cout << "\n1. Equip 2. Sell - " << this->sell_price << "g\nb. Back\n";
                string choice;
                cin >> choice;
                lower(choice);
                if(choice=="1" | choice=="equip") {
                    p->equip(this,2);
                } else if (choice=="2" | choice=="sell") {
                    cout << "\nAre you sure you want to sell "<< this->name << "? (y/n)\n";
                    cin >> choice;lower(choice);
                    if (choice=="y" | choice=="yes") {
                        auto it = std::find(owned_items.begin(), owned_items.end(), this);
                        if (it != owned_items.end()) {
                            owned_items.erase(it);
                        }
                        p->gold+=this->sell_price;
                        this->owned=false;
                        if(from_shop==0) {
                            return;
                        } 
                    }
                } else if (choice=="b" | choice=="back") {
                    break;
                }
            }
        }
    }
}

Player::Player(string s,int hp,int arm, int dmg, int lvl, int g) {
    name=s;
    playerStats.maxHealth=hp;
    playerStats.health=hp;
    playerStats.armor=arm;
    playerStats.damage=dmg;
    exp=0;
    expLevel=10;
    level=lvl;
    gold=g;
    none = new Item(0,0,0,0,0
    ,req_stats{0,0,0,0},"None","none",0,true);
    none_armor = new Item(0,0,0,0,0
    ,req_stats{0,0,0,0},"None","none",1,true);
    primary_equipped = none;
    secondary_equipped = none;
    armor_equipped = none_armor;
}

Player::~Player() {
    delete none;
}

void Player::equip(Item* e, int slot) {
    req_stats r = e->req;
    stats s = playerStats;
    if(s.armor >= r.arm &&
    s.damage >= r.dmg &&
    s.health >= r.hp &&
    level >= r.lvl) {
        if((secondary_equipped==e ||primary_equipped==e) && e->name!="None") {
            cout << "You already have this item equipped!\n";
            Sleep(SLEEP);
            return;
        }
        if(slot==0) {
            if(primary_equipped->itemStats.health!=0) {
                playerStats.health-=primary_equipped->itemStats.health;
            }
            primary_equipped=e;
            if(e->itemStats.health!=0) {
                playerStats.health+=e->itemStats.health;
            }
        } else if (slot==1) {
            if(secondary_equipped->itemStats.health!=0) {
                playerStats.health-=secondary_equipped->itemStats.health;
            }
            secondary_equipped=e;
            if(e->itemStats.health!=0) {
                playerStats.health+=e->itemStats.health;
            }
        } else if (slot==2) {
            if(armor_equipped==e) {
                return;
            }
            if(armor_equipped->itemStats.health!=0) {
                playerStats.health-=armor_equipped->itemStats.health;
            }
            armor_equipped=e;
            if(e->itemStats.health!=0) {
                playerStats.health+=e->itemStats.health;
            }
        }
    } else {
        cout << "You do not meet the requirements to equip this item\n";
        Sleep(SLEEP);
    }
}

void Player::unequip(Item* item, int slot) {
    if(slot>=0){
        if (slot==1 | slot==0) {
            equip(none, slot);
        } else if (slot==2) {
            equip(none_armor, slot);
        }
    } else {
        if(primary_equipped==item) {
            equip(none,0);
        } else if (secondary_equipped==item) {
            equip(none,1);
        } else if (armor_equipped==item) {
            equip(none,2);
        }
    }
}

void Player::display_stats() {
    print_name();
    cout <<"\n❤️  : " << 
    playerStats.health << 
    "/" << totalHealth()
    << " (" << primary_equipped->itemStats.health+secondary_equipped->itemStats.health+armor_equipped->itemStats.health << ") "
     "🛡️  :" << totalArmor()
    << " (" << primary_equipped->itemStats.armor+secondary_equipped->itemStats.armor+armor_equipped->itemStats.armor << ") "
     "🗡️  :" << damage()
    << " (" << primary_equipped->itemStats.damage+secondary_equipped->itemStats.damage+armor_equipped->itemStats.damage << ") "
    << "\nLevel: "<<level<< " - " << exp << "/"
    << expLevel << " 🪙  : " << gold << "g" << endl;
}

void Player::print_name() {
    cout << name << " - <" << primary_equipped->name << "> <" << secondary_equipped->name << "> <" << armor_equipped->name << ">";
}

void Player::take_damage(Enemy *e) {
    int effective_damage;
    if(totalArmor() > e->enemyStats.damage) {
        effective_damage=1;
    } else {
        effective_damage=e->enemyStats.damage-totalArmor();
    }
    playerStats.health-=effective_damage;

    cout << "\n\n" << e->name << " hit you for " <<
    effective_damage << " damage!\n";
    Sleep(SLEEP);
    if(playerStats.health <=0) {
        death_screen();
    }
}

int Player::damage() {
    return playerStats.damage+primary_equipped->itemStats.damage
    +secondary_equipped->itemStats.damage+armor_equipped->itemStats.damage;
}

int Player::totalHealth() {
    return playerStats.maxHealth+primary_equipped->itemStats.maxHealth
    +secondary_equipped->itemStats.maxHealth+armor_equipped->itemStats.maxHealth;
}

int Player::totalArmor() {
    return playerStats.armor+primary_equipped->itemStats.armor
    +secondary_equipped->itemStats.armor+armor_equipped->itemStats.armor;
}

void Player::gain(int e, int g) {
    cout << "You gained " << e 
    << " exp and " << g << " gold!" << endl;
    exp+=e;
    gold+=g;
    Sleep(SLEEP);
    while (exp >= expLevel) {
        level+=1;
        cout << 
        "\nYou have leveled up!\nYou are now level "
        << level;
        exp -=expLevel;
        expLevel+=expLevel/2;
        Sleep(SLEEP);
        system("cls");
        display_stats();

        cout << "\nChoose a stat to increase:"
        << "\n1. +5❤️   2. +1🛡️   3. +1🗡️\n";
        int choice;
        cin >> choice;
        if (choice==1) {
            playerStats.maxHealth+=5;
        } else if (choice==2) {
            playerStats.armor+=1;
        } else if (choice==3) {
            playerStats.damage+=1;
        }
        playerStats.health=playerStats.maxHealth+
        primary_equipped->itemStats.health+
        secondary_equipped->itemStats.health+
        armor_equipped->itemStats.health;
    }

}

Enemy::Enemy(enemy_template e) {
    name = e.name;
    level = e.level;
    exp = e.exp;
    gold = e.gold;
    stat_roll r = e.statroll;
    enemyStats.maxHealth = r.bh+rand()%(r.h+1);
    enemyStats.health= enemyStats.maxHealth;
    enemyStats.armor = r.ba+rand()%(r.a+1);
    enemyStats.damage = r.bd + rand()%(r.d+1);
    drops=e.drops;
}

int Enemy::take_damage(int dmg) {
    int effective_dmg;
    if(enemyStats.armor >= dmg) {
        effective_dmg = 1;
    } else {
        effective_dmg = dmg-enemyStats.armor;
    }
    cout << "You hit " << name << 
    " for " << effective_dmg << " damage!\n\n";
    enemyStats.health-=effective_dmg;
    Sleep(SLEEP);
    if(enemyStats.health <=0 ) {
        cout << "You have slain " << name << endl;
        return 1;
    }
    return 0;
}

void Enemy::display_stats() {
    cout << name << "\n❤️  :" << enemyStats.health << 
    "/" << enemyStats.maxHealth << " 🛡️  :" << 
    enemyStats.armor << " 🗡️  :" << enemyStats.damage
    << "\nLevel: " << level;
}

Area::Area(string n, vector<enemy_template> enemies, vector<Item*> items,
vector<Item*> shop_items, string d, bool unlcked, int i) {
    name = n;
    enemy_list = enemies;
    item_list = item_list;
    shop_list = shop_items;
    description = d;
    unlocked=unlcked;
    index = i;
}

void Area::print_description() {
    system("cls");
    string input;
    cout << name << "\n\n";
    cout << description << endl;
    cout << "Enter any key to continue..." << endl;
    cin >> input;
}

void combatHUD(Enemy *enemy, Player *player) {
    player->display_stats();
    cout << "\nVS\n\n";
    enemy->display_stats();
}

void combat(Player *p) {
    enemy_template e;
    int rng;
    do {
        rng = rand()%100 + current_area->enemy_list.size();
        rng%=current_area->enemy_list.size();
        e = current_area->enemy_list[rng];
    } while(e.name == previous_encounter);
    Enemy *enemy = new Enemy(current_area->enemy_list[rng]);
    previous_encounter = enemy->name;
    while(1) {
        start_combat:
        system("cls");
        combatHUD(enemy,p);
        string choice;
        cout << "\n\n1. Attack 2. Run\n";
        cin >> choice;
        lower(choice);
        if(choice=="1" || choice=="attack") {
            int effective_damage = p->damage();
            if (rand()%100 <= CRIT_CHANCE) {
                cout << "You landed a critical strike!\n";
                effective_damage*=2;
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
                break;
            }
            
        } else if(choice=="2" || choice=="run") {
            system("cls");
            combatHUD(enemy,p);
            Sleep(1000);
            p->take_damage(enemy);
            cout << "\nYou ran away after taking a hit";
            Sleep(SLEEP);
            break;
        } else {
            cout << "Enter a valid action";
            Sleep(SLEEP);
            goto start_combat;
        }
        system("cls");
        combatHUD(enemy,p);
        Sleep(1000);
        p->take_damage(enemy);
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
    srand(time(NULL));
    for(int i=0;i<3;++i) {
        print_explore();
    }
    int temp = rand() % 100;
    if (temp>=0) {
        cout << "\nMonster encountered! Get ready for combat!";
        Sleep(SLEEP);
        system("cls");
        combat(p);
    } else {
        cout << "\nNothing of interest happened";
        Sleep(1000);
        system("cls");
    }
}

void __rest(Player *p) {
    if(rest<2) {
        if (p->playerStats.health==p->playerStats.maxHealth) {
            cout << "\nYou had a nice rest";
        } else {
            int heal=p->totalHealth()/20
            + rand()%p->totalHealth()/20;
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
        cout << "Items ⚔️\n\n";
        int i,index=1;
        for(i=0;i<owned_items.size();++i) {
            if(owned_items[i]->owned) {
                if(owned_items[i]==p->primary_equipped) {
                    cout << index << ". " << owned_items[i]->name << " - primary" << endl;
                } else if(owned_items[i]==p->secondary_equipped) {
                    cout << index << ". " << owned_items[i]->name << " - secondary" << endl;
                } else if(owned_items[i]==p->armor_equipped) {
                    cout << index << ". " << owned_items[i]->name << " - armor" << endl;
                } else {
                    cout << index << ". " << owned_items[i]->name << endl;
                }
                ++index;
            }
        }
        cout << "e. Unequip primary\nf. Unequip secondary\ng. Unequip armor\nb. Exit items\n";
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
    cout << "\n1. Explore 2. Shop 3. Rest\n4. Items   5. Save";
    cout << " 6. Travel\n7. Arena   8. Exit\n";
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
    } else if(choice=="8" || choice=="exit") {
        exit(0);
    } 
}

void arena(Player *p) {

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
    Player *p = new Player("placeholder",10,0,1,1,0);
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

unordered_map<string,Item*> create_items() {
    unordered_map<string,Item*> all_items;
    // required stats - hp, armor, damage, level
    // item stats - hp, armor, damage, price, sell price

    // Goblin Village items
    all_items["dagger"] = new Item(0,0,2,50,35
    ,req_stats{0,0,1,1},"Dagger", "dagger", 0,false);

    all_items["short_sword"] = new Item(0,0,3,100,70
    ,req_stats{0,0,2,1},"Short Sword", "short_sword", 0,false);

    all_items["sword"] = new Item(0,0,4,200,140
    ,req_stats{0,0,3,3},"Sword", "sword", 0,false);

    all_items["knife"] = new Item(0,0,1,0,10
    ,req_stats{0,0,0,1},"Knife", "knife", 0,false);

    all_items["goblin_spear"] = new Item(0,1,3,0,50
    ,req_stats{0,0,2,3},"Goblin Spear", "goblin_spear", 0,false);

    all_items["bow"] = new Item(5,0,2,0,40
    ,req_stats{0,0,2,1},"Bow", "bow", 0,false);

    all_items["staff"] = new Item(-3,0,5,0,50
    ,req_stats{15,0,0,1},"Staff", "staff", 0,false);

    all_items["cloth_armor"] = new Item(5,1,0,0,50
    ,req_stats{0,0,0,1},"Cloth Armor", "cloth_armor", 1,false);

    //Rocky Mountains items
    all_items["steel_sword"] = new Item(0,0,6,300,210
    ,req_stats{0,0,0,4},"Steel Sword", "steel_sword", 0,false);

    all_items["emerald_sword"] = new Item(0,0,7,400,280
    ,req_stats{0,0,0,5},"Emerald Sword", "emerald_sword", 0,false);

    all_items["diamond_sword"] = new Item(0,0,8,500,350
    ,req_stats{0,0,0,6},"Diamond Sword", "diamond_sword", 0, false);

    return all_items;
}

array<Area*,AREAS> create_areas() {
    
    array<Area*,AREAS> arr;
    string areas[AREAS] = {"Goblin Village",
    "Rocky Mountain", "Murky Swamp",
    "Searing Desert"};
    vector<enemy_template> enemies[AREAS] = {
        {
            // base hp, var hp, base armor, var armor, base damage, var damage;
            // level, exp drop, gold drop, name
            // drops(percentage share, item*)
            {stat_roll{4,0,0,0,1,0},1,3,5,"Goblin Thief",{{100,all_items["knife"]}}},
            {stat_roll{5,0,0,0,1,0},2,4,10,"Goblin Peon",{{25,all_items["goblin_spear"]}}},
            {stat_roll{6,0,1,0,1,0},3,5,15,"Goblin Hunter",{{25,all_items["bow"]}}},
            {stat_roll{6,0,0,0,2,0},3,5,15,"Goblin Mage",{{25,all_items["staff"]}}},
            {stat_roll{7,0,1,0,2,0},4,7,20,"Goblin Warrior",{{25,all_items["sword"]}}},
            {stat_roll{10,0,1,0,3,0},5,20,50,"Goblin Chieftain",{{}}},
        },
        {
            {stat_roll{8,0,1,0,3,2},1,3,5,"Lava Slime",{{}}},
            {stat_roll{9,0,1,0,4,2},2,4,10,"Lava Bat",{{}}},
            {stat_roll{10,0,2,0,5,2},3,5,15,"Lava Wolf",{{}}},
            {stat_roll{11,0,1,0,6,2},3,5,15,"Lava Snake",{{}}},
            {stat_roll{12,0,2,0,7,2},3,5,15,"Lava Demon",{{}}},
            {stat_roll{20,0,3,0,9,0},5,20,50,"Isolated Frost Demon",{{}}},
        },
        {},
        {}
    };
    vector<Item*> items[AREAS] {
        {
            all_items["dagger"],
            all_items["short_sword"],
            all_items["sword"],
            all_items["goblin_spear"],
            all_items["bow"],
            all_items["knife"],
            all_items["cloth_armor"],
        },
        {
            all_items["steel_sword"],
            all_items["emerald_sword"],
            all_items["diamond_sword"]
        },
        {},
        {}
    };
    vector<Item*> shop_items[AREAS] {
        {
            all_items["dagger"],
            all_items["short_sword"],
            all_items["sword"]
        },
        {
            all_items["steel_sword"],
            all_items["emerald_sword"],
            all_items["diamond_sword"]
        },
        {},
        {}
    };
    string descriptions[AREAS] {
        "Goblin village is home to goblins.\n"
        "A green hobbit-like species that likes to attack humans.\n",
        "Rocky mountains are home to lava creatures\n"
        "Imbued with the power of fire they burn unlucky bypassers\n",
        "",
        "",
    };
    bool locks[AREAS] {
        true,false,false,false,
    };
    for(int i=0;i<AREAS;++i) {
        arr[i] = new Area(areas[i],enemies[i],items[i],shop_items[i],descriptions[i],locks[i],i);
    }
    return arr;
}
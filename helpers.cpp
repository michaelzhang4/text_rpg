#include "helpers.hpp"

using namespace std;

Area *area[1];
Area *current_area;
int rest=0;
vector<Item*> all_items = create_items();


void death_screen() {
    system("cls");
    cout << "You died!\n\nGAME OVER";
    exit(0);
}

Item::Item(int hp, int arm, int dmg, int p, int sell_p, req_stats required, string n, bool oo) {
    itemStats.health=hp;
    itemStats.maxHealth=hp;
    itemStats.armor=arm;
    itemStats.damage=dmg;
    price=p;
    sell_price=sell_p;
    req=required;
    name=n;
    owned=oo;
}

void Item::inspect_item(Player *p) {
    while(1) {
        system("cls");
        cout << name << " - " << price << "g" << endl;
        if(itemStats.damage!=0) {
            cout << "🗡️  :" << itemStats.damage << endl;
        }
        if(itemStats.armor!=0) {
            cout << "🛡️  :" << itemStats.armor << endl;
        }
        if(itemStats.health!=0) {
            cout << "❤️  :" << itemStats.health << endl;
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
        if (this->owned==false) {
            cout << "\n1. Buy\nB. Back\n";
            string choice;
            cin >> choice;
            lower(choice);
            if(choice=="1" | choice=="buy") {
                if(p->gold >= price) {
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
            if (p->equipped==this) {
                cout << "\n1. Unequip\nB. Back\n";
                string choice;
                cin >> choice;
                lower(choice);
                if(choice=="1" | choice=="unequip") {
                    p->unequip();
                } else if (choice=="b" | choice=="back") {
                    break;
                }
            } else if (p->equipped!=this) {
                cout << "\n1. Equip\nB. Back\n";
                string choice;
                cin >> choice;
                lower(choice);
                if(choice=="1" | choice=="equip") {
                    p->equip(this);
                } else if (choice=="b" | choice=="back") {
                    break;
                }
            }
        }
    }
}

Player::Player(string s) {
    name=s;
    playerStats.maxHealth=10;
    playerStats.health=10;
    playerStats.armor=1;
    playerStats.damage=1;
    exp=0;
    expLevel=10;
    level=1;
    gold=0;
    none = new Item(0,0,0,0,0
    ,req_stats{0,0,0,0},"None",true);
    equip(none);
}

Player::~Player() {
    delete none;
}

void Player::equip(Item* e) {
    req_stats r = e->req;
    stats s = playerStats;
    if(s.armor >= r.arm &&
    s.damage >= r.dmg &&
    s.health >= r.hp &&
    level >= r.lvl) {
        equipped=e;
    } else {
        cout << "You do not meet the requirements to equip this item\n";
        Sleep(SLEEP);
    }
}

void Player::unequip() {
    equip(none);
}

void Player::display_stats() {
    cout << name <<"\n❤️  : " << 
    playerStats.health << 
    "/" << totalHealth()
    << " (" << equipped->itemStats.health << ") "
     " 🛡️  :" << totalArmor()
    << " (" << equipped->itemStats.armor << ") "
     " 🗡️  :" << damage()
    << " (" << equipped->itemStats.damage << ") "
    << "\nLevel: "<<level<< " - " << exp << "/"
    << expLevel << " 🪙  : " << gold << endl;
}

void Player::print_name() {
    cout << name << "\n";
}

void Player::take_damage(Enemy *e) {
    int effective_damage;
    if(playerStats.armor > e->enemyStats.damage) {
        effective_damage=1;
    } else {
        effective_damage=e->enemyStats.damage-playerStats.armor;
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
    return playerStats.damage+equipped->itemStats.damage;
}

int Player::totalHealth() {
    return playerStats.maxHealth+equipped->itemStats.maxHealth;
}

int Player::totalArmor() {
    return playerStats.armor+equipped->itemStats.armor;
}

void Player::gain(int e, int g) {
    cout << "You gained " << e 
    << " exp and " << g << " gold!" << endl;
    exp+=e;
    gold+=g;
    Sleep(SLEEP);
    if (exp >= expLevel) {
        level+=1;
        cout << 
        "\nYou have leveled up!\nYou are now level "
        << level;
        exp -=expLevel;
        expLevel+=2;
        Sleep(SLEEP);
        system("cls");
        display_stats();

        cout << "\nChoose a stat to increase:"
        << "\n1. +10❤️   2. +1🛡️   3. +1🗡️\n";
        int choice;
        cin >> choice;
        if (choice==1) {
            playerStats.maxHealth+=10;
        } else if (choice==2) {
            playerStats.armor+=1;
        } else if (choice==3) {
            playerStats.damage+=1;
        }
        playerStats.health=playerStats.maxHealth;
    }

}

Enemy::Enemy()
    : types{"Goblin","Troll","Orc"},
        statroll{stat_roll{10,8,1,0,4,2},
        stat_roll{40,50,3,2,7,2},
        stat_roll{25,10,2,1,6,2}}{
    int choice = rand()%3;
    name = types[choice];
    level = 1;
    stat_roll r = statroll[choice];
    enemyStats.maxHealth = r.bh+rand()%(r.h+1);
    enemyStats.health= enemyStats.maxHealth;
    enemyStats.armor = r.ba+rand()%(r.a+1);
    enemyStats.damage = r.bd + rand()%(r.d+1);

    if (choice==0) {
        exp = 5;
        gold = 10;
    } else if (choice==1) {
        exp = 10;
        gold = 20;
    } else if (choice==2) {
        exp = 7;
        gold = 14;
    }
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

void combatHUD(Enemy *enemy, Player *player) {
    player->display_stats();
    cout << "\nVS\n\n";
    enemy->display_stats();
}

void combat(Player *p) {
    // Write combat function
    Enemy *enemy = new Enemy();
    while(1) {
        system("cls");
        combatHUD(enemy,p);
        string choice;
        cout << "\n\n1. Attack\n2. Run\n";
        cin >> choice;
        lower(choice);
        if(choice=="1" || choice=="attack") {
            int died = enemy->take_damage(p->damage());
            if(died==1) {
                p->gain(enemy->exp, enemy->gold);
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
        }
        system("cls");
        combatHUD(enemy,p);
        Sleep(1000);
        p->take_damage(enemy);
    }
}

string start() {
    string name;
    system("cls");
    cout << "Welcome to my text-based RPG game\n\n";
    cout << "Enter your characters name: ";
    cin >> name;
    return name;
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
        cout << p->equipped->name << " - Equipped\n";
        int i;
        for(i=0;i<all_items.size();++i) {
            if(all_items[i]->owned) {
                if(all_items[i]!=p->equipped) {
                    cout << i+1 << ". " << all_items[i]->name << endl;
                }
            }
        }
        cout << "E. Unequip Current Item\nB. Exit items\n";
        string choice;cin >> choice;lower(choice);
        try {
            if (choice == "b" || choice == "exit" || choice=="back") {
                break;
            } else if (choice=="e" || choice == "unequip") {
                p->unequip();
            } else if(1<= stoi(choice) && stoi(choice)<=i) {
                Item* item = all_items[stoi(choice)-1];
                if (item->owned && item!=p->equipped) {
                    item->inspect_item(p);
                }
            }
        } catch (...) {
            cout << "Enter a valid input...";
            Sleep(SLEEP);
        }
    }
}

void choices(Player *p) {
    cout << "\n1. Explore\n2. Shop\n3. Rest\n4. Items\n";
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
        cout << "Shop 🛒\n\n";
        int i;
        for(i=0;i<all_items.size();++i) {
            cout << i+1 << ". ";
            print_item(all_items[i]);
        }
        cout <<"B. Exit shop\n";
        string choice;cin >> choice;lower(choice);
        if (choice == "b" || choice == "exit" || choice=="back") {
            break;
        } else if(1<=stoi(choice) && stoi(choice)<=i) {
            all_items[stoi(choice)-1]->inspect_item(p);
        }
    }
}

vector<Item*> create_items() {
    vector<Item*> all_items;

    Item* dagger = new Item(0,0,3,50,35
    ,req_stats{0,0,1,1},"Dagger",false);
    all_items.push_back(dagger);

    Item* shortSword = new Item(0,0,5,100,70
    ,req_stats{0,0,2,1},"Short Sword",false);
    all_items.push_back(shortSword);

    Item* sword = new Item(0,0,7,200,140
    ,req_stats{0,0,3,3},"Sword",false);
    all_items.push_back(sword);

    return all_items;
}

array<Area*,4> create_areas() {
    array<Area*,4> arr;
    return arr;
}
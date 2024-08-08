#include "helpers.hpp"

using namespace std;

Item::Item(int hp, int arm, int dmg, int chance, double cdmg, int p, int sell_p, req_stats required, string n, string h, int t, bool oo) {
    itemStats.health=hp;
    itemStats.maxHealth=hp;
    itemStats.armor=arm;
    itemStats.damage=dmg;
    itemStats.critChance=chance;
    itemStats.critDamage=cdmg;
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
        if(itemStats.critChance!=0) {
            cout << "💥 : " << itemStats.critChance << "%  ";
        }
        if(itemStats.critDamage!=0) {
            cout << "🔥 : " << itemStats.critDamage << "x ";
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
    playerStats.critChance=10;
    playerStats.critDamage=1.75;
    exp=0;
    expLevel=10;
    level=lvl;
    gold=g;
    none = new Item(0,0,0,0,0,0,0
    ,req_stats{0,0,0,0},"None","none",0,true);
    none_armor = new Item(0,0,0,0,0,0,0
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
    "💥 : " << totalCritChance()
    << "% (" << primary_equipped->itemStats.critChance+secondary_equipped->itemStats.critChance+armor_equipped->itemStats.critChance << ") "
    "🔥 : " << totalCritDmg()
    << "x (" << primary_equipped->itemStats.critDamage+secondary_equipped->itemStats.critDamage+armor_equipped->itemStats.critDamage << ") "
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

int Player::totalCritChance() {
    return playerStats.critChance + primary_equipped->itemStats.critChance
    +secondary_equipped->itemStats.critChance+armor_equipped->itemStats.critChance;
}

double Player::totalCritDmg() {
    return playerStats.critDamage+primary_equipped->itemStats.critDamage
    +secondary_equipped->itemStats.critDamage+armor_equipped->itemStats.critDamage;
}

void Player::gain(int e, int g) {
    if(e>0 || g>0) {
        cout << "You gained " << e 
        << " exp and " << g << " gold!" << endl;
        exp+=e;
        gold+=g;
        Sleep(SLEEP);
    }
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
    enemyStats.critChance = 0;
    enemyStats.critDamage = 0;
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

Area::Area(string n, vector<enemy_template> enemies,
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
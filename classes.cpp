#include "helpers.hpp"

using namespace std;

// 🗡️🛡️❤️🎯💥🔥🌀⚡🌿

Event::Event(event_type t, string d, vector<any> v) {
    type=t;
    args=v;
    descript=d;
}


bool Event::pass(Player *p, int stat, int threshold) {
    bool success = true;
    if(stat==0) {
        if(p->baseHealth() >= threshold) {
            cout << "You passed! - " << threshold << "❤️  (base) ";
        } else {
            cout << "You do not have " << threshold << "❤️  (base) ";
            success=false;
        }
    } else if(stat==1) {
        if(p->totalArmor() >= threshold) {
            cout << "You passed! - " << threshold << "🛡️  (total) ";
        } else {
            cout << "You do not have " << threshold << "🛡️  (total) ";
            success=false;
        }
    } else if(stat==2) {
        if(p->damage() >= threshold) {
            cout << "You passed! - " << threshold << "🗡️  (total) ";
        } else {
            cout << "You do not have " << threshold << "🗡️  (total) ";
            success=false;
        }
    } else if(stat==3) {
        if(p->totalPen() >= threshold) {
            cout << "You passed! - " << threshold << "🎯  (total)";
        } else {
            cout << "You do not have " << threshold << "🎯  (total)";
            success=false;
        }
    } else if(stat==4) {
        if(p->totalCritChance() >= threshold) {
            cout << "You passed! - " << threshold << "💥  (total)";
        } else {
            cout << "You do not have " << threshold << "💥  (total)";
            success=false;
        }
    } else if(stat==5) {
        if(p->totalCritDmg() >= threshold) {
            cout << "You passed! - " << threshold << "🔥  (total)";
        } else {
            cout << "You do not have " << threshold << "🔥  (total)";
            success=false;
        }
    } else if(stat==6) {
        if((int)(p->totalMana()) >= threshold) {
            cout << "You passed! - " << threshold << " 🌀  (total)";
        } else {
            cout << "You do not have " << threshold << " 🌀  (total)";
            success=false;
        }
    } else if(stat==7) {
        if((int)(p->totalSpeed()) >= threshold) {
            cout << "You passed! - " << threshold << " ⚡  (total)";
        } else {
            cout << "You do not have " << threshold << " ⚡  (total)";
            success=false;
        }
    } else if(stat==8) {
        if((int)(p->recoveryRate()*100) >= threshold) {
            cout << "You passed! - " << threshold << "🌿  (total)";
        } else {
            cout << "You do not have " << threshold << "🌿  (total)";
            success=false;
        }
    }
    cout << endl;
    return success;
}

void Event::execute_event(Player *p) {
    string choice;
    ClearScreen();
    cout << descript << "\n\n";
    SleepMs(SLEEP);
    int branch = any_cast<Branch>(args[0]);
    switch(type) {
        case event_type::currency: {
            // args, 0==branch(0==none,1==decision,2==test),
            // 1==exp, 2==gold;
            int exp = any_cast<int>(args[1]);
            int gold = any_cast<int>(args[2]);
            switch(branch) {
                case 0:
                    p->gain(exp,gold);
                    break;
                case 1:
                    cout << "Do you take it? (y/n)\n";
                    cin >> choice;lower(choice);
                    if(choice=="y" || choice=="yes") {
                        if (-gold > p->gold) {
                            cout << "\nYou don't have enough gold!\n";
                        } else {
                            p->gain(exp,gold);
                        }
                    } else {
                        cout << "\nWhat a kind individual you are!\n";
                    }
                    break;
                case 2: {
                    // 3==stat(hp,armor,dmg,critc,cdmg,restoration)
                    // 4==threshold
                    int stat = any_cast<int>(args[3]);
                    int threshold = any_cast<int>(args[4]);
                    if(pass(p,stat,threshold)) {
                        SleepMs(SLEEP);
                        cout << endl;
                        p->gain(exp,gold);
                    }
                    break;
                }
                case 3:
                    cout << "Do you accept the deal? (y/n)\n";
                    cin >> choice;lower(choice);
                    if(choice=="y" || choice=="yes") {
                        if (-gold > p->gold) {
                            cout << "\nYou don't have enough gold!\n";
                        } else {
                            p->gain(exp,gold);
                        }
                    } else {
                        cout << "\nThey shrug and leave\n";
                    }
                    break;
            }
            break;
        }
        case event_type::encounter: {
            cout << branch << endl;
            vector<enemy_template> e = any_cast<vector<enemy_template>>(args[1]);
            int rng = rand() % e.size();
            while (e[rng].name==previous_encounter) {
                rng = rand() % e.size();
            }
            Enemy* encounter_enemy = new Enemy(e[rng]);
            previous_encounter=e[rng].name;
            int dec = combat(p,encounter_enemy);
            if(dec!=1 && current_area->index==1) {
                cout << "\nThe people of the city praise your efforts in protecting the peace!\n";
            }
            break;
        }
        case event_type::hp: {
            // args, 0==branch(0==none,1==decision,2==test),
            // 1==hp_change
            int hp_change = any_cast<int>(args[1]);
            switch(branch) {
                case 0:
                    p->event_hp_change(hp_change);
                    SleepMs(1000);
                    break;
                case 1: {
                    int cost = any_cast<int>(args[2]);
                    cout << "Do you accept? (y/n)\n";
                    cin >> choice;lower(choice);
                    if(choice=="y" || choice=="yes") {
                        p->gain(0,cost);
                        p->event_hp_change(hp_change);
                    }
                    SleepMs(SLEEP);
                    break;
                }
                case 2:
                    // 3==stat(hp,armor,dmg,critc,cdmg,restoration)
                    // 4==threshold
                    int stat = any_cast<int>(args[2]);
                    int threshold = any_cast<int>(args[3]);
                    if(pass(p,stat,threshold)) {
                    } else {
                        p->event_hp_change(hp_change);
                    }
                    SleepMs(SLEEP);
                    break;
            }
            break;
        }
            break;
        case event_type::item: {
            Item* gear = any_cast<Item*>(args[1]);
            if(branch==0) {
                if(gear->owned) {
                    cout << "Unfortunately you already have this item\n";
                    
                } else {
                    gear->owned=true;
                    owned_items.push_back(gear);
                    cout << "You gained a " << gear->name << "!\n";
                }
            } else if (branch==1) {
                cout << "Do you take it? (y/n)\n";
                cin >> choice;lower(choice);
                if(choice=="y" || choice=="yes") {
                    if(gear->owned) {
                        cout << "Unfortunately you already have this item\n";
                    } else {
                        gear->owned=true;
                        owned_items.push_back(gear);
                        cout << "You gained a " << gear->name << "!\n";
                    }
                }
            } else if (branch==2) {
                int stat = any_cast<int>(args[2]);
                int threshold = any_cast<int>(args[3]);
                if(pass(p,stat,threshold)) {
                    if(gear->owned) {
                        cout << "Unfortunately you already have this item\n";
                    } else {
                        gear->owned=true;
                        owned_items.push_back(gear);
                        cout << "You gained a " << gear->name << "!\n";
                    }
                }
            }
            break;
        }
        case event_type::stat: {
            int stat = any_cast<int>(args[1]);
            int change = any_cast<int>(args[2]);
            if(branch==0) {

            } else if(branch==1) {
                stat = rand()%3;
                if(stat == 0) {
                    change = rand()%4+1;
                } else {
                    change = rand()%2+1;
                }
                cout << "Do you accept? (y/n)\n";
                cin >> choice;lower(choice);
                if(choice=="y" || choice=="yes") {
                    if (all_items["past_memories"]->owned) {
                        all_items["past_memories"]->owned=false;
                        cout << "You gave the demon part of your past life.\n";
                        if(stat==0) {
                            p->playerStats.maxHealth+=change;
                            p->playerStats.health+=change;
                            cout << "You gained " << change << "❤️  as a reward.\n";
                        } else if(stat==1) {
                            p->playerStats.damage+=change;
                            cout << "You gained " << change << "🗡️  as a reward.\n";
                        } else if(stat==2) {
                            p->playerStats.armor+=change;
                            cout << "You gained " << change << "🛡️  as a reward.\n";
                        }
                    } else {
                        cout << "You do not own anything of worth to the demon.\n";
                    }
                }
                SleepMs(SLEEP);
            } else if(branch==2) {
                stat = rand()%3;
                if(stat == 0) {
                    change = rand()%4+1;
                } else {
                    change = rand()%2+1;
                }
                vector<pair<string,regex>> riddles= any_cast<vector<pair<string,regex>>>(args[3]);
                string answer;
                int rng=rand()%riddles.size();
                cout << riddles[rng].first;
                cout << "\nYour response: ";
                cin.ignore();
                getline(cin, answer);
                lower(answer);
                if(pattern_match(answer,riddles[rng].second)) {
                    cout << endl;
                    if(stat==0) {
                        p->playerStats.maxHealth+=change;
                        p->playerStats.health+=change;
                        cout << "You gained " << change << "❤️  as a reward for answering his riddle correctly.\n";
                    } else if(stat==1) {
                        p->playerStats.damage+=change;
                        cout << "You gained " << change << "🗡️  as a reward for answering his riddle correctly.\n";
                    } else if(stat==2) {
                        p->playerStats.armor+=change;
                        cout << "You gained " << change << "🛡️  as a reward for answering his riddle correctly.\n";
                    }
                } else {
                    if(stat==0) {
                        p->playerStats.maxHealth-=change;
                        if (p->playerStats.health > p->playerStats.maxHealth) {
                            p->playerStats.health=p->playerStats.maxHealth;
                        }
                        cout << "You lost " << change << "❤️  for failing to answer his riddle correctly.\n";
                    } else if(stat==1) {
                        p->playerStats.damage-=change;
                        cout << "You lost " << change << "🗡️  for failing to answer his riddle correctly.\n";
                    } else if(stat==2) {
                        p->playerStats.armor-=change;
                        cout << "You lost " << change << "🛡️  for failing to answer his riddle correctly.\n";
                    }
                }
            }
            break;
        }
        case event_type::mine: {
            int rng = rand() %100;
            int ore = any_cast<Ores>(args[1]);
            int big = rng>80 ? 1 : 0;
            if(pattern_match(p->primary_equipped->name,regex("Pickaxe")) || pattern_match(p->secondary_equipped->name,regex("Pickaxe"))) {
                if(ore==Ores::Iron) {
                    mining();
                    if(big==1) {
                        cout << "\nYou get lucky and there's a lot of iron ore!.\n";
                        p->gain(10,50);
                    } else {
                        cout << "\nYou mined the iron ore for a nice profit.\n";
                        p->gain(10,25);
                    }
                } else if(ore==Ores::Emerald) {
                    if(pattern_match(p->primary_equipped->name,regex("Emerald|Ruby|Diamond")) || pattern_match(p->secondary_equipped->name,regex("Emerald|Ruby|Diamond"))) {
                        mining();
                        if(big==1) {
                            cout << "\nYou get lucky and there's a lot of emerald ore!.\n";
                            p->gain(20,100);
                        } else {
                            cout << "\nYou mined the emerald ore for a nice profit.\n";
                            p->gain(20,50);
                        }
                    } else {
                        cout << "\nYou try to mine the emerald ore but your steel pickaxe won't make a dent!\n";
                    }
                } else if(ore==Ores::Ruby) {
                    if(pattern_match(p->primary_equipped->name,regex("Ruby|Diamond")) || pattern_match(p->secondary_equipped->name,regex("Ruby|Diamond"))) {
                        mining();
                        if(big==1) {
                            cout << "\nYou get lucky and there's a lot of ruby ore!.\n";
                            p->gain(30,200);
                        } else {
                            cout << "\nYou mined the ruby ore for a nice profit.\n";
                            p->gain(30,100);
                        }
                    } else {
                        cout << "\nYou try to mine the ruby ore but your pickaxe won't make a dent!\n";
                    }
                } else if(ore==Ores::Diamond) {
                    if(pattern_match(p->primary_equipped->name,regex("Diamond")) || pattern_match(p->secondary_equipped->name,regex("Diamond"))) {
                        mining();
                        if(big==1) {
                            cout << "\nYou get lucky and there's a lot of diamond ore!.\n";
                            p->gain(20,400);
                        } else {
                            cout << "\nYou mined the diamond ore for a nice profit.\n";
                            p->gain(20,200);
                        }
                    } else {
                        cout << "\nYou try to mine the diamond ore but your pickaxe won't make a dent!\n";
                    }
                }
            } else {
                cout << "\nYou don't have the suitable equipment to mine this ore.\n";
            }
        }
    }
    cout << "\nEnter any key to continue..." << endl;
    cin>>choice;
}

Item::Item(int hp, int arm, int dmg, int chance, double cdmg, double rr, int p, int sell_p, req_stats required, string n, string h, int t, Skill* _skill, bool oo) {
    itemStats.health=hp;
    itemStats.maxHealth=hp;
    itemStats.armor=arm;
    itemStats.damage=dmg;
    itemStats.critChance=chance;
    itemStats.critDamage=cdmg;
    itemStats.recoveryRate=rr;
    price=p;
    sell_price=sell_p;
    req=required;
    name=n;
    hash=h;
    type=t;
    skill=_skill;
    owned=oo;
}

void Item::inspect_item(Player *p, int from_shop) {
    while(1) {
        ClearScreen();
        if(from_shop==1) {
            if(this->type==0) {
                cout << name << " - Weapon\n" << price << "🪙" << endl;
            } else if(this->type==1) {
                cout << name << " - Armor\n" << price << "🪙" << endl;
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
            cout << "❤️  : " << itemStats.health << " ";
        }
        if(itemStats.pen!=0) {
            cout << "🎯 : " << itemStats.pen << " ";
        }
        if(itemStats.critChance!=0) {
            cout << "💥 : " << itemStats.critChance << "%  ";
        }
        if(itemStats.critDamage!=0) {
            cout << "🔥 : " << itemStats.critDamage << "x ";
        }
        if(itemStats.mana!=0) {
            cout << "🌀 : " << itemStats.mana << " ";
        }
        if(itemStats.speed!=0) {
            cout << "⚡ : " << itemStats.speed << " ";
        }
        if(itemStats.recoveryRate!=0) {
            cout << "🌿 : " << itemStats.recoveryRate << "x ";
        }
        cout << "\nRequirements:\n";
        if(req.hp>0) {
            cout << req.hp << "❤️  (base) ";
        }
        if(req.arm>0) {
            cout << req.arm << "🛡️  (base) ";
        }
        if(req.dmg>0) {
            cout << req.dmg << "🗡️  (base) ";
        }
        if(req.mana>0) {
            cout << req.mana << "🌀  (base) ";
        }
        if(req.speed>0) {
            cout << req.speed << "⚡  (base) ";
        }
        if(req.lvl>0) {
            cout << "level "<< req.lvl << endl;
        }
        if(from_shop==1) {
            cout << "\n" << p->gold << "🪙" << endl;
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
                    SleepMs(SLEEP);
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

Player::Player(string s,int hp,int arm, int dmg, int mna, int spd,int lvl, int g) {
    name=s;
    playerStats.maxHealth=hp;
    playerStats.health=hp;
    playerStats.armor=arm;
    playerStats.damage=dmg;
    playerStats.critChance=10;
    playerStats.critDamage=1.50;
    playerStats.recoveryRate=0.06;
    playerStats.mana=mna;
    playerStats.pen=0;
    playerStats.speed=spd;
    exp=0;
    expLevel=7;
    level=lvl;
    gold=g;
    none = new Item(0,0,0,0,0,0,0,0
    ,req_stats{0,0,0,0},"None","none",0, nullptr,true);
    none_armor = new Item(0,0,0,0,0,0,0,0
    ,req_stats{0,0,0,0},"None","none",1, nullptr,true);
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
    baseHealth() >= r.hp &&
    level >= r.lvl) {
        if((secondary_equipped==e ||primary_equipped==e) && e->name!="None") {
            cout << "You already have this item equipped!\n";
            SleepMs(SLEEP);
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
        SleepMs(SLEEP);
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
    "/" << totalHealth() << " ";
    if(playerStats.health<10) {
        cout << " ";
    }
    cout << "🛡️  :" << totalArmor() << " "
    "   🗡️  :" << damage() << " "
    "\n🎯 : " << totalPen() << " "
    "    💥 : " << totalCritChance() << "% "
    " 🔥 : " << totalCritDmg() << "x "
    "\n🌀 : " << (totalMana()) << " "
    "    ⚡ : " << (totalSpeed()) << " "
    "   🌿 : " << (int)(recoveryRate()*100.0) << "% ";
    cout << "\nLevel "<< level << " - " << exp << "/"
    << expLevel << "✨  " << gold << "🪙" << endl;
}

void Player::print_name() {
    cout << name << " - <" << primary_equipped->name << "> <" << secondary_equipped->name << "> <" << armor_equipped->name << ">";
}

void Player::take_damage(Enemy *e) {
    int effective_damage;
    if(totalArmor() >= e->enemyStats.damage) {
        effective_damage=1;
    } else {
        effective_damage=e->enemyStats.damage-totalArmor();
    }
    playerStats.health-=effective_damage;

    cout << endl << e->name << " hit you for " <<
    effective_damage << " damage!\n";
    SleepMs(SLEEP);
    if(playerStats.health <=0) {
        if(current_area->color==Color::Red) {
            delete_save();
        }
        death_screen();
    }
}

void Player::event_hp_change(int c) {
    playerStats.health+=c;
    if(playerStats.health>totalHealth()) {
        playerStats.health=totalHealth();
    }
    if(c<0) {
        cout << "You lost " <<
        -c << " health!\n";
    } else if(c>0) {
        cout << "You gained " << c << " health!\n";
    }
    if(playerStats.health <=0) {
        SleepMs(SLEEP);
        if(current_area->color==Color::Red) {
            delete_save();
        }
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

int Player::baseHealth() {
    return playerStats.maxHealth;
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

double Player::recoveryRate() {
    return playerStats.recoveryRate+primary_equipped->itemStats.recoveryRate
    +secondary_equipped->itemStats.recoveryRate+armor_equipped->itemStats.recoveryRate;
}

int Player::baseMana() {
    return playerStats.mana;
}

int Player::totalMana() {
    return playerStats.mana + primary_equipped->itemStats.mana
    +secondary_equipped->itemStats.mana+armor_equipped->itemStats.mana;
}

int Player::totalPen() {
    return playerStats.pen + primary_equipped->itemStats.pen
    +secondary_equipped->itemStats.pen+armor_equipped->itemStats.pen;
}

int Player::baseSpeed() {
    return playerStats.speed;
}

int Player::totalSpeed() {
    return playerStats.speed + primary_equipped->itemStats.speed
    +secondary_equipped->itemStats.speed+armor_equipped->itemStats.speed;
}

void Player::gain(int e, int g) {
    if(e>0 && g>0) {
        cout << "You gained " << e 
        << " exp and " << g << " gold!" << endl;
        exp+=e;
        gold+=g;
        SleepMs(SLEEP);
    } else if (e>0) {
        cout << "You gained " << e 
        << " exp!" << endl;
        exp+=e;
        SleepMs(SLEEP);
    } else if (g>0) {
        cout << "You gained " << g 
        << " gold!" << endl;
        gold+=g;
        SleepMs(SLEEP);
    }
    if(g<0) {
        if(-g > gold) {
            cout << "You spent " << gold << " gold!" << endl;
            gold=0;
        } else {
            cout << "You spent " << -g << " gold!" << endl;
            gold+=g;
        }
        SleepMs(SLEEP);
    }
    while (exp >= expLevel) {
        level+=1;
        cout << 
        "\nYou have leveled up!\nYou are now level "
        << level << endl;
        exp -=expLevel;
        expLevel+=(4+expLevel/6);
        SleepMs(SLEEP);
        ClearScreen();
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
    enemyStats.maxHealth = r.baseHp+rand()%(r.hp+1);
    enemyStats.health= enemyStats.maxHealth;
    enemyStats.armor = r.baseArmor+rand()%(r.armor+1);
    enemyStats.damage = r.baseDamage + rand()%(r.damage+1);
    enemyStats.critChance = 0;
    enemyStats.critDamage = 0;
    enemyStats.mana = r.baseMana+rand()%(r.mana+1);
    enemyStats.speed = r.baseSpeed+rand()%(r.speed+1); 
    enemyStats.pen = r.basePen+rand()%(r.pen+1);
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
    SleepMs(SLEEP);
    if(enemyStats.health <=0 ) {
        cout << "You have slain " << name << endl;
        return 1;
    }
    return 0;
}

void Enemy::display_stats() {
    cout << name;
    cout <<"\n❤️  : " << 
    enemyStats.health << 
    "/" << enemyStats.maxHealth << " ";
    if(enemyStats.health<10 && enemyStats.maxHealth<10) {
        cout << "  ";
    } else if(enemyStats.health<10) {
        cout << " ";
    }
    cout << "🛡️  :" << enemyStats.armor << " "
    "  🗡️  :" << enemyStats.damage<< " "
    "\n🎯 : " << enemyStats.pen << " "
    "    💥 : " << enemyStats.critChance << "% "
    " 🔥 : " << enemyStats.critDamage << "x "
    "\n🌀 : " << enemyStats.mana << " "
    "    ⚡ : " << enemyStats.speed << " \n";
}

Area::Area(string n, vector<enemy_template> enemies,
vector<Item*> shop_items, string d, vector<Event*> e_list, bool unlcked, int i, Color c,int encounterc, int eventc,string shop_d) {
    name = n;
    enemy_list = enemies;
    shop_list = shop_items;
    description = d;
    unlocked=unlcked;
    index = i;
    event_list=e_list;
    color = c;
    chance_split[0]=encounterc;
    chance_split[1]=eventc;
    shop_descript=shop_d;
}

void Area::print_description() {
    ClearScreen();
    string input;
    cout << name << "\n\n";
    cout << description << endl;
    cout << "Enter any key to continue..." << endl;
    cin >> input;
}

Skill::Skill(string _name, string _hash,
skillType _type, int _value, int _hpCost, int _manaCost, bool _owned) {
    name=_name;
    hash=_hash;
    type=_type;
    value=_value;
    hpCost=_hpCost;
    manaCost=_manaCost;
    owned=_owned;
}

void Skill::print_info() {
    if(type==skillType::damage) {
        cout << name << " - " << value <<"🗡️  ";
        cout << "Cost - ";
        if (hpCost>0) {
            cout << hpCost << "❤️  ";
        }
        if (manaCost>0) {
            cout << manaCost << "🌀 ";
        }
    } else if(type==skillType::buff) {
    } else if(type==skillType::debuff) {
    }
}

void Skill::execute_skill(Player *p, Enemy *e, int &effective_damage) {
    if(type==skillType::damage) {
        effective_damage=value;
    } else if(type==skillType::buff) {
    } else if(type==skillType::debuff) {
    }
}

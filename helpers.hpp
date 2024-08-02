#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <array>

#define MOBS 3
#define SLEEP 1800

struct stats {
    int health,maxHealth,armor,damage;
};

struct stat_roll {
    int bh,h,ba,a,bd,d;
};

struct req_stats {
    int hp,arm,dmg,lvl;
};

class Item;
class Player;
class Enemy;

class Enemy {
public:
    stats enemyStats;
    Enemy();
    void display_stats();
    int take_damage(int dmg);
    std::string name;
    int exp;
    int gold;
private:
    int level;
    std::string types[MOBS];
    stat_roll statroll[MOBS];
};

class Player {
public:
    stats playerStats;
    int exp,expLevel,level,gold;
    Player(std::string s);
    ~Player();
    void display_stats();
    void print_name();
    void take_damage(Enemy *e);
    void gain(int e, int g);
    Item* equipped;
    int damage();
    int totalHealth();
    int totalArmor();
    Item* none;
    void equip(Item* e);
    void unequip();
private:
    std::string name;
};

class Item {
public:
    Item(int hp, int arm, int dmg, int price, int sell_price, req_stats req, std::string name, bool oo);
    stats itemStats;
    int price, sell_price;
    req_stats req;
    std::string name;
    bool owned;
    void inspect_item(Player *p);
private:
    //empty
};

class Area {
public:
    std::string name;
    std::vector<Enemy*> enemy_list;
    std::vector<Item*> item_list;
    std::vector<Item*> shop_list;
private:
};

void death_screen();

void combat(Player *p);

std::string start();

void lower(std::string &s);

void print_explore();

void explore(Player *p);

void __rest(Player *p);

void choices(Player *p);

void title(Player *p);

void print_item(Item* item);

void items(Player *p);

void shop(Player *p);

std::vector<Item*> create_items();

std::array<Area*,4> create_areas();

extern Area *area[1];
extern std::vector<Item*> all_items;
extern int rest;
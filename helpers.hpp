#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <array>
#include <unordered_map>

#define MOBS 3
#define AREAS 4
#define SLEEP 1800

class Item;
class Player;
class Enemy;

struct stats {
    int health,maxHealth,armor,damage;
};

struct stat_roll {
    int bh,h,ba,a,bd,d;
};

struct req_stats {
    int hp,arm,dmg,lvl;
};

struct enemy_template {
    stat_roll statroll;
    int level,exp,gold;
    std::string name;
};

class Enemy {
public:
    stats enemyStats;
    Enemy(enemy_template e);
    void display_stats();
    int take_damage(int dmg);
    std::string name;
    int exp;
    int gold;
private:
    int level;
    stat_roll statroll;
};

class Player {
public:
    stats playerStats;
    int exp,expLevel,level,gold;
    Player(std::string s,int hp,int arm, int dmg, int lvl, int g);
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
    Area(std::string n, std::vector<enemy_template> enemies, std::vector<Item*> items,
std::vector<Item*> shop_items);
    std::string name;
    std::vector<enemy_template> enemy_list;
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

std::unordered_map<std::string,Item*> create_items();

std::array<Area*,AREAS> create_areas();

extern std::array<Area*,AREAS> areas;
extern std::unordered_map<std::string,Item*> all_items;
extern std::vector<std::string> item_hashes;
extern int rest;
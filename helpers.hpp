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
#include <fstream>

#define MOBS 3
#define AREAS 4
#define SLEEP 1800
#define CRIT_CHANCE 10

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
    std::vector<std::pair<int,Item*>> drops;    
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
    std::vector<std::pair<int,Item*>> drops;
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
    Item* primary_equipped;
    Item* secondary_equipped;
    Item* armor_equipped;
    int damage();
    int totalHealth();
    int totalArmor();
    Item* none;
    Item* none_armor;
    void equip(Item* e, int slot);
    void unequip(Item *item, int slot);
    std::string name;
private:
};

class Item {
public:
    Item(int hp, int arm, int dmg, int price, int sell_price, req_stats req, std::string name, std::string hash, int type, bool oo);
    stats itemStats;
    int price, sell_price, type;
    req_stats req;
    std::string name, hash;
    bool owned;
    void inspect_item(Player *p, int from_shop);
private:
};

class Area {
public:
    Area(std::string n, std::vector<enemy_template> enemies, std::vector<Item*> items,
std::vector<Item*> shop_items, std::string description, bool unlocked, int index);
    std::string name,description;
    std::vector<enemy_template> enemy_list;
    std::vector<Item*> item_list;
    std::vector<Item*> shop_list;
    bool unlocked;
    int index;
    void print_description();
private:
};

void death_screen();

void HUD(Player *p);

void combat(Player *p);

int start();

void lower(std::string &s);

void print_explore();

void explore(Player *p);

void __rest(Player *p);

void choices(Player *p);

void title(Player *p);

void print_item(Item* item);

void items(Player *p);

void arena(Player *p);

void travel(Player* p);

void unlock_stages(Enemy* e);

void shop(Player *p);

void load_game();

void save_game(Player *p);

Player *create_player(int option);

std::unordered_map<std::string,Item*> create_items();

std::array<Area*,AREAS> create_areas();

extern std::vector<std::string> item_hashes;
extern std::vector<Item*> owned_items;
extern std::unordered_map<std::string,Item*> all_items;
extern std::array<Area*,AREAS> areas;
extern Area *current_area;
extern std::vector<Area*> unlocked_areas;
extern int rest;
extern std::string previous_encounter;
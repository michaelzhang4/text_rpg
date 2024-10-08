#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdlib.h>
#include <time.h>
#include <array>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <sstream>
#include <any>
#include <regex>
#include <cstdio>
#include <cstdlib>

#define MOBS 3
#define AREAS 7
#define SLEEP 1800
#define HPPERLVL 4

#ifdef _WIN32
    #include <windows.h>
    inline void SleepMs(int milliseconds) { Sleep(milliseconds); }
    inline void ClearScreen() { system("cls"); }
    inline void enableEmoji() {
        const char* psModulePath = std::getenv("PSModulePath");
        if (psModulePath) {
            system("powershell -Command \"[Console]::OutputEncoding = [System.Text.Encoding]::UTF8\"");
        } else {
            system("chcp 65001 > NUL");
        }
    }
    #define FLUSH()
#elif __linux__ || __APPLE__
    #include <unistd.h>
    #define FLUSH() fflush(stdout)
    inline void enableEmoji() {}
    inline void SleepMs(int milliseconds) { usleep(milliseconds * 1000); }
    inline void ClearScreen() { system("clear"); }
#endif


class Item;
class Player;
class Enemy;
class Skill;

struct stats {
    int health,maxHealth,armor,damage,critChance;
    double critDamage,recoveryRate;
    int mana, pen, speed;
};

struct stat_roll {
    int baseHp,hp,baseArmor,armor,baseDamage,damage,baseMana,mana,baseSpeed,speed,basePen,pen;
};

struct req_stats {
    int hp,arm,dmg,mana,speed,lvl;
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
    int take_damage(Player *p, int dmg);
    std::string name;
    int exp;
    int gold;
    std::vector<std::pair<int,Item*>> drops;
    int level;
private:
    stat_roll statroll;
};

class Player {
public:
    stats playerStats;
    int exp,expLevel,level,gold;
    Player(std::string s,int hp,int arm, int dmg, int mna, int spd, int lvl, int g);
    ~Player();
    void display_stats();
    void print_name();
    void take_damage(Enemy *e, int dmg);
    void event_hp_change(int i);
    void gain(int e, int g);
    Item* primary_equipped;
    Item* secondary_equipped;
    Item* armor_equipped;
    int damage();
    int totalHealth();
    int baseHealth();
    int totalArmor();
    int totalCritChance();
    double totalCritDmg();
    double recoveryRate();
    int baseMana();
    int totalMana();
    int totalPen();
    int baseSpeed();
    int totalSpeed();
    int spell_dmg = 0;
    Item* none;
    Item* none_armor;
    void equip(Item* e, int slot);
    void unequip(Item *item, int slot);
    std::string name;
private:
};

enum event_type {
    item,
    currency,
    stat,
    hp,
    encounter,
    mine,
};

enum Ores {
    Iron,
    Emerald,
    Ruby,
    Diamond,
};

enum Color {
    Green,
    Amber,
    Red,
    Black,
    White,
};

enum Branch {
    None,
    Decision,
    Test,
    Trade,
};

enum skillType {
    damage,
    buff,
    debuff,
};


class Event {
public:
    Event(event_type type, std::string d,std::vector<std::any> v);
    event_type type;
    std::vector<std::any> args;
    std::string descript;
    void execute_event(Player *p);
    bool pass(Player *p, int stat, int threshold);
};

class Item {
public:
    Item(int hp, int arm, int dmg, int c, double cdmg,double rr, int _pen, int _mana, int _speed, int price, int sell_price, req_stats req, std::string name, std::string hash, int type, Skill* skill, bool oo);
    stats itemStats;
    int price, sell_price, type;
    req_stats req;
    std::string name, hash;
    Skill* skill;
    bool owned;
    void inspect_item(Player *p, int from_shop);
private:
};

class Area {
public:
    Area(std::string n, std::vector<enemy_template> enemies,
std::vector<Item*> shop_items, std::string description, std::vector<Event*> event_list, bool unlocked, int index, Color color, int encounter, int event,std::string shop_d);
    std::string name,description,shop_descript;
    std::vector<enemy_template> enemy_list;
    std::vector<Item*> shop_list;
    std::vector<Event*> event_list;
    bool unlocked;
    int index;
    Color color;
    int chance_split[2];
    void print_description();
private:
};

class Skill {
public:
    Skill(std::string _name, std::string _hash,
skillType _type, int _value, stats values, int _hpCost, int _manaCost, bool _owned);
    std::string name;
    std::string hash;
    skillType type;
    stats values;
    int value;
    int hpCost;
    int manaCost;
    bool owned;
    void print_info();
    void execute_skill(Player *p, Enemy *e, int &effective_damage, std::string &msg);
};

void set_up();

void death_screen();

void HUD(Player *p);

int SkillHUD(Player *p, Enemy *e, int &effective_damage, std::string &msg);

int player_turn(Player *p, Enemy *enemy);

void enemy_turn(Player *p, Enemy *enemy, int surprised);

int combat(Player *p, Enemy *e);

int start();

void slow_print(std::string s);

bool isNumber(const std::string& str);

void lower(std::string &s);

void print_explore();

void event(Player *p);

void explore(Player *p);

void __rest(Player *p);

void choices(Player *p);

void chance(Player *p);

void print_chance();

void cleanup_memory();

void add_item(int hp, int arm, int dmg, int c,
            double cdmg, double rr, int _pen, int _mana, int _speed, int price, int sell_price, req_stats h,
            std::string name,std::string hash, int type, Skill* skill, bool owned);

void add_skill(std::string name, std::string hash, skillType type, int value, stats values, 
            int hpCost, int manaCost, bool owned);

void title(Player *p);

void print_item(Item* item);

void items(Player *p);

void arena(Player *p);

void travel(Player* p);

void unlock_stages(Enemy* e);

void mining();

void cleared(Player *p, Enemy* e);

bool pattern_match(std::string &input, std::regex pattern);

void delete_save();

void shop(Player *p);

void intro();

std::string xor_encrypt_decrypt(const std::string &data, const std::string &key);

void load_game();

void save_game(Player *p, bool force);

Player *create_player(int option);

void create_items();

void create_skills();

void create_areas();

extern std::vector<std::string> item_hashes;
extern std::vector<Item*> owned_items;
extern std::unordered_map<std::string,Skill*> all_skills;
extern std::vector<Item*> gamba;
extern std::unordered_map<std::string,Item*> all_items;
extern std::array<Area*,AREAS> areas;
extern Area *current_area;
extern std::vector<Area*> unlocked_areas;
extern int rest;
extern std::string previous_encounter;
extern std::vector<std::pair<Enemy*,bool>> arena_bosses;
extern Event *previous_event;
#include "helpers.hpp"

using namespace std;

int rest=0;
vector<string> item_hashes;
vector<Enemy*> arena_bosses = {new Enemy({stat_roll{50,0,10,0,10,0},50,0,0,"Dungeon Keeper",{{}}}),
};
vector<Item*> owned_items;
unordered_map<string,Item*> all_items;
vector<Item*> gamba;
array<Area*,AREAS> areas = create_areas();
Area *current_area = areas[0];
vector<Area*> unlocked_areas;
string previous_encounter = "None";

void create_items() {
    // required stats - hp, armor, damage, level
    // item stats - hp, armor, damage, crit_chance, crit_dmg, 
    // price, sell price, req_stats, name, armor/weapon, owned

    // Goblin Village items
    add_item("dagger",0,0,2,0,0.0,50,35,{0,0,0,0},"Dagger",0,false);
    add_item("short_sword",0,0,3,0,0.0,100,70,{0,0,0,0},"Short Sword",0,false);
    add_item("sword",0,0,4,0,0.0,200,140,{0,0,0,0},"Sword",0,false);
    add_item("knife",0,0,1,0,0.0,0,10,{0,0,0,0},"Knife",0,false);
    add_item("goblin_spear",0,1,3,0,0.0,0,50,{0,0,0,0},"Goblin Spear",0,false);
    add_item("bow",5,0,2,0,0.0,0,40,{0,0,0,0},"Bow",0,false);
    add_item("staff",-3,-1,5,0,0.0,0,50,{0,0,0,0},"Staff",0,false);
    add_item("cloth_armor",5,1,0,0,0.0,0,50,{0,0,0,0},"Cloth Armor",1,false);

    //Rocky Mountains items
    add_item("sword",0,0,4,0,0.0,200,140,{0,0,0,0},"Sword",0,false);
    add_item("knife",0,0,1,0,0.0,0,10,{0,0,0,0},"Knife",0,false);
    add_item("goblin_spear",0,1,3,0,0.0,0,50,{0,0,0,0},"Goblin Spear",0,false);
    add_item("bow",5,0,2,0,0.0,0,40,{0,0,0,0},"Bow",0,false);
    add_item("staff",-3,-1,5,0,0.0,0,50,{0,0,0,0},"Staff",0,false);
    add_item("cloth_armor",5,1,0,0,0.0,0,50,{0,0,0,0},"Cloth Armor",1,false);
    add_item("fire_spell",-5,-1,9,0,0.0,0,150,{0,0,4,0},"Fire Spell",0,false);
    add_item("fire_staff",-5,-2,10,0,0.0,0,150,{0,0,5,0},"Fire Staff",0,false);

    
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
            {stat_roll{10,0,1,0,3,0},5,20,50,"Goblin Chieftain",{{100,all_items["cloth_armor"]}}},
        },
        {
            {stat_roll{8,0,1,0,3,2},6,10,25,"Lava Slime",{{}}},
            {stat_roll{9,0,1,0,4,2},7,15,30,"Lava Bat",{{}}},
            {stat_roll{10,0,2,0,5,2},8,20,40,"Lava Wolf",{{}}},
            {stat_roll{11,0,1,0,6,2},8,30,40,"Lava Snake",{{}}},
            {stat_roll{12,0,2,0,7,2},9,30,50,"Lava Demon",{{}}},
            {stat_roll{20,0,3,0,9,0},10,100,100,"Isolated Frost Demon",{{}}},
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
    gamba.push_back(all_items["steel_sword"]);
    gamba.push_back(all_items["emerald_sword"]);
    gamba.push_back(all_items["diamond_sword"]);

    string descriptions[AREAS] {
        "Goblin village is home to goblins.\n"
        "A green hobbit-like species that likes to attack humans.\n",
        "Rocky mountains are home to lava creatures\n"
        "As well as many rare ores...\n",
        "",
        "",
    };
    bool locks[AREAS] {
        true,false,false,false,
    };
    for(int i=0;i<AREAS;++i) {
        arr[i] = new Area(areas[i],enemies[i],shop_items[i],descriptions[i],locks[i],i);
    }
    return arr;
}
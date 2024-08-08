#include "helpers.hpp"

using namespace std;

int rest=0;
vector<string> item_hashes;
vector<pair<Enemy*,bool>> arena_bosses = {
    {new Enemy({stat_roll{50,0,10,0,10,0},50,0,1000,"Dungeon Keeper",{{}}}),false},
};
vector<Item*> owned_items;
unordered_map<string,Item*> all_items;
vector<Item*> gamba;
array<Area*,AREAS> areas;
Area *current_area;
vector<Area*> unlocked_areas;
string previous_encounter = "None";

void create_items() {
    // required stats - hp, armor, damage, level
    // item stats - hp, armor, damage, crit_chance, crit_dmg, recRate 
    // price, sell price, req_stats, name, armor/weapon, owned
    // template
    // add_item(0,0,0,0,0.0,0,0,{0,0,0,1},"","",0,false);

    // Goblin Village items
    add_item(0,0,1,0,0.20,0,50,35,{0,0,0,1},"Dagger","dagger",0,false);
    add_item(0,0,3,0,0.0,0,100,70,{0,0,0,1},"Short Sword","short_sword",0,false);
    add_item(0,0,4,0,0.0,0,200,140,{0,0,0,1},"Sword","sword",0,false);
    add_item(0,0,1,10,0.0,0,0,10,{0,0,0,1},"Knife","knife",0,false);
    add_item(1,1,1,0,0.0,0,0,50,{0,0,0,1},"Goblin Spear","goblin_spear",0,false);
    add_item(5,0,2,0,0.0,0,0,40,{0,0,0,1},"Bow","bow",0,false);
    add_item(-3,-1,5,0,0.0,0,0,50,{0,0,0,1},"Staff","staff",0,false);
    add_item(5,1,0,0,0.0,0,0,50,{0,0,0,1},"Cloth Armor","cloth_armor",1,false);

    //Rocky Mountains items
    add_item(0,0,5,0,0.0,0,300,210,{20,0,0,1},"Steel Sword","steel_sword",0,false);
    add_item(0,0,6,0,0.0,0,400,280,{25,0,0,1},"Emerald Sword","emerald_sword",0,false);
    add_item(0,0,7,0,0.0,0,500,350,{30,0,0,1},"Diamond Sword","diamond_sword",0,false);
    add_item(0,0,3,20,0.0,0,0,150,{0,3,0,1},"Molten Dagger","molten_dagger",0,false);
    add_item(0,0,4,0,0.30,0,0,150,{0,4,0,1},"Obsidian Dagger","obsidian_dagger",0,false);
    add_item(-5,-1,9,0,0.0,0,0,150,{0,0,3,1},"Lava Spell","fire_spell",0,false);
    add_item(-5,-2,10,0,0.0,0,0,150,{0,0,4,1},"Fire Staff","fire_staff",0,false);
    add_item(15,5,0,10,0.0,0,0,150,{0,0,0,1},"Sunfire Cape","sunfire_cape",1,false);
}


void create_areas() {
    string area_names[AREAS] = {"Goblin Village",
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
            {stat_roll{8,0,1,0,3,2},6,10,25,"Lava Slime",{{10,all_items["molten_dagger"]},{5,all_items["obsidian_dagger"]}}},
            {stat_roll{9,0,1,0,4,2},7,15,30,"Lava Bat",{{{10,all_items["molten_dagger"]},{5,all_items["obsidian_dagger"]}}}},
            {stat_roll{10,0,2,0,5,2},8,20,40,"Lava Wolf",{{20,all_items["molten_dagger"]},{10,all_items["obsidian_dagger"]}}},
            {stat_roll{11,0,1,0,6,2},8,30,40,"Lava Snake",{{20,all_items["fire_spell"]},{10,all_items["fire_staff"]}}},
            {stat_roll{12,0,2,0,7,2},9,30,50,"Lava Demon",{{{20,all_items["fire_spell"]},{15,all_items["fire_staff"]}}}},
            {stat_roll{20,0,3,0,9,0},10,100,100,"Isolated Frost Demon",{{100,all_items["sunfire_cape"]}}},
        },
        {},
        {}
    };

    vector<Item*> shop_items[AREAS] = {
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
        areas[i] = new Area(area_names[i],enemies[i],shop_items[i],descriptions[i],locks[i],i);
    }
    current_area=areas[0];
}
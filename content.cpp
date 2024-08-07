#include "helpers.hpp"

using namespace std;

int rest=0;
vector<string> item_hashes = {"dagger",
"short_sword","sword","knife","goblin_spear",
"bow", "staff", "cloth_armor", "steel_sword","emerald_sword","diamond_sword"};
vector<Enemy*> arena_bosses = {new Enemy({stat_roll{50,0,10,0,10,0},50,0,0,"Dungeon Keeper",{{}}}),
};
vector<Item*> owned_items;
unordered_map<string,Item*> all_items = create_items();
array<Area*,AREAS> areas = create_areas();
Area *current_area = areas[0];
vector<Area*> unlocked_areas;
string previous_encounter = "None";

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
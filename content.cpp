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
Event *previous_event = nullptr;//areas[0]->event_list[5]

void create_items() {
    // item stats - hp, armor, damage, crit_chance, crit_dmg, recRate 
    // price, sell price, req_stats, name, armor/weapon, owned
    // required stats - hp, armor, damage, level
    // template
    // add_item(0,0,0,0,0.0,0,0,{0,0,0,1},"","",0,false);

    // Goblin Village items
    add_item(0,0,1,0,0.20,0,50,35,{0,0,0,1},"Dagger","dagger",0,false);
    add_item(0,0,3,0,0.0,0,100,70,{0,0,0,1},"Short Sword","short_sword",0,false);
    add_item(0,0,4,0,0.0,0,200,140,{0,0,0,1},"Sword","sword",0,false);
    add_item(0,0,1,10,0.0,0,0,10,{0,0,0,1},"Knife","knife",0,false);
    add_item(1,1,1,0,0.0,0,0,50,{0,0,0,1},"Goblin Spear","goblin_spear",0,false);
    add_item(3,0,2,0,0.0,0,0,40,{0,0,0,1},"Bow","bow",0,false);
    add_item(-3,-1,5,0,0.0,0,0,50,{0,0,0,1},"Staff","staff",0,false);
    add_item(0,2,0,0,0,0,0,35,{0,0,0,1},"Fishing Rod","fishing_rod",0,false);
    add_item(5,1,0,0,0.0,0,0,50,{0,0,0,1},"Cloth Armor","cloth_armor",1,false);

    // Magical Forest items
    add_item(5,0,4,10,0.0,0,300,210,{0,3,0,5},"Elven Bow","elven_bow",0,false);
    add_item(-5,-1,7,0,0.0,0,300,210,{0,0,3,5},"Leaf Spell","leaf_spell",0,false);
    add_item(0,1,6,0,0.0,0,300,210,{25,0,0,5},"Elven Blade","elven_blade",0,false);
    add_item(5,1,0,10,0.15,0.04,0,100,{15,1,1,5},"Elven Cloth","elven_cloth",1,false);

    //Rocky Mountains items
    add_item(0,0,5,0,0.0,0,250,210,{15,0,0,1},"Steel Pickaxe","steel_pickaxe",0,false);
    add_item(0,0,6,0,0.0,0,300,280,{20,0,0,1},"Emerald Pickaxe","emerald_pickaxe",0,false);
    add_item(0,0,7,0,0.0,0,400,350,{25,0,0,1},"Diamond Pickaxe","diamond_pickaxe",0,false);
    add_item(0,0,3,20,0.0,0,0,150,{0,3,0,1},"Molten Dagger","molten_dagger",0,false);
    add_item(0,0,4,0,0.30,0,0,150,{0,4,0,1},"Obsidian Dagger","obsidian_dagger",0,false);
    add_item(-5,-1,9,0,0.0,0,0,150,{0,0,3,1},"Lava Spell","fire_spell",0,false);
    add_item(-5,-2,10,0,0.0,0,0,150,{0,0,4,1},"Fire Staff","fire_staff",0,false);
    add_item(15,5,0,10,0.0,0,0,150,{0,0,0,1},"Sunfire Cape","sunfire_cape",1,false);
}


void create_areas() {
    string area_names[AREAS] = {"Goblin Village",
    "Magical Forest", "Rocky Mountain",
    "Searing Desert"};
    vector<enemy_template> enemies[AREAS] = {
        {
            // base hp, var hp, base armor, var armor, base damage, var damage;
            // level, exp drop, gold drop, name
            // drops(percentage share, item*)
            {stat_roll{4,0,0,0,1,0},1,3,5,"Goblin Thief",{{25,all_items["knife"]},{12.5,all_items["dagger"]}}},
            {stat_roll{6,0,2,0,1,0},2,4,10,"Goblin Peon",{{12.5,all_items["goblin_spear"]}}},
            {stat_roll{8,0,1,0,2,0},3,5,15,"Goblin Hunter",{{12.5,all_items["bow"]}}},
            {stat_roll{8,0,0,0,3,0},3,5,15,"Goblin Mage",{{12.5,all_items["staff"]}}},
            {stat_roll{12,0,2,0,2,0},4,7,20,"Goblin Warrior",{{12.5,all_items["sword"]}}},
            {stat_roll{15,0,3,0,4,0},5,20,50,"Goblin Chieftain",{{100,all_items["cloth_armor"]}}},
        },
        {
            {stat_roll{25,0,2,0,4,0},7,10,25,"Troll",{{}}},
        },
        {
            {stat_roll{15,0,1,0,3,2},6,10,25,"Lava Slime",{{5,all_items["molten_dagger"]},{5,all_items["obsidian_dagger"]}}},
            {stat_roll{15,0,1,0,4,2},7,15,30,"Lava Bat",{{{5,all_items["molten_dagger"]},{5,all_items["obsidian_dagger"]}}}},
            {stat_roll{20,0,2,0,5,2},8,20,40,"Lava Wolf",{{8,all_items["molten_dagger"]},{8,all_items["obsidian_dagger"]}}},
            {stat_roll{22,0,1,0,6,2},8,30,40,"Lava Snake",{{8,all_items["fire_spell"]},{8,all_items["fire_staff"]}}},
            {stat_roll{30,0,2,0,7,2},9,30,50,"Lava Demon",{{{10,all_items["fire_spell"]},{10,all_items["fire_staff"]}}}},
            {stat_roll{50,0,3,0,9,0},10,100,100,"Isolated Frost Demon",{{100,all_items["sunfire_cape"]}}},
        },
        {}
    };

    vector<Item*> shop_items[AREAS] = {
        {
            all_items["dagger"],
            all_items["short_sword"],
            all_items["sword"]
        },
        {
            all_items["elven_bow"],
            all_items["leaf_spell"],
            all_items["elven_blade"]
        },
        {
            all_items["steel_pickaxe"],
            all_items["emerald_pickaxe"],
            all_items["diamond_pickaxe"]
        },
        {}
    };

    gamba.push_back(all_items["steel_sword"]);
    gamba.push_back(all_items["emerald_sword"]);
    gamba.push_back(all_items["diamond_sword"]);

    string descriptions[AREAS] = {
        "You find the 1st floor is home to goblins.\n"
        "While few are friendly, most are out to get you.\n"
        "You see another person farming them for loot.\n",
        "The 2nd floor is a friendly floor, indicated by the green sign next to the floor name.\n"
        "This enchanted forest is inhabited by elves, a race which are amiable to you otherworldly tower climbers.\n"
        "The elves have already cleared this floor and are happy to let you pass to the next.\n"
        "In the future you are welcome to return at any point.\n",
        "These 3rd floor mountains have been heating up for over a century and are close to eruption.\n"
        "Lava creatures roam the floor and many rare ores can be found within the caverns.\n",
        "",
    };

    vector<Event*> area_events[AREAS] = {
        // branch(0==none,1==decision,2==test)
        // curr(branch,exp,gold,stat(hp,ar,d,crit,cdmg,rest),threshhold)
        // hp(branch,change,stat(hp,ar,d,crit,cdmg,rest),threshold)
        {
            new Event(event_type::currency,
            "You almost fall into a Goblin trap, that was a close one!",
            {0,4,0}),
            new Event(event_type::currency,
            "You discover a Goblin's loot!",
            {1,0,10}),
            new Event(event_type::currency,
            "You find a locked chest... are you strong enough to open it?",
            {2,0,50,0,20}),
            new Event(event_type::hp,
            "You get caught in a landslide near the village!"
            "\nIs your armor strong enough to shield you?",
            {2,-3,1,3}),
            new Event(event_type::hp,
            "You find a friendly Goblin Sage who heals your wounds.",
            {0,4}),
            new Event(event_type::item,
            "You find an old fishing rod by the lake!",
            {0,all_items["fishing_rod"]}),
        },
        {
            new Event(event_type::currency,
            "While walking through the city you stop a thief from stealing from an elf.\n"
            "The elf is grateful and wants to pay you back as a reward.",
            {1,0,25}),
            new Event(event_type::encounter,
            "While queuing at the city entrance you see an armed troll attacking civilians.",
            {enemies[1][0]}),
            new Event(event_type::currency,
            "You hang out with friends you made in the city.",
            {0,10,-25}),
            new Event(event_type::hp,
            "You visit the local healer who heals your wounds.",
            {0,100}),
            new Event(event_type::item,
            "A friendly elven blacksmith offers you elven cloth.",
            {1,all_items["elven_cloth"]}),
        },
        {
            new Event(event_type::hp,
            "A lava slime cannonballs into a pool of fire.\n"
            "You were splashed by the lava.",
            {2,-3,0,30}),
            new Event(event_type::currency,
            "You meet another tower climber.\n"
            "They want to trade you 30 exp for 50 gold.",
            {1,30,-50}),
        },
        
        {}
    };

    bool locks[AREAS] {
        true,false,false,false,
    };

    Color colours[AREAS] {
        Color::Amber, Color::Green, Color::Amber, Color::Amber
    };

    int encounters[AREAS] {
        50,100,50,60,
    };

    int events[AREAS] {
        10,30,10,30,
    };

    for(int i=0;i<AREAS;++i) {
        areas[i] = new Area(area_names[i],enemies[i],shop_items[i],descriptions[i],area_events[i],locks[i],i,colours[i],encounters[i],events[i]);
    }
    current_area=areas[0];
    previous_event = areas[0]->event_list[5];
}
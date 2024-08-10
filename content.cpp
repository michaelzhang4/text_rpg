#include "helpers.hpp"

using namespace std;

int rest=0;
vector<string> item_hashes;
vector<pair<Enemy*,bool>> arena_bosses = {
    {new Enemy({stat_roll{10,0,50,0,15,0},25,0,0,"Dungeon Keeper",{{}}}),false},
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
    add_item(0,0,4,0,0.0,0,200,140,{15,0,0,1},"Sword","sword",0,false);
    add_item(0,0,1,10,0.0,0,0,10,{0,0,0,1},"Knife","knife",0,false);
    add_item(1,1,1,0,0.0,0,0,50,{0,0,0,1},"Goblin Spear","goblin_spear",0,false);
    add_item(3,0,2,0,0.0,0,0,40,{0,0,0,1},"Bow","bow",0,false);
    add_item(-3,-1,5,0,0.0,0,0,50,{0,0,3,1},"Staff","staff",0,false);
    add_item(0,2,0,0,0,0,0,35,{0,0,0,1},"Fishing Rod","fishing_rod",0,false);
    add_item(5,1,0,0,0.0,0,0,50,{0,0,0,1},"Cloth Armor","cloth_armor",1,false);

    // Magical Forest items
    add_item(5,0,4,10,0.0,0,300,210,{0,3,0,8},"Elven Bow","elven_bow",0,false);
    add_item(-5,-1,7,0,0.0,0,300,210,{0,0,3,8},"Leaf Spell","leaf_spell",0,false);
    add_item(0,1,6,0,0.0,0,300,210,{25,0,0,8},"Elven Blade","elven_blade",0,false);
    add_item(10,0,4,0,0.0,0,0,100,{20,2,0,8},"Troll Club","troll_club",0,false);
    add_item(2,2,2,0,0,0,0,100,{15,1,1,8},"Orc Spear","orc_spear",0,false);
    add_item(0,0,4,10,0.30,0,0,100,{0,4,0,8},"Ogre Horn","ogre_horn",0,false);
    add_item(0,3,0,0,0,0.05,0,100,{0,0,0,8},"Magical Ring","magical_ring",0,false);
    add_item(-10,-2,9,10,0.15,0,0,100,{0,0,5,8},"Dark Grimoire","dark_grimoire",0,false);
    add_item(5,1,0,10,0.15,0.04,0,100,{15,1,1,8},"Elven Cloth","elven_cloth",1,false);

    //Rocky Mountains items
    add_item(0,0,5,0,0.0,0,250,210,{25,0,0,15},"Steel Pickaxe","steel_pickaxe",0,false);
    add_item(0,1,7,0,0.0,0,400,280,{30,0,0,15},"Emerald Pickaxe","emerald_pickaxe",0,false);
    add_item(0,2,9,0,0.0,0,600,420,{30,0,0,15},"Ruby Pickaxe","ruby_pickaxe",0,false);
    add_item(0,3,13,0,0.0,0,1000,420,{30,0,0,15},"Diamond Pickaxe","diamond_pickaxe",0,false);
    add_item(0,0,3,20,0.0,0,0,150,{0,3,0,15},"Steel Dagger","steel_dagger",0,false);
    add_item(0,0,4,0,0.30,0,0,150,{0,4,0,15},"Gold Dagger","gold_dagger",0,false);
    add_item(-5,-1,9,0,0.0,0,0,150,{0,0,3,15},"Rock Spell","rock_spell",0,false);
    add_item(-5,-2,10,0,0.0,0,0,150,{0,0,4,15},"Earth Staff","earth_staff",0,false);
    add_item(10,3,0,10,0.0,0,0,150,{0,0,0,15},"Earthen Cover","earthern_cover",1,false);

    //Searing Desert items
    add_item(0,0,9,0,0.0,0,0,200,{30,0,0,20},"Scimitar","scimitar",0,false);
    add_item(0,6,0,0,0.0,0,0,200,{30,0,0,20},"Sand Shield","sand_shield",0,false);
    add_item(0,0,0,0,0.0,0.25,0,200,{0,0,0,20},"Water","water",0,false);
    add_item(0,0,-4,20,0.50,0.0,0,200,{0,0,0,20},"Sand Sheathe","sand_sheathe",0,false);
    add_item(-5,-2,6,0,0.50,0,0,200,{0,6,0,20},"Poisoned Dagger","poisoned_dagger",0,false);
    add_item(-10,-2,10,0,0.0,0,0,200,{0,0,9,20},"Sandstorm Spell","sandstorm_spell",0,false);
    add_item(0,3,0,15,0.20,0,0,200,{0,6,0,20},"Sun Amulet","sun_amulet",0,false);
    add_item(5,2,0,15,0.30,0.09,0,200,{0,7,0,20},"Mirage Cloak","mirage_cloak",1,false);
    add_item(5,8,2,-10,0.0,0,0,200,{35,0,0,20},"Desert Armor","desert_armor",1,false);

    // Frozen Tundra items
    add_item(0,0,10,0,0.0,0,0,250,{35,0,0,25},"Ice Claw","ice_claw",0,false);
    add_item(0,0,12,0,0.0,0,0,300,{40,0,0,25},"Frozen Club","frozen_club",0,false);
    add_item(0,0,8,20,0.50,0,0,350,{0,0,0,25},"Ice Shield","ice_shield",0,false);
    add_item(5,3,0,10,0.20,0.05,0,250,{0,0,0,25},"Frozen Amulet","frozen_amulet",0,false);
    add_item(15,10,5,-20,0.0,0,0,500,{60,0,0,35},"Frost Hammer","frost_hammer",0,false);
    add_item(10,3,0,15,0.30,0.09,0,300,{0,0,0,25},"Frost Cloak","frost_cloak",1,false);
    add_item(5,10,3,-10,0.0,0,0,400,{50,0,0,30},"Dragon Scale Armor","dragon_scale_armor",1,false);
    add_item(5,5,2,10,0.0,0,0,300,{40,0,0,25},"Giant Boots","giant_boots",1,false);
    add_item(0,0,15,0,0.30,0,0,400,{0,0,0,25},"Frozen Armor","frozen_armor",1,false);

}


void create_areas() {
    string area_names[AREAS] = {"Goblin Village",
    "Magical Forest", "Rocky Mountain",
    "Searing Desert", "Frozen Tundra", "Underwater Kingdom", "Hell"};
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
            {stat_roll{11,11,2,0,5,0},7,20,30,"Orc",{{10,all_items["orc_spear"]},{2,all_items["magical_ring"]}}},
            {stat_roll{25,5,3,0,4,0},15,30,40,"Troll",{{10,all_items["troll_club"]},{2,all_items["magical_ring"]}}},
            {stat_roll{40,10,4,0,5,0},20,40,50,"Ogre",{{7,all_items["ogre_horn"]},{2,all_items["magical_ring"]}}},
            {stat_roll{3,0,30,0,10,0},30,50,70,"Dark Elf",{{5,all_items["dark_grimoire"]},{2,all_items["magical_ring"]}}},
        },
        {

            {stat_roll{15,0,3,0,7,2},12,10,25,"Bats",{{5,all_items["steel_dagger"]},{5,all_items["gold_dagger"]}}},
            {stat_roll{15,0,4,0,8,2},14,15,30,"Slime",{{{5,all_items["steel_dagger"]},{5,all_items["gold_dagger"]}}}},
            {stat_roll{20,0,5,0,10,2},15,20,40,"Serpant",{{8,all_items["steel_dagger"]},{8,all_items["gold_dagger"]}}},
            {stat_roll{22,0,6,0,11,2},18,30,40,"Rock Golem",{{8,all_items["rock_spell"]},{8,all_items["earth_staff"]}}},
            {stat_roll{30,0,7,0,12,2},21,30,50,"Giant",{{{10,all_items["rock_spell"]},{10,all_items["earth_staff"]}}}},
            {stat_roll{50,0,10,0,15,0},25,100,100,"Dragon",{{100,all_items["earthern_cover"]}}},
        },
        {
            {stat_roll{50,0,10,0,10,0},30,80,150,"Desert Bandit",{{8,all_items["water"]},{5,all_items["scimitar"]}}},
            {stat_roll{100,0,10,0,12,0},30,160,300,"Desert Rogues",{{16,all_items["water"]},{8,all_items["scimitar"]}}},
            {stat_roll{100,0,4,0,15,0},33,100,200,"Sand Golem",{{5,all_items["sum_amulet"]}, {5,all_items["sand_sheathe"]}}},
            {stat_roll{70,0,9,0,20,0},37,100,200,"Poisonous Scorpion",{{5,all_items["poisoned_dagger"]},{3,all_items["sandstorm_spell"]}}},
            {stat_roll{100,0,2,0,22,0},40,150,300,"Desert Wyrm",{{5,all_items["sandstorm_spell"]},{5,all_items["sun_amulet"]}}},
            {stat_roll{150,0,8,0,25,0},50,300,500,"Sand Elemental",{{5,all_items["mirage_cloak"]},{3,all_items["desert_armor"]}}},
        },
        {
            {stat_roll{60,0,12,0,10,0},35,150,200,"Frost Wolf",{{8,all_items["ice_claw"]},{5,all_items["wolf_pelt"]}}},
            {stat_roll{60,0,12,0,10,0},35,150,200,"Frost Wolves",{{8,all_items["ice_claw"]},{5,all_items["wolf_pelt"]}}},
            {stat_roll{80,0,15,0,15,0},40,200,250,"Yeti",{{10,all_items["frozen_club"]},{5,all_items["yeti_fur"]}}},
            {stat_roll{100,0,8,0,20,0},45,250,300,"Ice Elemental",{{5,all_items["ice_crystal"]},{5,all_items["frozen_amulet"]}}},
            {stat_roll{90,0,20,0,18,0},48,275,320,"Snow Golem",{{5,all_items["ice_shield"]},{5,all_items["frozen_armor"]}}},
            {stat_roll{120,0,10,0,25,0},50,300,350,"Frost Giant",{{10,all_items["frost_hammer"]},{10,all_items["giant_boots"]}}},
            {stat_roll{150,0,18,0,30,0},60,500,600,"Ice Dragon",{{100,all_items["dragon_scale_armor"]}}},
        },
        {},
        {},
    };
    vector<enemy_template> floor2encounters;
    floor2encounters.push_back(enemies[1][0]);
    floor2encounters.push_back(enemies[1][1]);
    floor2encounters.push_back(enemies[1][2]);
    floor2encounters.push_back(enemies[1][3]);

    vector<enemy_template> floor4encounters;
    floor4encounters.push_back(enemies[3][1]);

    vector<enemy_template> floor5encounters;
    floor4encounters.push_back(enemies[4][0]);




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
            all_items["ruby_pickaxe"],
            all_items["diamond_pickaxe"]
        },
        {},
        {},
        {},
        {},
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
        "The 3rd floor mountains contain deep caverns and many secrets.\n"
        "A variety of rare ores are unique to this location.\n"
        "So despite the many creatures who roam the floor, tower climbing parties gather here in hopes of striking it rich.\n",
        "The 4th floor is the first red floor and an endless expanse of scorching sand dunes and ancient ruins.\n"
        "Red floors are uninhabitable by tower climbers and are the most difficult floors to clear.\n"
        "The sun beats down relentlessly, and only the strongest can survive here.\n"
        "Dying on a red floor results in permadeath overwriting your save file! Move cautiously through the desert.\n",
        "The 5th floor is a cold land of ice that has formed over a vast ocean.\n"
        "There is a small tribe of tower climbers who have forgone continuing and are hiding from the tower's aura that lures many upward.\n"
        "After narrowly escaping death on the first red floor, they have made the decision to run away from the fight.\n"
        "Despite their camp, the floor is still much in control of the cold monsters who have adapted to this harsh environment.\n",
        "",
        "",
    };

    vector<pair<string,regex>> Riddles = {
        {"To keep this you must first give it to another?\n", regex("word")},
        {"The more I eat the larger I grow, but if I should ever drink I will die.\nWhat am I?\n", regex("fire|fires")},
        {"Two bodies have I, though together we are one. The more I stand still the faster I run.\nWhat am I?\n", regex("hourglass")},
        {"There are two sisters, the first gave birth to the second and the second gave birth to the first.\nWho are they?\n", regex("sun.*moon|moon.*sun")},
        {"What walks on 4 legs in the morning, 2 legs in the evening, and 3 legs at night?\n", regex("man|human|us")},
        {"I exist in the past, I am made in the present, but the future will never taint me.\nWhat am I?\n", regex("memories|memory")},
        {"I appear once in the beginning, twice in a lifetime, and once at the end.\nWhat am I?\n", regex("e")},
        {"What can run but never walks, has a mouth but never talks, has a head but never weeps, has a bed but never sleeps?\n", regex("river")},
    };

    vector<Event*> area_events[AREAS] = {
        // curr(branch,exp,gold,stat(hp,ar,d,crit,cdmg,rest),threshhold)
        // hp(branch,change,stat(hp,ar,d,crit,cdmg,rest),threshold)
        {
            new Event(event_type::currency,
            "You almost fall into a Goblin trap, that was a close one!",
            {Branch::None,4,0}),
            new Event(event_type::currency,
            "You discover a Goblin's loot!",
            {Branch::Decision,0,20}),
            new Event(event_type::currency,
            "You find a locked chest... are you strong enough to open it?",
            {Branch::Test,0,50,0,20}),
            new Event(event_type::item,
            "You find a dark artifact that is enshrined in a protective curse... what is it doing here?",
            {Branch::Test,all_items["dark_grimoire"],2,20}),
            new Event(event_type::hp,
            "You get caught in a landslide near the village!"
            "\nIs your armor strong enough to shield you?",
            {Branch::Test,-2,1,3}),
            new Event(event_type::hp,
            "You find a friendly Goblin Sage who heals your wounds.",
            {Branch::None,4}),
            new Event(event_type::hp,
            "You find a health potion by the side of the road.",
            {Branch::None,3}),
            new Event(event_type::item,
            "You find an old fishing rod by the lake!",
            {Branch::None,all_items["fishing_rod"]}),
        },
        {
            new Event(event_type::currency,
            "While walking through the city you stop a thief from stealing from an elf.\n"
            "The elf is grateful and wants to pay you back as a reward.",
            {Branch::Decision,0,25}),
            new Event(event_type::encounter,
            "While queuing at the city entrance you see an armed figure attacking civilians.",
            {Branch::None,floor2encounters}),
            new Event(event_type::currency,
            "You hang out with friends you made in the city.",
            {Branch::None,10,-25}),
            new Event(event_type::hp,
            "You visit the local healer who heals your wounds.",
            {Branch::None,100}),
            new Event(event_type::item,
            "A friendly elven blacksmith offers you elven cloth.",
            {Branch::Decision,all_items["elven_cloth"]}),
        },
        {
            new Event(event_type::hp,
            "A slime cannonballs into a pool of fire.\n"
            "You were splashed by the lava.",
            {Branch::Test,-8,0,30}),
            new Event(event_type::currency,
            "You meet another tower climber.\n"
            "They want to trade you 30 exp for 50 gold.",
            {Branch::Trade,30,-50}),
            new Event(event_type::stat,
            "The devil appears to test you with the following riddle.",
            {Branch::Test,0,0,Riddles}),
        },
        {
            new Event(event_type::hp,
            "While wandering around you were poisoned by sand scorpions.",
            {Branch::None,-10}),
            new Event(event_type::hp,
            "A sudden sandstorm hits you, reducing your visibility and causing damage.",
            {Branch::Test,-5,1,7}),
            new Event(event_type::currency,
            "You discover a hidden oasis. You find some valuables left by previous climbers.",
            {Branch::None,0,100}),
            new Event(event_type::encounter,
            "You encounter a group of evil Tower Climbers who attack you.",
            {Branch::None,floor4encounters}),
            new Event(event_type::hp,
            "You find an unused health potion nearby a skeleton.",
            {Branch::None,20}),
            new Event(event_type::item,
            "You find a buried chest in the sand containing a powerful artifact.",
            {Branch::Decision,all_items["sun_amulet"]}),
        },
        {
            new Event(event_type::currency,
            "While exploring on the tundra, thin ice cracks beneath you."
            "However, you manage to sprint to safety and make it to hard ice again.",
            {Branch::None,30,0}),
            new Event(event_type::currency,
            "You discover a returner's notes - \'Through Hell and back\'.",
            {Branch::Decision,50,0}),
            new Event(event_type::encounter,
            "You are ambushed by a pack of Frost Wolves.",
            {Branch::None,floor5encounters}),
            new Event(event_type::hp,
            "A sudden blizzard hits, draining your strength as you push forward.",
            {Branch::None,-20}),
            new Event(event_type::item,
            "You find an abandoned yellow relic, no doubt from the previous floor.",
            {Branch::None,all_items["desert_armor"]}),
            new Event(event_type::hp,
            "You find a hidden cave and take shelter from the storm, recovering some health.",
            {Branch::None,15}),
            new Event(event_type::hp,
            "You have a successful hunt and bring back food to the outpost.\n"
            "The settlers are happy to pay you for the provisions.\n",
            {Branch::Decision,0,100}),
        },
        {},
        {},
    };

    string shop_ds[AREAS] {
        "",
        "A starter item for every class!\n",
        "They think people only want pickaxes here...\n",
        "No one has been able to set up shop here...\n",
        "",
        "",
        "",
    };

    bool locks[AREAS] {
        true,false,false,
        false,false,false,
        false,
    };

    Color colours[AREAS] {
        Color::Amber, Color::Green, Color::Amber,
        Color::Red, Color::Amber, Color::Amber,
        Color::Amber,
    };

    int encounters[AREAS] {
        50,100,40,
        60,60,60,
        60
    };

    int events[AREAS] {
        10,20,10,
        30,30,30,
        30
    };

    for(int i=0;i<AREAS;++i) {
        areas[i] = new Area(area_names[i],enemies[i],shop_items[i],descriptions[i],area_events[i],locks[i],i,colours[i],encounters[i],events[i],shop_ds[i]);
    }
    current_area=areas[0];
    previous_event = areas[0]->event_list[5];
}
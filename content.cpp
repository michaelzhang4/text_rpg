#include "helpers.hpp"

using namespace std;

int rest=0;
vector<string> item_hashes;
vector<pair<Enemy*,bool>> arena_bosses = {
    {new Enemy({stat_roll{10,0,50,0,15,0},10,0,0,"Eris",{{}}}),false},
    {new Enemy({stat_roll{50,0,30,0,20,0},20,0,0,"Nemesis",{{}}}),false},
    {new Enemy({stat_roll{5,0,50,0,50,0},30,0,0,"Hecate",{{}}}),false},
    {new Enemy({stat_roll{100,0,50,0,25,0},40,0,0,"Erebus",{{}}}),false},
    {new Enemy({stat_roll{200,0,50,0,30,0},50,0,0,"Hypnos",{{}}}),false},
    {new Enemy({stat_roll{400,0,50,0,35,0},60,0,0,"Nyx",{{}}}),false},
    {new Enemy({stat_roll{800,0,50,0,40,0},70,0,0,"Thanatos",{{}}}),false},
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
    add_item(5,0,4,10,0.0,0,300,210,{0,3,0,5},"Elven Bow","elven_bow",0,false);
    add_item(-5,-1,7,0,0.0,0,300,210,{0,0,3,5},"Leaf Spell","leaf_spell",0,false);
    add_item(0,1,6,0,0.0,0,300,210,{25,0,0,5},"Elven Blade","elven_blade",0,false);
    add_item(10,0,4,0,0.0,0,0,100,{20,2,0,8},"Troll Club","troll_club",0,false);
    add_item(2,2,2,0,0,0,0,100,{15,1,1,8},"Orc Spear","orc_spear",0,false);
    add_item(0,0,4,10,0.30,0,0,100,{0,4,0,8},"Ogre Horn","ogre_horn",0,false);
    add_item(0,3,0,0,0,0.05,0,100,{0,0,0,8},"Magical Ring","magical_ring",0,false);
    add_item(-7,-2,9,10,0.15,0,0,100,{0,0,5,8},"Dark Grimoire","dark_grimoire",0,false);
    add_item(5,1,0,10,0.15,0.04,0,25,{15,1,1,8},"Elven Cloth","elven_cloth",1,false);

    //Rocky Mountains items
    add_item(0,0,5,0,0.0,0,250,210,{25,0,0,4},"Steel Pickaxe","steel_pickaxe",0,false);
    add_item(0,0,7,0,0.0,0,400,280,{30,0,0,5},"Emerald Pickaxe","emerald_pickaxe",0,false);
    add_item(0,0,9,0,0.0,0,600,420,{35,0,0,6},"Ruby Pickaxe","ruby_pickaxe",0,false);
    add_item(0,0,11,0,0.0,0,1000,420,{40,0,0,7},"Diamond Pickaxe","diamond_pickaxe",0,false);
    add_item(0,5,0,0,0.0,0,0,150,{30,0,0,5},"Wooden Shield","wooden_shield",0,false);
    add_item(0,0,5,20,0.40,0,0,150,{0,5,0,10},"Emerald Dagger","emerald_dagger",0,false);
    add_item(0,0,6,20,0.40,0,0,150,{0,6,0,10},"Ruby Dagger","ruby_dagger",0,false);
    add_item(0,0,7,20,0.40,0,0,150,{0,7,0,10},"Diamond Dagger","diamond_dagger",0,false);
    add_item(-3,0,10,5,0.10,0,0,150,{0,0,6,10},"Rock Spell","rock_spell",0,false);
    add_item(-3,0,12,5,0.10,0,0,150,{0,0,7,10},"Earth Staff","earth_staff",0,false);
    add_item(-1,-2,12,5,0.10,0,0,150,{0,0,6,10},"Gem Spell","gem_spell",0,false);
    add_item(10,3,0,0,0.0,0,0,150,{0,0,0,10},"Earthen Cover","earthern_cover",1,false);

    //Searing Desert items
    add_item(0,0,15,0,0.0,0,0,200,{50,0,0,15},"Scimitar","scimitar",0,false);
    add_item(0,10,0,0,0.0,0,0,200,{50,0,0,15},"Sand Shield","sand_shield",0,false);
    add_item(0,0,0,0,0.0,0.20,0,200,{0,0,0,15},"Water","water",0,false);
    add_item(10,0,0,15,0.60,0.0,0,200,{0,8,0,15},"Sand Sheathe","sand_sheathe",0,false);
    add_item(-10,0,6,0,0.50,0,0,200,{0,8,0,15},"Poisoned Dagger","poisoned_dagger",0,false);
    add_item(-5,-2,16,5,0.20,0,0,200,{0,0,9,15},"Sandstorm Spell","sandstorm_spell",0,false);
    add_item(-5,-3,16,10,0.10,0,0,200,{0,0,9,15},"Sand Grimoire","sand_grimoire",0,false);
    add_item(-5,-4,16,5,0.30,0,0,200,{0,0,9,15},"Sand Staff","sand_staff",0,false);
    add_item(20,0,0,15,0.60,0,0,50,{0,8,0,15},"Sun Amulet","sun_amulet",0,false);
    add_item(30,2,6,0,0,0.06,0,200,{0,0,9,15},"Mirage Cloak","mirage_cloak",1,false);
    add_item(10,6,6,0,0.0,0.06,0,200,{50,0,0,15},"Desert Armor","desert_armor",1,false);

    // Frozen Tundra items
    add_item(0,0,22,0,0.0,0,0,300,{75,0,0,20},"Frozen Club","frozen_club",0,false);
    add_item(0,0,25,0,0.0,0,0,500,{80,0,0,20},"Frost Sword","frost_sword",0,false);
    add_item(0,0,10,25,0.40,0,0,250,{0,14,0,20},"Ice Claw","ice_claw",0,false);
    add_item(0,0,7,20,0.70,0,0,250,{0,14,0,20},"Frost Dagger","frost_dagger",0,false);
    add_item(0,18,0,0,0.0,0,0,350,{80,0,0,20},"Ice Shield","ice_shield",0,false);
    add_item(-10,3,24,10,0.0,0.05,0,250,{0,0,14,20},"Frozen Amulet","frozen_amulet",0,false);
    add_item(-10,-3,24,10,0.40,0,0,300,{0,0,14,20},"Ice Crystal","ice_crystal",0,false);
    add_item(-10,-2,24,10,0.40,0,0,300,{0,0,14,20},"Avalanche Spell","avalanche_spell",0,false);
    add_item(40,0,4,10,0.40,0.21,0,300,{0,0,14,20},"Frost Cloak","frost_cloak",1,false);
    add_item(9,9,9,9,0.09,0.09,0,400,{75,0,0,20},"Dragon Scale Armor","dragon_scale_armor",1,false);
    add_item(25,0,5,25,0.40,0,0,400,{0,14,0,20},"Ice Cloth","ice_cloth",1,false);

    // Underwater Kingdom items
    add_item(0,0,-5,0,0.0,0.1,0,400,{0,0,0,25},"Magic Bubble","magic_bubble",0,false);
    add_item(0,2,5,0,0.0,0,150,100,{10,0,0,25},"Trident","trident",0,false);
    add_item(0,0,3,10,0.0,0,200,150,{0,0,10,25},"Serpent Fang","serpent_fang",0,false);
    add_item(0,3,0,0,0,0.05,0,250,{0,0,0,25},"Kraken Ink","kraken_ink",0,false);
    add_item(5,1,0,10,0.15,0.04,0,300,{10,0,0,25},"Shark Tooth Necklace","shark_tooth",1,false);
    add_item(10,3,0,10,0.20,0,0,400,{15,0,0,25},"Leviathan Scale Shield","leviathan_scale",1,false);
    add_item(15,5,0,20,0.30,0,0,500,{20,0,0,25},"Sea Guardian Armor","sea_guardian_armor",1,false);

    // Hell items
    add_item(10,0,20,0,0.0,0,500,350,{40,0,0,30},"Hellhound Fang","hellhound_fang",0,false);
    add_item(20,0,30,0,0.0,0,700,500,{50,0,0,35},"Demon Sword","demon_sword",0,false);
    add_item(30,0,40,0,0.0,0,900,600,{60,0,0,40},"Pit Fiend Armor","pit_fiend_armor",1,false);
    add_item(40,0,50,0,0.0,0,1200,800,{70,0,0,45},"Balrog Whip","balrog_whip",0,false);
    add_item(50,0,60,0,0.0,0,1500,1000,{80,0,0,50},"Archdemon Helm","archdemon_helm",1,false);
    add_item(100,0,100,0,0.0,0,2000,1500,{100,0,0,60},"Satanic Blade","satanic_blade",0,false);

    // Chance items
    add_item(0,0,0,0,0.0,0.35,0,1000,{1,1,1,5},"Healing Ring","healing_ring",0,false);
    add_item(0,0,0,30,0.0,0,0,1000,{0,0,6,7},"Stun Spell","stun_spell",0,false);
    add_item(0,9,-2,0,0.0,0,0,1000,{40,0,0,7},"Night Shield","night_shield",0,false);
    add_item(0,0,7,35,-15.0,0,0,1000,{0,6,0,7},"Night Bow","night_bow",0,false);
    add_item(0,0,14,35,-20.0,0,0,1000,{0,12,0,14},"Bow of Darkness","bow_of_darkness",0,false);
    add_item(0,18,-3,0,0,0,0,1000,{80,0,0,14},"Shield of Darkness","shield_of_darkness",0,false);
    add_item(0,0,28,20,0,0,0,1000,{0,0,14,14},"Umbral Spell","umbral_spell",0,false);
}


void create_areas() {
    gamba.push_back(all_items["healing_ring"]);
    gamba.push_back(all_items["stun_spell"]);
    gamba.push_back(all_items["night_shield"]);
    gamba.push_back(all_items["night_bow"]);
    gamba.push_back(all_items["bow_of_darkness"]);
    gamba.push_back(all_items["shield_of_darkness"]);
    gamba.push_back(all_items["umbral_spell"]);

    string area_names[AREAS] = {"Goblin Village",
    "Magical Forest", "Rocky Mountain",
    "Searing Desert", "Frozen Tundra", "Underwater Kingdom", "Tartarus"};
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
            {stat_roll{15,0,3,0,4,0},5,20,50,"Goblin Chieftain <BOSS>",{{100,all_items["cloth_armor"]}}},
        },
        {
            {stat_roll{11,11,2,0,5,0},6,20,30,"Orc",{{10,all_items["orc_spear"]},{2,all_items["magical_ring"]}}},
            {stat_roll{25,5,5,0,4,0},7,30,40,"Troll",{{10,all_items["troll_club"]},{2,all_items["magical_ring"]}}},
            {stat_roll{40,10,5,0,5,0},8,40,50,"Ogre",{{7,all_items["ogre_horn"]},{2,all_items["magical_ring"]}}},
            {stat_roll{3,0,30,0,10,0},9,50,70,"Dark Elf",{{5,all_items["dark_grimoire"]},{2,all_items["magical_ring"]}}},
        },
        {
            {stat_roll{15,0,3,2,7,2},7,10,25,"Kobold Gang",{{5,all_items["emerald_dagger"]},{5,all_items["wooden_shield"]}}},
            {stat_roll{15,0,4,2,8,2},8,15,30,"Mining Party",{{5,all_items["emerald_dagger"]},{5,all_items["wooden_shield"]}}},
            {stat_roll{20,0,5,2,10,2},9,20,40,"Large Serpent",{{8,all_items["wooden_shield"]},{3,all_items["ruby_dagger"]},{4,all_items["gem_spell"]},{4,all_items["diamond_dagger"]}}},
            {stat_roll{22,0,6,2,11,2},10,30,40,"Rock Golem",{{3,all_items["rock_spell"]},{3,all_items["earth_staff"]},{3,all_items["ruby_dagger"]},{3,all_items["diamond_dagger"]},{4,all_items["gem_spell"]}}},
            {stat_roll{30,0,7,2,12,2},11,30,50,"Giant",{{4,all_items["rock_spell"]},{4,all_items["earth_staff"]},{4,all_items["ruby_dagger"]},{4,all_items["diamond_dagger"]},{4,all_items["gem_spell"]}}},
            {stat_roll{50,0,10,0,15,0},12,100,100,"Dragon <BOSS>",{{40,all_items["earthern_cover"]}}},
        },
        {
            {stat_roll{30,0,10,0,10,0},13,80,150,"Desert Bandit",{{8,all_items["water"]},{3,all_items["scimitar"]},{3,all_items["sand_shield"]}}},
            {stat_roll{50,0,10,0,12,0},14,100,300,"Desert Rogues",{{16,all_items["water"]},{8,all_items["scimitar"]},{5,all_items["sand_shield"]}}},
            {stat_roll{70,0,4,0,15,0},15,100,200,"Sand Golem",{{3,all_items["sand_grimoire"]},{3,all_items["sandstorm_spell"]},{3,all_items["sun_amulet"]}, {3,all_items["sand_sheathe"]},{3,all_items["sand_shield"]}}},
            {stat_roll{40,0,9,0,20,0},16,100,200,"Poisonous Scorpion",{{4,all_items["sand_staff"]},{4,all_items["sand_grimoire"]},{4,all_items["poisoned_dagger"]}, {3,all_items["sand_sheathe"]},{3,all_items["sandstorm_spell"]}}},
            {stat_roll{80,0,3,0,22,0},17,150,300,"Desert Wyrm",{{4,all_items["sand_staff"]},{4,all_items["sand_grimoire"]},{4,all_items["poisoned_dagger"]},{4,all_items["sandstorm_spell"]}, {3,all_items["sand_sheathe"]},{5,all_items["sun_amulet"]}}},
            {stat_roll{100,0,8,0,25,0},18,300,500,"Sand Elemental <BOSS>",{{5,all_items["sandstorm_spell"]},{5,all_items["poisoned_dagger"]},{20,all_items["mirage_cloak"]},{20,all_items["desert_armor"]}}},
        },
        {
            {stat_roll{60,40,12,4,30,4},19,150,200,"Frost Wolf",{{3,all_items["frozen_amulet"]},{3,all_items["ice_claw"]},{3,all_items["frozen_club"]},{3,all_items["ice_shield"]}}},
            {stat_roll{50,40,8,4,40,4},20,150,200,"Frost Wolves",{{3,all_items["frost_sword"]},{3,all_items["frost_dagger"]},{3,all_items["ice_crystal"]},{3,all_items["ice_shield"]}}},
            {stat_roll{60,40,12,4,33,4},21,200,250,"Yeti",{{2,all_items["frozen_amulet"]},{2,all_items["ice_claw"]},{2,all_items["frozen_club"]},{2,all_items["frost_sword"]},{2,all_items["frost_dagger"]},{2,all_items["ice_crystal"]},{3,all_items["ice_shield"]}}},
            {stat_roll{40,40,11,4,33,4},22,250,300,"Ice Elemental",{{3,all_items["ice_crystal"]},{3,all_items["avalanche_spell"]},{3,all_items["ice_shield"]},{3,all_items["frost_dagger"]}}},
            {stat_roll{60,40,20,4,25,4},23,275,320,"Ice Golem",{{5,all_items["ice_shield"]},{5,all_items["frozen_armor"]}}},
            {stat_roll{100,40,10,4,30,4},24,300,350,"Frost Giant",{{5,all_items["ice_shield"]},{5,all_items["frozen_armor"]}}},
            {stat_roll{150,0,18,0,35,0},25,500,600,"Ice Queen <BOSS>",{{10,all_items["dragon_scale_armor"]},{10,all_items["ice_cloth"]},{10,all_items["frost_cloak"]}}},
        },
        {
            {stat_roll{24,5,14,5,14,5},15,225,550,"Sea Serpent",{{5,all_items["serpent_fang"]}}},
            {stat_roll{29,5,14,5,15,5},14,200,500,"Merman Warrior",{{10,all_items["trident"]}}},
            {stat_roll{34,5,14,5,16,5},17,275,650,"Sharkman",{{10,all_items["shark_tooth"]}}},
            {stat_roll{39,5,14,5,17,5},16,250,600,"Kraken Tentacle",{{7,all_items["kraken_ink"]}}},
            {stat_roll{44,5,14,5,18,5},18,300,700,"Leviathan Spawn",{{15,all_items["leviathan_scale"]}}},
            {stat_roll{49,5,14,5,19,5},19,325,750,"Ancient Sea Guardian",{{100,all_items["sea_guardian_armor"]}}},
            {stat_roll{60,0,22,0,25,0},20,500,900,"Poseidon <BOSS>",{{5,all_items["dragon_scale_armor"]}}},  
        },
        {
            {stat_roll{50,0,10,0,20,0},40,200,300,"Hellhound",{{10,all_items["hellhound_fang"]}}},
            {stat_roll{60,0,15,0,25,0},50,300,400,"Demon",{{15,all_items["demon_sword"]}}},
            {stat_roll{70,0,20,0,30,0},60,400,500,"Pit Fiend",{{20,all_items["pit_fiend_armor"]}}},
            {stat_roll{80,0,25,0,35,0},70,500,600,"Balrog",{{25,all_items["balrog_whip"]}}},
            {stat_roll{100,0,30,0,40,0},80,700,800,"Archdemon",{{50,all_items["archdemon_helm"]}}},
            {stat_roll{150,0,40,0,50,0},100,1000,1000,"Satan Lord of Hell <BOSS>",{{5,all_items["satanic_blade"]}}}
        },
    };
    vector<enemy_template> floor2encounters;
    floor2encounters.push_back(enemies[1][0]);
    floor2encounters.push_back(enemies[1][1]);
    floor2encounters.push_back(enemies[1][2]);
    floor2encounters.push_back(enemies[1][3]);

    vector<enemy_template> floor4encounters;
    floor4encounters.push_back(enemies[3][1]);

    vector<enemy_template> floor5encounters;
    floor4encounters.push_back(enemies[4][1]);




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
        "So despite the many creatures who roam the floor, tower climbing parties gather here in hopes of striking it rich.\n"
        "This floor may feel like a jump in difficulty due to the 2nd floor being cleared by the elves\n",

        "The 4th floor is the first red floor and an endless expanse of scorching sand dunes and ancient ruins.\n"
        "Red floors are uninhabitable by tower climbers and are the most difficult floors to clear.\n"
        "The sun beats down relentlessly, and only the strongest can survive here.\n"
        "\n*Dying on a red floor results in permadeath overwriting your save file!*\nMove cautiously through the desert.\n",

        "The 5th floor is a cold land of ice that has formed over a vast ocean.\n"
        "There is a small tribe of tower climbers who have forgone continuing and are hiding from the tower's aura that lures many upward.\n"
        "After narrowly escaping death on the first red floor, they have made the decision to run away from the fight.\n"
        "Despite their camp, the floor is still much in control of the cold monsters who have adapted to this harsh environment.\n",

        "The 6th floor is completely underwater except for the entrance.\n"
        "The kingdom here is ruled by Poseidon the god of the sea.\n"
        "The inhabitants of this kingdom have become more interested in other activities than fighting over time\n"
        "This floor will be difficult for those who cannot swim or breathe underwater...\n",

        "The 7th and final floor is Hell itself, a nightmarish realm of eternal fire and brimstone.\n"
        "The air is thick with the stench of sulfur, and the ground beneath your feet feels as if it could give way to a molten abyss at any moment.\n"
        "Demons of unimaginable power roam these lands, and the very environment seems to be alive, intent on dragging you into the depths.\n"
        "This is the ultimate test of your strength, courage, and willpower. Only those who conquer this inferno can escape the tower.\n",
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
            {Branch::Test,all_items["dark_grimoire"],2,15}),
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
            "While queuing at the city entrance you see an armed figure attacking civilians.\n"
            "You go to confront them.\n",
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
            "Rocks fall on you from above.",
            {Branch::Test,-8,0,30}),
            new Event(event_type::mine,
            "You see a sliver of shiny metal in the darkness.",
            {Branch::None,Ores::Iron}),
            new Event(event_type::mine,
            "You see a faint sparkle of green in the darkness.",
            {Branch::None,Ores::Emerald}),
            new Event(event_type::mine,
            "You see a red glow in the darkness!",
            {Branch::None,Ores::Ruby}),
            new Event(event_type::mine,
            "You see a blue aura radiate in the darkness!",
            {Branch::None,Ores::Diamond}),
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
            {Branch::None,140,0}),
            new Event(event_type::currency,
            "You discover a returner's notes - \'Through Hell and back\'.",
            {Branch::Decision,400,0}),
            new Event(event_type::encounter,
            "You are ambushed by a pack of Frost Wolves.",
            {Branch::None,floor5encounters}),
            new Event(event_type::hp,
            "A sudden blizzard hits, draining your strength as you push forward.",
            {Branch::None,-40}),
            new Event(event_type::item,
            "You find an abandoned yellow relic, no doubt from the previous floor.",
            {Branch::None,all_items["desert_armor"]}),
            new Event(event_type::hp,
            "You find a hidden cave and take shelter from the storm, recovering some health.",
            {Branch::None,30}),
            new Event(event_type::currency,
            "You have a successful hunt and bring back food to the outpost.\n"
            "The settlers are happy to pay you for the provisions.",
            {Branch::Decision,0,100}),
        },
        {
            new Event(event_type::hp,
            "A sudden strong current sweeps you off your feet, injuring you as you're tossed against the rocks.",
            {2,-10,0,20}),
            new Event(event_type::currency,
            "You discover a sunken treasure chest filled with gold coins.",
            {0,0,100}),
            new Event(event_type::encounter,
            "A group of Merman Warriors ambushes you from behind a coral reef.",
            {0,enemies[5]}),
            new Event(event_type::item,
            "You find a rare pearl inside a giant clam.",
            {0,all_items["pearl"]}),
            new Event(event_type::hp,
            "You encounter a friendly dolphin who heals your wounds and guides you to safety.",
            {0,80}),
        },
        {
            new Event(event_type::stat,
            "Satan appears from the depths to test you with the following riddle.",
            {Branch::Test,0,0,Riddles}),
            new Event(event_type::hp,
            "A river of lava suddenly erupts beneath you, burning your flesh and soul.",
            {2,-30,0,50}),
            new Event(event_type::currency,
            "You discover a pile of cursed gold, but its touch drains your life force.",
            {1,100,-50}),
            new Event(event_type::encounter,
            "You are ambushed by a horde of demons led by a Hellhound.",
            {0,enemies[6]}),
            new Event(event_type::stat,
            "A demon offers you a deal: power in exchange for a piece of your soul.",
            {1,50,-25}),
            new Event(event_type::item,
            "You find a powerful artifact imbued with the essence of Hell.",
            {0,all_items["archdemon_helm"]}),
            new Event(event_type::hp,
            "You stumble upon a hidden spring of unholy water that heals your wounds, but at a cost.",
            {0,50,-20}),
        },
    };

    string shop_ds[AREAS] {
        "",
        "A starter item for every class! - Elf Merchant\n",
        "They think people only want pickaxes here...\n",
        "No one has been able to set up shop here...\n",
        "",
        "You probably need something to breathe with",
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
        Color::Red,
    };

    int encounters[AREAS] {
        50,100,45,
        40,40,40,
        40
    };

    int events[AREAS] {
        10,20,10,
        20,20,20,
        20
    };

    for(int i=0;i<AREAS;++i) {
        areas[i] = new Area(area_names[i],enemies[i],shop_items[i],descriptions[i],area_events[i],locks[i],i,colours[i],encounters[i],events[i],shop_ds[i]);
    }
    current_area=areas[0];
    previous_event = areas[0]->event_list[5];
}
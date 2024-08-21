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
vector<Skill*> owned_skills;
unordered_map<string,Item*> all_items;
unordered_map<string,Skill*> all_skills;
vector<Item*> gamba;
array<Area*,AREAS> areas;
Area *current_area;
vector<Area*> unlocked_areas;
string previous_encounter = "None";
Event *previous_event = nullptr;//areas[0]->event_list[5]

void create_items() {
    // item stats - hp, armor, damage, crit_chance, crit_dmg, recRate, armor pen, mana, speed 
    // price, sell price, req_stats, name, armor/weapon, owned
    // required stats - hp, armor, damage, mana, speed, level
    // template
    // add_item(0,0,0,0,0.0,0,0,0,0,0{0,0,0,0,0,1},"","",0,false);

    // Goblin Village items
    add_item(0,0,1, 0,0.20,0, 0,0,0, 50,35,{0,0,0,0,0,1},"Dagger","dagger",0,nullptr,false);
    add_item(0,0,3, 0,0.0,0, 0,0,0, 100,70,{0,0,0,0,0,1},"Short Sword","short_sword",0,nullptr,false);
    add_item(0,0,4, 0,0.0,0, 0,0,0, 200,140,{15,0,0,0,0,1},"Sword","sword",0,nullptr,false);
    add_item(0,0,1, 10,0.0,0, 100,0,0, 0,10,{0,0,0,0,0,1},"Knife","knife",0,nullptr,false);
    add_item(1,1,1, 0,0.0,0, 0,0,0, 0,50,{0,0,0,0,0,1},"Goblin Spear","goblin_spear",0,nullptr,false);
    add_item(3,0,2, 0,0.0,0, 0,0,0, 0,40,{0,0,0,0,0,1},"Bow","bow",0,nullptr,false);
    add_item(0,0,5, 0,0.0,0, 0,0,0, 0,50,{0,0,3,0,0,1},"Staff","staff",0,nullptr,false);
    add_item(0,2,0, 0,0,0, 0,0,0, 0,35,{0,0,0,0,0,1},"Fishing Rod","fishing_rod",0,nullptr,false);
    add_item(5,1,0, 0,0.0,0, 0,0,0, 0,50,{0,0,0,0,0,1},"Cloth Armor","cloth_armor",1,nullptr,false);

    // Magical Forest items
    add_item(5,0,4, 10,0.0,0, 0,0,0, 300,210,{0,3,0,0,0,5},"Elven Bow","elven_bow",0,nullptr,false);
    add_item(0,0,7, 0,0.0,0, 0,0,0, 300,210,{0,0,3,0,0,5},"Leaf Spell","leaf_spell",0,nullptr,false);
    add_item(0,1,6, 0,0.0,0, 0,0,0, 300,210,{25,0,0,0,0,5},"Elven Blade","elven_blade",0,nullptr,false);
    add_item(10,0,4, 0,0.0,0, 0,0,0, 0,100,{20,2,0,0,0,8},"Troll Club","troll_club",0,nullptr,false);
    add_item(2,2,2, 0,0,0, 0,0,0, 0,100,{15,1,1,0,0,8},"Orc Spear","orc_spear",0,nullptr,false);
    add_item(0,0,4, 10,0.30,0, 0,0,0, 0,100,{0,4,0,0,0,8},"Ogre Horn","ogre_horn",0,nullptr,false);
    add_item(0,3,0, 0,0,0.05, 0,0,0, 0,100,{0,0,0,0,0,8},"Magical Ring","magical_ring",0,nullptr,false);
    add_item(-3,-1,9, 10,0.15,0, 0,0,0, 0,100,{0,0,5,0,0,8},"Dark Grimoire","dark_grimoire",0,nullptr,false);
    add_item(5,1,0, 10,0.15,0.04, 0,0,0, 0,25,{15,1,1,0,0,8},"Elven Cloth","elven_cloth",1,nullptr,false);

    //Rocky Mountains items
    add_item(0,0,5, 0,0.0,0, 0,0,0, 250,210,{25,0,0,0,0,4},"Steel Pickaxe","steel_pickaxe",0,nullptr,false);
    add_item(0,0,7, 0,0.0,0, 0,0,0, 400,280,{30,0,0,0,0,5},"Emerald Pickaxe","emerald_pickaxe",0,nullptr,false);
    add_item(0,0,9, 0,0.0,0, 0,0,0, 600,420,{35,0,0,0,0,6},"Ruby Pickaxe","ruby_pickaxe",0,nullptr,false);
    add_item(0,0,11, 0,0.0,0, 0,0,0, 1000,420,{40,0,0,0,0,7},"Diamond Pickaxe","diamond_pickaxe",0,nullptr,false);
    add_item(0,5,0, 0,0.0,0, 0,0,0, 0,150,{30,0,0,0,0,5},"Wooden Shield","wooden_shield",0,nullptr,false);
    add_item(0,0,5, 20,0.40,0, 0,0,0, 0,150,{0,5,0,0,0,10},"Emerald Dagger","emerald_dagger",0,nullptr,false);
    add_item(0,0,6, 20,0.40,0, 0,0,0, 0,150,{0,6,0,0,0,10},"Ruby Dagger","ruby_dagger",0,nullptr,false);
    add_item(0,0,7, 20,0.40,0, 0,0,0, 0,150,{0,7,0,0,0,10},"Diamond Dagger","diamond_dagger",0,nullptr,false);
    add_item(0,0,10, 5,0.10,0, 0,0,0, 0,150,{0,0,6,0,0,10},"Rock Spell","rock_spell",0,nullptr,false);
    add_item(0,0,12, 0,0.20,0, 0,0,0, 0,150,{0,0,7,0,0,10},"Earth Staff","earth_staff",0,nullptr,false);
    add_item(0,0,11, 10,0,0, 0,0,0, 0,150,{0,0,6,0,0,10},"Gem Spell","gem_spell",0,nullptr,false);
    add_item(20,5,0, 0,0.0,0, 0,0,0, 0,150,{0,0,0,0,0,10},"Earthen Cover","earthern_cover",1,nullptr,false);

    //Searing Desert items
    add_item(0,0,15, 0,0.0,0, 0,0,0, 0,200,{50,0,0,0,0,15},"Scimitar","scimitar",0,nullptr,false);
    add_item(0,10,0, 0,0.0,0, 0,0,0, 0,200,{50,0,0,0,0,15},"Sand Shield","sand_shield",0,nullptr,false);
    add_item(0,0,0, 0,0.0,0.20, 0,0,0, 0,200,{0,0,0,0,0,15},"Water","water",0,nullptr,false);
    add_item(10,0,0, 15,0.60,0.0, 0,0,0, 0,200,{0,8,0,0,0,15},"Sand Sheathe","sand_sheathe",0,nullptr,false);
    add_item(-5,0,12, 10,0.50,0, 0,0,0, 0,200,{0,8,0,0,0,15},"Poisoned Dagger","poisoned_dagger",0,nullptr,false);
    add_item(0,0,16, 5,0.20,0, 0,0,0, 0,200,{0,0,9,0,0,15},"Sandstorm Spell","sandstorm_spell",0,nullptr,false);
    add_item(0,0,15, 10,0.10,0, 0,0,0, 0,200,{0,0,9,0,0,15},"Sand Grimoire","sand_grimoire",0,nullptr,false);
    add_item(0,0,14, 5,0.30,0, 0,0,0, 0,200,{0,0,9,0,0,15},"Sand Staff","sand_staff",0,nullptr,false);
    add_item(20,0,2, 15,0.60,0, 0,0,0, 0,50,{0,8,0,0,0,15},"Sun Amulet","sun_amulet",0,nullptr,false);
    add_item(30,2,6, 0,0,0.06, 0,0,0, 0,200,{0,0,9,0,0,15},"Mirage Cloak","mirage_cloak",1,nullptr,false);
    add_item(10,6,6, 0,0.0,0.06, 0,0,0, 0,200,{50,0,0,0,0,15},"Desert Armor","desert_armor",1,nullptr,false);

    // Frozen Tundra items
    add_item(0,0,22, 0,0.0,0, 0,0,0, 0,300,{75,0,0,0,0,18},"Frozen Club","frozen_club",0,nullptr,false);
    add_item(0,0,25, 0,0.0,0, 0,0,0, 0,500,{80,0,0,0,0,18},"Frost Sword","frost_sword",0,nullptr,false);
    add_item(0,0,10, 20,0.40,0, 0,0,0, 0,250,{0,14,0,0,0,18},"Ice Claw","ice_claw",0,nullptr,false);
    add_item(0,0,7, 18,0.30,0, 0,0,0, 2500,1750,{0,14,0,0,0,18},"Ice Dagger","ice_dagger",0,nullptr,false);
    add_item(0,1,20, 0,0.0,0, 0,0,0, 2500,1750,{75,0,0,0,0,18},"Ice Blade","ice_blade",0,nullptr,false);
    add_item(0,0,7, 20,0.70,0, 0,0,0, 0,250,{0,14,0,0,0,18},"Frost Dagger","frost_dagger",0,nullptr,false);
    add_item(0,18,0, 0,0.0,0, 0,0,0, 2700,1400,{80,0,0,0,0,18},"Ice Shield","ice_shield",0,nullptr,false);
    add_item(0,3,23, 10,0.0,0.05, 0,0,0, 0,250,{0,0,14,0,0,18},"Frozen Amulet","frozen_amulet",0,nullptr,false);
    add_item(0,0,24, 20,0,0, 0,0,0, 3000,1400,{0,0,14,0,0,18},"Ice Crystal","ice_crystal",0,nullptr,false);
    add_item(0,0,25, 0,0.40,0, 0,0,0, 0,300,{0,0,14,0,0,18},"Avalanche Spell","avalanche_spell",0,nullptr,false);
    add_item(40,0,4, 10,0.20,0.09, 0,0,0, 0,300,{0,0,14,0,0,18},"Frost Cloak","frost_cloak",1,nullptr,false);
    add_item(9,9,9, 9,0.09,0.09, 0,0,0, 0,400,{75,0,0,0,0,18},"Dragon Scale Armor","dragon_scale_armor",1,nullptr,false);
    add_item(25,0,5, 20,0.40,0, 0,0,0, 0,400,{0,14,0,0,0,18},"Ice Cloth","ice_cloth",1,nullptr,false);

    // Atlantis items
    add_item(0,0,-5, 0,0.0,0.12, 0,0,0, 4000,2800,{0,0,0,0,0,20},"Magic Bubble","magic_bubble",0,nullptr,false);
    add_item(0,0,-5, 0,0.40,0.12, 0,0,0, 4000,2800,{0,0,0,0,0,20},"Fish Scales","fish_scales",0,nullptr,false);
    add_item(0,4,20, 0,0.40,0.06, 0,0,0, 150,100,{80,0,0,0,0,20},"Trident","trident",0,nullptr,false);
    add_item(0,8,10, 20,0.40,0.06, 0,0,0, 150,100,{80,0,0,0,0,20},"Net","net",0,nullptr,false);
    add_item(0,2,22, 0,0.40,0.06, 0,0,0, 200,150,{80,0,0,0,0,20},"Depths Blade","depths_blade",0,nullptr,false);
    add_item(0,0,24, 10,0.40,0.06, 0,0,0, 200,150,{80,0,0,0,0,22},"Abyssal Sword","abyssal_sword",0,nullptr,false);
    add_item(0,0,23, 20,0.40,0.06, 0,0,0, 200,150,{0,18,0,0,0,22},"Abyssal Dagger","abyssal_dagger",0,nullptr,false);
    add_item(0,0,23, 0,0.40,0.06, 0,0,0, 200,150,{0,0,18,0,0,22},"Abyssal Orb","abyssal_orb",0,nullptr,false);
    add_item(0,0,7, 25,0.70,0.06, 0,0,0, 200,150,{0,16,0,0,0,20},"Serpent Fang","serpent_fang",0,nullptr,false);
    add_item(0,0,9, 20,0.70,0.06, 0,0,0, 200,150,{0,16,0,0,0,20},"Depths Dagger","depths_dagger",0,nullptr,false);
    add_item(20,0,11, 10,0.70,0.06, 0,0,0, 200,150,{0,16,0,0,0,20},"Depths Bow","depths_bow",0,nullptr,false);
    add_item(0,4,20, 10,0.10,0.06, 0,0,0, 0,250,{0,0,16,0,0,20},"Whirlpool Spell","whirlpool_spell",0,nullptr,false);
    add_item(0,4,21, 20,0,0.06, 0,0,0, 0,250,{0,0,16,0,0,20},"Tsunami Spell","tsunami_spell",0,nullptr,false);
    add_item(0,0,27, 0,0,0.06, 0,0,0, 0,250,{0,0,16,0,0,20},"Water Staff","water_spell",0,nullptr,false);
    add_item(0,4,22, 0,0,0.06, 0,0,0, 0,300,{0,0,16,0,0,20},"Shark Tooth Necklace","shark_tooth",0,nullptr,false);
    add_item(0,16,2, 0,0,0.06, 0,0,0, 0,400,{80,0,0,0,0,20},"Leviathan Scale Shield","leviathan_scale",0,nullptr,false);
    add_item(15,8,2, 10,0.40,0.24, 0,0,0, 0,500,{20,2,2,0,0,20},"Poseidon's Chariot","poseidons_chariot",1,nullptr,false);

    // Hell items
    add_item(0,0,0, 10,1.0,0.1, 0,0,0, 0,0,{30,14,4,0,0,25},"Past Memories","past_memories",0,nullptr,false);
    add_item(6,6,30, 26,0.76,0.06, 0,0,0, 0,0,{30,14,4,0,0,25},"Stygian Dagger","stygian_dagger",0,nullptr,false);
    add_item(16,6,29, 26,0.76,0.06, 0,0,0, 0,0,{30,14,4,0,0,25},"Soul Reaver","soul_reaver",0,nullptr,false);
    add_item(26,6,28, 26,0.76,0.06, 0,0,0, 0,0,{30,14,4,0,0,25},"Demon Scythe","demon_scythe",0,nullptr,false);
    add_item(36,6,27, 26,0.76,0.06, 0,0,0, 0,0,{30,14,4,0,0,25},"Cerberus Fang","cerberus_fang",0,nullptr,false);
    add_item(66,6,29, 16,0.56,0.06, 0,0,0, 0,0,{75,4,4,0,0,25},"Demon Sword","demon_sword",0,nullptr,false);
    add_item(76,6,28, 16,0.56,0.06, 0,0,0, 0,0,{75,4,4,0,0,25},"Molten Mace","molten_mace",0,nullptr,false);
    add_item(86,6,27, 16,0.56,0.06, 0,0,0, 0,0,{75,4,4,0,0,25},"Inferno Blade","inferno_blade",0,nullptr,false);
    add_item(96,6,26, 16,0.56,0.06, 0,0,0, 0,0,{75,4,4,0,0,25},"Hellfire Axe","hellfire_axe",0,nullptr,false);
    add_item(0,26,0, 0,0.0,0.06, 0,0,0, 0,0,{75,4,4,0,0,25},"Infernal Shield","infernal_shield",0,nullptr,false);
    add_item(6,6,36, 0,0.0,0.06, 0,0,0, 0,0,{30,4,14,0,0,25},"Netherworld Staff","netherworld_staff",0,nullptr,false);
    add_item(6,6,26, 26,0.16,0.06, 0,0,0, 0,0,{30,4,14,0,0,25},"Eternal Flame","eternal_flame",0,nullptr,false);
    add_item(16,6,26, 16,0.16,0.06, 0,0,0, 0,0,{30,4,14,0,0,25},"Fires of Tartarus","fires_of_tartarus",0,nullptr,false);
    add_item(26,6,26, 6,0.16,0.06, 0,0,0, 0,0,{30,4,14,0,0,25},"Hellfire Spell","hellfire_spell",0,nullptr,false);
    add_item(66,6,6, 6,0.66,0.06, 0,0,0, 0,0,{100,0,0,0,0,25},"Hades Crown","hades_crown",0,nullptr,false);

    // Chance items
    add_item(0,0,0, 0,0.0,0.35, 0,0,0, 0,1000,{1,1,1,0,0,5},"Healing Ring","healing_ring",0,nullptr,false);
    add_item(0,0,0, 30,0.0,0,0, 0,0, 0,1000,{0,0,6,0,0,7},"Stun Spell","stun_spell",0,nullptr,false);
    add_item(0,9,-2, 0,0.0,0,0, 0,0, 0,1000,{40,0,0,0,0,7},"Night Shield","night_shield",0,nullptr,false);
    add_item(0,0,7, 35,-15.0,0, 0,0,0, 0,1000,{0,6,0,0,0,7},"Night Bow","night_bow",0,nullptr,false);
    add_item(0,0,14, 35,-20.0,0, 0,0,0, 0,1000,{0,12,0,0,0,14},"Bow of Darkness","bow_of_darkness",0,nullptr,false);
    add_item(0,18,-3, 0,0,0, 0,0,0, 0,1000,{80,0,0,0,0,14},"Shield of Darkness","shield_of_darkness",0,nullptr,false);
    add_item(0,0,28, 20,0,0, 0,0,0, 0,1000,{0,0,14,0,0,14},"Umbral Spell","umbral_spell",0,nullptr,false);
}

void create_skills() {
    add_skill("Fireball","fireball",skillType::damage,5,0,0,false);
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
    "Searing Desert", "Frozen Tundra", "Atlantis", "The Underworld"};
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
            {stat_roll{49,20,15,5,25,5},15,225,550,"Merman Warriors",{{4,all_items["abyssal_orb"]},{4,all_items["abyssal_sword"]},{4,all_items["abyssal_dagger"]}}},
            {stat_roll{44,20,14,5,24,5},15,225,550,"Sea Serpent",{{4,all_items["trident"]},{4,all_items["serpent_fang"]},{4,all_items["whirlpool_spell"]}}},
            {stat_roll{54,20,14,5,26,5},17,275,650,"Scylla",{{4,all_items["depths_bow"]},{4,all_items["water_staff"]},{4,all_items["net"]}}},
            {stat_roll{49,20,14,5,25,5},14,325,500,"Cetus",{{4,all_items["depths_blade"]},{4,all_items["tsunami_spell"]},{4,all_items["depths_dagger"]}}},
            {stat_roll{59,20,14,5,27,5},16,375,600,"Kraken",{{4,all_items["depths_bow"]},{4,all_items["water_staff"]},{4,all_items["trident"]}}},
            {stat_roll{64,20,14,5,28,5},18,425,700,"Leviathan",{{4,all_items["depths_blade"]},{4,all_items["whirlpool_spell"]},{4,all_items["depths_dagger"]}}},
            {stat_roll{69,20,14,5,29,5},19,475,750,"Ancient Sea Guardian",{{4,all_items["abyssal_orb"]},{4,all_items["abyssal_sword"]},{4,all_items["abyssal_dagger"]}}},
            {stat_roll{80,0,22,0,34,0},20,500,900,"Poseidon <BOSS>",{{5,all_items["poseidons_chariot"]}}},  
        },
        {
            {stat_roll{150,66,26,6,66,0},30,700,0,"Erinyes",{{3,all_items["stygian_dagger"]},{3,all_items["demon_sword"]},{3,all_items["netherworld_staff"]}}},
            {stat_roll{100,66,26,6,66,0},26,500,0,"Hellhound",{{3,all_items["stygian_dagger"]},{3,all_items["demon_sword"]},{3,all_items["netherworld_staff"]}}},
            {stat_roll{120,66,26,6,66,0},27,550,0,"Demon",{{3,all_items["soul_reaver"]},{3,all_items["molten_mace"]},{3,all_items["eternal_flame"]}}},
            {stat_roll{140,66,26,6,66,0},28,600,0,"Soul Keeper",{{3,all_items["demon_scythe"]},{3,all_items["inferno_blade"]},{3,all_items["fires_of_tartarus"]}}},
            {stat_roll{160,66,26,6,66,0},29,650,0,"Cerberus",{{3,all_items["cerberus_fang"]},{3,all_items["hellfire_axe"]},{3,all_items["hellfire_spell"]}}},
            {stat_roll{180,66,26,6,66,0},30,750,0,"Archdemon",{{3,all_items["infernal_shield"]},{3,all_items["stygian_dagger"]},{3,all_items["eternal_flame"]}}},
            {stat_roll{666,0,66,0,66,0},35,1000,0,"Hades <BOSS>",{{5,all_items["hades_crown"]}}}
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
    floor5encounters.push_back(enemies[4][1]);
    
    vector<enemy_template> floor6encounters;
    floor6encounters.push_back(enemies[5][0]);

    vector<enemy_template> floor7encounters;
    floor7encounters.push_back(enemies[6][0]);




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
        {
            all_items["ice_dagger"],
            all_items["ice_blade"],
            all_items["ice_shield"],
            all_items["ice_crystal"],
        },
        {
            all_items["magic_bubble"],
            all_items["fish_scales"],
        },
        {
            all_items["past_memories"],
        },
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
        // curr(branch,exp,gold,stat(hp,ar,d,pen,crit,cdmg,mana,speed,rest),threshhold)
        // hp(branch,change,stat(hp,ar,d,pen,crit,cdmg,mana,speed,rest),threshold)
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
            {Branch::Test,-2,1,2}),
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
            {Branch::Decision,240,0}),
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
            {Branch::Decision,0,300}),
        },
        {
            new Event(event_type::hp,
            "You realise you can't hold your breath underwater forever!"
            "Did you come prepared?\n",
            {Branch::Test,-100,5,30}),
            new Event(event_type::currency,
            "You discover an altar of Poseidon. Can you get the altar's blessing?.",
            {Branch::Test,200,300,5,30}),
            new Event(event_type::currency,
            "You discover a rare pearl inside a giant clam. Can you remove it?",
            {Branch::Test,0,800,5,30}),
            new Event(event_type::currency,
            "You discover a shipwreck. You try to explore it.",
            {Branch::Test,0,800,5,30}),
            new Event(event_type::hp,
            "You get sucked into a whirlpool as you swim around.",
            {Branch::Test,-150,5,30}),
            new Event(event_type::encounter,
            "You are ambushed by a group of Merman Warriors.",
            {Branch::Test,floor6encounters}),
            new Event(event_type::hp,
            "You encounter a friendly dolphin who heals your wounds and guides you to safety.",
            {0,80}),
        },
        {
            new Event(event_type::stat,
            "Hades appears from the depths to test you with the following riddle.",
            {Branch::Test,0,0,Riddles}),
            new Event(event_type::hp,
            "While Charon ferries you across the Styx, the water splashes dangerously around you.\n"
            "You do not want to lose your memories in the underworld...",
            {Branch::Test,-50,5,36}),
            new Event(event_type::currency,
            "You discover a pile of cursed gold, but its touch drains your life force.",
            {Branch::None,-300,1000}),
            new Event(event_type::encounter,
            "You find that the Furies are on your tail.",
            {Branch::None,floor7encounters}),
            new Event(event_type::stat,
            "A demon offers you a deal: power in exchange for a piece of your soul.",
            {Branch::Decision,0,0,}),
            new Event(event_type::hp,
            "The souls of the dead drain your life essence, they want you to become one of them.",
            {Branch::None,-25}),
        },
    };

    string shop_ds[AREAS] {
        "",
        "A starter item for every class! - Elf Merchant\n",
        "They think people only want pickaxes here...\n",
        "No one has been able to set up shop here...\n",
        "A hoard of items from the Tundra that have been collected over time.\n",
        "A returner's shop set up so climbers can explore the Atlantis safely!\n",
        "Gold is a worthless currency in the underworld.\n"
        "Only a soul's memories of a past life are valued here.\n",
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
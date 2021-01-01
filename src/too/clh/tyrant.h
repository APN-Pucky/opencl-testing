
#include "ocl.h"
#ifndef CLTYRANT_H_INCLUDED
#define CLTYRANT_H_INCLUDED

#define Player int

/*
enum CLFix
{
    no_fix,
    enhance_early,
    revenge_on_death,
    death_from_bge,
    legion_under_mega,
    num_fixes
};*/

//class Card;

enum CLFaction
{
    allfactions,
    imperial,
    raider,
    bloodthirsty,
    xeno,
    righteous,
    progenitor,
    num_factions
};

enum CLSkill
{
    // Placeholder for no-skill:
    no_skill,

    // Activation (harmful):
    enfeeble, jam, mortar, siege, strike, sunder, weaken,

    // Activation (helpful):
    evolve, heal, mend, overload, protect, rally, fortify,
    enrage, entrap, rush,

    // Activation (unclassified/polymorphic):
    mimic,

    // Defensive:
    armor, avenge, scavenge, corrosive, counter, evade, subdue, absorb, flying,
    payback, revenge, tribute, refresh, wall, barrier_,

    // Combat-Modifier:
    coalition, legion, pierce, rupture, swipe, drain, venom, hunt,mark,

    // Damage-Dependent:
    berserk,  leech, poison,

    // Instant-Debuff:
    inhibit, sabotage, disease,

    // Triggered:
    allegiance, flurry, valor, stasis, summon, bravery, enhance,

    // End of skills
    num_skills
};
enum CLTrigger
{
    activate,
    played, // TODO revert?
    attacked,
    death,
    num_triggers
};
struct CLSkillSpec
{
    enum CLSkill id;
    unsigned x;
    enum CLFaction y;
    unsigned n;
    unsigned c;
    enum CLSkill s;
    enum CLSkill s2;
    bool all;
    unsigned card_id;
};
const __constant int size_skillspec = 9;
void skillspec_to_data(int* data,struct CLSkillSpec ss); 
/*enum PassiveBGE
{
    // Placeholder for no-bge:
    no_bge,

    // Passive BGEs
    bloodlust, brigade, counterflux, divert, enduringrage, fortification, heroism,
    zealotspreservation, metamorphosis, megamorphosis, revenge, turningtides, virulence,
    haltedorders, devour, criticalreach, temporalbacklash, furiosity, oath_of_loyalty,
    bloodvengeance, coldsleep,ironwill,unity,devotion,crackdown,superheroism,

    // End of BGEs
    num_passive_bges
};*/

enum CLCardType {
    commander,
    assault,
    structure,
    num_cardtypes
};

enum CLCardCategory {
    normal,
    special,
    fortress_defense,
    fortress_siege,
    fortress_conquest,
    dominion_alpha,
    dominion_material,
    num_cardcategories
};
#endif
#include "tyrant.h"
#ifndef CLCARD_H_INCLUDED
#define CLCARD_H_INCLUDED
struct CLCard 
{
    unsigned m_attack;
    unsigned m_base_id;  // The id of the original card if a card is unique and alt/upgraded. The own id of the card otherwise.
    unsigned m_delay;
    enum CLFaction m_faction;
    unsigned m_health;
    unsigned m_id;
    unsigned m_level;
    unsigned m_fusion_level;
    //std::string m_name;
    unsigned m_rarity;
    unsigned m_set;
    struct CLSkillSpec m_skills[3];
    struct CLSkillSpec m_skills_on_play[3];
    //APN
    struct CLSkillSpec m_skills_on_attacked[3];
    struct CLSkillSpec m_skills_on_death[3];
    unsigned m_skill_value[num_skills];
    enum CLTrigger m_skill_trigger[num_skills];
    enum CLCardType m_type;
    enum CLCardCategory m_category;
    //const Card* m_top_level_card; // [TU] corresponding full-level card
    //unsigned m_recipe_cost;
    //std::map<const Card*, unsigned> m_recipe_cards;
    //std::map<const Card*, unsigned> m_used_for_cards;
};
//const __constant int size_card = sizeof(Card)/sizeof(unsigned);//10+4*3*size_skillspec+num_skills*2+2;
const __constant int size_card = 10+4*3*size_skillspec+num_skills*2+2;
struct CLCardStatus
{
    const struct CLCard* m_card;
    unsigned m_index;
    unsigned m_action_index;
    unsigned m_player;
    unsigned m_delay;
    signed m_hp;
    unsigned m_absorption;
    //CardStep m_step;
    unsigned m_perm_health_buff;
    unsigned m_perm_attack_buff;
    signed m_temp_attack_buff;
    unsigned m_attack;

    unsigned m_corroded_rate;
    unsigned m_corroded_weakened;
    unsigned m_subdued;
    unsigned m_enfeebled;
    unsigned m_evaded;
    unsigned m_inhibited;
    unsigned m_sabotaged;
    unsigned m_paybacked;
    unsigned m_tributed;
    unsigned m_poisoned;
    unsigned m_protected;
    unsigned m_protected_stasis;
    unsigned m_enraged;
    unsigned m_entrapped;
    unsigned m_marked;
    unsigned m_diseased;

    //signed m_primary_skill_offset[Skill::num_skills];
    //signed m_evolved_skill_offset[Skill::num_skills];
    //unsigned m_enhanced_value[Skill::num_skills];
    //unsigned m_skill_cd[Skill::num_skills];

    bool m_jammed;
    bool m_overloaded;
    bool m_rush_attempted;
    bool m_sundered;
    bool m_summoned;

    //CardStatus() {}

    //void set(const Card* card);
    //void set(const Card& card);
    //std::string description() const;
    //inline unsigned skill_base_value(Skill::Skill skill_id) const;
    //inline unsigned skill(Skill::Skill skill_id) const;
    //inline bool has_skill(Skill::Skill skill_id) const;
    //inline unsigned enhanced(Skill::Skill skill) const;
    //inline unsigned protected_value() const;
    //inline unsigned attack_power() const;
    //inline signed calc_attack_power() const;
    //inline unsigned max_hp() const;
    //inline unsigned add_hp(unsigned value);
    //inline unsigned ext_hp(unsigned value);
};

void clear_cardstatus(struct CLCardStatus * cs);

void card_from_data(struct CLCard* cp, __global int* all_cards, const int size_all_cards, int id);
  
void card_to_data(int* data, const struct CLCard c);

void print_card(const struct CLCard* c);
void print_cardstatus(const struct CLCardStatus* c);

void card_from_data(struct CLCard* pc,__global int* all_cards, const int size_all_cards, int id) ;
void cardstatus_from_card(struct CLCardStatus* pcs,struct CLCard* pc);

struct CLCardStatus cardstatus(__global int* all_cards, int id) ;

__global int* card(__global int* all_cards, int id) ;
int hp(__global int* card) ;
int attack(__global int* card) ;
#endif 
struct Card 
{
    unsigned m_attack;
    unsigned m_base_id;  // The id of the original card if a card is unique and alt/upgraded. The own id of the card otherwise.
    unsigned m_delay;
    enum Faction m_faction;
    unsigned m_health;
    unsigned m_id;
    unsigned m_level;
    unsigned m_fusion_level;
    //std::string m_name;
    unsigned m_rarity;
    unsigned m_set;
    struct SkillSpec m_skills[3];
    struct SkillSpec m_skills_on_play[3];
    //APN
    struct SkillSpec m_skills_on_attacked[3];
    struct SkillSpec m_skills_on_death[3];
    unsigned m_skill_value[num_skills];
    enum Trigger m_skill_trigger[num_skills];
    enum CardType m_type;
    enum CardCategory m_category;
    //const Card* m_top_level_card; // [TU] corresponding full-level card
    //unsigned m_recipe_cost;
    //std::map<const Card*, unsigned> m_recipe_cards;
    //std::map<const Card*, unsigned> m_used_for_cards;
};
const __constant int size_card = 10+4*3*size_skillspec+num_skills*2+2;
struct CardStatus
{
    const struct Card* m_card;
    unsigned m_index;
    unsigned m_action_index;
    unsigned m_player;
    unsigned m_delay;
    unsigned m_hp;
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


struct Card card_from_data(__global int* all_cards, const int size_all_cards, int id) {
    struct Card c;
    for(__global int* i= all_cards; i < all_cards+size_all_cards;i+=size_card) {
        if(*(i+5)==id) {
            for(int j =0; j<size_card;++j) {
                ((int*)(&c))[j] = *(i+j);
            }
            return c;
        }

    }
}

void card_to_data(int* data, struct Card c) {
    data[0] = c.m_attack;
    data[1] = c.m_base_id;
    data[2] = c.m_delay;
    data[3] = c.m_faction;
    data[4] = c.m_health;
    data[5] = c.m_id;
    data[6] = c.m_level;
    data[7] = c.m_fusion_level;
    data[8] = c.m_rarity;
    data[9] = c.m_set;
    skillspec_to_data(data+10,c.m_skills[0]);
    skillspec_to_data(data+10+size_skillspec,c.m_skills[1]);
    skillspec_to_data(data+10+2*size_skillspec,c.m_skills[2]);
    skillspec_to_data(data+10+3*size_skillspec,c.m_skills_on_play[0]);
    skillspec_to_data(data+10+4*size_skillspec,c.m_skills_on_play[1]);
    skillspec_to_data(data+10+5*size_skillspec,c.m_skills_on_play[2]);
    skillspec_to_data(data+10+6*size_skillspec,c.m_skills_on_attacked[0]);
    skillspec_to_data(data+10+7*size_skillspec,c.m_skills_on_attacked[1]);
    skillspec_to_data(data+10+8*size_skillspec,c.m_skills_on_attacked[2]);
    skillspec_to_data(data+10+9*size_skillspec,c.m_skills_on_death[0]);
    skillspec_to_data(data+10+10*size_skillspec,c.m_skills_on_death[1]);
    skillspec_to_data(data+10+11*size_skillspec,c.m_skills_on_death[2]);
    for( int i = 0; i  < num_skills;++i) {
        data[i+10+12*size_skillspec] = c.m_skill_value[i];
        data[i+10+12*size_skillspec+num_skills] = c.m_skill_trigger[i];
    }
    data[10+12*size_skillspec+2*num_skills] = c.m_type;
    data[10+12*size_skillspec+2*num_skills+1] = c.m_category;

}


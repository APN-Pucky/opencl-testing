#include "card.h"
void card_from_data(struct Card* pc,__global int* all_cards, const int size_all_cards, int id) {
    for(__global int* i= all_cards; i < all_cards+size_all_cards;i+=size_card) {
        if(*(i+5)==id) {
            for(int j =0; j<size_card;++j) {
                ((int*)(pc))[j] = *(i+j);
            }
            return;
        }

    }
}

void card_to_data(int* data, const struct Card c) {
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

void print_card(const struct Card c) {
    printf("ID: %i " , c.m_id);
    printf("ATK: %i " , c.m_attack);
    printf("DEF: %i " , c.m_health);
    printf("\n");
}

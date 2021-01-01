#include "card.h"
#ifndef _OpenCL
#include "../../tuo/h/ocl_load.h"
#endif
#include "ocl_debug.h"

int attack(__global int* card) {
	return card[1];
}
int hp(__global int* card) {
	return card[0];
}

__global int* card(__global int* all_cards, int id) {
	return all_cards+id*size_card;
}

struct CLCardStatus cardstatus(__global int* all_cards, int id) {
	struct CLCardStatus ret;
	ret.m_hp =hp( card(all_cards,id));
	ret.m_attack =attack( card(all_cards,id));
	return ret;
}
void card_from_data(struct CLCard* pc,__global int* all_cards, const int size_all_cards, int id) {
    for(__global int* i= all_cards; i < all_cards+size_all_cards;i+=size_card) {
        if(*(i+5)==id) {
            for(int j =0; j<size_card;++j) {
                ((int*)(pc))[j] = *(i+j);
            }
            return;
        }

    }
}
void clear_cardstatus(struct CLCardStatus* pcs){
    pcs->m_card = 0;
    pcs->m_index = 0;
    pcs->m_action_index = 0;
    pcs->m_player= 0;
    pcs->m_delay= 0;
    pcs->m_hp = 0;
    pcs->m_absorption = 0;
    //pcs->m_step
    pcs->m_perm_health_buff = 0;
    pcs->m_perm_attack_buff = 0;
    pcs->m_temp_attack_buff = 0;
    pcs->m_corroded_rate = 0;
    pcs->m_corroded_weakened= 0;
    pcs->m_subdued= 0;
    pcs->m_enfeebled= 0;
    pcs->m_evaded= 0;
    pcs->m_inhibited= 0;
    pcs->m_sabotaged= 0;
    pcs->m_jammed= false;
    pcs->m_overloaded= false;
    pcs->m_paybacked= 0;
    pcs->m_tributed= 0;
    pcs->m_poisoned= 0;
    pcs->m_protected= 0;
    pcs->m_protected_stasis= 0;
    pcs->m_enraged= 0;
    pcs->m_entrapped= 0;
    pcs->m_marked= 0;
    pcs->m_diseased= 0;
    pcs->m_rush_attempted= false;
    pcs->m_sundered= false;
    pcs->m_summoned= false;
}
void cardstatus_from_card(struct CLCardStatus* pcs,struct CLCard* pc){
    pcs->m_card = pc;
    pcs->m_index = 0;
    pcs->m_action_index = 0;
    pcs->m_player= 0;
    pcs->m_delay= pc->m_delay;
    pcs->m_hp = pc->m_health;
    pcs->m_absorption = 0;
    //pcs->m_step
    pcs->m_perm_health_buff = 0;
    pcs->m_perm_attack_buff = 0;
    pcs->m_temp_attack_buff = 0;
    pcs->m_attack = pc->m_attack;
    pcs->m_corroded_rate = 0;
    pcs->m_corroded_weakened= 0;
    pcs->m_subdued= 0;
    pcs->m_enfeebled= 0;
    pcs->m_evaded= 0;
    pcs->m_inhibited= 0;
    pcs->m_sabotaged= 0;
    pcs->m_jammed= false;
    pcs->m_overloaded= false;
    pcs->m_paybacked= 0;
    pcs->m_tributed= 0;
    pcs->m_poisoned= 0;
    pcs->m_protected= 0;
    pcs->m_protected_stasis= 0;
    pcs->m_enraged= 0;
    pcs->m_entrapped= 0;
    pcs->m_marked= 0;
    pcs->m_diseased= 0;
    pcs->m_rush_attempted= false;
    pcs->m_sundered= false;
    pcs->m_summoned= false;
}
void card_to_data(int* data, const struct CLCard c) {
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

void print_faction(enum CLFaction f) {
#ifdef OCL_DEBUG
    switch (f)
    {
    case bloodthirsty:
        printf("bloodthirsty");
        break;
    case xeno:
        printf("xeno");
        break;
    case imperial:
        printf("imperial");
        break;
    case righteous:
        printf("righteous");
        break;
    case raider:
        printf("raider");
        break;
    default:
        printf("undefined");
        break;
    }
#endif
}

void print_cardtype(enum CLCardType t) {
#ifdef OCL_DEBUG
    switch(t){
        case commander:
            printf("Commander");
            break;
        case assault:
            printf("Assault");
            break;
        case structure:
            printf("Structure");
            break;
        default:
            printf("undefined");
            break;
    }
#endif
}

void print_card(const struct CLCard* c) {
#ifdef OCL_DEBUG
    printf("[");
#ifndef _OpenCL
    printf("%s: ",name_by_id(c->m_id));
#else
    printf("|%i|: " , c->m_id);
#endif
    printf("%i/%i/%i ", c->m_attack,c->m_health,c->m_delay);
    print_faction(c->m_faction);
    printf("]");
#endif
}
void print_cardstatus(const struct CLCardStatus* c) {
#ifdef OCL_DEBUG
    printf("P%d ",c->m_player); 
    print_cardtype(c->m_card->m_type);
    if(c->m_card->m_type!=commander)printf(" %d",c->m_index);
    printf(" [");
#ifndef _OpenCL
    printf("%s ",name_by_id(c->m_card->m_id));
#else
    printf("|%i| " , c->m_card->m_id);
#endif
    if(c->m_card->m_type!=commander)printf("att:%i " , c->m_attack);
    printf("hp:%i" , c->m_hp);
    printf("]");
#endif
}

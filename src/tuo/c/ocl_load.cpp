
#include <cstring>
#include "cards.h"
#include "deck.h"
#include "tyrant.h"
#include "tyrant_optimize.h"
#include "read.h"
#include "xml.h"
#include "ocl_load.h"

const char* name_by_id(const unsigned card_id) {
    const auto cardIter = all_cards.cards_by_id.find(card_id);
    if (cardIter == all_cards.cards_by_id.end()) return nullptr;//"UnknownCard.id[" + to_string(card_id) + "]"); }
    return cardIter->second->m_name.c_str();
}
CLCard* load_all_cards(int * n) {
    init();
    std::string prefix("/home/apn/git/tyrant_optimize/");
	std::vector<std::string> fn_suffix_list{"",};

    all_cards = Cards();
	Decks decks;

	std::unordered_set<unsigned> allowed_candidates;
	std::unordered_set<unsigned> disallowed_candidates;
	std::unordered_set<unsigned> disallowed_recipes;

	std::unordered_map<std::string, std::string> bge_aliases;

	load_skills_set_xml(all_cards, prefix+"data/skills_set.xml", true);
	for (unsigned section = 1;
			load_cards_xml(all_cards, prefix+"data/cards_section_" + to_string(section) + ".xml", false);
			++ section);
	all_cards.organize();
	load_levels_xml(all_cards, prefix+"data/levels.xml", true);
	all_cards.fix_dominion_recipes();
	for (const auto & suffix: fn_suffix_list)
	{
		load_decks_xml(decks, all_cards, prefix+"data/missions" + suffix + ".xml", prefix+"data/raids" + suffix + ".xml", suffix.empty());
		load_recipes_xml(all_cards, prefix+"data/fusion_recipes_cj2" + suffix + ".xml", suffix.empty());
		read_card_abbrs(all_cards, prefix+"data/cardabbrs" + suffix + ".txt");
	}
	for (const auto & suffix: fn_suffix_list)
	{
		load_custom_decks(decks, all_cards, prefix+"data/customdecks" + suffix + ".txt");
		map_keys_to_set(read_custom_cards(all_cards, prefix+"data/allowed_candidates" + suffix + ".txt", false), allowed_candidates);
		map_keys_to_set(read_custom_cards(all_cards, prefix+"data/disallowed_candidates" + suffix + ".txt", false), disallowed_candidates);
		map_keys_to_set(read_custom_cards(all_cards, prefix+"data/disallowed_recipes" + suffix + ".txt", false), disallowed_recipes);
	}

    *n = all_cards.all_cards.size();
    CLCard* allclcards = new CLCard[*n];
    for(unsigned i =0; i  < *n;++i) {
        Card* c = all_cards.all_cards[i];
        copy_card( &allclcards[i],c);
    }
    return allclcards;
}

void copy_skill(CLSkillSpec* out,SkillSpec* in) {
    out->id = static_cast<CLSkill>(in->id);
    out->x = (in->x);
    out->y = static_cast<CLFaction>(in->y);
    out->n = (in->n);
    out->c = (in->c);
    out->s = static_cast<CLSkill>(in->s);
    out->s2 = static_cast<CLSkill>(in->s2);
    out->all= (in->all);
    out->card_id= (in->card_id);
}
void copy_card( CLCard* out,Card* in) {
    out->m_attack =  in->m_attack;
    out->m_base_id=  in->m_base_id;
    out->m_delay=    in->m_delay;
    out->m_faction= static_cast<CLFaction>(in->m_faction);
    out->m_health=   in->m_health;
    out->m_id=       in->m_id;
    out->m_level=    in->m_level;
    out->m_fusion_level= in->m_fusion_level;
    out->m_type = static_cast<CLCardType>(in->m_type);
    if(in->m_skills.size()>0)copy_skill(&out->m_skills[0],&in->m_skills[0]);
    if(in->m_skills.size()>1)copy_skill(&out->m_skills[1],&in->m_skills[1]);
    if(in->m_skills.size()>2)copy_skill(&out->m_skills[2],&in->m_skills[2]);

}


#include "../h/card.h"
#include "../../too/clh/card.h"
//TODO includes!

void copy_skill(CLSkillSpec* out,SkillSpec* in) ;
void copy_card( CLCard* out,Card* in) ;
const char* name_by_id(const unsigned card_id) ;
CLCard* load_all_cards(int * n); 
#include "sim_attack.h"
#include "ocl_debug.h"

void sim_attack_damage(struct TUOState* s,struct  CLCardStatus* atk,struct  CLCardStatus* def) {
#ifdef OCL_DEBUG
    print_cardstatus(atk);
    printf(" attacks ");
    print_cardstatus(def);
    printf(" for %d damage\n",atk->m_attack);
#endif
    def->m_hp -= atk->m_attack;
    //printf("|%d| deals %d attack damage to %d now at %d\n",atk->m_card->m_id,atk->m_attack,def->m_card->m_id, def->m_hp);
}

void sim_attack(struct TUOState* s,struct  CLCardStatus* cs) {
    //printf("|%d| attacks\n",cs->m_card->m_id);
    Player e = s->enemy[s->currentPlayer];
    if(sim_exists_assault_by_index(s,cs->m_index, e)){
        sim_attack_damage(s,cs,sim_get_assault_by_index(s,cs->m_index,e));
    }
    else {
        sim_attack_damage(s,cs,&s->sides[e].board.commander);
    }
}
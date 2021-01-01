#include "sim_attack.h"
#include "debug.h"

void sim_attack_damage(TUOState* s, CLCardStatus* atk, CLCardStatus* def) {
    print_cardstatus(atk);
    printf(" attacks ");
    print_cardstatus(def);
    printf(" for %d damage\n",atk->m_attack);
    def->m_hp -= atk->m_attack;
    //printf("|%d| deals %d attack damage to %d now at %d\n",atk->m_card->m_id,atk->m_attack,def->m_card->m_id, def->m_hp);
}

void sim_attack(TUOState* s, CLCardStatus* cs) {
    //printf("|%d| attacks\n",cs->m_card->m_id);
    if(sim_exists_assault_by_index(s,cs->m_index, enemy(s->currentPlayer))){
        sim_attack_damage(s,cs,sim_get_assault_by_index(s,cs->m_index,enemy(s->currentPlayer)));
    }
    else {
        sim_attack_damage(s,cs,&s->sides[enemy(s->currentPlayer)].board.commander);
    }
}
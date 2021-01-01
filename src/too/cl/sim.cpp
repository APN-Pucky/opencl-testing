#include "state.h"
#include "sim.h"
#include "sim_attack.h"
#include "debug.h"


bool sim_exists_assault_by_index(TUOState * s, int j,Player p) {
	if(j>=_size_assaults) debug_printf("ERR j > size");
    return s->sides[p].board.assaults_mask[j]!=-1;
}
CLCardStatus* sim_get_assault_by_index(TUOState * s, int j,Player p) {
	if(j>=_size_assaults) debug_printf("ERR j > size");
	if(s->sides[p].board.assaults_mask[j]==-1) debug_printf("ERR non existing assault");
	return &s->sides[p].board.assaults[s->sides[p].board.assaults_mask[j]];
}

void sim_remove_assault_by_index(TUOBoard* b,int j) {
	printf("REMOVED ");print_cardstatus(&b->assaults[b->assaults_mask[j]]);printf("\n");
    for(int k = j+1; k < _size_assaults-1;k++)
        b->assaults_mask[k-1]=b->assaults_mask[k];
    b->assaults_mask[_size_assaults-1]=-1;
}

void sim_remove_dead_cards(TUOState* s) {
    Player p = s->currentPlayer;
	for(int j =0; j<_size_assaults;j++) {
        if(sim_exists_assault_by_index(s,j,enemy(p))){
            if(sim_get_assault_by_index(s,j,enemy(p))->m_hp <= 0) {
                sim_remove_assault_by_index(&s->sides[enemy(p)].board, j);
                j--;
            }
        }
    }
}

void sim_step(TUOState* s) {
	//TODO add simulation logic here
	printf("== ROUND %d ==\n", s->round);
	Player p = s->currentPlayer;
	for(int j =0; j<_size_assaults;j++) {
		//printf("sim_step for %d at %d\n",p,j);
		if(sim_exists_assault_by_index(s,j,p)){
			sim_attack(s,sim_get_assault_by_index(s,j,p));
		}
	}
    sim_remove_dead_cards(s);
}
void sim_play_card(struct TUOBoard* board, CLCard* c) {
    if(c->m_type == assault) {
		int n = board->index_assaults;
		if(n >= _size_assaults) 
			debug_printf("ERR too many assaults");
		cardstatus_from_card(&board->assaults[n],c);
		int j=0;
		while(board->assaults_mask[j]!=-1)j++;
		board->assaults[n].m_index = j;
		board->assaults_mask[j] = n;
		board->index_assaults++;
	}
	if(c->m_type == structure) {
		int n = board->index_structures;
		if(n >= _size_structures) 
			debug_printf("ERR too many structures");
		cardstatus_from_card(&board->structures[n],c);
		int j=0;
		while(board->structures_mask[j]!=-1)j++;
		board->structures[n].m_index = j;
		board->structures_mask[j] = n;
		board->index_structures++;
	}
}
#include "state.h"
#include "sim.h"
#include "sim_attack.h"
#include "ocl_debug.h"


bool sim_exists_assault_by_index(struct TUOState * s, int j,Player p) {
#ifdef OCL_DEBUG
	if(j>=size_assaults) printf("ERR j > size");
#endif
    return s->sides[p].board.assaults_mask[j]!=-1;
}
struct CLCardStatus* sim_get_assault_by_index(struct TUOState * s, int j,Player p) {
#ifdef OCL_DEBUG
	if(j>=size_assaults) printf("ERR j > size");
	if(s->sides[p].board.assaults_mask[j]==-1) printf("ERR non existing assault");
#endif
	return &s->sides[p].board.assaults[s->sides[p].board.assaults_mask[j]];
}

void sim_remove_assault_by_index(struct TUOBoard* b,int j) {
#ifdef OCL_DEBUG
	printf("REMOVED ");print_cardstatus(&b->assaults[b->assaults_mask[j]]);
	printf("\n");
#endif
    for(int k = j+1; k < size_assaults-1;k++){
        b->assaults_mask[k-1]=b->assaults_mask[k];
		b->assaults[b->assaults_mask[k-1]].m_index=k-1;
	}
    b->assaults_mask[size_assaults-1]=-1;
}

void sim_remove_dead_cards(struct TUOState* s) {
    Player p = s->currentPlayer;
	Player e = s->enemy[p];
	for(int j =0; j<size_assaults;j++) {
        if(sim_exists_assault_by_index(s,j,e)){
            if(sim_get_assault_by_index(s,j,e)->m_hp <= 0) {
                sim_remove_assault_by_index(&s->sides[e].board, j);
                j--;
            }
        }
    }
}

void sim_step(struct TUOState* s) {
	//TODO add simulation logic here
	Player p = s->currentPlayer;
	for(int j =0; j<size_assaults;j++) {
		//printf("sim_step for %d at %d\n",p,j);
		if(sim_exists_assault_by_index(s,j,p)){
			sim_attack(s,sim_get_assault_by_index(s,j,p));
		}
	}
    sim_remove_dead_cards(s);
}
void sim_play_card(struct TUOBoard* board, struct CLCard* c) {
	//printf("Type : %d\n", c->m_type);
    if(c->m_type == assault) {
		int n = board->index_assaults;
#ifdef OCL_DEBUG
		if(n >= size_assaults) 
			printf("ERR too many assaults");
#endif
		cardstatus_from_card(&board->assaults[n],c);
		int j=0;
		while(board->assaults_mask[j]!=-1)j++;
		board->assaults[n].m_index = j;
		board->assaults[n].m_player = board->player;
		board->assaults_mask[j] = n;
		board->index_assaults++;
#ifdef OCL_DEBUG
		print_cardstatus(&board->commander); 
		printf(" plays Assault %d ",j); print_card(c);
		printf("\n");
#endif
	}
	if(c->m_type == structure) {
		int n = board->index_structures;

#ifdef OCL_DEBUG
		if(n >= size_structures) 
			printf("ERR too many structures");
#endif
		cardstatus_from_card(&board->structures[n],c);
		int j=0;
		while(board->structures_mask[j]!=-1)j++;
		board->structures[n].m_index = j;
		board->assaults[n].m_player = board->player;
		board->structures_mask[j] = n;
		board->index_structures++;

#ifdef OCL_DEBUG
		printf("P%d Commander ",board->player);print_cardstatus(&board->commander); 
		printf(" plays Structure %d ",j); print_card(c);
		printf("\n");
#endif

	}
}
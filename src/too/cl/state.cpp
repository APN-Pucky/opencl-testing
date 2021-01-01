#include "state.h"
#include "sim.h"

void state_DoApplyAction(TUOState* s, ActionType a) {
	Player p = s->currentPlayer;
	sim_play_card(&s->sides[p].board,&s->sides[p].deck.cards[s->sides[p].hand.shuffle_mask[s->sides[p].hand.index]]);
	sim_step(s);
	s->sides[p].hand.index++;
	state_NextPlayer(s);
}


Player enemy(Player p) {
	return (p+1)%2;
}

void state_NextPlayer(TUOState* s) {
	s->currentPlayer = enemy(s->currentPlayer);
	s->round++;
}

bool state_IsTerminal(TUOState* s) {
	return  s->sides[0].board.commander.m_hp<=0 || s->sides[1].board.commander.m_hp<=0 || s->round > kMaxRounds;
}

int* state_Returns(TUOState* s) {
	if(s->round > kMaxRounds) {
		s->returns[0] = 0;
		s->returns[1] = 0;
	}
	else if(s->sides[0].board.commander.m_hp>0){
		s->returns[0] = 1;
		s->returns[1] = -1;
	}
	else if(s->sides[1].board.commander.m_hp>0){
		s->returns[0] = -1;
		s->returns[1] = 1;
	}
}

void state_Reset(struct TUOState* s) {
    s->currentPlayer = 0;
	s->isTerminal = false;
	s->returns[0] = 0;
	s->returns[1] = 0;
	s->round = 1;
	for(Player p= 0; p < 2; p++) {
		s->sides[p].hand.index=0;
		for(int i =0; i  < size_deck-2;++i) {
			s->sides[p].hand.shuffle_mask[i]=i;
		}
		s->sides[p].board.index_assaults = 0;
		s->sides[p].board.index_structures= 0;

		//Already set Commander CardStatus
		cardstatus_from_card(&s->sides[p].board.commander,&s->sides[p].deck.commander);

		for(int i =0; i  < _size_assaults;++i) {
			//clear_cardstatus(&s->sides[p].board.assaults[i]);
			s->sides[p].board.assaults_mask[i] = -1;
		}
		for(int i =0; i  < _size_structures;++i) {
			//clear_cardstatus(&s->sides[p].board.structures[i]);
			s->sides[p].board.structures_mask[i] = -1;
		}
	}
}
void state_sim(TUOState* state)	 {
	state_Reset(state);

	while(!state_IsTerminal(state)) {
		ActionType action;
		action = kPlayFirst;
		// action = cur_agent.step
		state_DoApplyAction(state,action);
	}
	state_Returns(state);
}

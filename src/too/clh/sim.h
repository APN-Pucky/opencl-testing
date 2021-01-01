#ifndef SIM_H_INCLUDED
#define SIM_H_INCLUDED
#include "state.h"

bool sim_exists_assault_by_index(TUOState * s, int j,Player p);
CLCardStatus* sim_get_assault_by_index(TUOState * s, int j,Player p);
void sim_step(TUOState* s);
void sim_play_card(TUOBoard* b, CLCard* c);
#endif
#ifndef SIM_H_INCLUDED
#define SIM_H_INCLUDED
#include "state.h"

bool sim_exists_assault_by_index(struct TUOState * s, int j,Player p);
struct CLCardStatus* sim_get_assault_by_index(struct TUOState * s, int j,Player p);
void sim_step(struct TUOState* s);
void sim_play_card(struct TUOBoard* b, struct CLCard* c);
#endif
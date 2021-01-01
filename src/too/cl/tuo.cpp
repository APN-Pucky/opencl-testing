
#include "ocl.h"
#include "ocl_debug.h"
#include "tuo.h"
#include "constants.h"
#include "rng.h"
#include "tyrant.h"
#include "step.h"
#include "card.h"
#include "state.h"



//__kernel void noop() {}
__kernel void simulate_test(  __global int* all_cards, const int count, const int gseed)
{
	int id = get_global_id(0);	
}


__kernel void simulate( const int size_all_cards, __global int* g_all_cards, const int size_gmydeck, __global int* g_mydeck, const int size_genemydeck, __global int* g_enemydeck, const int size_gwinner,__global int* g_winner, const int count, const int g_seed) 
{
	int id = get_global_id(0);	
	unsigned long seed = g_seed + id;

	
        
	if(id < count) {
		struct TUOState state;
		//TODO reuse thread => do multiple sims with one thread to avoid multple card_from_data calls
		// Set deck cards
		for(Player p= 0; p < 2; p++) {
			card_from_data(&state.sides[p].deck.commander,g_all_cards,size_all_cards, p?g_enemydeck[0]:g_mydeck[0]);
			card_from_data(&state.sides[p].deck.alpha_dominion,g_all_cards,size_all_cards, p?g_enemydeck[1]:g_mydeck[1]);
			for( int i = 2; i  < size_gmydeck;++i) {
			 	card_from_data(&state.sides[p].deck.cards[i-2],g_all_cards,size_all_cards,p?g_enemydeck[i]: g_mydeck[i]);
				//if(id==0) {
#ifdef OCL_DEBUG
				print_card(&state.sides[p].deck.cards[i-2]);printf("\n");
#endif
				//}
			}
			state.sides[p].deck.num_cards = p?size_genemydeck-2:size_gmydeck-2;
		}
		//for(int i = 0; i  < 100; ++i)
			state_sim(&state);
		g_winner[id] = state.returns[0];	
	}
}

#include "ocl.h"
#include "tuo.h"
#include "constants.h"
#include "rng.h"
#include "tyrant.h"
#include "step.h"
#include "card.h"
int attack(__global int* card) {
	return card[1];
}
int hp(__global int* card) {
	return card[0];
}

__global int* card(__global int* all_cards, int id) {
	return all_cards+id*size_card;
}

struct CardStatus cardstatus(__global int* all_cards, int id) {
	struct CardStatus ret;
	ret.m_hp =hp( card(all_cards,id));
	ret.m_attack =attack( card(all_cards,id));
	return ret;
}


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
		struct Card mydeck[10];
		struct Card enemydeck[10];
		for( int i = 0; i  < size_deck;++i) {
		 	card_from_data(&mydeck[i],g_all_cards,size_all_cards, g_mydeck[i]);
		 	card_from_data(&enemydeck[i],g_all_cards,size_all_cards, g_enemydeck[i]);
			if(id==0) {
				print_card(mydeck[i]);
				print_card(enemydeck[i]);
			}
		}
		g_winner[id] = mydeck[0].m_health<enemydeck[0].m_attack? 1:-1;	
		//g_winner[id] = hp(card(all_cards,mydeck[0]))>hp(card(all_cards,enemydeck[0]))? 1:-1;	
		//*/
	}
}
#ifndef _OpenCL
#define __global
#define __constant
#define __kernel
#define get_global_id
#define ulong unsigned long
#define false 0
#define true 1
#define bool int
#endif

#include "0_constants.h"
#include "1_rng.h"
#include "1_tyrant.h"
#include "2_card.h"

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


__kernel void simulate_test(  __global int* all_cards, const unsigned int count, const int gseed)
{
	int id = get_global_id(0);	
}


__kernel void simulate( __global int* all_cards, const unsigned int size_all_cards, __global int* gmydeck,__global int* genemydeck, __global int* g_winner, const unsigned int count, const int gseed) 
{
	int id = get_global_id(0);	
	ulong seed = gseed + id;

	
        
	if(id < count) {
		struct Card mydeck[10];
		struct Card enemydeck[10];
		for( int i = 0; i  < size_deck;++i) {
		 	mydeck[i]= card_from_data(all_cards,size_all_cards, gmydeck[i]);
		 	enemydeck[i]= card_from_data(all_cards,size_all_cards, genemydeck[i]);
		}
		g_winner[id] = mydeck[0].m_health>enemydeck[0].m_attack? 1:-1;	
		//g_winner[id] = hp(card(all_cards,mydeck[0]))>hp(card(all_cards,enemydeck[0]))? 1:-1;	
	}
}

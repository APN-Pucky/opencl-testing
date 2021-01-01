#include "catch.hpp"
#include "state.h"
#include "tyrant.h"
void set_commander(CLCard* c,unsigned id,unsigned health) {
    c->m_base_id = id;
    c->m_id = id;
    c->m_health = health;
    c->m_type = commander;
}
void set_assault(CLCard* c,unsigned id,unsigned health,unsigned attack) {
    c->m_base_id = id;
    c->m_id = id;
    c->m_health = health;
    c->m_attack= attack;
    c->m_type = assault;
}
TEST_CASE( "commander vs commander", "[CvC]") {
    TUOState s;
    CLCard* c1 = &s.sides[0].deck.commander;
    CLCard* c2 = &s.sides[1].deck.commander;
    set_commander(c1,1,5);
    set_commander(c2,2,15);
    s.sides[0].deck.num_cards = 0;
    s.sides[1].deck.num_cards = 0;

    state_sim(&s);
    REQUIRE (s.returns[0] == 0);
}
TEST_CASE( "assault vs commander" , "[AvC]") {
    TUOState s;
    CLCard* c1 = &s.sides[0].deck.commander;
    CLCard* c = &s.sides[0].deck.cards[0];
    CLCard* c2 = &s.sides[1].deck.commander;
    set_commander(c1,1,5);
    set_commander(c2,2,15);
    set_assault(c,3,1,1);
    s.sides[0].deck.num_cards = 1;
    s.sides[1].deck.num_cards = 0;

    state_sim(&s);
    REQUIRE (s.returns[0] == 1);
}
TEST_CASE( "assault vs assault" , "[AvA]") {
    TUOState s;
    CLCard* c0 = &s.sides[0].deck.commander;
    CLCard* c0c0 = &s.sides[0].deck.cards[0];
    CLCard* c1 = &s.sides[1].deck.commander;
    CLCard* c1c0 = &s.sides[1].deck.cards[0];
    set_commander(c0,1,5);
    set_commander(c1,2,15);
    set_assault(c0c0,3,1,1);
    set_assault(c1c0,4,2,2);
    s.sides[0].deck.num_cards = 1;
    s.sides[1].deck.num_cards = 1;

    state_sim(&s);
    REQUIRE (s.returns[0] == -1);
}

/*
TEST_CASE( "test sim run and return", "[cpu]" ) {
    const unsigned int size_card=10+4*3*9+num_skills*2+2;
    const unsigned int size_all_cards = 2*size_card;
    const unsigned int size_deck=10;
    const unsigned int sims = 1024;
    const unsigned int N = sims;
    const unsigned long seed = 5;

    int mydeck[size_deck];
    int enemydeck[size_deck];
    int allcards[size_all_cards];
    for( int i =0; i < size_all_cards;++i) {
        allcards[i] = 0;
    }
    allcards[0] = 2;
    allcards[1] = 2;
    allcards[2] = 2;
    allcards[4] = 2;
    allcards[5] = 2;

    allcards[size_card +0] = 1;
    allcards[size_card +1] = 1;
    allcards[size_card +2] = 1;
    allcards[size_card +4] = 1;
    allcards[size_card +5] = 1;

    int results[N];

    for(int i =0; i  < size_deck ;++i) {
	    mydeck[i] = 1;
	    enemydeck[i] = 2;
    }
    for(int i =0; i  < N; ++i) {
	    results[i] = -2;
    }

    auto r = Runner<int,int*,int,int*,int,int*,int,int*,int,int>("simulate",simulate,N,size_all_cards,allcards,size_deck,mydeck,size_deck,enemydeck,N,results,N,seed);
    r.set_mem<1>(CL_MEM_READ_ONLY,size_all_cards,false);
    r.set_mem<3>(CL_MEM_READ_ONLY,size_deck,false);
    r.set_mem<5>(CL_MEM_READ_ONLY,size_deck,false);
    r.set_mem<7>(CL_MEM_WRITE_ONLY,N,true);
    //r.run_opencl(N);
    //for (int i = 0; i  < N ; ++i) {
	   // printf("%d",results[i]);
    //}

    global_ids[0] = 0;
    r.run_cpu();
    printf("\nsingle run %d\n",results[0]);
    REQUIRE (results[0] >= 0);




    //simulate(allcards,size_all_cards,mydeck,enemydeck,results,N,seed);
	//printf("\nsingle run %d\n",results[0]);
}
//*/
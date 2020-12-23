#include "clew.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
//#include "tyrant.h"
#include "runner.h"
#include "tuo.h"
#include "test.h"
#include "clh/card.h"
#include "clh/step.h"

#define MALLOC_CHECK_ 2

int runX() {
    //const unsigned int size_card=10+4*3*size_skillspec+Skill::num_skills*2+2;
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

    r.run_opencl();
    //r.run_noparallel();
    for (int i = 0; i  < N ; ++i) {
	    printf("%d",results[i]);
    }

    //r.run_cpu();
    printf("\nsingle run %d\n",results[0]);




    //simulate(allcards,size_all_cards,mydeck,enemydeck,results,N,seed);
	//printf("\nsingle run %d\n",results[0]);

    //r.~Runner();
    return 0;
}

int run() {
    return runX();
}
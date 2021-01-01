#ifndef CLDECK_H_INCLUDED
#define CLDECK_H_INCLUDED

#include "card.h"

const __constant int size_deck= 12;
const __constant int size_cards= 10;

struct CLDeck {
    CLCard commander;
    CLCard alpha_dominion;
    //CLCard fortress_cards[4];
    CLCard cards[size_cards];
    unsigned num_cards =10;
};

#endif
#ifndef CLDECK_H_INCLUDED
#define CLDECK_H_INCLUDED

#include "card.h"

const __constant int size_deck= 12;
#define SIZE_CARDS 10
const __constant int size_cards= SIZE_CARDS;

struct CLDeck {
    struct CLCard commander;
    struct CLCard alpha_dominion;
    //struct CLCard fortress_cards[4];
    struct CLCard cards[SIZE_CARDS];
    unsigned num_cards;
};

#endif
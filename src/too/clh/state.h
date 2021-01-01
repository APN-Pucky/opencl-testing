#ifndef CLSTATE_H_INCLUDED
#define CLSTATE_H_INCLUDED

#include "deck.h"
#include "tyrant.h"

#define Action int

const __constant int kDefaultPlayers = 2;
const __constant int kMaxRounds = 50;
#define SIZE_ASSAULTS 20
const __constant int size_assaults= SIZE_ASSAULTS;
#define SIZE_STRUCTURES 20
const __constant int size_structures= SIZE_STRUCTURES;

struct TUOState;

struct TUOHand {
    int shuffle_mask[SIZE_CARDS];
    unsigned index;
};

struct TUOBoard {
    struct TUOState* state;
    Player player;
    struct CLCardStatus commander;
    struct CLCardStatus assaults[SIZE_ASSAULTS];
    int assaults_mask[SIZE_ASSAULTS];
    unsigned index_assaults;
    struct CLCardStatus structures[SIZE_STRUCTURES];
    int structures_mask[SIZE_STRUCTURES];
    unsigned index_structures;
};

struct TUOSide {
    //Player p;
    struct TUOHand hand;
    struct CLDeck deck;    
    struct TUOBoard board;
    unsigned stasis_faction_bitmap;
    //unsigned total_cards_destroyed;
    //unsigned total_nonsummon_cards_destroyed;
};

struct TUOState { 
    Player currentPlayer;
    Player enemy[2];
    bool isTerminal;
    int returns[2];
    int round;
    struct TUOSide sides[2];
};


enum ActionType {kPlayFirst,kPlaySecond,kPlayThird};

void state_DoApplyAction(struct TUOState* state, enum ActionType a);
void state_NextPlayer(struct TUOState* s);
void state_Reset(struct TUOState* state);
//Player state_CurrentPlayer() {}
bool state_IsTerminal(struct TUOState* s);
int* state_Returns(struct TUOState *s);
//bool state_hasWon(Player p){}

void state_sim(struct TUOState* state);

Player enemy(Player p);
#endif
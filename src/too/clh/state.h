#ifndef CLSTATE_H_INCLUDED
#define CLSTATE_H_INCLUDED

#include "deck.h"
#include "tyrant.h"

#define Action int

const __constant int kDefaultPlayers = 2;
const __constant int kMaxRounds = 50;
const __constant int _size_assaults= 20;
const __constant int _size_structures= 20;
struct TUOState;

struct TUOHand {
    int shuffle_mask[size_cards];
    unsigned index;
};

struct TUOBoard {
    TUOState* state;
    Player player;
    CLCardStatus commander;
    CLCardStatus assaults[_size_assaults];
    int assaults_mask[_size_assaults];
    unsigned index_assaults;
    CLCardStatus structures[_size_structures];
    int structures_mask[_size_structures];
    unsigned index_structures;
};

struct TUOSide {
    //Player p;
    TUOHand hand;
    CLDeck deck;    
    TUOBoard board;
    unsigned stasis_faction_bitmap;
    //unsigned total_cards_destroyed;
    //unsigned total_nonsummon_cards_destroyed;
};

struct TUOState { 
    Player currentPlayer;
    bool isTerminal;
    int returns[2];
    int round;
    TUOSide sides[2];
};


enum ActionType {kPlayFirst,kPlaySecond,kPlayThird};

void state_DoApplyAction(struct TUOState* state, ActionType a);
void state_NextPlayer(struct TUOState* s);
void state_Reset(struct TUOState* state);
//Player state_CurrentPlayer() {}
bool state_IsTerminal(struct TUOState* s);
int* state_Returns(struct TUOState *s);
//bool state_hasWon(Player p){}

void state_sim(TUOState* state);

Player enemy(Player p);
#endif
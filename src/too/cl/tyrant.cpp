#include "tyrant.h"
void skillspec_to_data(int* data,struct CLSkillSpec ss) {
    data[0] = ss.id;
    data[1] = ss.x;
    data[2] = ss.y;
    data[3] = ss.n;
    data[4] = ss.c;
    data[5] = ss.s;
    data[6] = ss.s2;
    data[7] = ss.all;
    data[8] = ss.card_id;
}
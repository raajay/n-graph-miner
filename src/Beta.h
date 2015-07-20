//
// Created by raajay on 7/19/15.
//

#ifndef N_GRAPH_MINER_BETA_H
#define N_GRAPH_MINER_BETA_H
#include "Automaton.h"
#include "Alpha.h"
class beta
{
public:
    alpha* ptrAlpha;
    automaton* ptrAuto;
    //bool* q;
    int j;
    beta *pPrev,*pNext;
    beta(int len);
    ~beta();
};

#endif //N_GRAPH_MINER_BETA_H

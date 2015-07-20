//
// Created by raajay on 7/20/15.
//

#ifndef N_GRAPH_MINER_STEPISODEGEL_H
#define N_GRAPH_MINER_STEPISODEGEL_H

#include "Alpha.h"

class stEpisodeGel
{
public:
    int iEpisodeListLength;
    int iEventLength;
    alpha* ptrAlpha;
    alpha* pstTail;
    void filter();
    void ent_filter(double);
    stEpisodeGel();
};

#endif //N_GRAPH_MINER_STEPISODEGEL_H

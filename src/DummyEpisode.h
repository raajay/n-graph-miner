//
// Created by raajay on 7/20/15.
//

#ifndef N_GRAPH_MINER_DUMMYEPISODE_H
#define N_GRAPH_MINER_DUMMYEPISODE_H
class dummyEpisode {
  public:
    int* paiEvents;
    bool** ppEventMatrix;
    dummyEpisode(int);
    void deleteMat(int);
    ~dummyEpisode();
};
#endif //N_GRAPH_MINER_DUMMYEPISODE_H

//
// Created by raajay on 7/19/15.
//

#ifndef N_GRAPH_MINER_ALPHA_H
#define N_GRAPH_MINER_ALPHA_H
#include "Automaton.h"
#include "super.h"
class alpha {
  public:
    bool flag;
    bool *initWaits;
    long int iFrequency;
    int* paiEvents;
    int** ppEntropyMatrix;
    bool** ppEventMatrix;
    automaton* automataList;
    double expiryTime;
    alpha* pstBlockStart;
    int blocStart;
    alpha* pNext;
    double calculate_entropy(int len);
    double entropy;
    void initialize(int);
    void deleteMat(int);
    void display(int);
    void display(fstream&,int);
    void transredMatDisp(fstream& out,int len);
    alpha(int);
    ~alpha();
};

#endif //N_GRAPH_MINER_ALPHA_H

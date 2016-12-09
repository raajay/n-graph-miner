//
// Created by raajay on 7/19/15.
//

#ifndef N_GRAPH_MINER_AUTOMATON_H
#define N_GRAPH_MINER_AUTOMATON_H
class beta;
class automaton {
  public:
    bool* q;
    bool* w;
    double t1;
    bool* flag;
    bool** trackMat;
    beta** betaAddress;
    automaton* pNext;
    void deleteMat(int len);
    void updateMat(int len,bool* q, int j);
    void copyMat(int len, bool **mat);
    automaton(int len);
    ~automaton();
};
#endif //N_GRAPH_MINER_AUTOMATON_H

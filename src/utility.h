//
// Created by raajay on 7/19/15.
//

#ifndef N_GRAPH_MINER_UTILITY_H
#define N_GRAPH_MINER_UTILITY_H

#include "Alpha.h"
#include "Beta.h"
#include "Automaton.h"

void errorAbort(string);
void errorEcho(string);
void displayVec(bool*,int);
void transred(bool**,int);
int maxDegree(bool**,int);
int lengthMaxPath(bool**,int);
int numMaxPath(bool**,int);
int pathFinder2(int,bool**,int*,int);
int pathFinder(int,bool**,int*,int);
int compare_containment(bool**,bool**,int);
int compare_event(int*,int*,int);
bool chkEvent(int*,int,int);
void initW(bool*,bool**,int);
void relinkDelBeta(beta*&,beta*);
void addAutoAtHead(alpha*, automaton*);
void addBetaAtHead(beta*&, beta*);
bool compareQ(bool*,bool*,int);
void assignVec(bool*,bool*,int);
void assignQNull(bool*,int);
bool checkQNull(bool*,int);
bool checkQallOne(bool*,int);
bool checkVecSame(bool*,bool*,int);
void addMatrix(int**,bool**,int);
#endif //N_GRAPH_MINER_UTILITY_H

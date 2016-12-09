//
// Created by raajay on 7/19/15.
//

#include "super.h"
#include "Automaton.h"

automaton::automaton(int len) {
    pNext = NULL;
    t1 = 0.0;
    q = new bool[len];
    w = new bool[len];
    flag = new bool[len];
    betaAddress = new beta*[len];
    trackMat = new bool*[len];
    for(int i=0; i<len; i++) {
        trackMat[i] = new bool[len];
    }
    for(int i=0; i<len; i++)
        for(int j=0; j<len; j++)
            trackMat[i][j] = false;
    for(int i=0; i<len; i++) {
        q[i] = 0;
        w[i] = 0;
        betaAddress[i] = NULL;
        flag[i] = false;
    }
}

automaton::~automaton() {
    delete []q;
    delete []w;
    delete []flag;
    delete []betaAddress;
}

void automaton::deleteMat(int len) {
    for(int i=0; i<len; i++) {
        delete[]trackMat[i];
    }
    delete []trackMat;
}



void automaton::updateMat(int len,bool* q,int j) {
    for(int i =0; i<len; i++)
        if(q[i] == false)
            trackMat[j][i] = true;
}

void automaton::copyMat(int len,bool** mat) {
    for(int i=0; i<len; i++)
        for(int j=0; j<len; j++)
            trackMat[i][j] = mat[i][j];
}

//
// Created by raajay on 7/20/15.
//

#include "DummyEpisode.h"
dummyEpisode::dummyEpisode(int len) {
    paiEvents = new int[len];
    ppEventMatrix = new bool*[len];
    for(int i=0; i<len; i++) {
        paiEvents[i] = 0;
        ppEventMatrix[i] = new bool[len];
    }
    for(int i=0; i<len; i++)
        for(int j=0; j<len; j++)
            ppEventMatrix[i][j] =0;

}


void dummyEpisode::deleteMat(int len) {
    for(int i=0; i<len; i++)
        delete []ppEventMatrix[i];
}

dummyEpisode::~dummyEpisode() {
    delete []paiEvents;
    delete []ppEventMatrix;
}
//
// Created by raajay on 7/19/15.
//

#ifndef N_GRAPH_MINER_FREQUENTEPISODES_H
#define N_GRAPH_MINER_FREQUENTEPISODES_H
#include "Beta.h"
#include "Alpha.h"
#include "Automaton.h"
#include "StEpisodeGel.h"


class FrequentEpisodes
{
    int frequencyThreshold;
    int maxSiz,maxLength,maxWidth;
    int maxEvents;
    double expTime,fudgeFac, bidirectionalEvidenceThreshold;
    int episodeType;
    char filename[100];
    string outfile;
    //class alpha;
    //class automaton;

    vector <stEpisodeGel> GelHeads;

public:

    int addCandidateEpisode(stEpisodeGel& Head, alpha *pstNode, alpha **pstBlockStart,const int iFrequency);
    int existSubEpisode(alpha*,int*,bool**,int);
    void generatePossibleCandidates(alpha*,alpha*,alpha**,int);
    alpha* catEpisodes(alpha*,alpha*,int);
    void cleanupWaits(beta**);
    void cleanupAutoList(alpha*,int);
    void initCandidates(stEpisodeGel&);
    void generateCandidates(stEpisodeGel,stEpisodeGel&);
    void findFreqCandids(stEpisodeGel&);
    void findFreqEpisodes();
    void displayFreqEpisodes(char*);
    void displayLevelWise(fstream&,int);
    void parser(string);
    void loadCandidate(alpha*,int,string);
    void filter();
    void filter(int);
    FrequentEpisodes(double expTime,int tresh,double entTresh,int maxLn1,int maxWid1,double fugeFac1,int maxSiz,int maxEvents,char*,string,int);
};

#endif //N_GRAPH_MINER_FREQUENTEPISODES_H

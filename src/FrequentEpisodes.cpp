//
// Created by raajay on 7/19/15.
//
#include "super.h"
#include "FrequentEpisodes.h"
#include "DummyEpisode.h"
#include "utility.h"
#include "StreamReader.h"
#define MAX_LONGINT 10000000
#define DEBUG 0

FrequentEpisodes::FrequentEpisodes(double expTime1,int tresh1,double entTresh1,int maxLen1,int maxWid1,double fudgeFac1,int maxSiz1,int maxEvents1, char* file,string outfile1,int epi1) {
    expTime = expTime1;
    //cout << "Expiry Time = "<<expTime;
    tresh = tresh1;
    entTresh = entTresh1;
    maxLength = maxLen1;
    maxWidth = maxWid1;
    maxSiz = maxSiz1;
    maxEvents = maxEvents1;
    fudgeFac = fudgeFac1;
    episodeType = epi1;
    outfile = outfile1;
    char* f = filename;
    while(*f++ = *file++);
}

void FrequentEpisodes::generateCandidates(stEpisodeGel prevHead,stEpisodeGel& Head) {
    int l = prevHead.iEventLength;
    for(alpha* mainIter = prevHead.ptrAlpha; mainIter!=NULL; mainIter = mainIter->pNext) {
        alpha* pstBlockStart = NULL;
        for(alpha* blockIter=mainIter->pNext; blockIter!=NULL && blockIter->pstBlockStart == mainIter->pstBlockStart; blockIter=blockIter->pNext) {
            alpha* (posEpisodes[3]) = {NULL};
            if(mainIter->paiEvents[l-1] == blockIter->paiEvents[l-1])
                continue;
            generatePossibleCandidates(mainIter,blockIter,posEpisodes,l);
            for(int iTemp=0; iTemp<3 && posEpisodes[iTemp] != NULL; iTemp++) {
                bool addFlag = true;
                dummyEpisode* ptrDummy = new dummyEpisode(l);
                for(int jTemp = 0; (jTemp<l-1) && (addFlag == true); jTemp++) {

#if 1
                    for(int kTemp=0; kTemp<jTemp; kTemp++) {
                        ptrDummy->paiEvents[kTemp] = posEpisodes[iTemp]->paiEvents[kTemp];
                        for(int zTemp =0; zTemp<jTemp; zTemp++) {
                            ptrDummy->ppEventMatrix[kTemp][zTemp] = posEpisodes[iTemp]->ppEventMatrix[kTemp][zTemp];
                        }
                        for(int zTemp =jTemp; zTemp<l; zTemp++) { // CCheck the condition
                            ptrDummy->ppEventMatrix[kTemp][zTemp] =  posEpisodes[iTemp]->ppEventMatrix[kTemp][zTemp+1];
                        }
                    }
                    for(int kTemp=jTemp; kTemp<l; kTemp++) {
                        ptrDummy->paiEvents[kTemp] = posEpisodes[iTemp]->paiEvents[kTemp+1];
                        for(int zTemp =0; zTemp<jTemp; zTemp++) {
                            ptrDummy->ppEventMatrix[kTemp][zTemp] = posEpisodes[iTemp]->ppEventMatrix[kTemp+1][zTemp];
                        }
                        for(int zTemp =jTemp; zTemp<l; zTemp++) { // CCheck the condition
                            ptrDummy->ppEventMatrix[kTemp][zTemp] = posEpisodes[iTemp]->ppEventMatrix[kTemp+1][zTemp+1];
                        }
                    }

#endif
                    if( -1  == existSubEpisode(prevHead.ptrAlpha,ptrDummy->paiEvents,ptrDummy->ppEventMatrix,l)) {
                        addFlag = false;
                    }

                }

                if(true == addFlag && lengthMaxPath(posEpisodes[iTemp]->ppEventMatrix,l+1) <= maxLength
                        && numMaxPath(posEpisodes[iTemp]->ppEventMatrix,l+1) <= maxWidth)
                    //if(true == addFlag)
                {
                    if(( addCandidateEpisode(Head,posEpisodes[iTemp],&pstBlockStart,0)) == -1) {
                        cout <<" Out of Memory - Higher Threshold"<<endl;
                        exit(0);
                    }
                } else {
                    posEpisodes[iTemp]->deleteMat(l+1);
                    delete posEpisodes[iTemp];
                }
                ptrDummy->deleteMat(l);
                delete ptrDummy;

            }

        }
    }

}

void FrequentEpisodes::generatePossibleCandidates(alpha *pstEpi1,
        alpha *pstEpi2,
        alpha **pstTemp,
        //alpha *(pstTemp[3]),
        int iEventLen) {
    int iTemp =0;
    int iDirFlag =0;
    if(episodeType == 1) {
        pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
        return;
    }
    for(iTemp =0; iTemp<iEventLen-1; iTemp++) {
        if((1 == pstEpi1->ppEventMatrix[iEventLen-1][iTemp]) && (1 == pstEpi2->ppEventMatrix[iTemp][iEventLen-1])) {
            iDirFlag = 1;
        }
        if((1 == pstEpi2->ppEventMatrix[iEventLen-1][iTemp]) && (1 == pstEpi1->ppEventMatrix[iTemp][iEventLen-1])) {
            iDirFlag = 2;
        }
    }

    if(iDirFlag != 0) {
        pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
        if(1 == iDirFlag) {
            pstTemp[0]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
        } else {
            pstTemp[0]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
        }
        return;

    } else {
        int c = 0;
        if(episodeType != 2) {
            pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
            c++;
        }

        for(iTemp =0; iTemp<iEventLen-1; iTemp++) {
            if(((1 == pstEpi1->ppEventMatrix[iEventLen-1][iTemp]) && (0 == pstEpi2->ppEventMatrix[iEventLen-1][iTemp])) ||
                    ((0 == pstEpi1->ppEventMatrix[iTemp][iEventLen-1]) && (1 == pstEpi2->ppEventMatrix[iTemp][iEventLen-1]))) {
                if(iDirFlag != 2) {
                    iDirFlag = 1;
                } else {
                    return;
                }
            }

            if(((0 == pstEpi1->ppEventMatrix[iEventLen-1][iTemp]) && (1 == pstEpi2->ppEventMatrix[iEventLen-1][iTemp])) ||
                    ((1 == pstEpi1->ppEventMatrix[iTemp][iEventLen-1]) && (0 == pstEpi2->ppEventMatrix[iTemp][iEventLen-1]))) {
                if(iDirFlag != 1) {
                    iDirFlag = 2;
                } else {
                    return;
                }
            }

        }
        switch(iDirFlag) {
        case 0: {
            pstTemp[c] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
            pstTemp[c]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
            pstTemp[c+1] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
            pstTemp[c+1]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
        }
        break;
        case 1: {
            pstTemp[c] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
            pstTemp[c]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
        }
        break;
        case 2: {
            pstTemp[c] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
            pstTemp[c]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
        }
        break;

        }
    }
    return ;
}

alpha* FrequentEpisodes::catEpisodes(alpha *pstEpi1,alpha *pstEpi2,int iEventLen) {
    alpha* pstTemp = NULL;
    int iTemp =0,jTemp =0;

    pstTemp = new alpha(iEventLen+1);

    for(iTemp =0; iTemp<iEventLen; iTemp++) {
        pstTemp->paiEvents[iTemp] = pstEpi1->paiEvents[iTemp];

        for(jTemp =0; jTemp<iEventLen; jTemp++) {
            pstTemp->ppEventMatrix[iTemp][jTemp] = pstEpi1->ppEventMatrix[iTemp][jTemp];
        }
    }

    pstTemp->paiEvents[iEventLen] = pstEpi2->paiEvents[iEventLen-1];
    for(jTemp =0; jTemp<iEventLen; jTemp++) {
        pstTemp->ppEventMatrix[iEventLen][jTemp] = pstEpi2->ppEventMatrix[iEventLen-1][jTemp];
        pstTemp->ppEventMatrix[jTemp][iEventLen] = pstEpi2->ppEventMatrix[jTemp][iEventLen-1];
    }
    pstTemp->ppEventMatrix[iEventLen][iEventLen] =0;
    return pstTemp;
}

int FrequentEpisodes::existSubEpisode(alpha *pstFreqEpisodeList,int *paiEvent,bool **ppaiEventMatrix,int iLen) {
    int iTemp =0,jTemp =0;
    int iCount =0;
    int izTemp =0;
    alpha *pstFreqEpisodeListIter = pstFreqEpisodeList;

    for(; pstFreqEpisodeListIter!= NULL; pstFreqEpisodeListIter = pstFreqEpisodeListIter->pNext) {
        izTemp++;
    }


    while(pstFreqEpisodeList!= NULL) {
        iCount =0;
        izTemp++;
        for(iTemp = iLen-1; iTemp >=0  ; iTemp--) {
            if(paiEvent[iTemp] != pstFreqEpisodeList->paiEvents[iTemp]) {
                break;
            }
            iCount++;
        }
        if(iCount == iLen) {
            for(iTemp = iLen-1; iTemp >=0  ; iTemp--) {
                for(jTemp = iLen-1; jTemp >=0  ; jTemp--) {
                    if(ppaiEventMatrix[iTemp][jTemp] != pstFreqEpisodeList->ppEventMatrix[iTemp][jTemp]) {
                        goto LABEL;
                    }
                }

            }
            return 0;
        }
LABEL :
        pstFreqEpisodeList = pstFreqEpisodeList->pNext;

    }



    return -1;
}
int FrequentEpisodes::addCandidateEpisode(stEpisodeGel& Head, alpha *pstNode, alpha **pstBlockStart,
        const int iFrequency) {


    alpha *pstIter = NULL;
    int iEventLen =Head.iEventLength;
    int iTemp =0,jTemp =0;



    if(MAX_LONGINT == Head.iEpisodeListLength) {
        cout<<"\nCandidate Epiosdes of Length : "<<Head.iEventLength <<" exceeds MAX"<<endl;
        return -1;
    }

    pstNode->iFrequency = iFrequency;
    if(NULL == *pstBlockStart) {
        *pstBlockStart = pstNode;
    }
    pstNode->pstBlockStart = *pstBlockStart;


    Head.iEpisodeListLength++;

    if(NULL == Head.ptrAlpha) {
        Head.ptrAlpha = pstNode;
        Head.pstTail = pstNode;
        return 0;
    }
    Head.pstTail->pNext = pstNode;
    Head.pstTail = pstNode;
    return 0;
}



void FrequentEpisodes::findFreqEpisodes() {
    fstream fout;
    fout.open(outfile.c_str(),ios::out);
    // open file for displaying output
    stEpisodeGel oneCandid;
    oneCandid.iEventLength = 1;
    initCandidates(oneCandid);
    GelHeads.push_back(oneCandid);
    findFreqCandids(GelHeads[0]);
    displayLevelWise(fout,0);
    for(int i=1; i<maxSiz; i++) {
        stEpisodeGel tempGelHead;
        tempGelHead.iEventLength = i+1;
        cout << "Generating "<<i+1<<"-Node Candidates..."<<endl;
        generateCandidates(GelHeads[i-1],tempGelHead);
        GelHeads.push_back(tempGelHead);
        findFreqCandids(GelHeads[i]);
        displayLevelWise(fout,i);
    }
    fout.close();
}
void FrequentEpisodes::initCandidates(stEpisodeGel& GelHead) {
    alpha* ptrPrev = NULL;
    for(int i=0; i<maxEvents; i++) {
        alpha *ptrTemp = new alpha(1);
        if(ptrTemp == NULL) errorAbort("in initCandidates :: out of memory");
        ptrTemp->paiEvents[0] = i+1;
        ptrTemp->ppEventMatrix[0][0] = 0;
        if(ptrPrev != NULL) {
            ptrPrev->pNext = ptrTemp;
            ptrTemp->pstBlockStart = GelHead.ptrAlpha;
        } else {
            GelHead.ptrAlpha = ptrTemp;
            ptrTemp->pstBlockStart = ptrTemp;
        }
        ptrPrev = ptrTemp;
    }
    GelHead.iEpisodeListLength = maxEvents;
    return;
}

void FrequentEpisodes::findFreqCandids(stEpisodeGel& GelHead) {
    cout<<"Number of "<<GelHead.iEventLength<<"-node candidates being counted = "<<GelHead.iEpisodeListLength<<endl;
    cout << "Frequency treshhold = " << tresh << endl;
    // Getting the event stream
    StreamReader evntStream(filename);
    // Initializing waitslist.. declaring the waitslist struc and adding the initwaits of all candidates
    // and the null automaton in the alpha.init list
    beta* waitsList[maxEvents];
    int eLen = GelHead.iEventLength;
    for(int i=0; i<maxEvents; i++)
        waitsList[i] = NULL;
    for(alpha* ptrIter1 = GelHead.ptrAlpha; ptrIter1 != NULL; ptrIter1=ptrIter1->pNext) {
        bool wPrev[eLen];
        // finding the waits events.
        initW(wPrev,ptrIter1->ppEventMatrix,eLen);
        assignVec(wPrev,ptrIter1->initWaits,eLen);
        // spawing a new automaton with q = 0 and t set to some negative value.
        automaton* nullAuto = new automaton(eLen);
        nullAuto->t1 = -1;
        // updating the values of flag and wPrev in the automaton
        assignVec(wPrev,nullAuto->w,eLen);
        assignVec(wPrev,nullAuto->flag,eLen);
        //ptrIter1->automataList = NULL;
        addAutoAtHead(ptrIter1,nullAuto);
        ptrIter1->iFrequency = 0;
        for(int i=0; i<eLen; i++) {
            if(wPrev[i] == true) {
                beta* ptrBetaTemp = new beta(eLen);
                ptrBetaTemp->ptrAlpha = ptrIter1;
                ptrBetaTemp->ptrAuto = nullAuto;
                //for(int j=0;j<eLen;j++)
                //{ ptrBetaTemp->q[j] = 0 ;}
                ptrBetaTemp->j = i;
                ptrBetaTemp->pPrev = NULL;
                addBetaAtHead(waitsList[ptrIter1->paiEvents[i] - 1] , ptrBetaTemp);
                nullAuto->betaAddress[i] = ptrBetaTemp;
            }
        }
    }


    int Event;
    //update : feb 22
    //bool masterFlag = true;
    bool chkFlag;
    while((Event = evntStream.GetNextEvent()) != -1) {
        double time = evntStream.GetTime();
        double nextTime;
        int nextEvent = evntStream.GetNextEvent();
        if(nextEvent != -1)
            nextTime = evntStream.GetTime();
        else nextTime = time + 1;   // just to ensure that next time is greater than time so that
        //	chkFlag is set TRUE in the case of the last event.
        evntStream.PushBackOne();

        if(Event > maxEvents) errorAbort("Wrong Event : observed event is greater that maxEvents");
        // chkFlag is TRUE if all the events at that time have been seen.
        if(nextTime > time) chkFlag = true;
        else chkFlag = false;
#if DEBUG
        cout << " %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " <<endl;
        cout << "Event = " << Event << endl;
        cout <<"chkflag = "<< chkFlag << endl;
        cout <<"masterflag = " <<masterFlag << endl<<endl;
#endif
        //setting all the flags to zero - why ??? - if an automata completes a count
        //on seeing event 'e' it shud not make new transitions further.
        //for(alpha* ptrIter1 = GelHead.ptrAlpha; ptrIter1 != NULL; ptrIter1=ptrIter1->pNext)
        //	ptrIter1->flag = false;

        //making the transitions of all the automata upon seeeing all the events at that time.
        beta* betaIterNext = NULL;
        for(beta* betaIter = waitsList[Event -1]; betaIter != NULL; betaIter = betaIterNext) {
            betaIterNext = betaIter->pNext;
            int j = betaIter->j;
            automaton* ptrAutoCurr = betaIter->ptrAuto;
            // update : feb 22 : removing the null flag check and usage of masterflag.
            //bool flagNull = checkQNull(ptrAutoCurr->flag,eLen);
            //bool proceedFlag = flagNull || ptrAutoCurr->flag[j] || (masterFlag == true);
            bool proceedFlag = ptrAutoCurr->flag[j];
            if(proceedFlag == false)
                continue;
            //cout << "am here " << endl;
            bool qPrev[eLen],qNext[eLen],wPrev[eLen],wNext[eLen];
            alpha* ptrAlpha = betaIter->ptrAlpha;
            //if(ptrAlpha->flag == true) continue;
            assignVec(ptrAutoCurr->q,qPrev,eLen);
            assignVec(ptrAutoCurr->q,qNext,eLen);
            assignVec(ptrAutoCurr->w,wPrev,eLen);
            assignVec(ptrAutoCurr->w,wNext,eLen);
            if(qNext[j] == true) errorAbort("Wrong value for qNext");
            qNext[j] = true;
            wNext[j] = false;
            //setting the flags for events that can be accepted at that time instant.
            //update : feb 22 : removed since flag null is avoided henceforth
            //if(flagNull || masterFlag == true)
            //	assignVec(wPrev,ptrAutoCurr->flag,eLen);
            //assigning the start time for the automaton moving out of 'q=0' state;
            if(checkQNull(qPrev,eLen) == true)
                ptrAutoCurr->t1 = time;

            for(int i = 0 ; i < eLen ; i++) {
                if(ptrAlpha->ppEventMatrix[j][i] == 0) continue;
                else {
                    bool parentSeenFlag = true;
                    for(int k = 0; k<eLen; k++) {
                        if(ptrAlpha->ppEventMatrix[k][i] == 1 && qNext[k] == false)
                            parentSeenFlag = false;
                    }
                    if(parentSeenFlag == true)
                        wNext[i] = true;
                }
            }
            assignVec(qNext,ptrAutoCurr->q,eLen);
            assignVec(wNext,ptrAutoCurr->w,eLen);
            ptrAutoCurr->updateMat(eLen,qNext,j);
            //deleting the entry in waitsList 'e'
            if(ptrAutoCurr->betaAddress[j] == betaIterNext) betaIterNext = betaIterNext->pNext;
            relinkDelBeta(waitsList[Event-1],ptrAutoCurr->betaAddress[j]);
            delete ptrAutoCurr->betaAddress[j];
            ptrAutoCurr->betaAddress[j] = NULL;
            //adding the new entries in the waitsList..
            for(int i=0; i<eLen; i++) {
                if(wNext[i] == true && wPrev[i] == false) {
                    beta* ptrBetaTemp = new beta(eLen);
                    ptrBetaTemp->ptrAlpha = ptrAlpha;
                    ptrBetaTemp->ptrAuto = ptrAutoCurr;
                    ptrBetaTemp->j = i;
                    ptrBetaTemp->pPrev = NULL;
                    //assignVec(qNext,ptrBetaTemp->q,eLen);
                    addBetaAtHead(waitsList[ptrAlpha->paiEvents[i]-1],ptrBetaTemp);
                    ptrAutoCurr->betaAddress[i] = ptrBetaTemp;
                }
            }
#if DEBUG
            cout << ">>>"  ;
            ptrAlpha->display(eLen);
            cout << "q = ";
            displayVec(ptrAutoCurr->q,eLen);
            cout << "w = ";
            displayVec(ptrAutoCurr->w,eLen);
            cout << " flag = ";
            displayVec(ptrAutoCurr->flag,eLen);
            cout << "beta address = " ;
            for(int l=0; l<eLen; l++)
                cout << ptrAutoCurr->betaAddress[l] << "  ";
            cout << endl<<endl;
#endif
        }
        // processing after all the events at a given time are seen
        if(chkFlag == true) {
            // update : feb 22:
            //masterFlag = true;
            // for each candidate
            for(alpha* ptrAlphaIter = GelHead.ptrAlpha; ptrAlphaIter != NULL ; ptrAlphaIter = ptrAlphaIter->pNext) {

#if 0
                //removing duplicate entries in alpha.init ... o(n2) method.. can be optimized
                for(automaton* autoIter1 = ptrAlphaIter->automataList; autoIter1!=NULL; autoIter1=autoIter1->pNext) {
                    automaton* autoIterPrev = autoIter1;
                    automaton* autoIter2Next;
                    for(automaton* autoIter2=autoIter1->pNext; autoIter2!=NULL; autoIter2=autoIter2Next) {
                        autoIter2Next = autoIter2->pNext;
                        if(checkVecSame(autoIter1->q,autoIter2->q,eLen) == true) {
                            autoIterPrev->pNext = autoIter2->pNext;
                            for(int i=0; i<eLen; i++) {
                                if(autoIter2->betaAddress[i]!=NULL) {
                                    int event = ptrAlphaIter->paiEvents[i];
                                    relinkDelBeta(waitsList[event-1],autoIter2->betaAddress[i]);
                                    delete autoIter2->betaAddress[i];
                                }
                            }
                            autoIter2->deleteMat(eLen);
                            delete autoIter2;
                        }
                    }
                }
#endif
                // removing duplicates in O(n)  -- assuming automatons with same 'q' occur consecutively...
                automaton *autoIter1,*autoIter2;
                for(autoIter1 = ptrAlphaIter->automataList,autoIter2=autoIter1->pNext; autoIter2!=NULL;) {
                    if(checkVecSame(autoIter1->q,autoIter2->q,eLen) == true) {
                        autoIter1->pNext = autoIter2->pNext;
                        for(int i=0; i<eLen; i++) {
                            if(autoIter2->betaAddress[i]!=NULL) {
                                int event = ptrAlphaIter->paiEvents[i];
                                relinkDelBeta(waitsList[event-1],autoIter2->betaAddress[i]);
                                delete autoIter2->betaAddress[i];
                            }
                        }
                        autoIter2->deleteMat(eLen);
                        delete autoIter2;
                        autoIter2 = autoIter1->pNext;
                    } else {
                        autoIter1 = autoIter1->pNext;
                        autoIter2 = autoIter2->pNext;
                    }
                }
                //cout << "duplicates removed " << endl;
                //
                //
                // finding the last automaton and updating the freq count ..
                // if a count is observed the we retire all the automatons ..
                automaton *it,*itPrev,*itNext;
                for(it=ptrAlphaIter->automataList; it->pNext!=NULL; it=it->pNext);
                if(checkQallOne(it->q,eLen) == true && (time - it->t1 <= expTime) ) {
                    //ptrAlphaIter->flag = true;
                    ptrAlphaIter->iFrequency++;
                    addMatrix(ptrAlphaIter->ppEntropyMatrix,it->trackMat,eLen);
                    automaton* autoIterNext = NULL;
                    for(automaton* autoIter = ptrAlphaIter->automataList; autoIter != NULL; autoIter = autoIterNext) {
                        autoIterNext = autoIter->pNext;
                        for(int i =0; i<eLen; i++) {
                            if(autoIter->betaAddress[i] != NULL) {
                                relinkDelBeta(waitsList[ptrAlphaIter->paiEvents[i]-1],autoIter->betaAddress[i]);
                                delete autoIter->betaAddress[i];
                            }
                        }
                        autoIter->deleteMat(eLen);
                        delete autoIter;
                    }
                    ptrAlphaIter->automataList = NULL;
                }
                //cout << "expiry checked " << endl;
                // adding a new automaton if none exists in start state ..
                if((ptrAlphaIter->automataList == NULL) || (checkQNull(ptrAlphaIter->automataList->q,eLen) == false)) {
                    bool wPrev[eLen];
                    assignVec(ptrAlphaIter->initWaits,wPrev,eLen);
                    automaton* nullAuto = new automaton(eLen);
                    nullAuto->t1 = -1;
                    assignVec(wPrev,nullAuto->w,eLen);
                    addAutoAtHead(ptrAlphaIter,nullAuto);
                    for(int i=0; i<eLen; i++) {
                        if(wPrev[i] == true) {
                            beta* ptrBetaTemp = new beta(eLen);
                            ptrBetaTemp->ptrAlpha = ptrAlphaIter;
                            ptrBetaTemp->ptrAuto = nullAuto;
                            ptrBetaTemp->j = i;
                            ptrBetaTemp->pPrev = NULL;
                            addBetaAtHead(waitsList[ptrAlphaIter->paiEvents[i] - 1] , ptrBetaTemp);
                            nullAuto->betaAddress[i] = ptrBetaTemp;
                        }
                    }
                }

                // resetting the flags ..
                for(automaton* autoIter=ptrAlphaIter->automataList; autoIter!=NULL; autoIter=autoIter->pNext) {
                    //update : feb 22 : need to copy the wPrev to flag.
                    //for(int i = 0;i<eLen;i++)
                    //{
                    //	autoIter->flag[i] = false;
                    //}
                    assignVec(autoIter->w,autoIter->flag,eLen);
                }

            }
        }
        //update : feb 22 : master flag is no more used.
        //else
        //	masterFlag = false;
    }
    //cleanupAutoList(GelHead.ptrAlpha,eLen);
    cleanupWaits(waitsList);
    alpha* alphaPrev = NULL;
    alpha* alphaNext = NULL;
#if 1
    //for(alpha* alphaIter = GelHead.ptrAlpha;alphaIter!=NULL;alphaIter = alphaIter->pNext)
    //	alphaIter->calculate_entropy(eLen);
    for(alpha* alphaIter = GelHead.ptrAlpha; alphaIter!=NULL; alphaIter = alphaNext) {
        alphaNext = alphaIter->pNext;
        //cout << alphaIter->iFrequency;
        automaton* autoIterNext;
        for(automaton* autoIter=alphaIter->automataList; autoIter!=NULL; autoIter=autoIterNext) {
            autoIterNext = autoIter->pNext;
            autoIter->deleteMat(eLen);
            delete autoIter;
        }
        alphaIter->automataList = NULL;
        double fcuk = alphaIter->calculate_entropy(GelHead.iEventLength);
        alphaIter->entropy = fcuk;
        if( fcuk < entTresh || alphaIter->iFrequency < tresh) {

            if(alphaPrev == NULL)
                GelHead.ptrAlpha = alphaIter->pNext;
            else
                alphaPrev->pNext = alphaIter->pNext;
            alphaIter->deleteMat(eLen);
            delete alphaIter;
            GelHead.iEpisodeListLength--;
        } else {
            alphaPrev = alphaIter;
        }
    }
#endif
    cout << "NUmber of Frequent "<<eLen<<"-Node episodes = "<<GelHead.iEpisodeListLength<<endl<<endl;
    tresh = int(tresh * fudgeFac);
}

void FrequentEpisodes::cleanupWaits(beta** waitsList) {
    for(int i=0; i<maxEvents; i++) {
        beta* betaIterNext = NULL;
        for(beta* betaIter = waitsList[i]; betaIter != NULL; betaIter = betaIterNext) {
            betaIterNext = betaIter->pNext;
            delete betaIter;
        }
    }
}
void FrequentEpisodes::cleanupAutoList(alpha* ptrAlpha,int len) {
    for(alpha* alphaIter=ptrAlpha; alphaIter!=NULL; alphaIter=alphaIter->pNext) {
        automaton* autoIterNext = NULL;
        for(automaton* autoIter=alphaIter->automataList; autoIter!=NULL; autoIter=autoIterNext) {
            autoIterNext = autoIter->pNext;
            autoIter->deleteMat(len);
            delete autoIter;

        }
        alphaIter->automataList = NULL;
    }
}
void FrequentEpisodes::displayFreqEpisodes(char* outfile) {
    fstream out;
    out.open(outfile,ios::out);
    for(int i=0; i<GelHeads.size(); i++) {
        int len = GelHeads[i].iEventLength;
        out << len << "-NOde Episodes = "<<GelHeads[i].iEpisodeListLength<<endl;
        for(alpha* alphaIter = GelHeads[i].ptrAlpha; alphaIter!=NULL; alphaIter=alphaIter->pNext) {
            alphaIter->display(out,len);
        }
    }
    out.close();
}

void FrequentEpisodes::displayLevelWise(fstream& out,int siz) {
    int len = GelHeads[siz].iEventLength;
    out << len << "-NOde Episodes = "<<GelHeads[siz].iEpisodeListLength<<endl;
    for(alpha* alphaIter = GelHeads[siz].ptrAlpha; alphaIter!=NULL; alphaIter=alphaIter->pNext) {
        alphaIter->display(out,len);
    }
}

void FrequentEpisodes::filter() {
    int flag = 0;
    int chk = 0;
    int iTemp,jTemp,eLen;

    alpha *pstEpisodeIter1 = NULL, *pstEpisodeIter2 = NULL;
    alpha *pstPrev1=NULL, *pstPrev2=NULL;
    alpha *pstIter1Next=NULL, *pstIter2Next=NULL;
    for(int headIter = 0; headIter < GelHeads.size(); headIter++) {
        eLen = GelHeads[headIter].iEventLength;
        pstPrev1 = NULL;
        chk =0;
        for(pstEpisodeIter1 = GelHeads[headIter].ptrAlpha ; pstEpisodeIter1 != NULL ; pstEpisodeIter1 = pstIter1Next) {
            pstIter1Next = pstEpisodeIter1->pNext;
            pstPrev2 = NULL;
            flag=0;
            for(pstEpisodeIter2 = GelHeads[headIter].ptrAlpha; pstEpisodeIter2 != pstEpisodeIter1 ; pstEpisodeIter2 = pstIter2Next) {
                flag = 0;
                pstIter2Next = pstEpisodeIter2->pNext;
                if(compare_event(pstEpisodeIter1->paiEvents,pstEpisodeIter2->paiEvents,eLen)) {
                    flag = compare_containment( (pstEpisodeIter1->ppEventMatrix), (pstEpisodeIter2->ppEventMatrix),eLen);
#if 0
#endif

                    if(flag == 1) { // to remove the structure pointed by pstEpisodeIter2.
                        if(pstPrev2 == NULL) {
                            GelHeads[headIter].ptrAlpha = pstEpisodeIter2->pNext;
                        } else {
                            pstPrev2->pNext = pstEpisodeIter2->pNext;
                        }
                        pstEpisodeIter2->deleteMat(eLen);
                        chk++;
                        delete pstEpisodeIter2;
                        GelHeads[headIter].iEpisodeListLength--;
                    } else if(flag == -1) { // remove the structure pointed by iter1
                        pstPrev1->pNext = pstEpisodeIter1->pNext;
                        pstEpisodeIter1->deleteMat(eLen);
                        delete pstEpisodeIter1;
                        GelHeads[headIter].iEpisodeListLength--;
                        chk++;
                        break;
                    }
                }
                if(flag == 0) { // wen the event types are different
                    pstPrev2 = pstEpisodeIter2;
                }
            }
            if(flag != -1) {
                pstPrev1 = pstEpisodeIter1 ;
            }
        }

        printf("\nNo. of episodes removed at level-%d = %d\n",eLen,chk);
    }
}

void FrequentEpisodes::filter(int siz) {
    int flag = 0;
    int chk = 0;
    int iTemp,jTemp,eLen;
    alpha *pstEpisodeIter1 = NULL, *pstEpisodeIter2 = NULL;
    alpha *pstPrev1=NULL, *pstPrev2=NULL;
    alpha *pstIter1Next=NULL, *pstIter2Next=NULL;
    //for(int headIter = 0;headIter < GelHeads.size();headIter++)
    //{
    int headIter = siz;
    eLen = GelHeads[headIter].iEventLength;
    pstPrev1 = NULL;
    chk =0;
    for(pstEpisodeIter1 = GelHeads[headIter].ptrAlpha ; pstEpisodeIter1 != NULL ; pstEpisodeIter1 = pstIter1Next) {
        pstIter1Next = pstEpisodeIter1->pNext;
        pstPrev2 = NULL;
        flag=0;
        for(pstEpisodeIter2 = GelHeads[headIter].ptrAlpha; pstEpisodeIter2 != pstEpisodeIter1 ; pstEpisodeIter2 = pstIter2Next) {
            flag = 0;
            pstIter2Next = pstEpisodeIter2->pNext;
            if(compare_event(pstEpisodeIter1->paiEvents,pstEpisodeIter2->paiEvents,eLen)) {
                flag = compare_containment( (pstEpisodeIter1->ppEventMatrix), (pstEpisodeIter2->ppEventMatrix),eLen);
#if 0
#endif

                if(flag == 1) { // to remove the structure pointed by pstEpisodeIter2.
                    if(pstPrev2 == NULL) {
                        GelHeads[headIter].ptrAlpha = pstEpisodeIter2->pNext;
                    } else {
                        pstPrev2->pNext = pstEpisodeIter2->pNext;
                    }
                    pstEpisodeIter2->deleteMat(eLen);
                    chk++;
                    delete pstEpisodeIter2;
                    GelHeads[headIter].iEpisodeListLength--;
                } else if(flag == -1) { // remove the structure pointed by iter1
                    pstPrev1->pNext = pstEpisodeIter1->pNext;
                    pstEpisodeIter1->deleteMat(eLen);
                    delete pstEpisodeIter1;
                    GelHeads[headIter].iEpisodeListLength--;
                    chk++;
                    break;
                }
            }
            if(flag == 0) { // wen the event types are different
                pstPrev2 = pstEpisodeIter2;
            }
        }
        if(flag != -1) {
            pstPrev1 = pstEpisodeIter1 ;
        }
    }

    printf("No. of episodes removed at level-%d = %d\n\n",siz+1,chk);
    //}
}

#define MAXLEN 100
void FrequentEpisodes::loadCandidate(alpha* ptrAlpha,int size,string lines) {
    char* line = new char[lines.size()+1];
    strcpy(line,lines.c_str());
    int iTemp,jTemp=0,kTemp;
    char strEventTemp[26];
    int i=0,j=0, iEvent = 0;
    int loc_i=-1,loc_j=-1;
    int colonloc;
    colonloc = (int)lines.find_first_of(":");
    for(iTemp=0,jTemp=0; iTemp<colonloc; iTemp++) {
        if(line[iTemp] == ',') {
            strEventTemp[jTemp] = '\0';
            jTemp=0;
            ptrAlpha->paiEvents[iEvent] = atoi(strEventTemp);
            iEvent++;
        } else if(isdigit(line[iTemp])) {
            strEventTemp[jTemp] = line[iTemp];
            jTemp++;
        } else if(!isspace(line[iTemp])) {
            printf("1485 ::ERROR ::: INvalid Char in the Input\n");
            exit(0);
        }
    }
    strEventTemp[jTemp] = '\0';
    ptrAlpha->paiEvents[iEvent] = atoi(strEventTemp);
    iEvent++;
    if(iEvent != size) {
        printf("1489 :: ERROR in number of Events\n");
    }
    //printf("Event Array : ");
    //for(iTemp=0;iTemp<size;iTemp++)
    //	printf("%d ",p->events[iTemp]);
    //printf("\n");
    for(iTemp=colonloc+1,jTemp=0; iTemp<lines.size(); iTemp++) {
        if(line[iTemp] == '>') {
            strEventTemp[jTemp] = '\0';
            i = atoi(strEventTemp);
            jTemp = 0;
        } else if(line[iTemp] == ','||line[iTemp]=='\n') {
            strEventTemp[jTemp] = '\0';
            j = atoi(strEventTemp);
            //for(jTemp=0;jTemp < size;jTemp++)
            //{
            //	if(ptrAlpha->paiEvents[jTemp] == i)
            //	{	loc_i = jTemp; break; }
            //}
            //if(jTemp == size) { printf("1503 :: ERROR : RHS and LHS doesnt MAtch\n"); }
            //for(jTemp=0;jTemp<size;jTemp++)
            //{
            //	if(ptrAlpha->paiEvents[jTemp] == j)
            //	{ 	loc_j = jTemp; break; }
            //}
            //if(jTemp == size) { printf("1509 :: ERROR : RHS and LHS doesnt MAtch\n"); }
            if(i <= size && j << size)
                ptrAlpha->ppEventMatrix[i-1][j-1] = 1;
            else {
                cout << "Error : In 'candidates' file ; input of partial order" << endl;
                exit(0);
            }
            jTemp = 0;
        } else if(isdigit(line[iTemp])) {
            strEventTemp[jTemp] = line[iTemp];
            jTemp++;
        } else if(isspace(line[iTemp] == ' '))
            //	jTemp = 0;
            ;
        else {
            printf("1518 :: ERROR ::: Invalid Character in Candidate Representation.\n");
            exit(0);
        }
    }
    for(iTemp=0; iTemp<size; iTemp++)
        for(jTemp=0; jTemp<size; jTemp++)
            if(ptrAlpha->ppEventMatrix[iTemp][jTemp] == 1)
                for(kTemp=0; kTemp<size; kTemp++)
                    if(ptrAlpha->ppEventMatrix[jTemp][kTemp] == 1)
                        ptrAlpha->ppEventMatrix[iTemp][kTemp] = 1;
}

void FrequentEpisodes::parser(string candid) {
    fstream pfCandid;
    pfCandid.open(candid.c_str(),ios::in);
    if(pfCandid == NULL) {
        printf("1459::Error in Reading the Candidates file\n");
        exit(0);
    }
    //Initializing the List header
    vector<stEpisodeGel> gelHeads;
    //vector<alpha*> prevPtrs;
    for(int i=0; i<maxSiz; i++) {
        stEpisodeGel stCandidateHead;
        stCandidateHead.iEventLength = i+1;
        gelHeads.push_back(stCandidateHead);
        //alpha* temp = NULL;
        //prevPtrs.push_back(temp);
    }
    int numCandids;
    pfCandid >> numCandids;
    for(int canIter=0; canIter<numCandids; canIter++) {
        int size;
        pfCandid >> size;
        string pattern;
        pfCandid >> pattern;
        alpha* ptrAlpha = new alpha(size);
        loadCandidate(ptrAlpha,size,pattern);
        gelHeads[size-1].iEpisodeListLength++;
        ptrAlpha->pNext = gelHeads[size-1].ptrAlpha;
        gelHeads[size-1].ptrAlpha = ptrAlpha;
    }
    for(int i=0; i<gelHeads.size(); i++)
        findFreqCandids(gelHeads[i]);
    for(int i=0; i<gelHeads.size(); i++) {
        for(alpha* ptrIter = gelHeads[i].ptrAlpha; ptrIter != NULL; ptrIter=ptrIter->pNext) {
            transred(ptrIter->ppEventMatrix,i+1);
            ptrIter->display(i+1);
        }
    }
    pfCandid.close();
}
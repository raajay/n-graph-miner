#define DEBUG 0
#include"super.h"
void freqEpisodes::findFreqEpisodes(void)
{
	fstream fout;
	fout.open(outfile.c_str(),ios::out);
	// open file for displaying output
	stEpisodeGel oneCandid;
	oneCandid.iEventLength = 1;
	initCandidates(oneCandid);
	GelHeads.push_back(oneCandid);
	findFreqCandids(GelHeads[0]);
	displayLevelWise(fout,0);
	for(int i=1;i<maxSiz;i++)
	{
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
void freqEpisodes::initCandidates(stEpisodeGel& GelHead)
{
	alpha* ptrPrev = NULL;
	for(int i=0;i<maxEvents;i++)
	{
		alpha *ptrTemp = new alpha(1);	if(ptrTemp == NULL) errorAbort("in initCandidates :: out of memory");
		ptrTemp->paiEvents[0] = i+1;
		ptrTemp->ppEventMatrix[0][0] = 0;
		if(ptrPrev != NULL) {ptrPrev->pNext = ptrTemp;ptrTemp->pstBlockStart = GelHead.ptrAlpha;}
		else {GelHead.ptrAlpha = ptrTemp;ptrTemp->pstBlockStart = ptrTemp;}
		ptrPrev = ptrTemp;
	}
	GelHead.iEpisodeListLength = maxEvents;
	return;
}

void freqEpisodes::findFreqCandids(stEpisodeGel& GelHead)
{
	cout<<"Number of "<<GelHead.iEventLength<<"-node candidates being counted = "<<GelHead.iEpisodeListLength<<endl;
	cout << "Frequency treshhold = " << tresh << endl;
	// Getting the event stream 
	readfile evntStream(filename);
	// Initializing waitslist.. declaring the waitslist struc and adding the initwaits of all candidates
	// and the null automaton in the alpha.init list 
	beta* waitsList[maxEvents];
	int eLen = GelHead.iEventLength;
	for(int i=0;i<maxEvents;i++)
		waitsList[i] = NULL;
	for(alpha* ptrIter1 = GelHead.ptrAlpha;ptrIter1 != NULL;ptrIter1=ptrIter1->pNext)
	{
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
		for(int i=0;i<eLen;i++)
		{
			if(wPrev[i] == true)
			{
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
	while((Event = evntStream.getnextevent()) != -1)
	{
		double time = evntStream.getTime();
		double nextTime;
		int nextEvent = evntStream.getnextevent();
		if(nextEvent != -1)
			nextTime = evntStream.getTime();
		else nextTime = time + 1;   // just to ensure that next time is greater than time so that
	       //	chkFlag is set TRUE in the case of the last event.
		evntStream.pushBackOne();

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
		for(beta* betaIter = waitsList[Event -1];betaIter != NULL;betaIter = betaIterNext)
		{
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

			for(int i = 0 ; i < eLen ; i++)
			{
				if(ptrAlpha->ppEventMatrix[j][i] == 0) continue;
				else
				{
					bool parentSeenFlag = true;
					for(int k = 0;k<eLen;k++)
					{
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
			for(int i=0;i<eLen;i++)
			{
				if(wNext[i] == true && wPrev[i] == false)
				{
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
			for(int l=0;l<eLen;l++)
				cout << ptrAutoCurr->betaAddress[l] << "  ";
			cout << endl<<endl;
#endif
		}
		// processing after all the events at a given time are seen
		if(chkFlag == true)
		{
            // update : feb 22:
			//masterFlag = true;
			// for each candidate
			for(alpha* ptrAlphaIter = GelHead.ptrAlpha; ptrAlphaIter != NULL ; ptrAlphaIter = ptrAlphaIter->pNext)
			{

				#if 0
				//removing duplicate entries in alpha.init ... o(n2) method.. can be optimized
 				for(automaton* autoIter1 = ptrAlphaIter->automataList;autoIter1!=NULL;autoIter1=autoIter1->pNext)
				{
					automaton* autoIterPrev = autoIter1;
					automaton* autoIter2Next;
					for(automaton* autoIter2=autoIter1->pNext;autoIter2!=NULL;autoIter2=autoIter2Next)
					{
						autoIter2Next = autoIter2->pNext;
						if(checkVecSame(autoIter1->q,autoIter2->q,eLen) == true)
						{
							autoIterPrev->pNext = autoIter2->pNext;
							for(int i=0;i<eLen;i++)
							{
								if(autoIter2->betaAddress[i]!=NULL)
								{
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
				for(autoIter1 = ptrAlphaIter->automataList,autoIter2=autoIter1->pNext;autoIter2!=NULL;)
				{
					if(checkVecSame(autoIter1->q,autoIter2->q,eLen) == true)
					{
						autoIter1->pNext = autoIter2->pNext;
						for(int i=0;i<eLen;i++)
						{
							if(autoIter2->betaAddress[i]!=NULL)
							{
								int event = ptrAlphaIter->paiEvents[i];
								relinkDelBeta(waitsList[event-1],autoIter2->betaAddress[i]);
								delete autoIter2->betaAddress[i];
							}
						}
						autoIter2->deleteMat(eLen);
						delete autoIter2;
						autoIter2 = autoIter1->pNext;
					}
					else
					{
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
				for(it=ptrAlphaIter->automataList;it->pNext!=NULL;it=it->pNext);
				if(checkQallOne(it->q,eLen) == true && (time - it->t1 <= expTime) )
				{
					//ptrAlphaIter->flag = true;
					ptrAlphaIter->iFrequency++;
					addMatrix(ptrAlphaIter->ppEntropyMatrix,it->trackMat,eLen);
					automaton* autoIterNext = NULL;
					for(automaton* autoIter = ptrAlphaIter->automataList;autoIter != NULL;autoIter = autoIterNext)
					{
						autoIterNext = autoIter->pNext;
						for(int i =0;i<eLen;i++)
						{
							if(autoIter->betaAddress[i] != NULL)
							{
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
				if((ptrAlphaIter->automataList == NULL) || (checkQNull(ptrAlphaIter->automataList->q,eLen) == false))
				{
					bool wPrev[eLen];
					assignVec(ptrAlphaIter->initWaits,wPrev,eLen);
					automaton* nullAuto = new automaton(eLen);
					nullAuto->t1 = -1;
					assignVec(wPrev,nullAuto->w,eLen);
					addAutoAtHead(ptrAlphaIter,nullAuto);
					for(int i=0;i<eLen;i++)
					{
						if(wPrev[i] == true)
						{
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
				for(automaton* autoIter=ptrAlphaIter->automataList;autoIter!=NULL;autoIter=autoIter->pNext)
				{
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
	for(alpha* alphaIter = GelHead.ptrAlpha;alphaIter!=NULL;alphaIter = alphaNext)
	{
		alphaNext = alphaIter->pNext;
		//cout << alphaIter->iFrequency;
		automaton* autoIterNext;
		for(automaton* autoIter=alphaIter->automataList;autoIter!=NULL;autoIter=autoIterNext)
		{ autoIterNext = autoIter->pNext; autoIter->deleteMat(eLen); delete autoIter;}
		alphaIter->automataList = NULL;
		double fcuk = alphaIter->calculate_entropy(GelHead.iEventLength);
		alphaIter->entropy = fcuk;
		if( fcuk < entTresh || alphaIter->iFrequency < tresh)
		{
			
			if(alphaPrev == NULL)
				GelHead.ptrAlpha = alphaIter->pNext;
			else
				alphaPrev->pNext = alphaIter->pNext;
			alphaIter->deleteMat(eLen);
			delete alphaIter;
			GelHead.iEpisodeListLength--;
		}
		else {alphaPrev = alphaIter;}
	}
#endif
	cout << "NUmber of Frequent "<<eLen<<"-Node episodes = "<<GelHead.iEpisodeListLength<<endl<<endl;
	tresh = int(tresh * fudgeFac);
}

void freqEpisodes::cleanupWaits(beta** waitsList)
{
	for(int i=0;i<maxEvents;i++)
	{
		beta* betaIterNext = NULL;
		for(beta* betaIter = waitsList[i];betaIter != NULL;betaIter = betaIterNext)
		{
			betaIterNext = betaIter->pNext;
			delete betaIter;
		}
	}
}
void freqEpisodes::cleanupAutoList(alpha* ptrAlpha,int len)
{
	for(alpha* alphaIter=ptrAlpha; alphaIter!=NULL;alphaIter=alphaIter->pNext)
	{	
		automaton* autoIterNext = NULL;
		for(automaton* autoIter=alphaIter->automataList;autoIter!=NULL;autoIter=autoIterNext)
		{
			autoIterNext = autoIter->pNext;
			autoIter->deleteMat(len);
			delete autoIter;

		}
		alphaIter->automataList = NULL;
	}
}
void freqEpisodes::displayFreqEpisodes(char* outfile)
{
	fstream out;
	out.open(outfile,ios::out);
	for(int i=0;i<GelHeads.size();i++)
	{
		int len = GelHeads[i].iEventLength;
		out << len << "-NOde Episodes = "<<GelHeads[i].iEpisodeListLength<<endl;
		for(alpha* alphaIter = GelHeads[i].ptrAlpha; alphaIter!=NULL; alphaIter=alphaIter->pNext)
		{
				alphaIter->display(out,len);
		}
	}
	out.close();
}

void freqEpisodes::displayLevelWise(fstream& out,int siz)
{
	int len = GelHeads[siz].iEventLength;
	out << len << "-NOde Episodes = "<<GelHeads[siz].iEpisodeListLength<<endl;
	for(alpha* alphaIter = GelHeads[siz].ptrAlpha;alphaIter!=NULL;alphaIter=alphaIter->pNext)
	{
		alphaIter->display(out,len);
	}
}

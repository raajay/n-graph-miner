#include"super.h"
#define MAX_LONGINT 10000000
void freqEpisodes::generateCandidates(stEpisodeGel prevHead,stEpisodeGel& Head)
{
	int l = prevHead.iEventLength;
	for(alpha* mainIter = prevHead.ptrAlpha;mainIter!=NULL;mainIter = mainIter->pNext)
	{
		alpha* pstBlockStart = NULL;
		for(alpha* blockIter=mainIter->pNext;blockIter!=NULL && blockIter->pstBlockStart == mainIter->pstBlockStart;blockIter=blockIter->pNext)
		{
			alpha* (posEpisodes[3]) = {NULL};
			if(mainIter->paiEvents[l-1] == blockIter->paiEvents[l-1])
				continue;
			generatePossibleCandidates(mainIter,blockIter,posEpisodes,l);
			for(int iTemp=0;iTemp<3 && posEpisodes[iTemp] != NULL;iTemp++)
			{
				bool addFlag = true;
				dummyEpisode* ptrDummy = new dummyEpisode(l);
				for(int jTemp = 0; (jTemp<l-1) && (addFlag == true);jTemp++)
				{
							
#if 1
					for(int kTemp=0;kTemp<jTemp;kTemp++)
					{
						ptrDummy->paiEvents[kTemp] = posEpisodes[iTemp]->paiEvents[kTemp];
						for(int zTemp =0;zTemp<jTemp;zTemp++)
						{
							ptrDummy->ppEventMatrix[kTemp][zTemp] = posEpisodes[iTemp]->ppEventMatrix[kTemp][zTemp];
						}
						for(int zTemp =jTemp;zTemp<l;zTemp++) // CCheck the condition
						{
							ptrDummy->ppEventMatrix[kTemp][zTemp] =  posEpisodes[iTemp]->ppEventMatrix[kTemp][zTemp+1];
						}
					}
					for(int kTemp=jTemp;kTemp<l;kTemp++)
					{
						ptrDummy->paiEvents[kTemp] = posEpisodes[iTemp]->paiEvents[kTemp+1];
						for(int zTemp =0;zTemp<jTemp;zTemp++)
						{
							ptrDummy->ppEventMatrix[kTemp][zTemp] = posEpisodes[iTemp]->ppEventMatrix[kTemp+1][zTemp];
						}
						for(int zTemp =jTemp;zTemp<l;zTemp++) // CCheck the condition
						{
							ptrDummy->ppEventMatrix[kTemp][zTemp] = posEpisodes[iTemp]->ppEventMatrix[kTemp+1][zTemp+1];
						}
					}

#endif				
					if( -1  == existSubEpisode(prevHead.ptrAlpha,ptrDummy->paiEvents,ptrDummy->ppEventMatrix,l))
					{
						addFlag = false;
					}

				}

				if(true == addFlag && lengthMaxPath(posEpisodes[iTemp]->ppEventMatrix,l+1) <= maxLength 
				&& numMaxPath(posEpisodes[iTemp]->ppEventMatrix,l+1) <= maxWidth)
				//if(true == addFlag)							
				{
					if(( addCandidateEpisode(Head,posEpisodes[iTemp],&pstBlockStart,0)) == -1)
					{
						cout <<" Out of Memory - Higher Threshold"<<endl;
						exit(0);
					}
				}	
				else
				{
					posEpisodes[iTemp]->deleteMat(l+1);
					delete posEpisodes[iTemp];
				}
				ptrDummy->deleteMat(l);
				delete ptrDummy;
				
			}
			
		}
	}
	
}
void freqEpisodes::generatePossibleCandidates(alpha *pstEpi1,alpha *pstEpi2,alpha *(pstTemp[3]),int iEventLen)
{
	int iTemp =0;
	int iDirFlag =0;
	if(episodeType == 1)
	{
		pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
		return;
	}
	for(iTemp =0;iTemp<iEventLen-1;iTemp++)
	{
		if((1 == pstEpi1->ppEventMatrix[iEventLen-1][iTemp]) && (1 == pstEpi2->ppEventMatrix[iTemp][iEventLen-1]))
		{
			iDirFlag = 1;
		}
		if((1 == pstEpi2->ppEventMatrix[iEventLen-1][iTemp]) && (1 == pstEpi1->ppEventMatrix[iTemp][iEventLen-1]))
		{
			iDirFlag = 2;
		}
	}

	if(iDirFlag != 0)
	{
		pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
		if(1 == iDirFlag)
		{
			pstTemp[0]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
		}
		else
		{
			pstTemp[0]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
		}
		return;

	}
	else
	{
		int c = 0;
		if(episodeType != 2)
		{
			pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
			c++;
		}
		
		for(iTemp =0;iTemp<iEventLen-1;iTemp++)
		{
			if(((1 == pstEpi1->ppEventMatrix[iEventLen-1][iTemp]) && (0 == pstEpi2->ppEventMatrix[iEventLen-1][iTemp])) ||
				((0 == pstEpi1->ppEventMatrix[iTemp][iEventLen-1]) && (1 == pstEpi2->ppEventMatrix[iTemp][iEventLen-1])))
			{
				if(iDirFlag != 2)
				{
					iDirFlag = 1;
				}
				else
				{
					return;
				}
			}

			if(((0 == pstEpi1->ppEventMatrix[iEventLen-1][iTemp]) && (1 == pstEpi2->ppEventMatrix[iEventLen-1][iTemp])) ||
				((1 == pstEpi1->ppEventMatrix[iTemp][iEventLen-1]) && (0 == pstEpi2->ppEventMatrix[iTemp][iEventLen-1])))
			{
				if(iDirFlag != 1)
				{
					iDirFlag = 2;
				}
				else
				{
					return;
				}
			}

		}
		switch(iDirFlag)
		{
			case 0:
					{
						pstTemp[c] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
						pstTemp[c]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
						pstTemp[c+1] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
						pstTemp[c+1]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
					}
					break;
			case 1:
					{
						pstTemp[c] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
						pstTemp[c]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
					}
					break;
			case 2:
					{
						pstTemp[c] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
						pstTemp[c]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
					}
					break;

		}
	}
	return ;
}
freqEpisodes::alpha* freqEpisodes::catEpisodes(alpha *pstEpi1,alpha *pstEpi2,int iEventLen)
{
	alpha* pstTemp = NULL;
	int iTemp =0,jTemp =0;

	pstTemp = new alpha(iEventLen+1);

	for(iTemp =0;iTemp<iEventLen;iTemp++)
	{
		pstTemp->paiEvents[iTemp] = pstEpi1->paiEvents[iTemp];

		for(jTemp =0;jTemp<iEventLen;jTemp++)
		{
			pstTemp->ppEventMatrix[iTemp][jTemp] = pstEpi1->ppEventMatrix[iTemp][jTemp];
		}
	}

	pstTemp->paiEvents[iEventLen] = pstEpi2->paiEvents[iEventLen-1];
	for(jTemp =0;jTemp<iEventLen;jTemp++)
	{
		pstTemp->ppEventMatrix[iEventLen][jTemp] = pstEpi2->ppEventMatrix[iEventLen-1][jTemp];
		pstTemp->ppEventMatrix[jTemp][iEventLen] = pstEpi2->ppEventMatrix[jTemp][iEventLen-1];
	}
	pstTemp->ppEventMatrix[iEventLen][iEventLen] =0;
	return pstTemp;
}

int freqEpisodes::existSubEpisode(alpha *pstFreqEpisodeList,int *paiEvent,bool **ppaiEventMatrix,int iLen)
{
	int iTemp =0,jTemp =0;
	int iCount =0;
	int izTemp =0;
	alpha *pstFreqEpisodeListIter = pstFreqEpisodeList;

	for(;pstFreqEpisodeListIter!= NULL;pstFreqEpisodeListIter = pstFreqEpisodeListIter->pNext)
	{
		izTemp++;
	}

 
	while(pstFreqEpisodeList!= NULL) 
	{
		iCount =0;	
		izTemp++;
		for(iTemp = iLen-1; iTemp >=0  ;iTemp--)
		{
			if(paiEvent[iTemp] != pstFreqEpisodeList->paiEvents[iTemp])
			{
				break;
			}
			iCount++;
		}
		if(iCount == iLen)
		{
			for(iTemp = iLen-1;iTemp >=0  ;iTemp--)
			{
				for(jTemp = iLen-1;jTemp >=0  ;jTemp--)
				{
					if(ppaiEventMatrix[iTemp][jTemp] != pstFreqEpisodeList->ppEventMatrix[iTemp][jTemp])
					{
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
int freqEpisodes::addCandidateEpisode(stEpisodeGel& Head, alpha *pstNode, alpha **pstBlockStart,
						 const int iFrequency)
{
	

	alpha *pstIter = NULL;
	int iEventLen =Head.iEventLength;
	int iTemp =0,jTemp =0;

	

	if(MAX_LONGINT == Head.iEpisodeListLength)
	{
		cout<<"\nCandidate Epiosdes of Length : "<<Head.iEventLength <<" exceeds MAX"<<endl;
		return -1;
	}

	pstNode->iFrequency = iFrequency;
	if(NULL == *pstBlockStart)
	{
		*pstBlockStart = pstNode;
	}
	pstNode->pstBlockStart = *pstBlockStart;	


	Head.iEpisodeListLength++;
	
	if(NULL == Head.ptrAlpha)
	{
		Head.ptrAlpha = pstNode;
		Head.pstTail = pstNode;
		return 0;
	}
	Head.pstTail->pNext = pstNode;
	Head.pstTail = pstNode;	
	return 0;	
}

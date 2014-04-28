#include"super.h"
void freqEpisodes::filter()
{
	int flag = 0;int chk = 0;
	int iTemp,jTemp,eLen;
	
	alpha *pstEpisodeIter1 = NULL, *pstEpisodeIter2 = NULL;
	alpha *pstPrev1=NULL, *pstPrev2=NULL;
	alpha *pstIter1Next=NULL, *pstIter2Next=NULL;
	for(int headIter = 0;headIter < GelHeads.size();headIter++)
	{
		eLen = GelHeads[headIter].iEventLength;
		pstPrev1 = NULL;chk =0;
		for(pstEpisodeIter1 = GelHeads[headIter].ptrAlpha ; pstEpisodeIter1 != NULL ;pstEpisodeIter1 = pstIter1Next)
		{
			pstIter1Next = pstEpisodeIter1->pNext;
			pstPrev2 = NULL;flag=0;
			for(pstEpisodeIter2 = GelHeads[headIter].ptrAlpha; pstEpisodeIter2 != pstEpisodeIter1 ;pstEpisodeIter2 = pstIter2Next)
                	{
                		flag = 0;
                		pstIter2Next = pstEpisodeIter2->pNext;
				if(compare_event(pstEpisodeIter1->paiEvents,pstEpisodeIter2->paiEvents,eLen))
				{
					flag = compare_containment( (pstEpisodeIter1->ppEventMatrix), (pstEpisodeIter2->ppEventMatrix),eLen);
#if 0
#endif					
					
					if(flag == 1) // to remove the structure pointed by pstEpisodeIter2. 		
					{
						if(pstPrev2 == NULL) 
							{GelHeads[headIter].ptrAlpha = pstEpisodeIter2->pNext;}
						else
							{pstPrev2->pNext = pstEpisodeIter2->pNext;}
						pstEpisodeIter2->deleteMat(eLen); chk++;
						delete pstEpisodeIter2;
						GelHeads[headIter].iEpisodeListLength--;
					}
					else if(flag == -1) // remove the structure pointed by iter1
					{
						pstPrev1->pNext = pstEpisodeIter1->pNext;
						pstEpisodeIter1->deleteMat(eLen); delete pstEpisodeIter1;
						GelHeads[headIter].iEpisodeListLength--;chk++;
						break;
					}
				}
				if(flag == 0) // wen the event types are different
				{
					pstPrev2 = pstEpisodeIter2;
				}
                	}
                	if(flag != -1) 
                		{pstPrev1 = pstEpisodeIter1 ;}	
		}
		
		printf("\nNo. of episodes removed at level-%d = %d\n",eLen,chk);
	}
}

void freqEpisodes::filter(int siz)
{
	int flag = 0;int chk = 0;
	int iTemp,jTemp,eLen;
	alpha *pstEpisodeIter1 = NULL, *pstEpisodeIter2 = NULL;
	alpha *pstPrev1=NULL, *pstPrev2=NULL;
	alpha *pstIter1Next=NULL, *pstIter2Next=NULL;
	//for(int headIter = 0;headIter < GelHeads.size();headIter++)
	//{
		int headIter = siz;
		eLen = GelHeads[headIter].iEventLength;
		pstPrev1 = NULL;chk =0;
		for(pstEpisodeIter1 = GelHeads[headIter].ptrAlpha ; pstEpisodeIter1 != NULL ;pstEpisodeIter1 = pstIter1Next)
		{
			pstIter1Next = pstEpisodeIter1->pNext;
			pstPrev2 = NULL;flag=0;
			for(pstEpisodeIter2 = GelHeads[headIter].ptrAlpha; pstEpisodeIter2 != pstEpisodeIter1 ;pstEpisodeIter2 = pstIter2Next)
                	{
                		flag = 0;
                		pstIter2Next = pstEpisodeIter2->pNext;
				if(compare_event(pstEpisodeIter1->paiEvents,pstEpisodeIter2->paiEvents,eLen))
				{
					flag = compare_containment( (pstEpisodeIter1->ppEventMatrix), (pstEpisodeIter2->ppEventMatrix),eLen);
#if 0
#endif					
					
					if(flag == 1) // to remove the structure pointed by pstEpisodeIter2. 		
					{
						if(pstPrev2 == NULL) 
							{GelHeads[headIter].ptrAlpha = pstEpisodeIter2->pNext;}
						else
							{pstPrev2->pNext = pstEpisodeIter2->pNext;}
						pstEpisodeIter2->deleteMat(eLen); chk++;
						delete pstEpisodeIter2;
						GelHeads[headIter].iEpisodeListLength--;
					}
					else if(flag == -1) // remove the structure pointed by iter1
					{
						pstPrev1->pNext = pstEpisodeIter1->pNext;
						pstEpisodeIter1->deleteMat(eLen); delete pstEpisodeIter1;
						GelHeads[headIter].iEpisodeListLength--;chk++;
						break;
					}
				}
				if(flag == 0) // wen the event types are different
				{
					pstPrev2 = pstEpisodeIter2;
				}
                	}
                	if(flag != -1) 
                		{pstPrev1 = pstEpisodeIter1 ;}	
		}
		
		printf("No. of episodes removed at level-%d = %d\n\n",siz+1,chk);
	//}
}

#define MAXLEN 100
void freqEpisodes::loadCandidate(alpha* ptrAlpha,int size,string lines)
{
	char* line = new char[lines.size()+1];
	strcpy(line,lines.c_str());
	int iTemp,jTemp=0,kTemp;
	char strEventTemp[26];
	int i=0,j=0, iEvent = 0;
	int loc_i=-1,loc_j=-1;
	int colonloc;
	colonloc = (int)lines.find_first_of(":");
	for(iTemp=0,jTemp=0;iTemp<colonloc;iTemp++)
	{
		if(line[iTemp] == ',')
		{	strEventTemp[jTemp] = '\0';jTemp=0; ptrAlpha->paiEvents[iEvent] = atoi(strEventTemp); iEvent++; }
		else if(isdigit(line[iTemp]))
		{	strEventTemp[jTemp] = line[iTemp]; jTemp++; }
		else if(!isspace(line[iTemp]))
		{	printf("1485 ::ERROR ::: INvalid Char in the Input\n"); exit(0); }
	}
	strEventTemp[jTemp] = '\0';
	ptrAlpha->paiEvents[iEvent] = atoi(strEventTemp); iEvent++;
	if(iEvent != size) {	printf("1489 :: ERROR in number of Events\n"); }
	//printf("Event Array : ");
	//for(iTemp=0;iTemp<size;iTemp++) 
	//	printf("%d ",p->events[iTemp]);
	//printf("\n");
	for(iTemp=colonloc+1,jTemp=0;iTemp<lines.size();iTemp++)
	{	
		if(line[iTemp] == '>')
		{	strEventTemp[jTemp] = '\0'; i = atoi(strEventTemp); jTemp = 0;}
		else if(line[iTemp] == ','||line[iTemp]=='\n')
		{
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
			else 
			{
				cout << "Error : In 'candidates' file ; input of partial order" << endl;
				exit(0);
			}
			jTemp = 0;
		}
		else if(isdigit(line[iTemp]))
		{ strEventTemp[jTemp] = line[iTemp];jTemp++;}
		else if(isspace(line[iTemp] == ' '))
		//	jTemp = 0;
		;
		else
		{	 printf("1518 :: ERROR ::: Invalid Character in Candidate Representation.\n"); exit(0); }
	}
	for(iTemp=0;iTemp<size;iTemp++)
		for(jTemp=0;jTemp<size;jTemp++)
			if(ptrAlpha->ppEventMatrix[iTemp][jTemp] == 1)
				for(kTemp=0;kTemp<size;kTemp++)
					if(ptrAlpha->ppEventMatrix[jTemp][kTemp] == 1)
						ptrAlpha->ppEventMatrix[iTemp][kTemp] = 1;
}

void freqEpisodes::parser(string candid)
{
	fstream pfCandid; 
	pfCandid.open(candid.c_str(),ios::in);
	if(pfCandid == NULL)
	{	printf("1459::Error in Reading the Candidates file\n");exit(0); }
	//Initializing the List header
	vector<stEpisodeGel> gelHeads;
	//vector<alpha*> prevPtrs;
	for(int i=0;i<maxSiz;i++)
	{
		stEpisodeGel stCandidateHead;
		stCandidateHead.iEventLength = i+1;
		gelHeads.push_back(stCandidateHead);
		//alpha* temp = NULL;
		//prevPtrs.push_back(temp);
	}
	int numCandids;
	pfCandid >> numCandids;
	for(int canIter=0;canIter<numCandids;canIter++)
	{
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
	for(int i=0;i<gelHeads.size();i++)
		findFreqCandids(gelHeads[i]);
	for(int i=0;i<gelHeads.size();i++)
	{
		for(alpha* ptrIter = gelHeads[i].ptrAlpha;ptrIter != NULL;ptrIter=ptrIter->pNext)
		{
			transred(ptrIter->ppEventMatrix,i+1);
			ptrIter->display(i+1);
		}
	}
	pfCandid.close();
}

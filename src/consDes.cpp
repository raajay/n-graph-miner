#include"super.h"
freqEpisodes::freqEpisodes(double expTime1,int tresh1,double entTresh1,int maxLen1,int maxWid1,double fudgeFac1,int maxSiz1,int maxEvents1, char* file,string outfile1,int epi1)	
{
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
freqEpisodes::stEpisodeGel::stEpisodeGel()
{
	iEventLength = 0;
	iEpisodeListLength = 0;
	ptrAlpha = NULL;
	pstTail = NULL;
}

freqEpisodes::alpha::alpha(int len)
{
	flag = false;
	iFrequency = 0;
	initWaits = new bool[len];
	paiEvents = new int[len];
	ppEventMatrix = new bool*[len];
	ppEntropyMatrix = new int*[len];
	//initWaits = new bool[len];
	for(int i=0;i<len;i++)
	{paiEvents[i] =0 ; ppEventMatrix[i] = new bool[len]; ppEntropyMatrix[i] = new int[len];}
	pstBlockStart = NULL;
	automataList = NULL;
	expiryTime = 0.0;
	pNext = NULL;
	blocStart = 0;
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			{ppEventMatrix[i][j] =0;ppEntropyMatrix[i][j] = 0;}
}

void freqEpisodes::alpha::deleteMat(int len)
{
	for(int i=0;i<len;i++)
		{delete []ppEventMatrix[i]; delete []ppEntropyMatrix[i];}
}
freqEpisodes::alpha::~alpha()
{
	delete []paiEvents;
	delete []ppEventMatrix;
	delete []ppEntropyMatrix;
}



freqEpisodes::dummyEpisode::dummyEpisode(int len)
{
	paiEvents = new int[len];
	ppEventMatrix = new bool*[len];
	for(int i=0;i<len;i++)
		{paiEvents[i] = 0 ;ppEventMatrix[i] = new bool[len];}
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			ppEventMatrix[i][j] =0;
			
}
void freqEpisodes::dummyEpisode::deleteMat(int len)
{
	for(int i=0;i<len;i++)
		delete []ppEventMatrix[i];
}
freqEpisodes::dummyEpisode::~dummyEpisode()
{
	delete []paiEvents;
	delete []ppEventMatrix;
}


freqEpisodes::beta::beta(int len)
{
	ptrAlpha = NULL;
	ptrAuto = NULL;
	//q = new bool[len];
	//for(int i=0;i<len;i++)
	//{ q[i] = 0;}
}

freqEpisodes::beta::~beta()
{
	//delete []q;
}

freqEpisodes::automaton::automaton(int len)
{
	pNext = NULL;
	t1 = 0.0;
	q = new bool[len];
	w = new bool[len];
	flag = new bool[len];
	betaAddress = new beta*[len];
	trackMat = new bool*[len];
	for(int i=0;i<len;i++)
	{
		trackMat[i] = new bool[len];
	}
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			trackMat[i][j] = false;
	for(int i=0;i<len;i++)
	{ 
		q[i] = 0; 
		w[i] = 0;
		betaAddress[i] = NULL;
		flag[i] = false;
	}
}

freqEpisodes::automaton::~automaton()
{
	delete []q;
	delete []w;
	delete []flag;
	delete []betaAddress;
}

void freqEpisodes::automaton::deleteMat(int len)
{
	for(int i=0;i<len;i++)
		delete []trackMat[i];
	delete []trackMat;
}


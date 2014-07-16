#include"super.h"
int main(int argc,char* argv[])
{
	if(argc != 6) 
	{	cout<<"usage : miner.exe <datastream> <freqTresh> <entropyTresh> <expiryTime> <output file>"<<endl;exit(0);}
	double expTime=0.01,fudgeFac = 1.0; int freqTresh=1,maxEvents=100,maxSiz=4,maxLength=10,maxWidth=1000;
	double entropyTresh;
	bool parser_flag = false,filter_flag=false;
	char infile[500];
	int episodeType =0;
	string filterOut,outfile;
	fstream in("inputfile");
	cout << "\nINPUT PARAMETERS\n"<<endl;
	strcpy(infile,argv[1]);
    freqTresh = atoi(argv[2]); cout << "FrequencyTresh = "<<freqTresh<<endl;
    entropyTresh = atof(argv[3]); cout <<"Entropy Tresh = "<<entropyTresh<<endl;
    expTime = atof(argv[4]); cout << "Expiry Time = "<<expTime<<endl;
    outfile += argv[5]; cout << "Output File = " << outfile << endl;
    filterOut = outfile;
    filterOut += "_fil"; 
    //cout << "Output with maximality filtering = " << filterOut << endl;
	while(in)
	{
		string str;
		in >> str;
		if(str == "maxLengthSize:")
				{in >> maxLength;cout << "maximum degree in an episode = " << maxLength<<endl;}
		else if(str == "numMaxPaths:")
				{in >> maxWidth; cout << "maximum number of paths = " << maxWidth << endl;}
		else if(str =="maxEvents:")
				{in >> maxEvents;cout << "maxEvents = "<<maxEvents<<endl;}
		else if(str == "maxSize:")
				{in >> maxSiz;cout << "maxSize = "<<maxSiz<<endl;}
		else if(str =="episodeType:")
		{
			string temp; in >> temp;
			if(temp == "parallel") episodeType = 1;
			else if(temp == "serial") episodeType = 2;
		}
		else
			;
	}
	cout<<endl<<endl;
	// expTime = expiry time 
	// maxLength = L_max
	// maxWidth = N_max
	// maxSiz = maximun level of mining
	// maxEvents = total number of events [1,2,... , maxEvents]
	// episodeType = serial(2) / parallel(1) / general(0)
	freqEpisodes theOne(expTime,freqTresh,entropyTresh,maxLength,maxWidth,fudgeFac,maxSiz,maxEvents,infile,outfile,episodeType);
	// parser_flag == false implies full apriori counting
	if(parser_flag == false)
	{
		// the foll func is defined in countingNew.cpp or counting.cpp
		theOne.findFreqEpisodes();
		//theOne.displayFreqEpisodes((char*)outfile.c_str());
		if(filter_flag == true)
		{
			theOne.filter();
			theOne.displayFreqEpisodes((char*)filterOut.c_str());
		}
	}
	// counting only the list of candidates in the text file "candidates".	
	else
	{
		theOne.parser("candidates");
	}
	
}


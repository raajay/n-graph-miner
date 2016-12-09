#include "super.h"
#include "FrequentEpisodes.h"

int main(int argc,char* argv[]) {
    if(argc != 6) {
        cout<<"usage : miner.exe <datastream> <freqTresh> <entropyTresh> <expiryTime> <output file>"<<endl;
        exit(0);
    }

    // expTime = expiry time
    // maxLength = L_max
    // maxWidth = N_max
    // maxSiz = maximun level of mining
    // maxEvents = total number of events [1,2,... , maxEvents]
    // episodeType = serial(2) / parallel(1) / general(0)

    // Expiry time for an occurrence of an episode
    double expTime=0.01;

    double fudgeFac = 1.0;

    // Frequency threshold used for mining at each stage
    int freqTresh = 1;

    // Maximum ID for an even in the stream. Events are identified by integers
    // starting from 1 till <maxEvents>.
    int maxEvents = 100;

    // The maximum size of episodes to mine for
    int maxSiz = 4;

    // Maximum length of episodes (Refer L_max, in the paper)
    int maxLength = 10;

    // Maximum width of episodes (Refer N_max, in the paper)
    int maxWidth = 1000;

    // Type of Episodes. Can take values in ]0-2].
    // 0 - General Episodes
    // 1 - Parallel Episodes
    // 2 - Serial Episodes
    int episodeType = 0;

    // Entropy threshold for episodes (default: 0)
    double entropyTresh = 0;


    bool parser_flag = false;
    bool filter_flag = false;

    char infile[500];


    cout << "\nINPUT PARAMETERS\n"<<endl;
    strcpy(infile,argv[1]);

    freqTresh = atoi(argv[2]);
    cout << "FrequencyTresh = "<<freqTresh<<endl;

    entropyTresh = atof(argv[3]);
    cout <<"Entropy Tresh = "<<entropyTresh<<endl;

    expTime = atof(argv[4]);
    cout << "Expiry Time = "<<expTime<<endl;

    string outfile;
    outfile += argv[5];
    cout << "Output File = " << outfile << endl;

    string filterOut;
    filterOut = outfile;

    filterOut += "_fil";

    // "inputfile" is used to specify parameters to the algorithm.
    fstream in("inputfile");
    while(in) {
        string str;
        in >> str;
        if(str == "maxLengthSize:") {
            in >> maxLength;
            cout << "maximum degree in an episode = " << maxLength<<endl;
        } else if(str == "numMaxPaths:") {
            in >> maxWidth;
            cout << "maximum number of paths = " << maxWidth << endl;
        } else if(str =="maxEvents:") {
            in >> maxEvents;
            cout << "maxEvents = "<<maxEvents<<endl;
        } else if(str == "maxSize:") {
            in >> maxSiz;
            cout << "maxSize = "<<maxSiz<<endl;
        } else if(str =="episodeType:") {
            string temp;
            in >> temp;
            if(temp == "parallel")
                episodeType = 1;
            else if(temp == "serial")
                episodeType = 2;
        } else
            ;
    }
    cout<<endl<<endl;

    FrequentEpisodes theOne(expTime,freqTresh,entropyTresh,maxLength,maxWidth,fudgeFac,maxSiz,maxEvents,infile,outfile,episodeType);
    // parser_flag == false implies full apriori counting
    if(parser_flag == false) {
        // the foll func is defined in countingNew.cpp or counting.cpp
        theOne.findFreqEpisodes();
        //theOne.displayFreqEpisodes((char*)outfile.c_str());
        if(filter_flag == true) {
            theOne.filter();
            theOne.displayFreqEpisodes((char*)filterOut.c_str());
        }
    }
    // counting only the list of candidates in the text file "candidates".
    else {
        theOne.parser("candidates");
    }

}


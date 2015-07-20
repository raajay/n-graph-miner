//
// Created by raajay on 7/19/15.
//

#ifndef N_GRAPH_MINER_READFILE_H
#define N_GRAPH_MINER_READFILE_H
class StreamReader
{
    int* buffer;
    double* time_buffer;
    double time_temp;
    int pos;
    ifstream file_ptr;
    void BufferIn();
    void Reset();
public:
    StreamReader(char *);
    ~StreamReader();
    int getnextevent(double&);
    int GetNextEvent();
    double GetTime();
    void PushBackOne();
};
#endif //N_GRAPH_MINER_READFILE_H

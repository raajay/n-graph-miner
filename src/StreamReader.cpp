//
// Created by raajay on 7/19/15.
//

#include "super.h"
#include "utility.h"
#include "StreamReader.h"

#define BUFFER_SIZE 10000

StreamReader::~StreamReader() {
    file_ptr.close();
    delete []buffer;
    delete []time_buffer;
}


void StreamReader::Reset() {
    for(int i=0; i< BUFFER_SIZE; i++) {
        buffer[i]=-1;
        time_buffer[i]=0;
    }
}


StreamReader::StreamReader(char* filename) {
    file_ptr.open(filename,ios::in);
    if(!file_ptr) errorAbort("ERROR :: Input file not read properly ");

    pos = BUFFER_SIZE;
    buffer = new int[BUFFER_SIZE];
    time_buffer = new double[BUFFER_SIZE];
}


void StreamReader::BufferIn() {
    Reset();
    for(int i=0; i< BUFFER_SIZE; i++)
        if(file_ptr) {
            char dummy;
            file_ptr >> buffer[i];
            file_ptr >> dummy;
            file_ptr >> time_buffer[i];
        } else break;
    pos = 0;
}


int StreamReader::getnextevent(double &time) {
    if(pos == BUFFER_SIZE)
        BufferIn();
    time_temp = time_buffer[pos];
    time = time_buffer[pos];
    return buffer[pos++];
}


int StreamReader::GetNextEvent() {
    if(pos == BUFFER_SIZE)
        BufferIn();
    time_temp = time_buffer[pos];
    return buffer[pos++];
}


double StreamReader::GetTime() {
    return time_temp;
}


void StreamReader::PushBackOne() {
    pos--;
}
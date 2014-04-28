#include"super.h"
#define BUFSIZE 10000
readfile::~readfile()
{
	fptr.close();
	delete []buffer;
	delete []timebuffer;
}
void readfile::reset()
{
	for(int i=0;i<BUFSIZE;i++)
	{	buffer[i]=-1;timebuffer[i]=0;}
}
readfile::readfile(char* filename)
{
	fptr.open(filename,ios::in);
	if(!fptr) errorAbort("ERROR :: Input file not read properly ");
	
	pos = BUFSIZE;
	buffer = new int[BUFSIZE];
	timebuffer = new double[BUFSIZE];
}
void readfile::buffin()
{
	reset();
	for(int i=0;i<BUFSIZE;i++)
		if(fptr)
		{	
			char dummy;
			fptr >> buffer[i];
			fptr >> dummy;
			fptr >> timebuffer[i];
		}
		else break;
	pos = 0;
}
int readfile::getnextevent(double &time)
{	
	if(pos == BUFSIZE)
		buffin();
	timetemp = timebuffer[pos];
	time = timebuffer[pos];
	return buffer[pos++];
}
int readfile::getnextevent()
{	
	if(pos == BUFSIZE)
		buffin();
	timetemp = timebuffer[pos];
	return buffer[pos++];
}
double readfile::getTime()
{
	return timetemp;
}
void readfile::pushBackOne()
{
	pos--;
}

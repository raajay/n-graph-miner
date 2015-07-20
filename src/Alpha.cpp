//
// Created by raajay on 7/19/15.
//
#include "super.h"
#include "Alpha.h"

alpha::alpha(int len)
{
    flag = false;
    iFrequency = 0;
    initWaits = new bool[len];
    paiEvents = new int[len];
    ppEventMatrix = new bool*[len];
    ppEntropyMatrix = new int*[len];
    //initWaits = new bool[len];
    for(int i=0;i<len;i++) {
        paiEvents[i] = 0;
        ppEventMatrix[i] = new bool[len];
        ppEntropyMatrix[i] = new int[len];
    }
    pstBlockStart = NULL;
    automataList = NULL;
    expiryTime = 0.0;
    pNext = NULL;
    blocStart = 0;
    for(int i=0;i<len;i++) {
        for (int j = 0; j < len; j++) {
            ppEventMatrix[i][j] = 0;
            ppEntropyMatrix[i][j] = 0;
        }
    }
}

void alpha::deleteMat(int len)
{
    for(int i=0;i<len;i++) {
        delete[]ppEventMatrix[i];
        delete[]ppEntropyMatrix[i];
    }
}


alpha::~alpha()
{
    delete []paiEvents;
    delete []ppEventMatrix;
    delete []ppEntropyMatrix;
}

void alpha::display(int len)
{
    for(int i=0;i<len;i++)
        cout << paiEvents[i]<<" ";
    cout << "\t: ";
    cout << iFrequency<<"\t: ";
    cout << entropy<<"\t: ";
    for(int i=0;i<len;i++)
    {
        for(int j=0;j<len;j++)
            if(ppEventMatrix[i][j])
                cout<<paiEvents[i]<<"->"<<paiEvents[j]<<" ";
    }
    cout << endl;
}
void alpha::display(fstream& out,int len)
{
    for(int i=0;i<len;i++)
        out << paiEvents[i]<<" ";
    out << ".\t: ";
    out << iFrequency<<"\t: ";
    out << entropy << "\t:. ";
    //out << pstBlockStart<<"\t:. ";
    //transredMatDisp(out,len);
#if 1
    for(int i=0;i<len;i++)
    {
        for(int j=0;j<len;j++)
            if(ppEventMatrix[i][j])
                out<<paiEvents[i]<<">"<<paiEvents[j]<<",";
    }
#endif
    out << endl;
}

double alpha::calculate_entropy(int len)
{
    int** entMat = ppEntropyMatrix;
    bool** adjMat = ppEventMatrix;
    int iTemp,jTemp;
    double prob;
    double ent = 1.0 ,entTemp = 0.0;
    //*avgent = 0;
    for(iTemp = 0 ; iTemp < len ; iTemp++)
    {
        for(jTemp = iTemp + 1 ; jTemp < len ; jTemp++)
        {
            if(adjMat[iTemp][jTemp] == 0 && adjMat[jTemp][iTemp] == 0 )
            {

                prob = ((double)entMat[iTemp][jTemp])/(entMat[iTemp][jTemp] + entMat[jTemp][iTemp]);
                if(prob != 0 && prob != 1)
                {
                    entTemp = (prob * log(1/prob) + (1-prob) * log(1/(1 - prob)))/log(2);
                    ent = (ent < entTemp) ? ent : entTemp ;
                }
                else
                {
                    return 0.0;
                }
            }

        }
    }
    return ent;
}
void alpha::transredMatDisp(fstream& out,int len)
{
    int i,j,k;
    int **copy;
    copy = new int*[len];
    for(i = 0 ; i < len ; i++)
        copy[i] = new int[len];
    for(i = 0 ; i < len ; i++)
        for(j = 0 ; j < len ; j++)
            copy[i][j] = ppEventMatrix[i][j];
    for(i = 0 ; i < len ; i++)
    {
        for(j = 0 ; j < len; j++)
        {
            if(copy[i][j])
            {
                for(k = 0 ; k < j ; k++)
                    if(ppEventMatrix[i][k] && ppEventMatrix[k][j])
                        copy[i][j] = 0;
                for(k = j+1 ; k < len ; k++)
                    if(ppEventMatrix[i][k] && ppEventMatrix[k][j])
                        copy[i][j] = 0;
            }

        }
    }
    for(int i=0;i<len;i++)
    {
        for(int j=0;j<len;j++)
            if(copy[i][j])
                out<<paiEvents[i]<<">"<<paiEvents[j]<<" ";
    }
    for(i=0;i<len;i++)
        delete []copy[i];
    delete []copy;
}
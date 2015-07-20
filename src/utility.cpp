#include "super.h"
#include "utility.h"


void errorAbort(string s) {cout<<s<<endl;exit(0);}
void errorEcho(string s) {cout << s<< endl;}

void displayVec(bool* v,int len)
{
	for(int i=0;i<len;i++)
		cout << v[i];
	cout << endl;
}

void transred(bool **mat , int len)
{	
	int i,j,k;
	int **copy;
	copy = new int*[len];
	for(i = 0 ; i < len ; i++)
		copy[i] = new int[len];
	for(i = 0 ; i < len ; i++)
		for(j = 0 ; j < len ; j++)
			copy[i][j] = mat[i][j];
	for(i = 0 ; i < len ; i++)
	{
		for(j = 0 ; j < len; j++)
		{
			if(mat[i][j])
			{
				for(k = 0 ; k < j ; k++)
					if(copy[i][k] && copy[k][j]) 
						mat[i][j] = 0;
				for(k = j+1 ; k < len ; k++)
					if(copy[i][k] && copy[k][j]) 
						mat[i][j] = 0;
			}
			
		}
	}
	for(i=0;i<len;i++)
		delete []copy[i];	
	delete []copy;
}
int maxDegree(bool** mat,int len)
{
	bool** copy;
	copy = new bool*[len];
	for(int i = 0;i<len;i++)
		copy[i] = new bool[len];
	for(int i=0;i<len;i++)
		for(int j = 0;j<len;j++)
			copy[i][j] = mat[i][j];
	transred(copy,len);
	int max = 0;
	for(int i=0;i<len;i++)
	{
		int count = 0;
		for(int j=0;j<len;j++)
		{
			if(copy[i][j] == 1) count++;
		}
		if(count > max) max = count;
	}
	for(int i=0;i<len;i++)
	{
		int count = 0;
		for(int j=0;j<len;j++)
		{
			if(copy[j][i] == 1) count++;
		}
		if(count > max) max = count;
	}
	for(int i = 0;i<len;i++)
		delete []copy[i];
	delete []copy;
	return max;
}

int lengthMaxPath(bool** mat,int len)
{
	bool** copy;
	copy = new bool*[len];
	for(int i = 0;i<len;i++)
		copy[i] = new bool[len];
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			copy[i][j] = mat[i][j];
	transred(copy,len);
	vector<int> source;
	vector<int> count;
	int* value = new int[len];
	for(int i=0;i<len;i++)
	{
		bool flag = true;
		for(int j=0;j<len;j++)
		{	
			if(copy[j][i] == 1)	
			{
				flag = false;break;
			}
		}
		if(flag == true) source.push_back(i);
		value[i] = -1;
	}
	for(int i=0;i<source.size();i++)
	{
		count.push_back(pathFinder(source[i],copy,value,len));
	}
	for(int i= 0;i<len;i++)
		delete []copy[i];
	delete []copy;
	delete []value;
	return (*max_element(count.begin(),count.end()));
}

int pathFinder(int source,bool** mat,int* value,int len)
{
	int max = 0;
	for(int i=0;i<len;i++)
	{
		if(mat[source][i] ==1)
		{
			int f;
			if(value[i] >= 0)
				f = 1 + value[i];
			else
				f = 1 + pathFinder(i,mat,value,len);
			if(f > max)
				max = f;
		}
	}
	value[source] = max;
	return max;
}


int numMaxPath(bool** mat,int len)
{
	bool** copy;
	copy = new bool*[len];
	for(int i = 0;i<len;i++)
		copy[i] = new bool[len];
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			copy[i][j] = mat[i][j];
	transred(copy,len);
	vector<int> source;
	int ret = 0;
	int* value = new int[len];
	for(int i=0;i<len;i++)
	{
		bool flag = true;
		for(int j=0;j<len;j++)
		{	
			if(copy[j][i] == 1)	
			{
				flag = false;break;
			}
		}
		if(flag == true) source.push_back(i);
		value[i] = -1;
	}
	for(int i=0;i<source.size();i++)
	{
		ret = ret + (pathFinder2(source[i],copy,value,len));
	}
	for(int i= 0;i<len;i++)
		delete []copy[i];
	delete []copy;
	delete []value;
	return ret;
}

int pathFinder2(int source,bool** mat,int* value,int len)
{
	int max = 0;
	bool flag = false;
	for(int i=0;i<len;i++)
	{
		if(mat[source][i]==1)
		{
			flag = true;
			if(value[i] >= 0)
				max = max + value[i];
			else
				max = max + pathFinder2(i,mat,value,len);	
		}
	}
	if(flag == true)
		return max;
	else 
		return 1;
}

int compare_containment(bool **a , bool **b, int eventLength)
{
	int i,j;
	int flag = 0;
	for(i = 0 ; i < eventLength ; i++)
	{
		for(j=0;j<eventLength;j++)
		{
			if(a[i][j] == 1 && b[i][j] == 0)
			{
				if(flag == -1) {flag = 2; break;}
				else { flag = 1; }
			}
			if(a[i][j] == 0 && b[i][j] == 1)
			{
				if(flag == 1) {flag = 2; break;}
				else { flag = -1 ;}
			}
		}
		if(flag == 2) {flag =0; break;}
	} 
	return flag;

}

int compare_event(int *a , int *b , int eventLength)
{
	int flag = 1;int i = 0;
	for(i=0;i<eventLength;i++,a++,b++)
	{
		if(*a != *b) {flag = 0 ; break;}
	}
	return flag;

}


bool chkEvent(int *array, int event,int len)
{
	bool flag = false;
	for(int i = 0;i<len;i++)
		if(array[i] == event) {flag = true; break;}
	return flag;
}


void initW(bool* w,bool ** adjMat,int len)
{
	for(int i=0;i<len;i++)
	{
		bool flag = true;
		for(int j=0;j<len;j++)
			if(adjMat[j][i] == true) {flag = false;break;}
		if(flag == true) w[i] = true;
		else w[i] = false;
	}
}


void relinkDelBeta(beta*& head,beta* ptrBeta)
{
	if(ptrBeta->pPrev == NULL) head = ptrBeta->pNext;
	else ptrBeta->pPrev->pNext = ptrBeta->pNext;
	if(ptrBeta->pNext) ptrBeta->pNext->pPrev = ptrBeta->pPrev;
}


void addAutoAtHead(alpha* ptrAlpha, automaton* node)
{
	node->pNext = ptrAlpha->automataList;
	ptrAlpha->automataList = node;
}


void addBetaAtHead(beta*& head, beta* node)
{
	node->pNext = head;
	head = node;
	if(node->pNext) node->pNext->pPrev = node;
}


bool compareQ(bool* q1,bool* q2,int len)
{
	bool flag = true;
	for(int i=0;i<len;i++)
		if(q1[i] != q2[i]) {flag = false; break;}
	return flag;
}


void assignVec(bool* source, bool* dest,int len)
{
	for(int i=0;i<len;i++)
		dest[i] = source[i];
}


void assignQNull(bool* dest,int len)
{
	for(int i=0;i<len;i++)
		dest[i] = 0;
}


bool checkQNull(bool* q,int len)
{
	bool flag = true;
	for(int i=0;i<len;i++)
		if(q[i] != 0) { flag = false; break;}
	return flag;
}


bool checkQallOne(bool* q,int len)
{
	bool flag = true;
	for(int i=0;i<len;i++)
		if(q[i] != 1) { flag = false; break;}
	return flag;
}


bool checkVecSame(bool* a,bool* b,int len)
{
	bool flag = true;
	for(int i=0;i<len;i++)
		if(a[i] != b[i]) {flag = false; break;}
	return flag;
}


void addMatrix(int** entMat,bool** trackMat,int len)
{
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			entMat[i][j] = entMat[i][j] + trackMat[i][j];
}


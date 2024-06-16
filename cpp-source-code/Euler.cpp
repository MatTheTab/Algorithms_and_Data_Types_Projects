#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <conio.h>
#include <vector>
#include <math.h>
#include <processthreadsapi.h>
#include <time.h>


double TableOfSatur[]={0.2,0.4,0.8,0.9};
int Nsatur=4;

using namespace std;

struct NLt
{
	int N;
	int * neighb;
	bool visited;
};

//for graph bulding and control

NLt* GenGraph(int n, double saturation,int &edges); //NList generate
bool call_topological_search(int n, NLt* graph, vector<int>& stack);
void topological_search(int n,vector <int>& stack,NLt* graph,int val_to_process);
bool checkIfEven(NLt*graph, int n);
bool IsConnectivity(NLt*graph,int n);
bool checkConnectivity(NLt* grpah,int n,vector<int>& stack);
void add(NLt & val,int vnext);
bool connected(NLt val,int randnext);
bool even(NLt val);
double get_cpu_time();

//load data and save data (NList)
NLt* LoadNList(int n,int saturatonIdx, int &edges);
void SaveNList(char *name,NLt* graph,int n);

//conv form NList to NMatrx
bool** NList2NMatrix(NLt* inGraph,int n);

//printing
 void PrintNLt(NLt* NLtTab,int n);   //print NList
 void PrintNMatrix(bool **NMatrix,int n); //print NMatrix
 void PrintPath(vector <int> &path);
//hamilton
 bool canAdd(int vertex, bool **graph,int pos,vector <int> &path);
 bool hamCycleFind(bool **graph,int n,vector <int>&path, int pos);
 bool hamiltonCycle(bool **graph,int n,	vector <int> & path);


//euler
bool EulerCycle(vector<int>&cycle);
void Euler(int ver,NLt* lista, vector<int>&cycle);
void removeV(NLt* lista,int ver,int tmp);


//adding vertex
void add(NLt & val,int vnext)
{
  try
  {
  int *tmp=new int [val.N+1];
  for (int i=0;i<val.N;i++)
	 tmp[i]=val.neighb[i];
  tmp[val.N]=vnext;
  delete [] val.neighb;
  val.neighb=tmp;
  val.N++;
}
 catch(...)
 {
 	cout<<"alloc"<<endl;
 	getch();
 }
}


//is connected
bool connected(NLt val,int randnext)
{
  for(int i=0;i<val.N;i++)
	if(val.neighb[i]==randnext)
	  return true;
 return false;
}

//is even (number of edges is even)
bool even(NLt val)
{
  if(val.N%2==0) return true;

  return false;
}
NLt* GenGraph2(int n,double saturation,int & edges)
{
   int Nedges=saturation*n*(n-1)/2;
  NLt *NLtTable=new NLt[n];
  edges=0;
  for(int i=0;i<n;i++)
   {
	NLtTable[i].N=0;
	NLtTable[i].neighb=NULL;
	NLtTable[i].visited=false;
   }
   int k;
   for(int v=0;v<n-1;v++)
   {
		add(NLtTable[v],v+1);
		add(NLtTable[v+1],v);
		edges++;
   }
        add(NLtTable[0],n-1);
		add(NLtTable[n-1],0);
		edges++;
	//	return NLtTable;
		
   int r1,r2,r3;



   while (edges< Nedges)
   {
	r1=rand()%n;
	r2=rand()%n;
	r3=rand()%n;
	if(!connected(NLtTable[r1],r2) && !connected(NLtTable[r1],r3) &&
	!connected(NLtTable[r3],r2))
	{
	add(NLtTable[r1],r2);
	add(NLtTable[r2],r1);
	add(NLtTable[r1],r3);
	add(NLtTable[r3],r1);
	add(NLtTable[r3],r2);
	add(NLtTable[r2],r3);
	edges+=3;
	}

   }
 
   
 return NLtTable;
}



NLt* GenGraph3(int n,double saturation,int & edges)
{
   int Nedges=saturation*n*(n-1)/2;
  NLt *NLtTable=new NLt[n];
  edges=0;
  for(int i=0;i<n;i++)
   {
	NLtTable[i].N=0;
	NLtTable[i].neighb=NULL;
	NLtTable[i].visited=false;
   }
   int k;
   int start=0;
   int *vertex=new int [n-1];
   int NN=n-1;
   for (int i=1;i<n;i++) vertex[i-1]=i;
   int los;   
   for(int v=0;v<n;v++)
   {
	    if(v<n-1)
	    {
		los=rand()%NN;
	    add(NLtTable[start],vertex[los]);
		add(NLtTable[vertex[los]],start);
		start=vertex[los];
		for (int i=los;i<(NN-1);i++)
		    vertex[i]=vertex[i+1];
		NN--;	
		edges++;
	   }
	   else
	   {
	   add(NLtTable[vertex[0]],0);
	   add(NLtTable[0],vertex[0]);
	   edges++;
      }
   }
   //return NLtTable;
   int r1,r2,r3;



   while (edges< Nedges)
   {
	r1=rand()%n;
	r2=rand()%n;
	r3=rand()%n;
	if(!connected(NLtTable[r1],r2) && !connected(NLtTable[r1],r3) &&
	!connected(NLtTable[r3],r2))
	{
	add(NLtTable[r1],r2);
	add(NLtTable[r2],r1);
	add(NLtTable[r1],r3);
	add(NLtTable[r3],r1);
	add(NLtTable[r3],r2);
	add(NLtTable[r2],r3);
	edges+=3;
	}

   }
 
   
 return NLtTable;
}

//generation of graph - list of neighb.
NLt* GenGraph(int n, double saturation,int &edges)
{

  int Nedges=saturation*n*(n-1)/2;
  NLt *NLtTable=new NLt[n];
  edges=0;
  for(int i=0;i<n;i++)
   {
	NLtTable[i].N=0;
	NLtTable[i].neighb=NULL;
	NLtTable[i].visited=false;
   }
   int k;
   for(int v=0;v<n-1;v++)
   {
		add(NLtTable[v],v+1);
		add(NLtTable[v+1],v);
		edges++;
   }

   int randv,randvnext;

   randv=0;
   while (edges< Nedges)
   {

	randvnext=rand()%n;

	if(randv==randvnext)
	 continue;
	if(connected(NLtTable[randv],randvnext))
	  continue;


	add(NLtTable[randv],randvnext);
	add(NLtTable[randvnext],randv);
	edges++;
	if(!even(NLtTable[randvnext]))
	   {
		 randv=randvnext;
	   }
	  else
	   {
		 if(!even(NLtTable[randv]))
		   randv=randv;
		 else
		  randv=rand()%n;
	   }
   }
  //find odd vertex and repair it (change connection i even vertex)
   int v=0,v2=0;

   for(v=0;v<n;v++)
	{
	 if(NLtTable[v].N %2!=0)
		for (v2=v+1;v2<n;v2++)
		  if(!connected(NLtTable[v],v2))
		   {
            add(NLtTable[v],v2);
			NLtTable[v2].neighb[NLtTable[v2].N-1]=v;
			break;
		   }
	}
 return NLtTable;
}


bool checkIfEven(NLt*graph, int n)
{
	for (int i=0;i<n;i++)
	 {
	   if(graph[i].N%2!=0)
		 return false;

	 }
   return true;
}

 //save list of neighbour
 void SaveNList(char *name,NLt* graph,int n)
 {
   ofstream out;
   out.open(name);
   for (int i=0;i<n;i++)
	 {
	   out<<graph[i].N<<" ";
	   for (int j=0;j<graph[i].N;j++)
		 out<<graph[i].neighb[j]<<" ";
	   out<<endl;
	 }
	out.close();
   }


  bool IsConnectivity(NLt*graph,int n)
  {
   bool res=false;
   for (int i=0;i<n;i++)
	 if(graph[i].visited==0)
	  {
	   return false;
	  }

   return true;
  }

 //for test of graph
void topological_search(int n,vector <int>& stack,NLt* graph,int val_to_process)
{
  graph[val_to_process].visited=true;     //graf nieskierowany!!!

  if(graph[val_to_process].N==0)
  {
   if(graph[val_to_process].visited==false)
	{stack.push_back(val_to_process);
	graph[val_to_process].visited=true;
	return;  }
  }
  int pot;
  for (int i=0;i<graph[val_to_process].N;i++)
	{
	  pot=graph[val_to_process].neighb[i];
	  if(graph[pot].N==0)
	   {
		 if(graph[pot].visited==false)
		  {stack.push_back(pot);
		  graph[pot].visited=true;
		  }
		continue;
	   }
	  pot=graph[val_to_process].neighb[i];
	  if(graph[pot].visited==false)
	   topological_search(n,stack,graph,graph[val_to_process].neighb[i]);

	 }
	 if(graph[val_to_process].visited==false)
		  {stack.push_back(val_to_process);
		  graph[val_to_process].visited=true;
		  }

}

bool call_topological_search(int n, NLt* graph, vector<int>& stack)
{
  bool allVisited=false;
  topological_search(n,stack,graph,0);
  allVisited=IsConnectivity(graph,n);
  return allVisited;
}

bool checkConnectivity( NLt* graph,int n,vector<int>& stack)
{
	for(int i=0;i<n;i++)
       graph[i].visited=false;
	return call_topological_search(n,graph,stack);
}
//--------------------------------------------------------------------

//load neighb. list

NLt* LoadNList(int n,int saturatonIdx, int &edges)
{
   char buffor[256];
   double saturation=TableOfSatur[saturatonIdx];
   sprintf(buffor,"Graph_%05d_%03d.txt",n,(int)(ceil(saturation*100)));
   //cout<<n<<" "<<saturation<<endl;
   ifstream in;
   in.open(buffor);
   if(!in.is_open())
    {
      cout<<"Problem"<<endl;
      getch();
      return NULL;
	}
   edges=0;

   NLt *NLtTable=new NLt[n];
  for(int i=0;i<n;i++)
   {
	NLtTable[i].N=0;
	NLtTable[i].neighb=NULL;
	NLtTable[i].visited=false;
   }
    try
    {
   for (int i=0;i<n;i++)
	 {
	   in>>NLtTable[i].N;
	   NLtTable[i].neighb=new int[NLtTable[i].N];
	   for (int j=0;j<NLtTable[i].N;j++)
	   {
		 in>>NLtTable[i].neighb[j];
		 edges++;
	   }
	 }
   }
   catch(...)
   {
   	cout<<"allocErr"<<endl;
   	getch();
   }
	in.close();


 return NLtTable;
}

 //print list of neighb.
 void PrintNLt(NLt* NLtTab,int n)
 {
	for(int i=0;i<n;i++)
	{
	 cout<<i<<" > ("<<NLtTab[i].N<<")"<<"->";
	 for (int j=0;j< NLtTab[i].N;j++)
	   cout<<NLtTab[i].neighb[j]<<" ";
	cout<<endl;
	}
   cout<<"***********************"<<endl;
 }


void showIfErr(bool r1,bool r2)
{
	if(!r1)
	  cout<<"Graph not fully connected!"<<endl;
	if(!r2)
	   cout<<"Graph with odd num of edges in some vertex"<<endl;
}

 //HAMILTON CYCLE

 bool canAdd(int vertex, bool **graph,int pos,vector <int> &path)
{
//can add vertex to path

	//is vertex neighb. of prev. added vertex
	if (graph [path[pos - 1]][ vertex ] == 0)
		return false;

	//vertex is in path???
    for (int i = 0; i < pos; i++)
		if (path[i] == vertex)
            return false;

    return true;
}

bool hamCycleFind(bool **graph,int n,vector <int>&path, int pos)
{

	if (pos == n)
	{
        if (graph[path[pos - 1]][path[0]] == 1)
            return true;
        else
            return false;
    }

	for (int v = 1; v < n; v++)
	{
		if (canAdd(v, graph,pos, path))
        {
            path[pos] = v;


			if (hamCycleFind (graph, n, path, pos + 1) == true)
                return true;

            path[pos] = -1;
        }
    }

    return false;
}

bool hamiltonCycle(bool **graph,int n,	vector <int> & path)
{

	for (int i = 0; i < n; i++)
		path.push_back(-1);

	path[0] = 0;
	if (hamCycleFind(graph, n,path, 1) == false )
	{
        return false;
    }

    return true;
}


bool canAdd2(int vertex, NLt* graph,int pos,vector <int> &path)
{
//can add vertex to path

	//is vertex neighb. of prev. added vertex
	//if (graph [path[pos - 1]][ vertex ] == 0)
	if(!connected(graph[vertex],path[pos-1]))
		return false;

	//vertex is in path???
    for (int i = 0; i < pos; i++)
		if (path[i] == vertex)
            return false;

    return true;
}

bool hamCycleFind2(NLt *graph,int n,vector <int>&path, int pos,int start,int timeOut,bool &TOut)
{
	if (pos == n)
	{
       // if (graph[path[pos - 1]][path[0]] == 1)
        if(connected(graph[path[pos - 1]],path[0]))
            return true;
        else
            return false;
    }
     
    if ((get_cpu_time()-start)>timeOut)
    {
	  TOut=true;
      return false;
  }
	 
	for (int v = 1; v < n; v++)
	{
		if (canAdd2(v, graph,pos, path))
        {
            path[pos] = v;
			if (hamCycleFind2(graph, n, path, pos + 1,start,timeOut,TOut) == true)
                return true;
            path[pos] = -1;
        }
    }

    return false;
}

bool hamiltonCycle2(NLt *graph,int n,vector <int> & path,int start,int timeOut,bool &TOut)
{

	for (int i = 0; i < n; i++)
		path.push_back(-1);
    TOut=false;
	path[0] = 0;
	if (hamCycleFind2(graph, n,path, 1,start,timeOut,TOut) == false )
	{
        return false;
    }
    path.push_back(0);
    return true;
}

 //EULER CYCLE
 void removeV(NLt* lista,int ver,int tmp)
{
  if(lista[ver].N<=0) return;
  for(int i=0;i<lista[ver].N;i++)
	   if (lista[ver].neighb[i]==tmp)
		{
			 for (int j=i;j<lista[ver].N-1;j++)
					  lista[ver].neighb[j]=lista[ver].neighb[j+1];
			 lista[ver].N--;
			 break;
		}
   for(int i=0;i<lista[tmp].N;i++)
	   if (lista[tmp].neighb[i]==ver)
		{
			 for (int j=i;j<lista[tmp].N-1;j++)
					  lista[tmp].neighb[j]=lista[tmp].neighb[j+1];
			 lista[tmp].N--;
			 break;
		}
}



bool Euler(int ver,NLt* lista, vector<int>&cycle,double start,int TOut=300000)
{
  int tmp=0;
  if((get_cpu_time()-start)>TOut) 
     return false;
  for(int i=0;i<lista[ver].N;i++)
	   {
		  tmp=lista[ver].neighb[i];
		  removeV(lista,ver,tmp);
		  Euler(tmp,lista,cycle,start);
	   }
	  cycle.push_back(ver);
	  return true;
}

bool EulerCycle(vector<int>&cycle)
{
	if(cycle[0]==cycle[cycle.size()-1])
	  return true;
	return false;
}


//convertion Nlist to NMatrix - for Hamilton cycle
bool** NList2NMatrix(NLt* inGraph,int n)
{
   bool **NMatrix;
   NMatrix =new bool*[n];
   for (int i=0;i<n;i++)
	 NMatrix[i]=new bool[n];

    for (int i=0;i<n;i++)
	 {
		for (int j=0;j<n;j++)
		  {
			  NMatrix[i][j]=0;
		  }
	 }

   for (int i=0;i<n;i++)
	 {
		for (int j=0;j<inGraph[i].N;j++)
		  {
			  NMatrix[i][inGraph[i].neighb[j]]=1;
		  }
	 }

   return NMatrix;
}

 void PrintNMatrix(bool **NMatrix,int n)
 {
	 for (int i=0;i<n;i++)
	  {
	   for(int j=0;j<n;j++)
		 cout<<NMatrix[i][j]<<" ";
	   cout<<endl;
	  }
 }

 void PrintPath(vector <int> &path)
 {
   for(int i=0;i<path.size();i++)
	 cout<< path[i]<<"-";
   cout<<endl;
 }



const int FROM=100;
const int TO=500;
const int STEP=100;

int main(void)
{

 
 
 double saturation;
 int edges,n;

 NLt *NLtTab=NULL;
 bool **NMatrix=NULL;

 vector <int> stack;  //for graph topology
 char buffor[256];

 bool result,result1;
 vector <int> path;  //hamilton cycle and euler cycle


       double start=0,stop=0,elapsed=0,overAll=0;
	   int path_exists=0;

	   ofstream val_file2;
	   val_file2.open("Results_Euler_Ewaluation.txt");
	   int TOCounts;
	    srand(time(NULL));
 for(int i=FROM;i<=TO;i+=STEP)
	 {
	  n=i;
	 
	  //***************************************
	  //Graph bulding
	  //*****************************************
	 for (int isatur=0;isatur<Nsatur;isatur++)
	   {
        
	   saturation=TableOfSatur[isatur];

       cout<<"Graph. n="<<n<<" saturation="<<saturation<<endl;
	   
	  // result=checkConnectivity(NLtTab, n,stack);
	  // result1=checkIfEven(NLtTab, n);
	  // showIfErr(result,result1);
	   // PrintNLt(NLtTab,n);
	   // getch();

	   //***************************************************************
	   //simulations
	   //**************************************************************
	   bool notTimeOut=false;
	   elapsed=0;
	   overAll=0;
	   TOCounts=0;
	 
	   int count=0;
	   while(true)
	   {
	   
	   NLtTab=GenGraph3(n, saturation, edges);
       cout<<"START"<<count++<<endl;
	   start=get_cpu_time();
	  // for(int ii=0;ii<=100;ii++){
	   	//hamiltonCycle2(NLtTab,n,path,start,100000,notTimeOut);
	   	if(Euler(0,NLtTab, path,start)) break;
	//	}
        }
	   	stop=get_cpu_time();
	    cout<<"STOP"<<endl;
	   	if(!notTimeOut)
	    	elapsed+=stop-start; //without time outs
	    else
	    {
		  cout<<"Time out"<<endl;
		  TOCounts++;
	    }
	    overAll+=stop-start; //with time outs if any	
		path.clear();
		for (int k=0;k<n;k++)
		  delete[] NLtTab[k].neighb;
		delete[] NLtTab; 	NLtTab=NULL;
		cout<<"Euler cycle ready"<<endl;
		//*********************************************************	
		cout<<"Graph. n="<<n<<" saturation="<<saturation<<endl;
		 count=0;
	   while(true)
	   {
	   	
	   NLtTab=GenGraph3(n, saturation, edges);
        cout<<"START"<<count++<<endl;
	   start=get_cpu_time();
	  
	   	if(Euler(0,NLtTab, path,start)) break;
        }
	   	stop=get_cpu_time();
	   	 cout<<"STOP"<<endl;
	   	if(!notTimeOut)
	   	{
	    	elapsed+=stop-start; //without time outs
	    	cout<<"Euler cycle ready"<<endl;
	    }
	    else 	
	    {
		  cout<<"Time out"<<endl;
		  TOCounts++;
	    }
	    overAll+=stop-start; //with time outs if any	
		path.clear();
		for (int k=0;k<n;k++)
		  delete[] NLtTab[k].neighb;
		delete[] NLtTab; 	NLtTab=NULL;
	
		//*********************************************************	
		cout<<"Graph. n="<<n<<" saturation="<<saturation<<endl;
			 count=0;
	   while(true)
	   {
	   	
	   NLtTab=GenGraph3(n, saturation, edges);
	     cout<<"START"<<count++<<endl;
      
	   start=get_cpu_time();
	  
	   	if(Euler(0,NLtTab, path,start)) break;
        }
	   	stop=get_cpu_time();
	   	 cout<<"STOP"<<endl;
	   	if(!notTimeOut)
	   	{
	    	elapsed+=stop-start; //without time outs
	    	cout<<"Euler cycle ready"<<endl;
    	}
	    else
		  {
		  cout<<"Time out"<<endl;
		  TOCounts++;
	    }
	    overAll+=stop-start; //with time outs if any	
		path.clear();
		for (int k=0;k<n;k++)
		  delete[] NLtTab[k].neighb;
		delete[] NLtTab; 	NLtTab=NULL;
	
		
		//*********************************************************	
	   cout<<"Graph. n="<<n<<" saturation="<<saturation<<endl;
	   	 count=0;
	   while(true)
	   {
	   NLtTab=GenGraph3(n, saturation, edges);
    cout<<"START"<<count++<<endl;
	   start=get_cpu_time();
	  
	   	if(Euler(0,NLtTab, path,start)) break;
        }
	   	stop=get_cpu_time();
	   	 cout<<"STOP"<<endl;
	   	if(!notTimeOut)
	   	{
	    	elapsed+=stop-start; //without time outs
	    	cout<<"Euler cycle ready"<<endl;
	    }
	    else
		  {
		  cout<<"Time out"<<endl;
		  TOCounts++;
	      }
	    overAll+=stop-start; //with time outs if any	
		path.clear();
		for (int k=0;k<n;k++)
		  delete[] NLtTab[k].neighb;
		delete[] NLtTab; 	NLtTab=NULL;

		//*********************************************************	
		cout<<"Graph. n="<<n<<" saturation="<<saturation<<endl;
			 count=0;
	   while(true)
	   {
	   NLtTab=GenGraph3(n, saturation, edges);
     cout<<"START"<<count++<<endl;
	   start=get_cpu_time();
	  
	   	if(Euler(0,NLtTab, path,start)) break;
        }
	   	stop=get_cpu_time();
	   	 cout<<"STOP"<<endl;
	   	if(!notTimeOut)
	   	  {
	    	elapsed+=stop-start; //without time outs
	    	cout<<"Euler cycle ready"<<endl;
	    }
	    else
		  {
		  cout<<"Time out"<<endl;
		  TOCounts++;
		  }
	    overAll+=stop-start; //with time outs if any	
		path.clear();
		for (int k=0;k<n;k++)
		  delete[] NLtTab[k].neighb;
		delete[] NLtTab; 	NLtTab=NULL;
		
		//*********************************************************	
		cout<<"Graph. n="<<n<<" saturation="<<saturation<<endl;
			 count=0;
	   while(true)
	   {
	   NLtTab=GenGraph3(n, saturation, edges);
        cout<<"START"<<count++<<endl;
	   start=get_cpu_time();
	   
	   	if(Euler(0,NLtTab, path,start)) break;
        }
	   	stop=get_cpu_time();
	   	 cout<<"STOP"<<endl;
	   	if(!notTimeOut)
	   	{
	    	cout<<"Euler cycle ready"<<endl;
			elapsed+=stop-start; //without time outs
	    }
	    else
		  {
		  cout<<"Time out"<<endl;
		  TOCounts++;
	      }
	    overAll+=stop-start; //with time outs if any	
		path.clear();
		for (int k=0;k<n;k++)
		  delete[] NLtTab[k].neighb;
		delete[] NLtTab; 	NLtTab=NULL;
	
		//*********************************************************	
		cout<<"Graph. n="<<n<<" saturation="<<saturation<<endl;
			 count=0;
	   while(true)
	   {
	   NLtTab=GenGraph3(n, saturation, edges);
     cout<<"START"<<count++<<endl;
	   start=get_cpu_time();
	  
	   	if(Euler(0,NLtTab, path,start)) break;
        }
	   	stop=get_cpu_time();
	   	 cout<<"STOP"<<endl;
	   	if(!notTimeOut)
	   	{
	    	elapsed+=stop-start; //without time outs
	    	cout<<"Euler cycle ready"<<endl;
	}
	    else
		  {
		  cout<<"Time out"<<endl;
		  TOCounts++;
	    }
	    overAll+=stop-start; //with time outs if any	
		path.clear();
		for (int k=0;k<n;k++)
		  delete[] NLtTab[k].neighb;
		delete[] NLtTab; 	NLtTab=NULL;
		
		//*********************************************************	
		cout<<"Graph. n="<<n<<" saturation="<<saturation<<endl;
			 count=0;
	   while(true)
	   {
	   NLtTab=GenGraph3(n, saturation, edges);
     cout<<"START"<<count++<<endl;
	   start=get_cpu_time();
	  
	   	if(Euler(0,NLtTab, path,start)) break;
        }
	   	stop=get_cpu_time();
	   	 cout<<"STOP"<<endl;
	   	if(!notTimeOut)
	   	{
	    	elapsed+=stop-start; //without time outs
	    	cout<<"Euler cycle ready"<<endl;
    	}
	    else
		  {
		  cout<<"Time out"<<endl;
		  TOCounts++;
	    }
	    overAll+=stop-start; //with time outs if any	
		path.clear();
		for (int k=0;k<n;k++)
		  delete[] NLtTab[k].neighb;
		delete[] NLtTab; 	NLtTab=NULL;
		
		//*********************************************************	
		cout<<"Graph. n="<<n<<" saturation="<<saturation<<endl;
			 count=0;
	   while(true)
	   {
	   NLtTab=GenGraph3(n, saturation, edges);
       cout<<"START"<<count++<<endl;
	   start=get_cpu_time();
	  
	   	if(Euler(0,NLtTab, path,start)) break;
        }
	   	stop=get_cpu_time();
	   	 cout<<"STOP"<<endl;
	   	if(!notTimeOut)
	   	{
	    	elapsed+=stop-start; //without time outs
	    	cout<<"Euler cycle ready"<<endl;
	    }
	    else
		 {
		  cout<<"Time out"<<endl;
		  TOCounts++;
	    }
	    overAll+=stop-start; //with time outs if any	
		path.clear();
		for (int k=0;k<n;k++)
		  delete[] NLtTab[k].neighb;
		delete[] NLtTab; 	NLtTab=NULL;
		
		//*********************************************************	
		cout<<"Graph. n="<<n<<" saturation="<<saturation<<endl;
			 count=0;
	   while(true)
	   {
	   NLtTab=GenGraph3(n, saturation, edges);
       cout<<"START"<<count++<<endl;
	   start=get_cpu_time();
	  
	   	if(Euler(0,NLtTab, path,start)) break;
        }
	   	stop=get_cpu_time();
	   	 cout<<"STOP"<<endl;
	   	if(!notTimeOut)
	   	{
	    	elapsed+=stop-start; //without time outs
	    	cout<<"Euler cycle ready"<<endl;
	    }
	    else
		  {
		  cout<<"Time out"<<endl;
		  TOCounts++;
	    }
	    overAll+=stop-start; //with time outs if any	
		path.clear();
		for (int k=0;k<n;k++)
		  delete[] NLtTab[k].neighb;
		delete[] NLtTab; 	NLtTab=NULL;
	
		
	   	//PrintPath(path);
	   //	getch();
	    cout<<"********************************"<<endl;
		val_file2<<"n= "<<n<<" saturation= "<<TableOfSatur[isatur]<<" time w.o TO= "
		<<elapsed<<" overAll= "<<overAll<<" Time outs="<<TOCounts<<"\n";
		val_file2.flush();
	   }

	 }


//	val_file1.close();
	val_file2.close();
	cout<<"end";
	getch();
	return 0;
}

double get_cpu_time(){
	//in ms
	FILETIME a,b,c,d;
    if (GetProcessTimes(GetCurrentProcess(),&a,&b,&c,&d) != 0){

        return
            (double)(d.dwLowDateTime |
            (((unsigned long long)d.dwHighDateTime) << 32)) * 0.0001; //ms
    }else{

        return 0;
    }
} 

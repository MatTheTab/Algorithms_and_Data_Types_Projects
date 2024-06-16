#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <conio.h>
#include <processthreadsapi.h>

using namespace std;

const int VALUE_RANGE=51;
const int WEIGHT_RANGE=51;
const int N_CAP=30;
const int B_CAP=1000;
const int N_CAP2=2000;
const int HARD_CAP=2000;
const int WEIGHT_CAP=100;

struct knapsack_item{
	int weight;
	int value;
	double value_density;
};




struct knapsack{
	vector<knapsack_item> elements;
	int total_weight;
	int total_value;
	int max_weight;
};
/*
struct knapsack_V2{
	vector<knapsack_item> elements;
	int total_weight;
	int total_value;
	int max_weight;
};   */

int generate_n_items(knapsack_item*, int);
void Brute_Force(knapsack_item*,int n,int maxWeight,int *listOfItems,int &items,int &optVal,int &optWei);
													//solution ....  number of items optimal values
													
void printSolution(knapsack_item*,int *listOfItems,int items,int optVal,int optWei);
void Decode(unsigned long long regItems,int n,int *tableOfItems,int &nItems);
int WeightCalc(knapsack_item* items,int *tableOfItems,int nItems);
int ValueCalc(knapsack_item* items,int *tableOfItems,int nItems);

void quicksort(knapsack_item* items, int first, int last);
void Greedy(knapsack_item* items,int n, knapsack &final_sack);
void printSolution(knapsack_item*,knapsack &final_sack);
double get_cpu_time();
void Dynamic(knapsack_item* items,knapsack &final_sack, int n);
void Dynamic_with_solution(knapsack_item* items,knapsack &final_sack, int n);


int main(void){
	int tot_weight[1000];
	int z=0;

  //Brute force

  int OptimalListOfItems[64];  //result - table of items
   int nItems; //number of items

  int optimalValue,optimalWeight; //result optimum values

  //greedy
   knapsack final_sack;
   double start=0,stop=0,elapsed=0;


  for(int n=4;n<=N_CAP;n+=1){
	cout<<"Performing step "<<n<<"\n";
	knapsack_item * arr_save;
	arr_save = new knapsack_item [n];
	tot_weight[z]=generate_n_items(arr_save, n);
	z++;
   ofstream out;
   char bufor[100];
   sprintf(bufor,"Nknapsack%06d.txt",n);
   out.open(bufor);
   for (int i=0;i<n;i++){
	 	out<<arr_save[i].value<<" "<<arr_save[i].weight<<"\n";
	}
	out.close();
	delete [] arr_save;
	}
	
	for(int n=50;n<=N_CAP2;n+=50){
	cout<<"Performing step "<<n<<"\n";
	knapsack_item * arr_save;
	arr_save = new knapsack_item [n];
	tot_weight[z]=generate_n_items(arr_save, n);
	z++;
   ofstream out;
   char bufor[100];
   sprintf(bufor,"Nknapsack%06d.txt",n);
   out.open(bufor);
   for (int i=0;i<n;i++){
	 	out<<arr_save[i].value<<" "<<arr_save[i].weight<<"\n";
	}
	out.close();
	delete [] arr_save;
	}

ofstream val_file;
val_file.open("Results_small_n_3");

	z=0;
   for(int n=4;n<=N_CAP;n+=1){
   	int b=tot_weight[z]/4+1;
   	z++;
	cout<<"\nPerforming loading operation, step "<<n<<"\n";
	ifstream in;
   char bufor[100];
   sprintf(bufor,"Nknapsack%06d.txt",n);
   in.open(bufor);
   knapsack_item * arr_load;
   try{

	arr_load = new knapsack_item [n];
	}
    catch(...)
    {
    cout<<"alloc!!";
    return 1;
	}

   for (int i=0;i<n;i++)
	 {
	   in>>arr_load[i].value;
	   in>>arr_load[i].weight;
	   arr_load[i].value_density=(double)arr_load[i].value/(double)arr_load[i].weight;
	 }
	 in.close();
	 cout<<"Brute force"<<endl;
	 start=get_cpu_time();
	 Brute_Force(arr_load,n, b,OptimalListOfItems,nItems,optimalValue,optimalWeight);
	 stop=get_cpu_time();
	 elapsed=stop-start;
	 val_file<<"Brute force "<<" n= "<<n<<" b= "<<b<<" time= "<<elapsed<<" knapsack_value= "<<optimalValue<<"\n";

	 final_sack.max_weight=b;
	 final_sack.total_value=0;
	 final_sack.total_weight=0;
	 final_sack.elements.clear();

	 cout<<"Greedy"<<endl;
	 start=get_cpu_time();
	 Greedy(arr_load,n, final_sack);
	 stop=get_cpu_time();
	 elapsed=stop-start;
	 val_file<<"Greedy "<<" n= "<<n<<" b= "<<b<<" time= "<<elapsed<<" knapsack_value= "<<final_sack.total_value<<"\n";
	 
	 cout<<"Dynamic"<<endl;
	 final_sack.max_weight=b;
	 final_sack.total_value=0;
	 final_sack.total_weight=0;
	 final_sack.elements.clear();
	 start=get_cpu_time();
	  Dynamic(arr_load,final_sack,n);
	  stop=get_cpu_time();
	  elapsed=stop-start;
	  val_file<<"Dynamic "<<" n= "<<n<<" b= "<<b<<" time= "<<elapsed<<" knapsack_value= "<<final_sack.total_value<<"\n";

	 delete [] arr_load;
}
	val_file.close();          
   	ofstream val_file2;
	val_file2.open("Results_big_n_3.txt");

   for(int n=50;n<=N_CAP2;n+=50){
   	int b=tot_weight[z]/16+1;
   	z++;
	cout<<"\nPerforming loading operation, step "<<n<<"\n";
	ifstream in;
   char bufor[100];
   sprintf(bufor,"Nknapsack%06d.txt",n);
   in.open(bufor);
   knapsack_item * arr_load, *tmp_arr;
   try{

	arr_load = new knapsack_item [n];
    tmp_arr= new knapsack_item [n];	

	}
    catch(...)
    {
    cout<<"alloc!!";
    return 1;
	}

   for (int i=0;i<n;i++)
	 {
	   in>>arr_load[i].value;
	   in>>arr_load[i].weight;
	   arr_load[i].value_density=(double)arr_load[i].value/(double)arr_load[i].weight;
	 }
	 	memcpy(tmp_arr,arr_load,sizeof(knapsack_item)*n);
	 in.close();
	 final_sack.max_weight=b;
	 final_sack.total_value=0;
	 final_sack.total_weight=0;
	 final_sack.elements.clear();

	 cout<<"Greedy"<<endl;
	 start=get_cpu_time();
	 for(int i=0;i<10000;i++){
	  final_sack.max_weight=b;
	 final_sack.total_value=0;
	 final_sack.total_weight=0;
	 final_sack.elements.clear();
	 Greedy(arr_load,n, final_sack);
	 memcpy(arr_load,tmp_arr,sizeof(knapsack_item)*n);
}
	 stop=get_cpu_time();
	 elapsed=stop-start;
	 val_file2<<"Greedy "<<" n= "<<n<<" b= "<<b<<" time= "<<elapsed/10000.0<<" knapsack_value= "<<final_sack.total_value<<"\n";

	 cout<<"Dynamic"<<endl;
	 final_sack.max_weight=WEIGHT_CAP;
	 final_sack.total_value=0;
	 final_sack.total_weight=0;
	 final_sack.elements.clear();
	 start=get_cpu_time();
	 for(int i=0;i<200;i++){
	  final_sack.max_weight=b;
	 final_sack.total_value=0;
	 final_sack.total_weight=0;
	 final_sack.elements.clear();
	 memcpy(arr_load,tmp_arr,sizeof(knapsack_item)*n);
	 Dynamic(arr_load,final_sack,n);
}
	  stop=get_cpu_time();
	  elapsed=stop-start;
	  val_file2<<"Dynamic "<<" n= "<<n<<" b= "<<b<<" time= "<<elapsed/200.0<<" knapsack_value= "<<final_sack.total_value<<"\n";

	 delete [] arr_load;
	 delete [] tmp_arr;
}

	
	val_file2.close();
	getch();
	return 0;
}

void printSolution(knapsack_item* items,int *listOfItems,int Nitems,int optVal,int optWei)
{

  cout<<"Optimal value:"<<optVal<<" optimal weight:"<<optWei<<endl;
  cout<<"Items:"<<endl;
  for (int i=0;i<Nitems;i++)
	{
	   cout<<"Item:"<<listOfItems[i]<<" weight:"<<items[listOfItems[i]].weight<<" value:"<<
	   items[listOfItems[i]].value<<endl;
	}
	cout<<endl;
}

void printSolution(knapsack_item* items,knapsack &final_sack)
{

   cout<<"Optimal value:"<<final_sack.total_value<<" optimal weight:"<<final_sack.total_weight<<endl;
  cout<<"Items:"<<endl;
  for (int i=0;i<final_sack.elements.size();i++)
	{
	   cout<<"weight:"<<final_sack.elements[i].weight<<" value:"<<
	   final_sack.elements[i].value<<endl;
	}
	cout<<endl;
}

int generate_n_items(knapsack_item* items, int n){
	int tot_weight=0;
	for(int i=0;i<n;i++){
		items[i].value=rand()%VALUE_RANGE;
		items[i].weight=rand()%WEIGHT_RANGE+1;
		tot_weight+=items[i].weight;
	}
	return tot_weight;
}

void Decode(unsigned long long regItems,int n,int *tableOfItems,int &nItems)
{
   for(int i=0;i<n;i++)
	  tableOfItems[i]=0;
   unsigned long long val=1;
   nItems=0;
   for (int i=0;i<n;i++)
   {
	   if((regItems& val)==val)
		{
		  tableOfItems[nItems]=i;
		  nItems++;

		}
		val=val<<1;
   }

}

int WeightCalc(knapsack_item* items,int *tableOfItems,int nItems)
{
  int weight=0;
  for (int i=0;i<nItems;i++)
   {
	  weight+=items[tableOfItems[i]].weight;
   }
   return weight;
}

int ValueCalc(knapsack_item* items,int *tableOfItems,int nItems)
{
  int value=0;
  for (int i=0;i<nItems;i++)
   {
	  value+=items[tableOfItems[i]].value;
   }
   return value;
}

void Brute_Force(knapsack_item* items,int n,int maxWeight,int *listOfItems,int & NListItems,
				 int &optVal,int &optWei)
{
	if(n>64)
	{
	  cout<<"To many items for bruteforce method"<<endl;
	  return;
    }
	optVal=0;
	int tableOfItems[64]={0};
	unsigned long long int regItems=0;
	unsigned long long int optSolution=0;
	int Value=0;
	int Weight=0;
	int nItems=0;
	unsigned long long numIter=0;
	unsigned long long bt=1;


	for (int i =0;i<n;i++)
	{
	   numIter=numIter | bt;
	   bt=bt<<1;
	 }

   while (regItems<=numIter)
	{
	 Decode(regItems,n,tableOfItems,nItems);
	 Weight=WeightCalc(items,tableOfItems,nItems);
	 if(Weight<=maxWeight)
	   {
		 Value=ValueCalc(items,tableOfItems,nItems);
		 if(optVal<Value)
		  {
			 optVal=Value;
			 optWei=Weight;
			 optSolution=regItems;
          }
	   }
	 regItems++;
	// val++;
	}
		Decode(optSolution,n,listOfItems,NListItems);
  }




void Greedy(knapsack_item* items,int n, knapsack &final_sack){
	quicksort(items,0,n-1);
	for(int i=0;i<n;i++){
		if(items[n-i-1].weight+final_sack.total_weight<=final_sack.max_weight){
		final_sack.elements.push_back(items[n-i-1]);
		final_sack.total_weight+=items[n-1-i].weight;
		final_sack.total_value+=items[n-1-i].value;}
	}
}

void quicksort(knapsack_item* items, int first, int last){
	int lower=first, upper=last;
	double pivot=items[(first+last)/2].value_density;
	while (lower<=upper){
		while(items[lower].value_density<pivot)
			lower++;
		while(pivot<items[upper].value_density)
             upper--;
		if (lower<upper)
			swap(items[lower++],items[upper--]);
		else lower++;
	}
	if (first<upper)
		quicksort(items,first,upper);
	if (upper+1<last)
		quicksort(items,upper+1,last);
}

void swap(knapsack_item &val1, knapsack_item &val2){
	knapsack_item temp_val=val1;
	val1=val2;
	val2=temp_val;
}



void Dynamic(knapsack_item* items,knapsack &final_sack, int n){

	int **arr=new int *[HARD_CAP+1];
	for(int i=0;i<(HARD_CAP+1);i++)
		arr[i]=new int [HARD_CAP+1];

	for(int i=0;i<(HARD_CAP+1);i++)
		for(int j=0;j<(HARD_CAP+1);j++)
		  arr[i][j]=0;


	for(int i=1;i<=n;i++){
		for(int j=1;j<=final_sack.max_weight;j++){
			if(items[i-1].weight<=j)
				arr[i][j]=max(arr[i-1][j],items[i-1].value+arr[i-1][j-items[i-1].weight]);
			else
                arr[i][j]=arr[i-1][j];
		}
	}
	final_sack.total_value=arr[n][final_sack.max_weight];
	for(int i=0;i<(HARD_CAP+1);i++)
      delete [] arr[i];
      delete [] arr;
}

int max(int a, int b){
	return a>b ? a : b;
}
/*
void print_knapsack_item(knapsack_item item){
	cout<<"\nvalue= "<<item.value<<" weight= "<<item.weight<<" density= "<<item.value_density<<"\n";
}

void print_knapsack(knapsack sack, int n){
	cout<<"\nmax weight= "<<sack.max_weight<<"total value= "<<sack.total_value<<" total weight= "<<sack.total_weight<<"items: ";
	for(int i=0;i<n;i++){
	print_knapsack_item(sack.elements[i]);
}
} */

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

void Dynamic_with_solution(knapsack_item* items,knapsack &final_sack, int n){

	int **arr=new int *[HARD_CAP+1];
	for(int i=0;i<(HARD_CAP+1);i++)
		arr[i]=new int [HARD_CAP+1];

	for(int i=0;i<(HARD_CAP+1);i++)
		for(int j=0;j<(HARD_CAP+1);j++)
		  arr[i][j]=0;


	for(int i=1;i<=n;i++){
		for(int j=1;j<=final_sack.max_weight;j++){
			if(items[i-1].weight<=j)
				arr[i][j]=max(arr[i-1][j],items[i-1].value+arr[i-1][j-items[i-1].weight]);
			else
                arr[i][j]=arr[i-1][j];
		}
	}
	final_sack.total_value=arr[n][final_sack.max_weight];
	final_sack.total_weight=0;
	int i=n;
	int j=final_sack.max_weight;
	while(i>0){
		if(arr[i-1][j]==arr[i][j]){
			i=i-1;
		}
		else{
			final_sack.elements.push_back(items[i]);
			final_sack.total_weight+=items[i].weight;
			i=i-1;
			j=j-items[i].weight;
		}
	}
	for(int i=0;i<(HARD_CAP+1);i++)
      delete [] arr[i];
      delete [] arr;
}

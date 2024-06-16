#include <iostream>
# include <stdlib.h>
//#include <time.h>
#include <fstream>
#include <processthreadsapi.h> 

using namespace std;

void swap(int &val1, int &val2);
void bubble_sort(int arr[],int arr_size);
void selection_sort(int arr[],int arr_size);
void insertion_sort(int arr[], int arr_size);
void heap_move(int arr[], int first, int last);
void heapsort(int arr[], int arr_size);
void quicksort(int arr[], int first, int last);
void shellsort(int arr[], int arr_size);
void merge(int arr[], int temp[], int left, int center, int right);
void mergesort(int arr[], int temp[], int left, int right);
void countingsort(int arr[], int arr_size, int out_arr[]);
void print_arr(int arr[], int arr_size);
void generate_data_random(int arr[], int arr_size, int range);
void copy_arr(int arr1[], int arr2[], int arr_size);
void generate_data_constant(int arr[], int arr_size, int val);
void generate_data_increasing(int arr[],int arr_size, int step=1);
void generate_data_decreasing(int arr[],int arr_size, int step=1);
void generate_big_A(int arr[], int arr_size, int step=1);
void generate_big_V(int arr[], int arr_size, int step=1);
void generate_small_V(int arr[], int arr_size, int step=1);
void generate_small_A(int arr[], int arr_size, int step=1);
void saveToFile(int** arr,int ntables,int num_of_elem,char* name);
void readFromFile(int** arr,int num_tabl,int num_of_elem, char* name);
void isSorted(int arr[],int count);
double get_cpu_time(void);

const int NUM_TABLES=50;
const int NUM_REPS=10; //number of repetiotions for beter time measusrement (interuptions ect.)
char nameOfFile[]="data_excersie1.dat";

int main()
{
    double start=0,stop=0,elapsed=0;
	ofstream file;
    file.open("results_ex1.txt");
    int **ptrToPtr=NULL;
    ptrToPtr=new int*[NUM_TABLES];
    for( int kk=0;kk<NUM_TABLES;kk++)
        ptrToPtr[kk]=NULL;
    
    int *out_arr=NULL;
    for(int i=100;i<=10000;i+=100){
    	try{
		
        for(int kk=0;kk<NUM_TABLES;kk++)
            {
            delete []ptrToPtr[kk];
            ptrToPtr[kk]=new int[i];
            }
         }
         catch(...)
         {
          cout<<"Problem"<<endl;
		  return 1;	
		 }
        for(int ii=0;ii<NUM_TABLES;ii++){
         generate_data_random(ptrToPtr[ii],i,i);
        }
        saveToFile(ptrToPtr,NUM_TABLES,i,nameOfFile);
        
        cout<<"**********************************"<<endl;
        cout<<"Size of table "<<i<<endl<<endl;
        //bubble sort
        elapsed=0;
        for (int jj=0;jj<NUM_REPS;jj++)
        {
		readFromFile(ptrToPtr,NUM_TABLES,i,nameOfFile);
        start = get_cpu_time();
        for (int ii=0;ii<NUM_TABLES;ii++)
            {   
			   bubble_sort(ptrToPtr[ii],i);  
            }
            stop = get_cpu_time();
            elapsed+=stop-start;
        }
        cout<<"bubble_sort"<<endl;
        file<<"bubble_sort"<<"\t"<<i<<"\t"<<elapsed/NUM_TABLES/NUM_REPS<<endl;
        //heap sort
        elapsed=0;
        for (int jj=0;jj<NUM_REPS;jj++)
        {
		readFromFile(ptrToPtr,NUM_TABLES,i,nameOfFile);
        start = get_cpu_time();
        for (int ii=0;ii<NUM_TABLES;ii++)
            {
			   
			   heapsort(ptrToPtr[ii],i);  
			   //isSorted(ptrToPtr[ii],i);
            }
            stop = get_cpu_time();
            elapsed+=stop-start;
        }
        cout<<"heap_sort"<<endl;
        file<<"heap_sort"<<"\t"<<i<<"\t"<<elapsed/NUM_TABLES/NUM_REPS<<endl;
        
         
        //countingsort
        delete [] out_arr;
        out_arr=new int [i];
        
        elapsed=0;
        for (int jj=0;jj<NUM_REPS;jj++)
        {
		readFromFile(ptrToPtr,NUM_TABLES,i,nameOfFile);
        start = get_cpu_time();
        for (int ii=0;ii<NUM_TABLES;ii++)
            {
                
				countingsort(ptrToPtr[ii],i,out_arr);
			    
            }
            stop = get_cpu_time();
            elapsed+=stop-start;
        }
        cout<<"countingsort"<<endl;
        file<<"countingsort"<<"\t"<<i<<"\t"<<elapsed/NUM_TABLES/NUM_REPS<<endl;
        
        //Shell sort
        elapsed=0;
        for (int jj=0;jj<NUM_REPS;jj++)
        {
		readFromFile(ptrToPtr,NUM_TABLES,i,nameOfFile);
        start = get_cpu_time();
        for (int ii=0;ii<NUM_TABLES;ii++)
            {
               // isSorted(ptrToPtr[ii],i);
				shellsort(ptrToPtr[ii],i);
			  //  isSorted(ptrToPtr[ii],i);
            }
            stop = get_cpu_time();
            elapsed+=stop-start;
        }
        cout<<"Shell sort"<<endl;
        file<<"Shell sort"<<"\t"<<i<<"\t"<<elapsed/NUM_TABLES/NUM_REPS<<endl; 

    }
    file.close();
    return 0;
}

void bubble_sort(int arr[],int arr_size){
    for(int i=0;i<arr_size;i++){
        for(int ii=arr_size-1;ii>i;ii--){
            if (arr[ii]<arr[ii-1]){
                swap(arr[ii],arr[ii-1]);
            }
        }
    }
}

void selection_sort(int arr[],int arr_size){
    int index=0;
    for(int i=0;i<arr_size;i++){
        index=i;
        for(int ii=i;ii<arr_size;ii++){
            if(arr[ii]<arr[index]){
                index=ii;
            }
        }
        swap(arr[i],arr[index]);
    }
}

void insertion_sort(int arr[], int arr_size){
    int tmp,j;
    for(int i=0;i<=arr_size;i++){
        tmp=arr[i]; j=i;
           while(j>0 && tmp<arr[j-1])
           {
                arr[j]=arr[j-1];
                j--;
            }
       arr[j]=tmp;
    }
}

void heap_move(int arr[], int first, int last){
    int largest=2*first+1;
    while(largest<=last){
        if(largest<last && arr[largest]<arr[largest+1]){
                largest++;
        }
        if(arr[first]<arr[largest]){
            swap(arr[first],arr[largest]);
            first=largest;
            largest=2*first+1;
        }
        else{
            largest=last+1;
        }
    }
}

void swap(int &val1, int &val2){
    int temp_val=val1;
    val1=val2;
    val2=temp_val;
}

void heapsort(int arr[], int arr_size){
    for(int i=arr_size/2-1;i>=0;i--){
        heap_move(arr,i,arr_size-1);
    }
    for(int j=arr_size-1;j>1;j--){
        swap(arr[0],arr[j]);
        heap_move(arr,0,j-1);
    }
    if(arr[0]>arr[1]){
        swap(arr[0],arr[1]);
    }
}

void quicksort(int arr[], int first, int last){
    int lower=first, upper=last, pivot=arr[(first+last)/2];
    while (lower<=upper){
        while(arr[lower]<pivot)
            lower++;
        while(pivot<arr[upper])
            upper--;
        if (lower<upper)
            swap(arr[lower++],arr[upper--]);
        else lower++;
    }
    if (first<upper)
        quicksort(arr,first,upper);
    if (upper+1<last)
        quicksort(arr,upper+1,last);
}

void countingsort(int arr[], int arr_size, int out_arr[]){
    int max_num=arr[0];
    int temp_val=0;
    for(int i=0;i<arr_size;i++){
        if (max_num<arr[i]){
            max_num=arr[i];
        }
    }
    int *count_arr=new int[max_num+1];
    for(int i=0;i<max_num+1;i++){
        count_arr[i]=0;
    }
    for(int i=0;i<arr_size;i++){
        count_arr[arr[i]]++;
    }
    for(int i=0;i<max_num+1;i++){
        temp_val=temp_val+count_arr[i];
        count_arr[i]=temp_val;
    }
    for(int i=0;i<arr_size;i++){
        out_arr[count_arr[arr[i]]-1]=arr[i];
        count_arr[arr[i]]--;
    }
    delete[] count_arr;
}

void shellsort(int arr[], int arr_size){
    register int i,j,h,temp,h_cnt;
    int sort_steps[30];
    int k;
    for(h=1, i=0;h<arr_size;i++){
        sort_steps[i]=h;
        h=3*h+1;
    }
    for(i--;i>=0;i--){
        h=sort_steps[i];
        for(int h_cnt=h;h_cnt<2*h;h_cnt++){
            for(int j=h_cnt;j<arr_size; ){
                temp=arr[j];
                k=j;
                while(k-h>=0 && temp<arr[k-h]){
                    arr[k]=arr[k-h];
                    k-=h;
                }
                arr[k]=temp;
                j+=h;
            }
        }
    }
}

void merge(int arr[], int temp[], int left, int center, int right){
    for(int i=left;i<=right;i++){
        temp[i]=arr[i];
    }
    int i=left, j=center+1;
    for(int k=left; k<=right; k++){
        if(i<=center){
            if(j<=right){
                if(temp[j]<temp[i])
                    arr[k]=temp[j++];
                else
                    arr[k]=temp[i++];
            }
            else
                arr[k]=temp[i++];
        }
    else
        arr[k]=temp[j++];
    }
}

void mergesort(int arr[], int temp[], int left, int right){
    if(right<=left)
        return;
    int center=(left+right)/2;
    mergesort(arr,temp,left,center);
    mergesort(arr,temp,center+1,right);

    merge(arr, temp, left, center, right);
}

void print_arr(int arr[], int arr_size){
    for(int i=0;i<arr_size;i++){
        cout<<arr[i]<<" ";
    }
    cout<<"\n";
}

void generate_data_random(int arr[], int arr_size, int range){
    for(int i=0;i<arr_size;i++){
        arr[i]=rand()%(range+1);
    }
}

void copy_arr(int arr1[], int arr2[], int arr_size){
    for(int i=0; i<arr_size; i++){
        arr2[i]=arr1[i];
    }
}

void generate_data_constant(int arr[], int arr_size, int val){
    for(int i=0;i<arr_size;i++){
        arr[i]=val;
    }
}

void generate_data_increasing(int arr[],int arr_size, int step){
    for(int i=0;i<arr_size;i++){
        if(i==0)
            arr[i]=0;
        else
            arr[i]=arr[i-1]+step;
    }
}

void generate_data_decreasing(int arr[],int arr_size, int step){
    for(int i=0;i<arr_size;i++){
        if(i==0)
            arr[i]=arr_size*step-1;
        else
            arr[i]=arr[i-1]-step-1;
    }
}

void generate_big_A(int arr[], int arr_size, int step){
    for(int i=0;i<=arr_size/2;i++){
        arr[i]=2*i*step+1;
    }
    for(int i=arr_size/2;i<arr_size;i++){
        if(i==arr_size/2)
            arr[i]=arr[i-1]-1;
        else
            arr[i]=arr[i-1]-2*step;
    }
}

void generate_big_V(int arr[], int arr_size, int step){
    int minimal;
    for(int i=0; i<=arr_size/2;i++){
        if(i==0)
            arr[i]=2*arr_size*step-1;
        else
            arr[i]=arr[i-1]-2*step;
    }
    for(int i=arr_size/2;i<arr_size;i++){
        if(i==arr_size/2)
            arr[i]=arr[i-1]+1;
        else
            arr[i]=arr[i-1]+2*step;
    }
    for(int i=0;i<arr_size;i++){
        if (i==0)
            minimal=arr[i];
        else
            if (minimal>arr[i])
                minimal=arr[i];
    }
    for(int i=0;i<arr_size;i++){
        arr[i]=arr[i]-minimal+1;
    }
}

void generate_small_V(int arr[], int arr_size, int step){
    for(int i=0;i<arr_size;i++){
        if(i%2==0)
            arr[i]=i*2*step;
        else
            arr[i]=(arr_size*2-1)-i*2*step;
    }
}

void generate_small_A(int arr[], int arr_size, int step){
    for(int i=0;i<arr_size;i++){
        if(i%2==0)
            arr[i]=(arr_size*2)-i*2*step;
        else
            arr[i]=i*2*step+1;
    }
}

void saveToFile(int** arr,int num_tabl,int num_of_elem,char* name){
    fstream file;
    file.open(name,ios::binary |ios::out);
    for (int i=0;i<num_tabl;i++)
     file.write((char*)(arr[i]),num_of_elem*sizeof(int));
    file.close();
}

void readFromFile(int** arr,int num_tabl,int num_of_elem, char* name){
   ifstream file;
    file.open(name,ios::binary );
    for (int i=0;i<num_tabl;i++)
     file.read((char*)(arr[i]),num_of_elem*sizeof(int));
    file.close();
}

void isSorted(int arr[],int count)
{
	for (int i=1;i<count;i++)
		if(arr[i-1]>arr[i])
		 {
		  cout<<"Not sorted"<<endl;
		  return;
	     }
   cout<<"Sorted"<<endl;
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

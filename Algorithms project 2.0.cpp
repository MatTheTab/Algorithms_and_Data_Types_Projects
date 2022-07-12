#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <string.h>

using namespace std;

struct student
{
  string name;
  string surname;
  int index;	
};

string generate_name();
string generate_surname();
int genetrate_id(int prev_id=1000000);
void generate_students(int num_students=100, int name_length=12, int surname_length=12, int starting_id=1000000);
void generate_students_struct(student*student_arr, int num_students);
void shuffle_students(student * student_arr, int num_students, int num_shuflles);
void swap(student &student_arr1, student &student_arr2);
void write_to_file(student *student_arr, int num_students);
void write_to_file_bin(student *student_arr, int num_students);
void read_from_file_bin(student *student_arr, int num_students);
void print_student(student *student_arr, int num_to_read);
void Count(void);

const int numberOfStudents=8999999;
const int NUM_SHUFFLES=numberOfStudents*3;


class BST{
	int index;
	string name;
	string surname;
	int left_index;
	int right_index;
};

class Ordered_list{
	int i;
	string name;
	string surname;
	int index;
};


int main(){
	
try
{
student * students= new student[numberOfStudents];


generate_students_struct(students, numberOfStudents);
shuffle_students(students,numberOfStudents,NUM_SHUFFLES);
write_to_file(students, numberOfStudents);
Count();
return 0;
}
catch(...)
{
   cout<<"Allocation error"	;
   return 1;
}	
}

string generate_name(){
	char name[13];
	name[0]=rand()%26+'A';
	for(int i=1;i<12;i++){
		name[i]=rand()%26+'a';
	}
	name[12]=0;
    string tmp=name;
	return tmp;
}

string generate_surname(){
	char surname[13];
	surname[0]=rand()%26+'A';
	for(int i=1;i<12;i++){
		surname[i]=rand()%26+'a';
	}
	surname[12]=0;
	string tmp=surname;
	return tmp;
}

int generate_id(int prev_id){
	int new_id=prev_id+1;
	return new_id;
}

void generate_students(int num_students, int name_length, int surname_length, int starting_id){
	string name;
	string surname;
	int index;
	ofstream studentsfile("Students.txt");
	for(int i=0;i<num_students;i++){
		name=generate_name();
		surname=generate_surname();
		index=generate_id(1000000+i);
		studentsfile<<name.c_str();
		studentsfile<<' ';
		studentsfile<<surname.c_str();
		studentsfile<<' ';
		studentsfile<<index;
		studentsfile<<'\n';
	}
	studentsfile.close();
}

void generate_students_struct(student* student_arr, int num_students){
	for(int i=0;i<num_students;i++){
		student_arr[i].name=generate_name();
		student_arr[i].surname=generate_surname();
		student_arr[i].index=generate_id(1000000+i);
	}
}

void shuffle_students(student * student_arr, int num_students, int num_shuffles){
	int a,b;
	for(int i=0; i<num_shuffles; i++){
		a=(rand()*rand())%num_students;
		b=(rand()*rand())%num_students;
		swap(student_arr[a], student_arr[b]);
	}
}

void swap(student &student_arr1, student &student_arr2){
	student temp;
	temp=student_arr2;
	student_arr2=student_arr1;
	student_arr1=temp;
}

void write_to_file(student *student_arr, int num_students){
	ofstream studentsfile("Students.txt");
	for(int i=0;i<num_students;i++){
		studentsfile<<student_arr[i].name.c_str();
		studentsfile<<' ';
		studentsfile<<student_arr[i].surname.c_str();
		studentsfile<<' ';
		studentsfile<<student_arr[i].index;
		studentsfile<<'\n';
	}
	studentsfile.close();
}

void write_to_file_bin(student *student_arr, int num_students){
	ofstream studentsfile("Students_bin.bin",ios::out | ios::binary);
	for(int i=0;i<num_students;i++){
		
		studentsfile.write((char*)student_arr[i].name.c_str(),13*sizeof(char));
		studentsfile.write((char*)student_arr[i].surname.c_str(),13*sizeof(char));
		studentsfile.write((char*)&student_arr[i].index,sizeof(int));
	}
	studentsfile.close();
}

void read_from_file_bin(student *student_arr, int num_students){
	ifstream studentsfile("Students_bin.bin",ios::in | ios::binary);
	char buffor[13];
	for(int i=0;i<num_students;i++){
		studentsfile.read(buffor,13*sizeof(char));
		student_arr[i].name=buffor;
		studentsfile.read(buffor,13*sizeof(char));
		student_arr[i].surname=buffor;
		studentsfile.read((char*)&student_arr[i].index,sizeof(int));
	}
	studentsfile.close ();
}
void Count(void)
{
	ifstream studentsfile("Students.txt",ios::in);
	int counter=0;
	char buffor[100];
	while(!studentsfile.eof())
	{
	  studentsfile.getline(buffor,100);
	  counter++;	
	}
	studentsfile.close();
	cout<<counter<<endl;
}

void print_student(student *student_arr, int num_to_read){
	
}

#include <unistd.h>     /* Symbolic Constants */
#include <errno.h>      /* Errors */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include<iostream>
#include<vector>
#include<sstream>
#include<iomanip>
using namespace std;

int Q;
pthread_mutex_t out;
struct ID{
	int id;
	int arrive;
	int time;
};
vector<ID> data;

void* DO_SJF ( void *ptr ){
	int burst = 0;
	int hold;
	int t = 0;
	vector<ID> keep ;
	vector<ID> data2;
	data2=data;
	for (int i = 0; i < data2.size(); i++) {
		for (int j = 0; j < data2.size(); j++) {
			if (data2[j].arrive <= t) {
				if (burst == 0) {
					burst = data2[j].time;
					hold = j;
				}
				else if (data2[j].time < burst) {
					burst = data2[j].time;
					hold = j;
				}
			}
		}
		t += data2[hold].time;
		burst = 0;
		keep.push_back(ID{data2[hold].id,data2[hold].arrive,data2[hold].time });
		data2.erase(data2.begin() + hold);
		i--;
	}
	pthread_mutex_lock(&out);
	cout<<"====SJF\n";
	float aver = 0;
	for (int i = 0; i < keep.size(); i++) {
		if (i == 0) {
			cout << keep[i].id << " " << keep[i].arrive << " " << keep[i].time << endl;
			continue;
		}
		aver += keep[i - 1].time - keep[i].arrive;
		keep[i].arrive = keep[i - 1].time;
		keep[i].time = keep[i - 1].time + keep[i].time;
		cout << keep[i].id << " " << keep[i].arrive << " " << keep[i].time<<endl;

	}
	aver /= keep.size();
	cout<<"Average waiting time:" << fixed << setprecision(2) << aver<<"ms" << endl;
	
	pthread_mutex_unlock(&out);
	pthread_exit(0);
}
void* DO_RR ( void *ptr ){
	int burst = 0;
	int q;
	int t = 0;
	q=Q;
	vector<ID> keep;
	vector<ID> data3;
	data3=data;
	vector<ID> data_copy;
	data_copy = data3;
	pthread_mutex_lock(&out);
	cout<<"====RR\n";
	
	while (!data3.empty()) {
		for (int i = 0; i < data3.size(); i++) {
			if (data3[i].arrive <= t) {
				if (data3[i].time >= q) {
					burst += q;
					keep.push_back(ID{ data3[i].id,t,burst});
					t += q;
					data3[i].time -= q;
				}
				else if (data3[i].time < q) {
					burst += data3[i].time;
					keep.push_back(ID{ data3[i].id,t,burst });
					t += data3[i].time;
					data3[i].time =0;
				}
			}
			if (data3[i].time == 0) {
				data3.erase(data3.begin() + i);
				i--;
			}

		}

	}
	for(int i=0;i<keep.size();i++){
		cout << keep[i].id << " " << keep[i].arrive << " " << keep[i].time<<endl;
	}
	float aver=0;

	for (int i = 0; i < data_copy.size(); i++) {
		aver += keep[i].arrive - data_copy[i].arrive;
	}

	for (int i =0; i < keep.size()-1; i++) {
		for (int j = i+1; j < keep.size(); j++) {

			if (keep[i].id == keep[j].id) {
				aver += keep[j].arrive - keep[i].time;
				break;
			}
			
		}
	}
	aver /= data_copy.size();
	cout<<"Average waiting time:" << fixed << setprecision(2) << aver<<"ms" << endl;
	pthread_mutex_unlock(&out);
	pthread_exit(0);
}
void* DO_FCFS ( void *ptr ){
	int t=0;
	ID hold;
	vector<ID> data1;
	data1=data;
	for (int i = 0; i < data1.size(); i++) {
		for (int j = i; j < data1.size() - 1; j++) {
			if (data1[i].arrive > data1[j + 1].arrive) {
				hold = data1[i];
				data1[i] = data1[j + 1];
				data1[j + 1] = hold;
			}
		}
	}
	pthread_mutex_lock(&out);
	cout<<"====FCFS\n";
	float aver = 0;
	for (int i = 0; i < data1.size(); i++) {
		if (i == 0) {
			cout << data1[i].id << " " << data1[i].arrive << " " << data1[i].time << endl;
			continue;
		}
		aver += data1[i - 1].time - data1[i].arrive;
		data1[i].arrive = data1[i - 1].time;
		data1[i].time = data1[i - 1].time + data1[i].time;
		cout << data1[i].id<<" " << data1[i].arrive<<" " << data1[i].time << endl;
	}
	
	aver /= data1.size();
	cout<<"Average waiting time:" << fixed << setprecision(2) << aver<<"ms" << endl;
	pthread_mutex_unlock(&out);
	pthread_exit(0);
}
int main()
{
	//sem_init(&syn,0, 1);
    pthread_t FCFS;
	pthread_t SJF;
	pthread_t RR;
	string in;
	vector<string> input;
	stringstream ss;
	string test;
	ID ta;
	string indata[3];
	int outdata[3];
	while (getline(cin,in)){
			input.push_back(in);
	}
	/*for(int i=0;i<input.size();i++){
		cout<<input[i]<<endl;	
	}*/
	for(int i=0;i<input.size();i++){
		if(input[i][0]=='Q'){
			input[i].erase(input[i].begin());
			if(input[i][0]=='='){
				input[i].erase(input[i].begin());
			}
			ss<<input[i];
			ss>>Q;
			continue;

		}
		if(input[i][0]=='#'){
			input.erase(input.begin()+i);
			i--;
			continue;
		}
		
		for(int a=0;a<3;a++){
			for(int j=0;j<input[i].size();j++){
				
				if(input[i][j]==','){
					input[i].erase(input[i].begin(),input[i].begin()+j+1);
					break;
				}
				indata[a].push_back(input[i][j]);
			}
			stringstream ss1;
			ss1<<indata[a];
			ss1>>outdata[a];
			indata[a].clear();
		}
		data.push_back(ID{outdata[0],outdata[1],outdata[2]});
		outdata[0]=0;
		outdata[1]=0;
		outdata[2]=0;
	}
	data.pop_back();
	pthread_create (&SJF, NULL, DO_SJF, NULL);
	pthread_create (&RR, NULL, DO_RR, NULL);	
	pthread_create (&FCFS, NULL, DO_FCFS, NULL);
	
	pthread_join(SJF, NULL);
	pthread_join(RR, NULL);
	pthread_join(FCFS, NULL);

 
    exit(0);
} 


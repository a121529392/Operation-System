#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include<iostream>
#include<ctime>
using namespace std;
int total;
int choice;
time_t t;
pthread_mutex_t chopstick[11];
pthread_mutex_t out;
pthread_mutex_t choose;
time_t now=time(0);
tm *ltm=localtime(&now) ;	
void* check(void *ptr){
	while (1) {
		if ((double)(clock() - t) / CLOCKS_PER_SEC > 20.0) {
			cout<<"deadlock\n";
			exit(1);
		}
	}
}
void* eating ( void *ptr ){
	int f=rand()%2;
	int p = *(int*)ptr;
	sleep(rand()%5+1);
	int sticknum[2];
	if(choice==2){
		if(p==0){
			sticknum[0]=p;
			sticknum[1]=total-1;
		}
		else{
			if(p%2==0){
				sticknum[0]=p;
				sticknum[1]=p-1;
			}
			else{
				sticknum[0]=p-1;
				sticknum[1]=p;
			}
		}
	}
	else{
		if(p==0){
			sticknum[f]=total-1;
		}
		else
			sticknum[f]=p-1;
		sticknum[1-f]=p;
	}
	for(int i=0;i<10;i++){
		if(choice==3){
			pthread_mutex_lock(&choose);
		}
		pthread_mutex_lock(&chopstick[sticknum[0]]);
		pthread_mutex_lock(&out);
		if(sticknum[0]==p){
		cout << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec;
		cout<<"-Phi "<<p<<"-take left chopstick\n";
		}
		else{
		cout << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec;
		cout<<"-Phi "<<p<<"-take right chopstick\n";
		}
		pthread_mutex_unlock(&out);
		now=time(0);
		ltm=localtime(&now) ;
		pthread_mutex_lock(&chopstick[sticknum[1]]);
		pthread_mutex_lock(&out);
		if(sticknum[1]!=p){
		cout << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec;
		cout<<"-Phi "<<p<<"-take right chopstick\n";
		}
		else{
		cout << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec;
		cout<<"-Phi "<<p<<"-take left chopstick\n";
		}
		pthread_mutex_unlock(&out);
		t=clock();
		now=time(0);
		ltm=localtime(&now) ;
		pthread_mutex_lock(&out);
		cout << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec;		
		cout<<"-Phi "<<p<<"-eat\n";
		sleep(rand()%5+1);
		pthread_mutex_unlock(&out);
		now=time(0);
		ltm=localtime(&now) ;
		pthread_mutex_lock(&out);
		pthread_mutex_unlock(&chopstick[sticknum[0]]);
		if(sticknum[0]==p){
		cout << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec;
		cout<<"-Phi "<<p<<"-put left chopstick\n";
		}
		else{
		cout << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec;
		cout<<"-Phi "<<p<<"-put right chopstick\n";
		}
		pthread_mutex_unlock(&out);
		now=time(0);
		ltm=localtime(&now) ;
		pthread_mutex_lock(&out);
		pthread_mutex_unlock(&chopstick[sticknum[1]]);
		if(sticknum[1]!=p){
		cout << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec;
		cout<<"-Phi "<<p<<"-put right chopstick\n";
		}
		else{
		cout << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec;
		cout<<"-Phi "<<p<<"-put left chopstick\n";
		}
		pthread_mutex_unlock(&out);
		now=time(0);
		ltm=localtime(&now) ;
		pthread_mutex_lock(&out);
		cout << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec;		
		cout<<"-Phi "<<p<<"-think\n";
		pthread_mutex_unlock(&out);
		if(choice==3){
			pthread_mutex_unlock(&choose);
		}	
		sleep(rand()%6+5);
		

	}	
	pthread_exit(0);
}
int main()
{
	srand(0);
	//sem_init(&syn,0, 1);
    pthread_t tid[13];
	pthread_t count;
	int people[13];
	int num;
	cin>>choice;
	cin>>num;
	total=num;
	t = clock();
	for(int i=0;i<13;i++){
		people[i]=i;
	}
	for(int i=0;i<num;i++){
		pthread_mutex_init(&chopstick[i],NULL);
	}
	pthread_mutex_init(&out,NULL);
	pthread_mutex_init(&choose,NULL);
	for(int i=0;i<num;i++){
		pthread_create (&tid[i], NULL, eating, (void *) &people[i]);
	}
	pthread_create (&count, NULL, check, NULL);
	for(int i=0;i<num;i++){
		pthread_join(tid[i], NULL);
	}
    now=time(0);
	ltm=localtime(&now) ;
	cout << ltm->tm_hour << ":";
	cout << ltm->tm_min << ":";
	cout << ltm->tm_sec;	
	cout<<"-All philosophers have taken 10 meals\n";          
    /* exit */ 
	
    exit(0);
} 


/* Includes */
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <sys/times.h>
#include <time.h>
#include <iostream>
#include <math.h>
using namespace std;
typedef struct str_thdata
{
    	pthread_t thread_no;
	int num;
	int size;
    	int res[3000];
} thdata;
struct answer{
	int size;
	int num[3000];
};
pthread_mutex_t mut =PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond =PTHREAD_COND_INITIALIZER;
clockid_t cid,cid1,cid2;
void* caculate_factorial ( void *ptr )
{
	pthread_mutex_lock(&mut);
	pthread_cond_wait(&cond,&mut);
	pthread_t ptid = pthread_self();
	struct timespec tp1;
    	thdata *data;            
    	data = (thdata *) ptr;
	data->thread_no=ptid;
	int num=data->num;
	int ans[3000] = {0};
	ans[0] = 1;
	int s=1;
	int i;
	int j;
	for ( i = 1; i <= num; i++){
		for ( j = 0; j < s; j++){
			ans[j] = ans[j] * i;

		}
		for ( j = 0; j < s; j++){
			if (ans[j] / 10>0&&j==s-1){
				s++;
			}
			if (ans[j] / 10>0){
				ans[j+1]+=ans[j]/10;
				ans[j] = ans[j] % 10;
			}
		}

	}
	int x;
	for (i = 2999; i >= 0; i--){
		if (ans[i] != 0){
			x = i;
			break;
		}
	}
	for(;x>=0;x--){
		data->res[x]=ans[x];
	}
	data->size=s;
	cout<<"[TID="<<ptid<<"] "<<data->size<<endl;
	pthread_getcpuclockid(pthread_self(), &cid1);
	clock_gettime(cid1, &tp1);
	cout<<"[TID="<<ptid<<"] CPU Time " <<tp1.tv_sec*1000+tp1.tv_nsec/1000000.000000<<" ms"<<endl;
    pthread_exit(0);
}
void* caculate_fibonacci ( void *ptr )
{
	answer *ans1 = (answer*)malloc(sizeof(answer) * 1001);
	for (int i = 0; i < 1001; i++){
		ans1[i].size = 1;
		for (int j = 0; j < 3000; j++){
			ans1[i].num[j] = 0;
		}
	}
	pthread_t ptid = pthread_self();
	struct timespec tp2;
    	thdata *data;            
   	 data = (thdata *) ptr;
	data->thread_no=ptid;
	ans1[0].num[0] = 0;
	ans1[1].num[0] = 1;
	int aa=data->num;
	for (int i = 2; i <= aa; i++){
		ans1[i].size = ans1[i - 1].size;
		for (int j = 0; j < ans1[i].size; j++){
			ans1[i].num[j] = ans1[i - 1].num[j] + ans1[i - 2].num[j];

		}
		for (int j = 0; j < ans1[i].size; j++){
			if (ans1[i].num[j] >= 10){
				ans1[i].num[j + 1] += ans1[i].num[j] / 10;
				ans1[i].num[j] = ans1[i].num[j] % 10;


				if (j == ans1[i].size - 1){
					ans1[i].size++;
				}
			}
		}
	}
	for (int i = ans1[aa].size-1; i >= 0; i--){
		data->res[i]=ans1[aa].num[i];
	}
	data->size=ans1[aa].size;
	cout<<"[TID="<<ptid<<"] "<<data->size<<endl;
	pthread_getcpuclockid(pthread_self(),&cid2);
	clock_gettime(cid2, &tp2);
	cout<<"[TID="<<ptid<<"] CPU Time " <<tp2.tv_sec*1000+tp2.tv_nsec/1000000.000000<<" ms"<<endl;   
    	pthread_cond_signal(&cond);
        pthread_exit(0);
}
int main()
{
	pthread_t ptid = pthread_self();
    	pthread_t thread1, thread2;  /* thread variables */
    	thdata data1, data2;         /* structs to be passed to threads */
	clockid_t cid;
	cid=CLOCK_PROCESS_CPUTIME_ID;
	struct timespec tp;
	cin>>data1.num;
	data2.num=data1.num;
    /* create threads 1 and 2 */  
	cout<<"[Main thread] gets "<<data1.num<<endl;
        pthread_create (&thread1, NULL, caculate_factorial, reinterpret_cast <void *>(&data1));
        pthread_create (&thread2, NULL, caculate_fibonacci, reinterpret_cast <void *>(&data2));
	cout<<"[Main thread] Thread ID of "<<data1.num<<"!="<<thread1<<endl;
	cout<<"[Main thread] Thread ID of F("<<data1.num<<")"<<thread2<<endl;

        pthread_join(thread1, NULL); 
 	pthread_join(thread2, NULL);
	pthread_getcpuclockid(thread1, &cid1);
	pthread_getcpuclockid(thread2, &cid2);
	cout<<"[Main thread] N!=";
	for(int x=data1.size-1;x>=0;x--){
		cout<<data1.res[x];
	}
	cout<<endl;
	cout<<"[Main thread]  F("<<data1.num<<")";
	for (int i = data2.size-1; i >= 0; i--){
		cout << data2.res[i];
	}
	cout<<endl;
	clock_gettime(cid, &tp);
	cout<<"[Main Thread] CPU Time "<< tp.tv_sec*1000+tp.tv_nsec/1000000.000000<<" ms"<<endl; 
    exit(0);
} /* main() */
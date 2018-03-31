#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>


void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int r;
  int num;
  printf("Input number:");
  scanf("%d",&num);

  const char *memname = "sample";
  const size_t region_size = 15000;

  int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
  if (fd == -1)
    error_and_die("shm_open");

  r = ftruncate(fd, region_size);
  if (r != 0)
    error_and_die("ftruncate");

  void *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED)
    error_and_die("mmap");
	close(fd);
    int *d = (int *) ptr;
	
	
    pid_t pid = fork();

  if (pid == 0) {	
    int num=*((int *) ptr+1);
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
	int *res=malloc(s);
	for(;x>=0;x--){
		res[x]=ans[x];
	}	
	for(i=s-1,j=3;i>=0;i--,j++){
		*(d+j)=res[i];
	}
	*(d+2) = s;
	printf("[%dChild]: %d\n",*(int *) ptr,s);
    exit(0);
  }
  else {
	*(d+1)=num;		  
    *d = pid;
    int status;
	wait(0);	  
	int i;
	
	printf("[%dParent]:", getpid());
	
	for(i=3;i<3+*((int *) ptr+2);i++){
		printf("%d",*((int *) ptr+i));
	}
	printf("\n");
	
  }

  r = munmap(ptr, region_size);
  if (r != 0)
    error_and_die("munmap");

  r = shm_unlink(memname);
  if (r != 0)
    error_and_die("shm_unlink");

  return 0;
}

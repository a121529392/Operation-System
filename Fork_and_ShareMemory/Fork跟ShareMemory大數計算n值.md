# Fork( )跟Share Memory 大數計算n!值



 **<font face="微软雅黑">輸入要計算的n!值<font>**
```c
  printf("Input number:");
  scanf("%d",&num);
```

  **<font face="微软雅黑">設定Share Memory以及定義Size為15000<font>**
 
```c
  const char *memname = "sample";
  const size_t region_size = 15000;
```
  **<font face="微软雅黑">創一個Share Memory空間以及執行Fork( )<font>**
```c
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
```


 **<font face="微软雅黑">Child Process中，先從Shared Memory取出階層n的值，宣告一個int的正列，s為計算位數，利用大數乘法，乘數乘以每個位數的被乘數，然後每個位數處理進位，用x找出最高位數，用res這個正列把計算過後正確的位數存入shared memory中<font>**
```c
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
```
 **<font face="微软雅黑">執行方法與Output範例:<font>**

![Output Example](/picture/input.jpg)

![Result](/picture/output.jpg)





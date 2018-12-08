#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Read from file */
int M,N,K,tmp;
int **A ,**B,**C;
void read_file()
{
	FILE *fp;
	int i,j;
	// Read from data.txt
	if((fp = fopen("data.txt","r")) == NULL){
		printf("fail to read file\n");
		exit(0);
	}
	fscanf(fp,"%d %d %d",&M,&K,&N);
	// Create matrices
	A= (int **) malloc(sizeof(int *)*M);
	for(tmp = 0; tmp<M ; ++tmp)
		A[tmp] = (int *)malloc(sizeof(int) * K);
	B= (int **) malloc(sizeof(int *)*K);
	for(tmp = 0; tmp<K ; ++tmp)
		B[tmp] = (int *)malloc(sizeof(int) * N);
	C= (int **) malloc(sizeof(int *)*M);
	for(tmp = 0; tmp<M ; ++tmp)
		C[tmp] = (int *)malloc(sizeof(int) * N);
	for(i=0; i<M ; ++i)
	{
		for(j=0; j<K; ++j)
		 fscanf(fp,"%d",&A[i][j]);
	}
	for(i=0; i<K ; ++i)
	{
		for(j=0; j<N; ++j)
		 fscanf(fp,"%d",&B[i][j]);
	}
}

/* Structure for passing data to threads */
struct v
{
	int i; /* row */
	int j; /* column */
};

/* The thread will begin control in this function */
void *runner(void *param){
 	struct v *data = param;
 	int n,sum = 0;
 	for(n = 0; n<K; ++n){
 		sum += A[data->i][n] * B[n][data->j];
 	}
 	C[data->i][data->j] = sum;
 	pthread_exit(0);
}

/* Main thread */
int main(int argc,char *argvs[])
{
	read_file();
	clock_t start,finish;
	double duration;
	start = clock();
	int x,y,count = 0;
	pthread_t *tid;
	// create the thread array
	tid = (pthread_t *) malloc(sizeof(pthread_t) * M * N);
	for(x = 0; x < M ; ++x){
		for(y=0; y < N; ++y)
		{
			// Calculate term C[x][y] in a thread 
			struct v *data = (struct v *) malloc(sizeof(struct v));
			data->i = x;
			data->j = y;
			// Create a thread and passing it data as a parameter
			pthread_attr_t attr;
			pthread_attr_init(&attr);	
			pthread_create(&tid[count],&attr,runner,data);
			++count;
		}
	}
	finish =clock();
	duration = (double) (finish - start) / CLOCKS_PER_SEC;
	printf("Creating threads costs %f seconds\n", duration);
	// Waiting for multiple threads
	for(x = 0; x < count ; ++x){
		pthread_join(tid[x],NULL);
	}
	finish =clock();
	duration = (double) (finish - start) / CLOCKS_PER_SEC;
	printf("Multithread programming costs %f seconds\n", duration);
	for(x = 0; x < M ; ++x){
		for(y=0; y < N; ++y){
		printf("%d ", C[x][y]);
		}
		printf("\n");
	}
}
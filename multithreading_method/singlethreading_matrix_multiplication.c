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
	for(i=0; i<M ; ++i)
	{
		for(j=0; j<N; ++j)
			C[i][j] = 0;
	}
}

int main(int argc,char *argvs[])
{
	read_file();
	clock_t start,finish;
	start = clock();
	double duration;
	int i,j,k;
	for(i = 0; i<M ; ++i)
	{
		for(j = 0;j<N;++j)
		{
			for(k = 0;k<K;++k)
				C[i][j] += A[i][k] * B[k][j];
		}
	}
	finish =clock();
	duration = (double) (finish - start) / CLOCKS_PER_SEC;
	printf("single-threaded program costs %f seconds\n", duration);
	for(i = 0; i < M ; ++i){
		for(j=0; j < N; ++j){
			printf("%d ", C[i][j]);
		}
		printf("\n");
	}
}

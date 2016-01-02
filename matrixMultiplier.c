#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define maxY 20
#define maxZ 20

struct data {
	int row; 
	int column; 
};
//global matrices
int matrix_A[maxY][maxZ];
int matrix_B[maxY][maxZ];
int matrix_C[maxY][maxZ];

//global dimension of 2 matrices to be multiplied
int row_1, column_1, row_2, column_2;

FILE *fp;			

void matrix_creater();
void printFunction();
void *thrdFunction( void *);

int main(){
	int i, j;
	matrix_creater();
	pthread_t threads[maxY * maxZ];
	int counter = -1;
	
	//create thread and pass in thrdFunction
	for (i = 0; i < row_1; i++) {
		for (j = 0; j < column_2; j++) {
			struct data *t = (struct data *)malloc(sizeof(struct data));  	   
			t-> row = i; 
			t-> column = j;
			counter = counter + 1;
			pthread_create(&threads[counter], NULL, thrdFunction, (void *)t); 
		}
	}

	//join all created threads
	counter = -1;
	for (i = 0; i < row_1; i++) {
		for (j = 0; j < column_2; j++)
		{
			counter = counter + 1;
			pthread_join(threads[counter], NULL);
		}			
	}

	//print the resulting thread
	printf("\n### All threads have terminated\n\n"); 
	printf("The matrix product of C[%d][%d] = {\n", row_1, column_2);
	printFunction(row_1, column_2, 2);
	return 0;
}

void printFunction(int a, int b, int c){
	int i, j;
	for (i = 0; i < a; i++) {
		for (j = 0; j < b; j++) {
			if (c == 0)
				printf("\t%d\t",matrix_A[i][j]);
			else if (c == 1)
				printf("\t%d\t",matrix_B[i][j]);
			else
				printf("\t%d\t",matrix_C[i][j]);				
		}
		printf("\n");
	}
	printf("}\n");					
}

void *thrdFunction(void * value){
	struct data *values = value;
	int n, sum = 0;
	for (n = 0; n < column_1; n++)
		sum += matrix_A[values ->row][n] * matrix_B[n][values-> column];
	matrix_C[values->row][values->column] = sum;	
	pthread_exit(NULL);
}

void matrix_creater(){
	int i, j;
	fp = fopen("input.dat", "r"); 
	fscanf(fp,"%i", &row_1);
	fscanf(fp,"%i", &column_1);
	
	//check to see if first matrix dimensions don't exceed the max value
	if ((row_1 > maxY) || (column_1 > maxZ)) {
			printf("Dimensions exceed max value\n");
			exit(0);
		}	
	for (i = 0; i < row_1; i++){
		for (j = 0; j < column_1; j++)
			fscanf(fp,"%i", &matrix_A[i][j]);
	}
	fscanf(fp,"%i", &row_2);
	fscanf(fp,"%i", &column_2);
	
	//check to see if second matrix dimensions don't exceed the max value	
	if ((row_2 > maxY) || (column_2 > maxZ)) {
			printf("Dimensions exceed max value\n");
			exit(0);
		}
	
	//check to see if the dimmensions allow for multiplication	
	if (column_1 != row_2) {	
		printf("Error: Cannot perform multiplication on specified dimensions\n");
		exit(0);
	}
	for (i = 0; i < row_2; i++){
		for (j = 0; j < column_2; j++)
			fscanf(fp,"%i", &matrix_B[i][j]);
	}
	fclose(fp);
	
	//print both matrices in proper format
	printf("A[%d][%d] = {\n", row_1, column_1);
	printFunction(row_1, column_1, 0);
	printf("B[%d][%d] = {\n", row_2, column_2);
	printFunction(row_2, column_2, 1);
}	

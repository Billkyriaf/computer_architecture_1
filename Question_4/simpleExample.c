
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MSIZE 3
/*	
*		A simple 2 table creating program 
*		
*/


// Definition of the three tables
double matrix_a[MSIZE][MSIZE]; // Matrix A
double matrix_b[MSIZE][MSIZE]; // Matrix B


/*
*	Initialisation with random values for previously defined tables (matrix_a , matrix_b)
*/
void initialize_matrices()
{
    int i, j;
    for (i = 0; i < MSIZE; i++)
    {
        for (j = 0; j < MSIZE; j++)
        {
            matrix_a[i][j] = (double)rand() / RAND_MAX;
            matrix_b[i][j] = (double)rand() / RAND_MAX;
        }
    }
}


// Function that print the table
void print_matrix(double matrix[MSIZE][MSIZE])
{
   for (int c = 0; c < MSIZE; c++) {
      for (int d = 0; d < MSIZE; d++)
        printf("%lf\t", matrix[c][d]);
 
      printf("\n");
   }
}

int main(){
	int i;
    printf("1. Initializing Matrices \n");
    initialize_matrices();
    printf("2. Matrix A is: \n");
    print_matrix(matrix_a);
    printf("3. Matrix B is: \n");
    print_matrix(matrix_b);
    return 0;
}
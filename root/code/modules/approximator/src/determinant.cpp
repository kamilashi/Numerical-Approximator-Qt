/*
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

void pivot(int startrow, int n, int m, float A[n][m]) 
{
	int k = startrow, h;
	int maxInd;
	float max, swap;
	max = A[k][k];
	maxInd = k;

	for (h = k; h < n; h++) 
	{
		if (A[h][k] >= max) 
		{
			max = A[h][k];
			maxInd = h;
		}
	}

	if (maxInd != k) 
	{
		for (h = 0; h < m; h++)
		{
			swap = A[k][h];
			A[k][h] = A[maxInd][h];
			A[maxInd][h] = swap;
		}
	}
}

float gauss(int n, int m, float A[n][m], float L[])
{
	float B[n][m];
	int i = 0, j = 0, k = 0, d = 0, p, r, s;

	for (d = 0; d < n; d++) {

		for (k = d; k < n; k++) 
		{
			for (p = 0; p < m; p++)
			{
				L[p] = A[k][p] / A[k][d];

				if (k == 0) {
					B[0][p] = A[0][p];
				}
			}

			for (j = 0; j < m; j++)
			{
				B[k + 1][j] = A[k + 1][j] - L[j] * A[k + 1][d];
			}

		}
		for (r = 0; r < n; r++)
		{
			for (s = 0; s < m; s++)
			{

				A[r][s] = B[r][s];

			}
		}
	}
}

double getDeterminant(int n, int m, float A[n][m])
{
	double det = 1; int i, j;

	for (i = 0, j = 0; i < n, j < m; i++, j++)
	{
		det = det * A[i][j];
	}
}

void print(int n, int m, float A[n][m]) 
{
	int i, j;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			printf("   %f   ", A[i][j]);
		}	
		printf("\n");
	}
	printf("\n");
	printf("\n");
}

void run() 
{
	int n, i, j, m;

	printf("Welcome to the Matrix Determinant calculator!\n\n");
	printf("Enter the matrix dimension :\n");

	scanf("%d", &n);

	m = n;
	float A[n][m];
	float L[m];
//	srand(time(NULL)); ????

	printf("Enter the elements a00, a01, ...  :\n");

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			scanf("%f", &A[i][j]);
		}
	}

	print(n, m, A);

	gauss(n, m, A, L);

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			printf("   %f   ", A[i][j]);
		}	printf("\n");
	}

	getDeterminant(n, m, A);

//	getch();
//	return 0;
}*/

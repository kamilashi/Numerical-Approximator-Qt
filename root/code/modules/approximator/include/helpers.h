#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <cstdio>
#include <cstring>
#include <cmath>

inline void printMatrix(int n, int m, float* A, char* pBuffer, int bufferSize)
{
	size_t len;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			len = strlen(pBuffer);
			snprintf(pBuffer + len, bufferSize - len, "%f	", A[i * n + j]);
		}

		len = strlen(pBuffer);
		snprintf(pBuffer + len, bufferSize - len, "\n");
	}
}

inline void pivotMatrix(int startrow, int n, int m, float* A)
{
	int k = startrow, h;
	int maxInd = k;
	float max = A[k * m + k];
	float swap;

	for (h = k; h < n; h++)
	{
		if (fabs(A[h * n + k]) >= fabs(max))
		{
			max = A[h * n + k];
			maxInd = h;
		}
	}

	if (maxInd != k)
	{
		for (h = 0; h < m; h++)
		{
			swap = A[k * m + h];
			A[k * m + h] = A[maxInd * m + h];
			A[maxInd * m + h] = swap;
		}
	}
}

inline void runGaussianElimination(int n, int m, float* A, char* pBuffer, int bufferSize)
{
	for (int d = 0; d < n; d++)
	{
		pivotMatrix(d, n, m, A);

		float pivotVal = A[d * m + d];
		if (fabs(pivotVal) < 1e-6f)
		{
			size_t len = strlen(pBuffer);
			snprintf(pBuffer + len, bufferSize - len, "\nZero or near-zero pivot at row %d! \n", d);
			return;
		}

		for (int i = d + 1; i < n; i++)
		{
			float factor = A[i * m + d] / pivotVal;
			for (int j = d; j < m; j++)
			{
				A[i * m + j] -= factor * A[d * m + j];
			}
		}
	}
}

inline double getMatrixDeterminant(int n, int m, float* A)
{
	double det = 1.0;

	for (int i = 0, j = 0; i < n && j < m; i++, j++)
	{
		det *= A[i * n + j];
	}

	return det;
}

#endif
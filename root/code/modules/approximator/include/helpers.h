#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#define PI 3.14159265358979323846 

#include <cstdio>
#include <cstring>
#include <cmath>


inline float polynomial(float x, int n, float* F)
{
	float y = 0;
	int i, power;

	for (i = 0; i < n; i++)
	{
		power = (n - 1) - i;
		y = y + F[i] * (pow(x, power));
	}

	return y;
}

inline void printMatrix(int n, int m, float* A, char* pBuffer, int bufferSize)
{
	size_t len;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			len = strlen(pBuffer);
			snprintf(pBuffer + len, bufferSize - len, "%f	", A[i * m + j]);
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
		if (fabs(A[h * m + k]) >= fabs(max))
		{
			max = A[h * m + k];
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

inline void runGaussianEliminationWithPivoting(int n, int m, float* A, char* pBuffer, int bufferSize)
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

inline void substUpperTAugmentedA(int n, int m, float* A, float* X, char* pBuffer, int bufferSize)
{
	float S = 0;

	for (int i = n - 1; i >= 0; i--)
	{
		S = 0;
		for (int j = n - 1; j > i; j--)
		{
			S += A[i * m + j] * X[j];
		}

		float result = (A[i * m + n] - S) / A[i * m + i];
		X[i] = result;
	}
}

inline void substUpperT(int n, float* A, float* Y, float* X, char* pBuffer, int bufferSize)
{
	int m = n + 1;
	float* A_temp = new float[n * m];

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			A_temp[i * m + j] = A[i * n + j];
		}

		A_temp[i * m + n] = Y[i];
	}

	substUpperTAugmentedA(n, m, A_temp, X, pBuffer, bufferSize);

	delete[] A_temp;
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

inline float normalizeAngle(float radians)
{
	float normalized = fmod(radians, 2 * PI);
	return normalized;
}

inline void getUnitMatrix(int n, float* A)
{
	for (int i = 0; i < n ; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j)
			{
				A[i * n + j] = 1.0f;
			}
			else
			{
				A[i * n + j] = 0.0f;
			}
		}
	}

}

inline void copyMatrix(int n,int m, float* Ato, float* Afrom)
{
	double det = 1.0;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			Ato[i * m + j] = Afrom[i * m + j];
		}
	}
}

inline void mulMatrix(int n, int p, int m, float* Ato, float* Afirst, float* Asecond)
{
	for (int i = 0; i < n; ++i) 
	{
		for (int j = 0; j < m; ++j) 
		{
			Ato[i * m + j] = 0.0f;

			for (int k = 0; k < p; ++k) 
			{
				Ato[i * m + j] += Afirst[i * p + k] * Asecond[k * m + j];
			}
		}
	}
}

#endif
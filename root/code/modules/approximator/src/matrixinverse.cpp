#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "matrixinverse.h"
#include "helpers.h"

MatrixInverse::MatrixInverse() : Program()
{
	m_stageCount = 3;
	m_matrixA = nullptr;
	reset();
}

MatrixInverse::~MatrixInverse()
{ 
	reset(); 
}

bool runPLUDecomposition(float* A, float* L, float* U, int* P, int n)
{
	copyMatrix(n, n, U, A);
	memset(L, 0, n * n * sizeof(float));

	for (int i = 0; i < n; ++i)
	{
		P[i] = i;
	}

	for (int k = 0; k < n; ++k)
	{
		float maxVal = 0.0f;
		int pivotRow = k;
		for (int i = k; i < n; ++i)
		{
			float val = fabs(U[i * n + k]);
			if (val > maxVal)
			{
				maxVal = val;
				pivotRow = i;
			}
		}

		if (maxVal < 1e-8f) return false;

		if (pivotRow != k) 
		{
			for (int j = 0; j < n; ++j)
			{
				swap(&U[k * n + j], &U[pivotRow * n + j]);
			}

			swap(&P[k], &P[pivotRow]);

			for (int j = 0; j < k; ++j)
			{
				swap(&L[k * n + j], &L[pivotRow * n + j]);
			}
		}

		for (int i = k + 1; i < n; ++i)
		{
			float factor = U[i * n + k] / U[k * n + k];
			L[i * n + k] = factor;
			for (int j = k; j < n; ++j)
			{
				U[i * n + j] -= factor * U[k * n + j];
			}
		}

		L[k * n + k] = 1.0f;
	}

	return true;
}

void substLowerT(int n, float* L, float* b, float* y)
{
	for (int i = 0; i < n; i++)
	{
		float S = 0.0f;
		for (int j = 0; j < i; j++)
		{
			S += L[i * n + j] * y[j];
		}

		y[i] = (b[i] - S) / L[i * n + i];
	}
}

void MatrixInverse::scanAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	m_matrixA[m_ScannedElementsCount] = input.inputFloat;

	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "%f	", m_matrixA[m_ScannedElementsCount]);

	m_ScannedElementsCount++;

	if (m_ScannedElementsCount % m_n == 0)
	{
		len = strlen(m_outputBuffer);
		snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\n");
	}

	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = m_outputBuffer;
}

void MatrixInverse::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = false;

	float* L0 = new float[m_n * m_n];
	float* U0 = new float[m_n * m_n];
	float* Original = new float[m_n * m_n];
	float* Inv = new float[m_n * m_n];
	int* P = new int[m_n];
	float* X = new float[m_n];
	float* unitV = new float[m_n];
	float* Y = new float[m_n];

	size_t len = strlen(m_outputBuffer);

	copyMatrix(m_n, m_m, Original, m_matrixA);

	double det = getMatrixDeterminant(m_n, m_m, m_matrixA);
	if (det == 0.0f || !runPLUDecomposition(m_matrixA, L0, U0, P, m_n))
	{
		snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nMatrix cannot be inverted.\n");
		return;
	}

	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nLower Triangular:\n");
	printMatrix(m_n, m_m, L0, m_outputBuffer, sizeof(m_outputBuffer));

	len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nUpper Triangular:\n");
	
	printMatrix(m_n, m_m, U0, m_outputBuffer, sizeof(m_outputBuffer));

	
	for (int i = 0; i < m_n; i++)
	{
		memset(unitV, 0, sizeof(float) * m_n);
		unitV[i] = 1.0f;

		substLowerT(m_n, L0, unitV, Y);
		substUpperT(m_n, U0, Y, X);

		for (int j = 0; j < m_n; j++)
		{
			Inv[j * m_n + P[i]] = X[j];
		}
	}

	len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nInverse Matrix:\n");
	printMatrix(m_n, m_m, Inv, m_outputBuffer, sizeof(m_outputBuffer));

	len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nChecking by multiplying Original * Inv:\n");

	mulMatrix(m_n, m_n, m_n, m_matrixA, Original, Inv);
	printMatrix(m_n, m_m, m_matrixA, m_outputBuffer, sizeof(m_outputBuffer));

	delete[] L0;
	delete[] U0;
	delete[] P;
	delete[] X;
	delete[] Inv;
	delete[] Original;
	delete[] Y;
}

void MatrixInverse::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(m_outputBuffer, sizeof(m_outputBuffer), "Welcome to the Inverse Matrix calculator!\n\n");
	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "Enter the matrix dimension:\n");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = false;
	m_currentStage = 2;
}

void MatrixInverse::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));
	m_m = input.inputInt;
	m_n = input.inputInt;

	m_matrixA = new float[m_n * m_m];

	snprintf(m_outputBuffer, sizeof(m_outputBuffer), "%d x %d matrix\n\n", m_n, m_m);

	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "Enter the elements one by one, separated by Enter \n");

	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	
	m_currentStage = 3;
}

void MatrixInverse::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanAndPrint(pProgramOutput, input);

	if(m_ScannedElementsCount == m_n * m_m)
	{
		calculateAndPrint(pProgramOutput, input);
		reset();
	}
}

void MatrixInverse::reset()
{
	m_currentStage = 0;
	m_ScannedElementsCount = 0;
	m_n = 0, m_m = 0;

	if (m_matrixA != nullptr)
	{
		delete[] m_matrixA;
		m_matrixA = nullptr;
	}
}

void MatrixInverse::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	runStage1(pProgramOutput);
}

void MatrixInverse::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	if (m_currentStage == 0)
	{
		return;
	}

	pProgramOutput->outputIsError = false;

	switch (m_currentStage)
	{
	case 2:
		runStage2(pProgramOutput, input);
		break;
	case 3:
		runStage3(pProgramOutput, input);
		break;
	}
}

void MatrixInverse::getCode(ProgramOutput* pProgramOutput)
{
	reset();
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	snprintf(m_outputBuffer, sizeof(m_outputBuffer),
		R"(bool runPLUDecomposition(float* A, float* L, float* U, int* P, int n)
{
	copyMatrix(n, n, U, A);
	memset(L, 0, n * n * sizeof(float));

	for (int i = 0; i < n; ++i)
	{
		P[i] = i;
	}

	for (int k = 0; k < n; ++k)
	{
		float maxVal = 0.0f;
		int pivotRow = k;
		for (int i = k; i < n; ++i)
		{
			float val = fabs(U[i * n + k]);
			if (val > maxVal)
			{
				maxVal = val;
				pivotRow = i;
			}
		}

		if (maxVal < 1e-8f) return false;

		if (pivotRow != k) 
		{
			for (int j = 0; j < n; ++j)
			{
				swap(&U[k * n + j], &U[pivotRow * n + j]);
			}

			swap(&P[k], &P[pivotRow]);

			for (int j = 0; j < k; ++j)
			{
				swap(&L[k * n + j], &L[pivotRow * n + j]);
			}
		}

		for (int i = k + 1; i < n; ++i)
		{
			float factor = U[i * n + k] / U[k * n + k];
			L[i * n + k] = factor;
			for (int j = k; j < n; ++j)
			{
				U[i * n + j] -= factor * U[k * n + j];
			}
		}

		L[k * n + k] = 1.0f;
	}

	return true;
}

void substLowerT(int n, float* L, float* b, float* y)
{
	for (int i = 0; i < n; i++)
	{
		float S = 0.0f;
		for (int j = 0; j < i; j++)
		{
			S += L[i * n + j] * y[j];
		}

		y[i] = (b[i] - S) / L[i * n + i];
	}
}

inline void substUpperT(int n, float* A, float* Y, float* X)
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

	substUpperTAugmentedA(n, m, A_temp, X); - see the code for Linear Equation System solver.

	delete[] A_temp;
}

... in main: ...

float* L0 = lower triangular matrix of size n x n;
float* U0 = upper triangular matrix of size n x n;
float* A = original matrix of size n x n;
float* Inv = inverse matrix of size n x n;
int* P = pivot permutations of size n;
float* X = output row of size n;
float* unitV = identity row of size n;
float* Y = right-hand-side row of size n;

double det = getMatrixDeterminant(n, m, A); -- see the code for the matrix determinant
if (det == 0.0f || !runPLUDecomposition(A, L0, U0, P, n))
{
	return;
}
	
for (int i = 0; i < n; i++)
{
	memset(unitV, 0, sizeof(float) * n);
	unitV[i] = 1.0f;

	substLowerT(n, L0, unitV, Y);
	substUpperT(n, U0, Y, X);

	for (int j = 0; j < n; j++)
	{
		Inv[j * n + P[i]] = X[j];
	}
}

)");

	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::TypesCount;
}
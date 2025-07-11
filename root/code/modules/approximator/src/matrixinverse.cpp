#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "matrixinverse.h"
#include "helpers.h"

MatrixInverse::MatrixInverse() : Program()
{
	stageCount = 3;
	A = nullptr;
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

void substLowerT(int n, float* L, float* b, float* y, char* buf, int bufsize)
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
	A[scannedElementsCount] = input.inputFloat;

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "%f	", A[scannedElementsCount]);

	scannedElementsCount++;

	if (scannedElementsCount % n == 0)
	{
		len = strlen(outputBuffer);
		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\n");
	}

	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = outputBuffer;
}

void MatrixInverse::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = false;

	float* L0 = new float[n * n];
	float* U0 = new float[n * n];
	float* Original = new float[n * n];
	float* Inv = new float[n * n];
	int* P = new int[n];
	float* X = new float[n];
	float* unitV = new float[n];
	float* Y = new float[n];

	size_t len = strlen(outputBuffer);

	copyMatrix(n, m, Original, A);

	double det = getMatrixDeterminant(n, m, A);
	if (det == 0.0f || !runPLUDecomposition(A, L0, U0, P, n))
	{
		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nMatrix cannot be inverted.\n");
		return;
	}

	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nLower Triangular:\n");
	printMatrix(n, m, L0, outputBuffer, sizeof(outputBuffer));

	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nUpper Triangular:\n");
	
	printMatrix(n, m, U0, outputBuffer, sizeof(outputBuffer));

	
	for (int i = 0; i < n; i++)
	{
		memset(unitV, 0, sizeof(float) * n);
		unitV[i] = 1.0f;

		substLowerT(n, L0, unitV, Y, outputBuffer, sizeof(outputBuffer));
		substUpperT(n, U0, Y, X, outputBuffer, sizeof(outputBuffer));

		for (int j = 0; j < n; j++)
		{
			Inv[j * n + P[i]] = X[j];
		}
	}

	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nInverse Matrix:\n");
	printMatrix(n, m, Inv, outputBuffer, sizeof(outputBuffer));

	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nChecking by multiplying Original * Inv:\n");

	mulMatrix(n, n, n, A, Original, Inv);
	printMatrix(n, m, A, outputBuffer, sizeof(outputBuffer));

	delete[] L0;
	delete[] U0;
	delete[] P;
	delete[] X;
	delete[] Inv;
	delete[] Original;
	delete[] Y;
}

void MatrixInverse::reset()
{
	currentStage = 0;
	scannedElementsCount = 0;
	n = 0, m = 0;

	if (A != nullptr)
	{
		delete[] A;
		A = nullptr;
	}
}

void MatrixInverse::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(outputBuffer, sizeof(outputBuffer), "Welcome to the Inverse Matrix calculator!\n\n");
	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter the matrix dimension:\n");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = false;
	currentStage = 2;
}

void MatrixInverse::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	memset(outputBuffer, 0, sizeof(outputBuffer));
	m = input.inputInt;
	n = input.inputInt;

	A = new float[n * m];

	snprintf(outputBuffer, sizeof(outputBuffer), "%d x %d matrix\n\n", n, m);

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter the elements one by one, separated by Enter \n");

	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	
	currentStage = 3;
}

void MatrixInverse::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanAndPrint(pProgramOutput, input);

	if(scannedElementsCount == n * m)
	{
		calculateAndPrint(pProgramOutput, input);
		reset();
	}
}

void MatrixInverse::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(outputBuffer, 0, sizeof(outputBuffer));

	runStage1(pProgramOutput);
}

void MatrixInverse::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	if (currentStage == 0)
	{
		return;
	}

	pProgramOutput->outputIsError = false;

	switch (currentStage)
	{
	case 2:
		runStage2(pProgramOutput, input);
		break;
	case 3:
		runStage3(pProgramOutput, input);
		break;
	}
}

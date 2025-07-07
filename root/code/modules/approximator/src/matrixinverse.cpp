#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "matrixinverse.h"
#include "helpers.h"

MatrixInverse::MatrixInverse() : Program()
{
	stageCount = 3;
	A = nullptr, L = nullptr;
	reset();
}

MatrixInverse::~MatrixInverse()
{ 
	reset(); 
}

void decompose(int n, int m, float* A, float* B, float* Diag) 
{
	int i, j, d, z = 0;

	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			B[i * n + j] = A[i * n + j];
			if (j == i) {
				Diag[z] = A[i * n + j];
				z += 1;
			}
		}
	}

	z = 0;
	for (d = 0; d < n; d++) {
		for (j = 0; j < n; j++) {
			for (i = d; i < n; i++) {
				if (i >= j) {

					B[i * n + j] = A[i * n + j] / Diag[j];
				}
				else {
					B[i * n + j] = 0;
				}
			}
		}
	}
}

void substLowerT(int columnnumb, int n, float* A, float* Y, char* pBuffer, int bufferSize)
{
	float S = 0;
	int m = n + 1;
	float* A_temp = new float[n * m];

	int i; int j;

	for (i = 0; i < n; i++) 
	{
		for (j = 0; j < m; j++) 
		{
			if (j != n)
			{
				A_temp[i * n + j] = A[i * n + j];
			}
			else 
			{
				if (i == columnnumb) 
				{
					A_temp[i * n + j] = 1;
				}
				else {
					A_temp[i * n + j] = 0;
				}
			}
		}
	}

	size_t len = strlen(pBuffer);
	snprintf(pBuffer + len, bufferSize - len, "Substitution into L: \n");
	printMatrix(n, m, A_temp, pBuffer, bufferSize);

	for (int i = 0; i < n; i++)
	{
		S = 0.0f;
		for (int j = 0; j < i; j++)  
		{
			S += A_temp[i * n + j] * Y[j];  
		}

		Y[i] = (A_temp[i * n + n] - S) / A_temp[i * n + i];  
	}
}


void substUpperT(int n, float* A, float* Y, float* X, char* pBuffer, int bufferSize)
{
	float S = 0;
	int m = n + 1;
	float* A_temp = new float[n * m];
	int i, j;

	for (i = 0; i < n; i++) 
	{
		for (j = 0; j < m; j++) 
		{
			if (j != n)
			{
				A_temp[i * n + j] = A[i * n + j];
			}
			else 
			{
				A_temp[i * n + n] = Y[i];
			}
		}
	}

	size_t len = strlen(pBuffer);
	snprintf(pBuffer + len, bufferSize - len, "Substitution into U: \n");

	printMatrix(n, m, A_temp, pBuffer, bufferSize);

	for (i = n - 1; i >= 0; i--) 
	{
		for (j = m - 2; j > i; j--) 
		{
			S += A_temp[i * n + j] * X[j];
		}

		X[i] = (A_temp[i * n + (m - 1)] - S) / A_temp[i * n + i];
		S = 0;
	}
}


void MatrixInverse::scanAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	A[scannedElementsCount] = input.inputFloat;

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "%.2f	", A[scannedElementsCount]);

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
	float* L0 = new float[n * m];
	float* Inv = new float[n * n];
	float* X = new float[n];
	float* Y = new float[n];
	float* Diag = new float[n];

	decompose(n, m, A, L0, Diag);

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nMatrix after decomposing:\n");
	printMatrix(n, m, A, outputBuffer, sizeof(outputBuffer));

	runGaussianElimination(n, m, A, L, outputBuffer, sizeof(outputBuffer));

	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nMatrix after Gaussian elimination:\n");
	
	printMatrix(n, m, A, outputBuffer, sizeof(outputBuffer));

	/*double det = getMatrixDeterminant(n, m, A);
	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nDeterminant: %.2f \n", det);*/

	for (i = 0; i < n; i++) 
	{
		substLowerT(i, n, L0, Y, outputBuffer, sizeof(outputBuffer));
		substUpperT(n, A, Y, X, outputBuffer, sizeof(outputBuffer));

		for (j = 0; j < m; j++) 
		{
			Inv[j * n + i] = X[j];
		}
	}

	printf("\nInverse Matrix =\n");
	printMatrix(n, m, Inv, outputBuffer, sizeof(outputBuffer));

	delete[] L0;
	delete[] X;
	delete[] Y;
	delete[] Diag;
	delete[] Inv;

	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = false;
}

void MatrixInverse::reset()
{
	currentStage = 0;
	scannedElementsCount = 0;
	n = 0, i = 0, j = 0, m = 0;

	if (A != nullptr)
	{
		delete[] A;
		A = nullptr;
	}

	if (L != nullptr)
	{
		delete[] L;
		L = nullptr;
	}

}

void MatrixInverse::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(outputBuffer, sizeof(outputBuffer), "Welcome to the Inverse Matrix calculator!\n\n");
	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter the matrix dimension:\n");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = outputBuffer;
	currentStage = 2;
}

void MatrixInverse::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	memset(outputBuffer, 0, sizeof(outputBuffer));
	m = input.inputInt;
	n = input.inputInt;

	A = new float[n * m];
	L = new float[m];

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

	pProgramOutput->outputIsError = false;
	currentStage = 2;
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

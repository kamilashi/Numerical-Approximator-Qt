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

void decompose(int n, int m, float* A, float* B, float* Diag) 
{
	int i, j, d, z = 0;

	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			B[i * m + j] = A[i * m + j];
			if (j == i) {
				Diag[z] = A[i * m + j];
				z += 1;
			}
		}
	}

	z = 0;
	for (d = 0; d < n; d++) {
		for (j = 0; j < m; j++) {
			for (i = d; i < n; i++) {
				if (i >= j) {

					B[i * m + j] = A[i * m + j] / Diag[j];
				}
				else {
					B[i * m + j] = 0;
				}
			}
		}
	}
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
	float* L0 = new float[n * n];
	float* Original = new float[n * n];
	float* Inv = new float[n * n];
	float* X = new float[n];
	float* Diag = new float[n];
	float* unitV = new float[n];
	float* Y = new float[n];

	copyMatrix(n, m, Original, A);

	decompose(n, m, A, L0, Diag);

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nLower Triangular:\n");
	printMatrix(n, m, L0, outputBuffer, sizeof(outputBuffer));

	runGaussianEliminationWithPivoting(n, m, A, outputBuffer, sizeof(outputBuffer));

	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nUpper Triangular via Gaussian Elimination:\n");
	
	printMatrix(n, m, A, outputBuffer, sizeof(outputBuffer));

	double det = getMatrixDeterminant(n, m, A);
	len = strlen(outputBuffer);

	if (det != 0.0f)
	{
		for (i = 0; i < n; i++)
		{
			memset(unitV, 0, sizeof(float)*n);
			unitV[i] = 1.0f;

			substLowerT(n, L0, unitV,Y, outputBuffer, sizeof(outputBuffer));
			substUpperT(n, A, Y, X, outputBuffer, sizeof(outputBuffer));

			for (j = 0; j < n; j++)
			{
				//int colNum = (n - 1 - i);
				Inv[j * n + i] = X[j];
			}
		}

		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nInverse Matrix:\n");
		printMatrix(n, m, Inv, outputBuffer, sizeof(outputBuffer));

		len = strlen(outputBuffer);
		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nChecking by multiplying Original * Inv:\n");

		mulMatrix(n, n, n, A, Original, Inv);
		printMatrix(n, m, A, outputBuffer, sizeof(outputBuffer));
	}
	else
	{
		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nDeterminant is zero, the matrix has no inverse! \n");
	}

	delete[] L0;
	delete[] X;
	delete[] Diag;
	delete[] Inv;
	delete[] Original;
	delete[] Y;

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

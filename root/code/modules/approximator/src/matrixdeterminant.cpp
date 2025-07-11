#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "matrixdeterminant.h"
#include "helpers.h"

MatrixDeterminant::MatrixDeterminant() : Program()
{
	stageCount = 3;
	A = nullptr;
	reset();
}

MatrixDeterminant::~MatrixDeterminant() 
{ 
	reset(); 
}

void MatrixDeterminant::scanAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MatrixDeterminant::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	size_t len = strlen(outputBuffer);

	runGaussianEliminationWithPivoting(n, m, A, outputBuffer, sizeof(outputBuffer));

	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nMatrix after Gaussian elimination:\n");
	
	printMatrix(n, m, A, outputBuffer, sizeof(outputBuffer));

	double det = getMatrixDeterminant(n, m, A);
	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nDeterminant: %.2f \n", det);

	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = false;
}

void MatrixDeterminant::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(outputBuffer, sizeof(outputBuffer), "Welcome to the matrix determinant calculator via Gaussian elimination with pivoting!\n\n");
	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter the matrix dimension:\n");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = false;
	currentStage = 2;
}

void MatrixDeterminant::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MatrixDeterminant::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanAndPrint(pProgramOutput, input);

	if(scannedElementsCount == n * m)
	{
		calculateAndPrint(pProgramOutput, input);
		reset();
	}
}

void MatrixDeterminant::reset()
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

void MatrixDeterminant::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(outputBuffer, 0, sizeof(outputBuffer));

	runStage1(pProgramOutput);
}

void MatrixDeterminant::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MatrixDeterminant::getCode(ProgramOutput* pProgramOutput)
{
	reset();
	memset(outputBuffer, 0, sizeof(outputBuffer));

	snprintf(outputBuffer, sizeof(outputBuffer),
		R"(inline double getMatrixDeterminant(int n, int m, float* A)
{
	double det = 1.0;

	for (int i = 0, j = 0; i < n && j < m; i++, j++)
	{
		det *= A[i * n + j];
	}

	return det;
}

... in main: ...


runGaussianEliminationWithPivoting(n, m, A, outputBuffer, sizeof(outputBuffer)); - see the code for Linear Equation System solver.

double det = getMatrixDeterminant(n, m, A);

)");

	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::TypesCount;
}

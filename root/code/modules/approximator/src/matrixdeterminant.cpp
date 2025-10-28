#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "matrixdeterminant.h"
#include "helpers.h"

MatrixDeterminant::MatrixDeterminant() : Program()
{
	m_stageCount = 3;
	m_matrixA = nullptr;
	reset();
}

MatrixDeterminant::~MatrixDeterminant() 
{ 
	reset(); 
}

void MatrixDeterminant::scanAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	m_matrixA[m_scannedElementsCount] = input.inputFloat;

	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "%.2f	", m_matrixA[m_scannedElementsCount]);

	m_scannedElementsCount++;

	if (m_scannedElementsCount % m_n == 0)
	{
		len = strlen(m_outputBuffer);
		snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\n");
	}

	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = m_outputBuffer;
}

void MatrixDeterminant::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	size_t len = strlen(m_outputBuffer);

	runGaussianEliminationWithPivoting(m_n, m_m, m_matrixA, m_outputBuffer, sizeof(m_outputBuffer));

	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nMatrix after Gaussian elimination:\n");
	
	printMatrix(m_n, m_m, m_matrixA, m_outputBuffer, sizeof(m_outputBuffer));

	double det = getMatrixDeterminant(m_n, m_m, m_matrixA);
	len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nDeterminant: %.2f \n", det);

	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = false;
}

void MatrixDeterminant::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(m_outputBuffer, sizeof(m_outputBuffer), "Welcome to the matrix determinant calculator via Gaussian elimination with pivoting!\n\n");
	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "Enter the matrix dimension:\n");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = false;
	m_currentStage = 2;
}

void MatrixDeterminant::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MatrixDeterminant::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanAndPrint(pProgramOutput, input);

	if(m_scannedElementsCount == m_n * m_m)
	{
		calculateAndPrint(pProgramOutput, input);
		reset();
	}
}

void MatrixDeterminant::reset()
{
	m_currentStage = 0;
	m_scannedElementsCount = 0;
	m_n = 0, m_m = 0;

	if (m_matrixA != nullptr)
	{
		delete[] m_matrixA;
		m_matrixA = nullptr;
	}
}

void MatrixDeterminant::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	runStage1(pProgramOutput);
}

void MatrixDeterminant::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MatrixDeterminant::getCode(ProgramOutput* pProgramOutput)
{
	reset();
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	snprintf(m_outputBuffer, sizeof(m_outputBuffer),
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

	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::TypesCount;
}

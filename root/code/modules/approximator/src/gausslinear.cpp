#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "helpers.h"
#include "gausslinear.h"

GaussLinear::GaussLinear() : Program()
{
	m_stageCount = 3;
	m_matrixA = nullptr;
	reset();
}

GaussLinear::~GaussLinear()
{ 
	reset(); 
}

void GaussLinear::scanTermsAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	m_matrixA[m_scannedElementsCount] = input.inputFloat;
	m_scannedElementsCount++;

	size_t len = strlen(m_outputBuffer);

	size_t index = m_scannedElementsCount - 1;
	char* signStr = index % m_termCount == 0 && (m_matrixA[index] > 0) ? "" : m_matrixA[index] < 0 ? "-" : "+";

	if (m_scannedElementsCount % m_termCount != 0)
	{
		int column = m_scannedElementsCount / m_termCount;
		int row = m_scannedElementsCount % m_termCount;
		snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "%s %f * x[%d][%d]  ", signStr, fabs(m_matrixA[index]), column, row);
	}
	else
	{
		snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "  =   %f\n", (m_matrixA[index]));
	}


	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = m_outputBuffer;
}

void GaussLinear::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nApplying Gaussian elimination:\n\n");

	float* X = new float[m_degree];

	runGaussianEliminationWithPivoting(m_degree, m_termCount, m_matrixA, m_outputBuffer, sizeof(m_outputBuffer));

	printMatrix(m_degree, m_termCount, m_matrixA, m_outputBuffer, sizeof(m_outputBuffer));

	substUpperTAugmentedA(m_degree, m_termCount, m_matrixA, X);

	len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nThe roots are: \n");
	for (int i = 0; i < m_degree; i++)
	{
		size_t len = strlen(m_outputBuffer);
		snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "x[%d]  =  %f \n", i, X[i]);
	}
	
	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = false;

	delete[] X;
}

void GaussLinear::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(m_outputBuffer, sizeof(m_outputBuffer), "Welcome to the linear equation system solver via Gauss elimination with pivoting!\n\n");
	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "Enter the number of unknowns:	");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	m_currentStage = 2;
}

void GaussLinear::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	m_degree = input.inputInt;
	m_termCount = m_degree + 1;

	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "%d \n\n", m_degree);

	m_matrixA = new float[m_termCount * m_degree];

	len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "Enter the coefficients and the RHS one by one, separated by Enter: \n");

	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	
	m_currentStage = 3;
}

void GaussLinear::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanTermsAndPrint(pProgramOutput, input);

	if(m_scannedElementsCount == m_termCount * m_degree)
	{
		calculateAndPrint(pProgramOutput, input);
	}
}

void GaussLinear::reset()
{
	m_currentStage = 0;
	m_scannedElementsCount = 0;
	m_degree = 0;
	m_termCount = 0;

	if (m_matrixA != nullptr)
	{
		delete[] m_matrixA;
		m_matrixA = nullptr;
	}
}

void GaussLinear::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	runStage1(pProgramOutput);
}

void GaussLinear::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void GaussLinear::getCode(ProgramOutput* pProgramOutput)
{
	reset();
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	snprintf(m_outputBuffer, sizeof(m_outputBuffer),
		R"(inline void pivotMatrix(int startrow, int n, int m, float* A)
{
	int k = startrow, h;
	int maxInd = k;
	float max = A[k * m + k];

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
			swap(&A[k * m + h], &A[maxInd * m + h]);
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
			snprintf(pBuffer + len, bufferSize - len, "\nZero or near-zero pivot at row %%d! \n", d);
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

inline void substUpperTAugmentedA(int n, int m, float* A, float* X)
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

.... in main: ...

A is the coefficients matrix augmented by the right-hand-side.
X is the array of unknowns / roots.

runGaussianEliminationWithPivoting(degree, termCount, A, outputBuffer, sizeof(outputBuffer));

substUpperTAugmentedA(degree, termCount, A, Roots, outputBuffer, sizeof(outputBuffer));
)");

	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::TypesCount;
}

#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "helpers.h"
#include "gausslinear.h"

GaussLinear::GaussLinear() : Program()
{
	stageCount = 3;
	A = nullptr;
	reset();
}

GaussLinear::~GaussLinear()
{ 
	reset(); 
}

void GaussLinear::scanTermsAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	A[scannedElementsCount] = input.inputFloat;
	scannedElementsCount++;

	size_t len = strlen(outputBuffer);

	size_t index = scannedElementsCount - 1;
	char* signStr = index % termCount == 0 && (A[index] > 0) ? "" : A[index] < 0 ? "-" : "+";

	if (scannedElementsCount % termCount != 0)
	{
		int column = scannedElementsCount / termCount;
		int row = scannedElementsCount % termCount;
		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "%s %f * x[%d][%d]  ", signStr, fabs(A[index]), column, row);
	}
	else
	{
		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "  =   %f\n", (A[index]));
	}


	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = outputBuffer;
}

void GaussLinear::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nApplying Gaussian elimination:\n\n");

	float* X = new float[degree];

	runGaussianEliminationWithPivoting(degree, termCount, A, outputBuffer, sizeof(outputBuffer));

	printMatrix(degree, termCount, A, outputBuffer, sizeof(outputBuffer));

	substUpperTAugmentedA(degree, termCount, A, X);

	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nThe roots are: \n");
	for (int i = 0; i < degree; i++)
	{
		size_t len = strlen(outputBuffer);
		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "x[%d]  =  %f \n", i, X[i]);
	}
	
	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = false;

	delete[] X;
}

void GaussLinear::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(outputBuffer, sizeof(outputBuffer), "Welcome to the linear equation system solver via Gauss elimination with pivoting!\n\n");
	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter the number of unknowns:	");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	currentStage = 2;
}

void GaussLinear::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	degree = input.inputInt;
	termCount = degree + 1;

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "%d \n\n", degree);

	A = new float[termCount * degree];

	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter the coefficients and the RHS one by one, separated by Enter: \n");

	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	
	currentStage = 3;
}

void GaussLinear::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanTermsAndPrint(pProgramOutput, input);

	if(scannedElementsCount == termCount * degree)
	{
		calculateAndPrint(pProgramOutput, input);
	}
}

void GaussLinear::reset()
{
	currentStage = 0;
	scannedElementsCount = 0;
	degree = 0;
	termCount = 0;

	if (A != nullptr)
	{
		delete[] A;
		A = nullptr;
	}
}

void GaussLinear::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(outputBuffer, 0, sizeof(outputBuffer));

	runStage1(pProgramOutput);
}

void GaussLinear::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void GaussLinear::getCode(ProgramOutput* pProgramOutput)
{
	reset();
	memset(outputBuffer, 0, sizeof(outputBuffer));

	snprintf(outputBuffer, sizeof(outputBuffer),
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

	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::TypesCount;
}

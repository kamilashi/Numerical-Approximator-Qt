#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "helpers.h"
#include "falseposition.h"

FalsePositionMethod::FalsePositionMethod() : Program()
{
	stageCount = 5;
	F = nullptr;
	reset();
}

FalsePositionMethod::~FalsePositionMethod()
{ 
	reset(); 
}

float runFalsePosition(float xl, float xr, int iter,int maxIter, int termCount, float* F, char* pBuffer, int bufferSize)
{
	const float zeroError = 0.0001f;

	if (iter < maxIter)
	{
		float yl = polynomial(xl, termCount, F);
		float yr = polynomial(xr, termCount, F);

		if (fabs(yl - yr) < zeroError)
		{
			size_t len = strlen(pBuffer);
			snprintf(pBuffer + len, bufferSize - len, "\nDenominator too small at iteration %d, returning midpoint.\n", iter);
			return (xl + xr) / 2.0f;
		}

		float xm = xr - (yr * (xl - xr)) / (yl - yr);
		iter++;

		float ym = polynomial(xm, termCount, F);

		size_t len = strlen(pBuffer);
		snprintf(pBuffer + len, bufferSize - len, "\nIteration %d: xm = %f \n", iter, xm);

		if (yl * ym < 0)
		{
			return runFalsePosition(xl, xm, iter, maxIter, termCount, F, pBuffer, bufferSize);
		}
		
		if (ym * yr < 0)
		{
			return runFalsePosition(xm, xr, iter, maxIter, termCount, F, pBuffer, bufferSize);
		}
		
		return xm;
	}

	size_t len = strlen(pBuffer);
	snprintf(pBuffer + len, bufferSize - len, "\nIteration limit of %d reached, returning midpoint.\n", maxIter);
	return (xl + xr) / 2.0f;
}

void FalsePositionMethod::scanTermsAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	F[scannedElementsCount] = input.inputFloat;

	size_t len = strlen(outputBuffer);

	char* signStr = scannedElementsCount == 0 && (F[scannedElementsCount] > 0) ? "" : F[scannedElementsCount] < 0 ? "-" : "+";

	if (scannedElementsCount != degree)
	{
		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "%s%f * x^%d ", signStr, fabs(F[scannedElementsCount]), degree - scannedElementsCount);
	}
	else
	{
		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "%s%f\n\n", signStr, fabs(F[scannedElementsCount]));
	}

	scannedElementsCount++;

	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = outputBuffer;
}

void FalsePositionMethod::scanRootGuessesAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	G[scannedElementsCount] = input.inputFloat;

	size_t len = strlen(outputBuffer);

	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "%f  ", (G[scannedElementsCount]));

	scannedElementsCount++;

	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = outputBuffer;
}

void FalsePositionMethod::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\n\nSolving with the False Position method:\n");

	int startIteration = 0;
	float result = runFalsePosition(G[0], G[1], startIteration, maxIterationCount, termCount, F, outputBuffer, sizeof(outputBuffer));

	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nFinal guess: %f\n", result);
	
	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = false;
}

void FalsePositionMethod::reset()
{
	currentStage = 0;
	scannedElementsCount = 0;
	degree = 0;
	termCount = 0;

	if (F != nullptr)
	{
		delete[] F;
		F = nullptr;
	}
}

void FalsePositionMethod::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(outputBuffer, sizeof(outputBuffer), "Welcome to the equation solver via the False Position method!\n\n");
	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter the degree of polynomial:	");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	currentStage = 2;
}

void FalsePositionMethod::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	degree = input.inputInt;
	termCount = degree + 1;

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "%d \n\n", degree);

	F = new float[termCount];

	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter the coefficients one by one, separated by Enter: \n");

	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	
	currentStage = 3;
}

void FalsePositionMethod::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanTermsAndPrint(pProgramOutput, input);

	if(scannedElementsCount == termCount)
	{
		size_t len = strlen(outputBuffer);
		snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Press any key to continue\n");

		pProgramOutput->requestedInputType = InputType::Any;
		pProgramOutput->outputIsError = false;
		currentStage = 4;
		scannedElementsCount = 0;
	}
}

void FalsePositionMethod::runStage4(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	memset(outputBuffer, 0, sizeof(outputBuffer));

	snprintf(outputBuffer, sizeof(outputBuffer), "Enter the xl and xr, separated by Enter: \n");

	currentStage = 5;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
}

void FalsePositionMethod::runStage5(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanRootGuessesAndPrint(pProgramOutput, input);

	if (scannedElementsCount == guessCount)
	{
		calculateAndPrint(pProgramOutput, input);
		reset();
	}
}

void FalsePositionMethod::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(outputBuffer, 0, sizeof(outputBuffer));

	runStage1(pProgramOutput);
}

void FalsePositionMethod::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
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
	case 4:
		runStage4(pProgramOutput, input);
		break;
	case 5:
		runStage5(pProgramOutput, input);
		break;
	}
}

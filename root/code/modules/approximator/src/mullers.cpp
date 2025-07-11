#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "helpers.h"
#include "mullers.h"

MullersMethod::MullersMethod() : Program()
{
	stageCount = 5;
	F = nullptr;
	reset();
}

MullersMethod::~MullersMethod()
{ 
	reset(); 
}

float runMullersMethod(float g0, float g1, float g2, int n, float* F, char* pBuffer, int bufferSize)
{
	float h0, h1;
	float d0, d1;
	float a, b, c;
	float g3;
	float discriminant;
	float error;
	const float errorMargin = 0.001f;

	h0 = g1 - g0;
	h1 = g2 - g1;
	d0 = (polynomial(g1, n, F) - polynomial(g0, n, F)) / (g1 - g0);
	d1 = (polynomial(g2, n, F) - polynomial(g1, n, F)) / (g2 - g1);
	a = (d1 - d0) / (h1 + h0);
	b = a * h1 + d1;
	c = polynomial(g2, n, F);

	size_t len = strlen(pBuffer);
	snprintf(pBuffer + len, bufferSize - len, "a = %f  b = %f  c = %f  \n\n", a, b, c);

	float discArg = b * b - 4 * a * c;

	len = strlen(pBuffer);
	if (discArg < 0)
	{
		snprintf(pBuffer + len, bufferSize - len, "The equation has no real roots!\n\n");
	}
	else
	{
		discriminant = sqrt(b * b - 4 * a * c);
		snprintf(pBuffer + len, bufferSize - len, "Discriminant = %f\n\n", discriminant);

		if (fabs(b + discriminant) < fabs(b - discriminant))
		{
			g3 = g2 + ((-2) * c) / (b - discriminant);
		}
		else
		{
			g3 = g2 + ((-2) * c) / (b + discriminant);
		}

		error = fabs((g3 - g2) / g3);
		if (error > errorMargin)
		{
			len = strlen(pBuffer);
			snprintf(pBuffer + len, bufferSize - len, "Temp. guess 3 = %f\n\n", g3);
			return runMullersMethod(g1, g2, g3, n, F, pBuffer, bufferSize);
		}

		return g3;
	}

	return 0;
}

void MullersMethod::scanTermsAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MullersMethod::scanRootGuessesAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	G[scannedElementsCount] = input.inputFloat;

	size_t len = strlen(outputBuffer);

	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "%f  ", (G[scannedElementsCount]));

	scannedElementsCount++;

	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = outputBuffer;
}

void MullersMethod::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\n\nSolving with Muller's method:\n");

	float result = runMullersMethod(G[0], G[1], G[2], termCount, F, outputBuffer, sizeof(outputBuffer));

	len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "\nFinal guess: %f\n", result);
	
	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = false;
}

void MullersMethod::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(outputBuffer, sizeof(outputBuffer), "Welcome to the equation solver via Muller's method!\n\n");
	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter the degree of polynomial:	");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	currentStage = 2;
}

void MullersMethod::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MullersMethod::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MullersMethod::runStage4(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	memset(outputBuffer, 0, sizeof(outputBuffer));

	snprintf(outputBuffer, sizeof(outputBuffer), "Enter the 3 guesses one by one, separated by Enter: \n");

	currentStage = 5;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
}

void MullersMethod::runStage5(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanRootGuessesAndPrint(pProgramOutput, input);

	if (scannedElementsCount == guessCount)
	{
		calculateAndPrint(pProgramOutput, input);
		reset();
	}
}

void MullersMethod::reset()
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

void MullersMethod::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(outputBuffer, 0, sizeof(outputBuffer));

	runStage1(pProgramOutput);
}

void MullersMethod::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MullersMethod::getCode(ProgramOutput* pProgramOutput)
{
	reset();
	memset(outputBuffer, 0, sizeof(outputBuffer));

	snprintf(outputBuffer, sizeof(outputBuffer),
		R"(float runMullersMethod(float g0, float g1, float g2, int n, float* F, char* pBuffer, int bufferSize)
{
	float h0, h1;
	float d0, d1;
	float a, b, c;
	float g3;
	float discriminant;
	float error;
	const float errorMargin = 0.001f;

	h0 = g1 - g0;
	h1 = g2 - g1;
	d0 = (polynomial(g1, n, F) - polynomial(g0, n, F)) / (g1 - g0);
	d1 = (polynomial(g2, n, F) - polynomial(g1, n, F)) / (g2 - g1);
	a = (d1 - d0) / (h1 + h0);
	b = a * h1 + d1;
	c = polynomial(g2, n, F);

	size_t len = strlen(pBuffer);
	snprintf(pBuffer + len, bufferSize - len, "a = %%f  b = %%f  c = %%f  \n\n", a, b, c);

	float discArg = b * b - 4 * a * c;

	len = strlen(pBuffer);
	if (discArg < 0)
	{
		snprintf(pBuffer + len, bufferSize - len, "The equation has no real roots!\n\n");
	}
	else
	{
		discriminant = sqrt(b * b - 4 * a * c);
		snprintf(pBuffer + len, bufferSize - len, "Discriminant = %%f\n\n", discriminant);

		if (fabs(b + discriminant) < fabs(b - discriminant))
		{
			g3 = g2 + ((-2) * c) / (b - discriminant);
		}
		else
		{
			g3 = g2 + ((-2) * c) / (b + discriminant);
		}

		error = fabs((g3 - g2) / g3);
		if (error > errorMargin)
		{
			len = strlen(pBuffer);
			snprintf(pBuffer + len, bufferSize - len, "Temp. guess 3 = %%f\n\n", g3);
			return runMullersMethod(g1, g2, g3, n, F, pBuffer, bufferSize);
		}

		return g3;
	}

	return 0;
})");

	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::TypesCount;
}
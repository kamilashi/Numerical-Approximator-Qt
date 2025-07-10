#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "maclaurincosine.h"
#include "helpers.h"

MaclaurinCosine::MaclaurinCosine() : Program()
{
	stageCount = 2;
	reset();
}

MaclaurinCosine::~MaclaurinCosine() 
{ 
	reset(); 
}

int factorial(int x) 
{
	if (x == 0)
	{
		return 1;
	}

	int ans = 1;

	for (int i = 1; i <= x; i++) 
	{
		ans *= i;
	}

	return ans;
}


float getCosine(float x, int maxIterationCount)
{
	const float error = 0.01;
	float cos = 0;
	float term;

	x = normalizeAngle(x);

	for (int i = 0; i < maxIterationCount; i++)
	{
		int power = 2 * i;
		float numerator = pow(x, power);
		float denominator = factorial(power);

		term = pow(-1, i) * (numerator / denominator);

		cos += term;

		if (fabs(term) < error)
		{
			break;
		}
	}

	return cos;
}

void MaclaurinCosine::scanArgumentAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	x = input.inputFloat;

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "%f \n\n", x);
}

void MaclaurinCosine::scanResolutionAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	maxIterationCount = input.inputInt;

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer)- len, "%d	\n\n", maxIterationCount);
}

void MaclaurinCosine::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	float cosine = getCosine(x, maxIterationCount);

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Cosine of %.2f = %f \n",x, cosine);

	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = false;
}

void MaclaurinCosine::reset()
{
	currentStage = 0;
	x = 0;
}

void MaclaurinCosine::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(outputBuffer, sizeof(outputBuffer), "Welcome to the cosine calculator using the Maclaurin series!\n\n");
	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter x in radians:  ");

	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	currentStage = 2;
}

void MaclaurinCosine::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanArgumentAndPrint(pProgramOutput, input);

	size_t len = strlen(outputBuffer);
	snprintf(outputBuffer + len, sizeof(outputBuffer) - len, "Enter the approximation limit:  ");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = outputBuffer;
	pProgramOutput->outputIsError = true;
	currentStage = 3;
}


void MaclaurinCosine::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanResolutionAndPrint(pProgramOutput, input);

	calculateAndPrint(pProgramOutput, input);
	reset();
}

void MaclaurinCosine::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(outputBuffer, 0, sizeof(outputBuffer));

	runStage1(pProgramOutput);
}

void MaclaurinCosine::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "helpers.h"
#include "bisection.h"

BisectionMethod::BisectionMethod() : Program()
{
	m_stageCount = 5;
	m_matrixF = nullptr;
	reset();
}

BisectionMethod::~BisectionMethod()
{ 
	reset(); 
}

float runBisection(float xl, float xr, int iter,int maxIter, int termCount, float* F, char* pBuffer, int bufferSize)
{
	float xm = (xl + xr) / 2.0f;

	if (iter < maxIter)
	{
		size_t len = strlen(pBuffer);
		snprintf(pBuffer + len, bufferSize - len, "\nIteration %d: xl = %f, xr = %f \n",iter, xl, xr);

		float ym = polynomial(xm, termCount, F);
		float yl = polynomial(xl, termCount, F);
		float yr = polynomial(xr, termCount, F);
		iter++;

		if (yl * ym < 0)
		{
			return runBisection(xl, xm, iter, maxIter, termCount, F, pBuffer, bufferSize);
		}
		
		if (ym * yr < 0)
		{
			return runBisection(xm, xr, iter,  maxIter, termCount,F, pBuffer, bufferSize);
		}
	}
	else
	{
		size_t len = strlen(pBuffer);
		snprintf(pBuffer + len, bufferSize - len, "\nIteration limit of %d reached, returning midpoint.\n", maxIter);
	}

	return xm;
}

void BisectionMethod::scanTermsAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	m_matrixF[m_scannedElementsCount] = input.inputFloat;

	size_t len = strlen(m_outputBuffer);

	char* signStr = m_scannedElementsCount == 0 && (m_matrixF[m_scannedElementsCount] > 0) ? "" : m_matrixF[m_scannedElementsCount] < 0 ? "-" : "+";

	if (m_scannedElementsCount != m_degree)
	{
		snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "%s%f * x^%d ", signStr, fabs(m_matrixF[m_scannedElementsCount]), m_degree - m_scannedElementsCount);
	}
	else
	{
		snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "%s%f\n\n", signStr, fabs(m_matrixF[m_scannedElementsCount]));
	}

	m_scannedElementsCount++;

	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = m_outputBuffer;
}

void BisectionMethod::scanRootGuessesAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	m_matrixG[m_scannedElementsCount] = input.inputFloat;

	size_t len = strlen(m_outputBuffer);

	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "%f  ", (m_matrixG[m_scannedElementsCount]));

	m_scannedElementsCount++;

	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = m_outputBuffer;
}

void BisectionMethod::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\n\nSolving with the Bisection method:\n");

	int startIteration = 0;
	float result = runBisection(m_matrixG[0], m_matrixG[1], startIteration, m_sMaxIterationCount, m_termCount, m_matrixF, m_outputBuffer, sizeof(m_outputBuffer));

	len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nFinal guess: %f\n", result);
	
	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = false;
}

void BisectionMethod::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(m_outputBuffer, sizeof(m_outputBuffer), "Welcome to the equation solver via the Bisection method!\n\n");
	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "Enter the degree of polynomial:	");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	m_currentStage = 2;
}

void BisectionMethod::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	m_degree = input.inputInt;
	m_termCount = m_degree + 1;

	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "%d \n\n", m_degree);

	m_matrixF = new float[m_termCount];

	len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "Enter the coefficients one by one, separated by Enter: \n");

	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	
	m_currentStage = 3;
}

void BisectionMethod::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanTermsAndPrint(pProgramOutput, input);

	if(m_scannedElementsCount == m_termCount)
	{
		size_t len = strlen(m_outputBuffer);
		snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "Press any key to continue\n");

		pProgramOutput->requestedInputType = InputType::Any;
		pProgramOutput->outputIsError = false;
		m_currentStage = 4;
		m_scannedElementsCount = 0;
	}
}

void BisectionMethod::runStage4(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	snprintf(m_outputBuffer, sizeof(m_outputBuffer), "Enter the xl and xr, separated by Enter: \n");

	m_currentStage = 5;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
}

void BisectionMethod::runStage5(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanRootGuessesAndPrint(pProgramOutput, input);

	if (m_scannedElementsCount == m_sGuessCount)
	{
		calculateAndPrint(pProgramOutput, input);
		reset();
	}
}

void BisectionMethod::reset()
{
	m_currentStage = 0;
	m_scannedElementsCount = 0;
	m_degree = 0;
	m_termCount = 0;

	if (m_matrixF != nullptr)
	{
		delete[] m_matrixF;
		m_matrixF = nullptr;
	}
}

void BisectionMethod::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	runStage1(pProgramOutput);
}

void BisectionMethod::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
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
	case 4:
		runStage4(pProgramOutput, input);
		break;
	case 5:
		runStage5(pProgramOutput, input);
		break;
	}
}

void BisectionMethod::getCode(ProgramOutput* pProgramOutput)
{
	reset();
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	snprintf(m_outputBuffer, sizeof(m_outputBuffer),
		R"(float runBisection(float xl, float xr, int iter,int maxIter, int termCount, float* F, char* pBuffer, int bufferSize)
{
	float xm = (xl + xr) / 2.0f;

	if (iter < maxIter)
	{
		size_t len = strlen(pBuffer);
		snprintf(pBuffer + len, bufferSize - len, "\nIteration %%d: xl = %%f, xr = %%f \n", iter, xl, xr);

		float ym = polynomial(xm, termCount, F);
		float yl = polynomial(xl, termCount, F);
		float yr = polynomial(xr, termCount, F);
		iter++;

		if (yl * ym < 0)
		{
			return runBisection(xl, xm, iter, maxIter, termCount, F, pBuffer, bufferSize);
		}

		if (ym * yr < 0)
		{
			return runBisection(xm, xr, iter, maxIter, termCount, F, pBuffer, bufferSize);
		}
	}
	else
	{
		size_t len = strlen(pBuffer);
		snprintf(pBuffer + len, bufferSize - len, "\nIteration limit of %%d reached, returning midpoint.\n", maxIter);
	}

	return xm;
})");

	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::TypesCount;
}
#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>

#include "helpers.h"
#include "mullers.h"

MullersMethod::MullersMethod() : Program()
{
	m_stageCount = 5;
	m_matrixF = nullptr;
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

void MullersMethod::scanRootGuessesAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	m_matrixG[m_scannedElementsCount] = input.inputFloat;

	size_t len = strlen(m_outputBuffer);

	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "%f  ", (m_matrixG[m_scannedElementsCount]));

	m_scannedElementsCount++;

	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
	pProgramOutput->pOutput = m_outputBuffer;
}

void MullersMethod::calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\n\nSolving with Muller's method:\n");

	float result = runMullersMethod(m_matrixG[0], m_matrixG[1], m_matrixG[2], m_termCount, m_matrixF, m_outputBuffer, sizeof(m_outputBuffer));

	len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "\nFinal guess: %f\n", result);
	
	pProgramOutput->requestedInputType = InputType::TypesCount;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = false;
}

void MullersMethod::runStage1(ProgramOutput* pProgramOutput)
{
	snprintf(m_outputBuffer, sizeof(m_outputBuffer), "Welcome to the equation solver via Muller's method!\n\n");
	size_t len = strlen(m_outputBuffer);
	snprintf(m_outputBuffer + len, sizeof(m_outputBuffer) - len, "Enter the degree of polynomial:	");

	pProgramOutput->requestedInputType = InputType::Int;
	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	m_currentStage = 2;
}

void MullersMethod::runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MullersMethod::runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MullersMethod::runStage4(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	snprintf(m_outputBuffer, sizeof(m_outputBuffer), "Enter the 3 guesses one by one, separated by Enter: \n");

	m_currentStage = 5;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::Float;
}

void MullersMethod::runStage5(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	scanRootGuessesAndPrint(pProgramOutput, input);

	if (m_scannedElementsCount == m_sGuessCount)
	{
		calculateAndPrint(pProgramOutput, input);
		reset();
	}
}

void MullersMethod::reset()
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

void MullersMethod::start(ProgramOutput* pProgramOutput)
{
	reset();
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	runStage1(pProgramOutput);
}

void MullersMethod::proceed(ProgramOutput* pProgramOutput, const ProgramInput& input)
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

void MullersMethod::getCode(ProgramOutput* pProgramOutput)
{
	reset();
	memset(m_outputBuffer, 0, sizeof(m_outputBuffer));

	snprintf(m_outputBuffer, sizeof(m_outputBuffer),
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

	pProgramOutput->pOutput = m_outputBuffer;
	pProgramOutput->outputIsError = true;
	pProgramOutput->requestedInputType = InputType::TypesCount;
}
#include "approximator.h"
#include "determinant.h"

Approximator::Approximator()
{
	Determinant* pDeterminant = new Determinant();
	programs[0] = pDeterminant;
}

Approximator::~Approximator() 
{
	delete programs[0];
}

void Approximator::startProgram(int programIdx, ProgramOutput* pProgramOutput)
{
	programs[programIdx]->start(pProgramOutput);
}

void Approximator::advanceProgram(int programIdx, ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	programs[programIdx]->proceed(pProgramOutput, input);
}

void Approximator::resetProgram(int programIdx)
{
	programs[programIdx]->reset();
}
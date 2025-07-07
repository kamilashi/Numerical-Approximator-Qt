#include "approximator.h"
#include "matrixdeterminant.h"
#include "matrixinverse.h"

Approximator::Approximator()
{
	MatrixDeterminant* pDeterminant = new MatrixDeterminant();
	MatrixInverse* pInverse = new MatrixInverse();
	programs[0] = pDeterminant;
	programs[1] = pInverse;
}

Approximator::~Approximator() 
{
	delete[] programs;
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
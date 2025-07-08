#include "approximator.h"
#include "matrixdeterminant.h"
#include "matrixinverse.h"
#include "mullers.h"

Approximator::Approximator()
{
	MatrixDeterminant* pDeterminant = new MatrixDeterminant();
	MatrixInverse* pInverse = new MatrixInverse();
	MullersMethod* pMullers = new MullersMethod();
	programs[0] = pDeterminant;
	programs[1] = pInverse;
	programs[2] = pMullers;
}

Approximator::~Approximator() 
{
	for (int i = 0; i < 3; ++i)
	{
		delete programs[i];  
		programs[i] = nullptr;
	}
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
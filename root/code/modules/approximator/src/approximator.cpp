#include "approximator.h"
#include "matrixdeterminant.h"
#include "matrixinverse.h"
#include "mullers.h"
#include "bisection.h"
#include "falseposition.h"

Approximator::Approximator()
{
	MatrixDeterminant* pDeterminant = new MatrixDeterminant();
	MatrixInverse* pInverse = new MatrixInverse();
	MullersMethod* pMullers = new MullersMethod();
	BisectionMethod* pBisection = new BisectionMethod();
	FalsePositionMethod* pFalsePosition = new FalsePositionMethod();
	programs[0] = pDeterminant;
	programs[1] = pInverse;
	programs[2] = pMullers;
	programs[3] = pBisection;
	programs[4] = pFalsePosition;
}

Approximator::~Approximator() 
{
	for (int i = 0; i < 4; ++i)
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
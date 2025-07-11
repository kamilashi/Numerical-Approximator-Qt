#include "approximator.h"
#include "matrixdeterminant.h"
#include "matrixinverse.h"
#include "mullers.h"
#include "bisection.h"
#include "falseposition.h"
#include "gausslinear.h"
#include "maclaurincosine.h"

Approximator::Approximator()
{
	MaclaurinCosine* pCosine = new MaclaurinCosine();
	MatrixDeterminant* pDeterminant = new MatrixDeterminant();
	MatrixInverse* pInverse = new MatrixInverse();
	MullersMethod* pMullers = new MullersMethod();
	BisectionMethod* pBisection = new BisectionMethod();
	FalsePositionMethod* pFalsePosition = new FalsePositionMethod();
	GaussLinear* pGaussLinear = new GaussLinear();

	programs[0] = pCosine;
	programs[1] = pDeterminant;
	programs[2] = pInverse;
	programs[3] = pMullers;
	programs[4] = pBisection;
	programs[5] = pFalsePosition;
	programs[6] = pGaussLinear;
}

Approximator::~Approximator() 
{
	for (int i = 0; i < programCount; ++i)
	{
		delete programs[i];  
		programs[i] = nullptr;
	}
}

void Approximator::getProgramCode(int programIdx, ProgramOutput* pProgramOutput)
{
	programs[programIdx]->getCode(pProgramOutput);
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
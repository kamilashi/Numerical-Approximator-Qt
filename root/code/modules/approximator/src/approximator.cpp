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

	m_programs[0] = pCosine;
	m_programs[1] = pDeterminant;
	m_programs[2] = pInverse;
	m_programs[3] = pMullers;
	m_programs[4] = pBisection;
	m_programs[5] = pFalsePosition;
	m_programs[6] = pGaussLinear;
}

Approximator::~Approximator() 
{
	for (int i = 0; i < programCount; ++i)
	{
		delete m_programs[i];  
		m_programs[i] = nullptr;
	}
}

void Approximator::getProgramCode(int programIdx, ProgramOutput* pProgramOutput)
{
	m_programs[programIdx]->getCode(pProgramOutput);
}

void Approximator::startProgram(int programIdx, ProgramOutput* pProgramOutput)
{
	m_programs[programIdx]->start(pProgramOutput);
}

void Approximator::advanceProgram(int programIdx, ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	m_programs[programIdx]->proceed(pProgramOutput, input);
}

void Approximator::resetProgram(int programIdx)
{
	m_programs[programIdx]->reset();
}
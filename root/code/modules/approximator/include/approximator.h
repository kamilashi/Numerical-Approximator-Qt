#ifndef APPROXIMATOR_H
#define APPROXIMATOR_H

#include "program.h"

class Approximator{

public:
	Approximator();
	~Approximator();

	void getProgramCode(int programIdx, ProgramOutput* pProgramOutput);
	void startProgram(int programIdx, ProgramOutput* pProgramOutput);
	void advanceProgram(int programIdx, ProgramOutput* pProgramOutput, const ProgramInput& input);
	void resetProgram(int programIdx);

	static const int programCount = 7;
	ProgramInterface programInterfaces[programCount] =
	{
		ProgramInterface("Cosine"),
		ProgramInterface("Matrix Determinant"),
		ProgramInterface("Matrix Inverse"),
		ProgramInterface("Muller's Method", "Roots of Polynomials"),
		ProgramInterface("Bisection", "Roots of Polynomials"),
		ProgramInterface("False Position", "Roots of Polynomials"),
		ProgramInterface("Linear Equation System"),
	};

private:
	Program* programs[programCount];
};

#endif
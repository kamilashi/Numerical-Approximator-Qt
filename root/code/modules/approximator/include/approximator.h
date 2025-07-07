#ifndef APPROXIMATOR_H
#define APPROXIMATOR_H

#include "program.h"

class Approximator{

public:
	Approximator();
	~Approximator();

	void startProgram(int programIdx, ProgramOutput* pProgramOutput);
	void advanceProgram(int programIdx, ProgramOutput* pProgramOutput, const ProgramInput& input);
	void resetProgram(int programIdx);

	static const int programCount = 5;
	ProgramInterface programInterfaces[programCount] =
	{ ProgramInterface("Determinant"),
		ProgramInterface("Matrix Inverse"),
		ProgramInterface("Muller"),
		ProgramInterface("Bracketing Methods"),
		ProgramInterface("Open Methods") };

private:
	Program* programs[programCount];
};

#endif
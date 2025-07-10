#ifndef COSINECALCULATOR_H
#define COSINECALCULATOR_H

#include "program.h"

class MaclaurinCosine : public Program 
{
public:
	MaclaurinCosine();
	~MaclaurinCosine() override;

	void start(ProgramOutput* pProgramOutput) override;
	void proceed(ProgramOutput* pProgramOutput, const ProgramInput& input) override;
	void reset() override;

private:
	void scanArgumentAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void scanResolutionAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void runStage1(ProgramOutput* pProgramOutput);
	void runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input);

	float x;
	int maxIterationCount;
	float* A = nullptr;
	float* L = nullptr;
};

#endif
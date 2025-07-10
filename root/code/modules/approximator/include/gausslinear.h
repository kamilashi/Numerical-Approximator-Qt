#ifndef GAUSSLINEAR_H
#define GAUSSLINEAR_H

#include "program.h"

class GaussLinear : public Program
{
public:
	GaussLinear();
	~GaussLinear() override;

	void start(ProgramOutput* pProgramOutput) override;
	void proceed(ProgramOutput* pProgramOutput, const ProgramInput& input) override;
	void reset() override;

private:
	void scanTermsAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void scanRootGuessesAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void runStage1(ProgramOutput* pProgramOutput);
	void runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input);

	int scannedElementsCount = 0;
	int degree = 0;
	int termCount = 0;
	float* A = nullptr;
};

#endif
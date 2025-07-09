#ifndef BISECTION_H
#define BISECTION_H

#include "program.h"

class BisectionMethod : public Program
{
public:
	BisectionMethod();
	~BisectionMethod() override;

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
	void runStage4(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void runStage5(ProgramOutput* pProgramOutput, const ProgramInput& input);

	int scannedElementsCount = 0;
	int degree = 0;
	int termCount = 0;
	float* F = nullptr;

	static const int maxIterationCount = 80;
	static const int guessCount = 2;
	float G[guessCount];
};

#endif
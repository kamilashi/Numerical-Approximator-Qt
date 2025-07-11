#ifndef FALSEPOSITION_H
#define FALSEPOSITION_H

#include "program.h"

class FalsePositionMethod : public Program
{
public:
	FalsePositionMethod();
	~FalsePositionMethod() override;

	void reset() override;
	void start(ProgramOutput* pProgramOutput) override;
	void proceed(ProgramOutput* pProgramOutput, const ProgramInput& input) override;
	void getCode(ProgramOutput* pProgramOutput) override;

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

	static const int maxIterationCount = 100;
	static const int guessCount = 2;
	float G[guessCount];
};

#endif
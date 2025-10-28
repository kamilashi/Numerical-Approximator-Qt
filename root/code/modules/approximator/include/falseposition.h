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

	int m_scannedElementsCount = 0;
	int m_degree = 0;
	int m_termCount = 0;
	float* m_matrixF = nullptr;

	static const int m_sMaxIterationCount = 100;
	static const int m_sGuessCount = 2;
	float m_matrixG[m_sGuessCount];
};

#endif
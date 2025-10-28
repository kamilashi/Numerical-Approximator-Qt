#ifndef GAUSSLINEAR_H
#define GAUSSLINEAR_H

#include "program.h"

class GaussLinear : public Program
{
public:
	GaussLinear();
	~GaussLinear() override;

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

	int m_scannedElementsCount = 0;
	int m_degree = 0;
	int m_termCount = 0;
	float* m_matrixA = nullptr;
};

#endif
#ifndef DETERMINANT_H
#define DETERMINANT_H

#include "program.h"

class Determinant : public Program 
{
public:
	Determinant();
	~Determinant() override;

	void start(ProgramOutput* pProgramOutput) override;
	void proceed(ProgramOutput* pProgramOutput, const ProgramInput& input) override;
	void reset() override;

private:
	void scanAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void calculateAndPrint(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void runStage1(ProgramOutput* pProgramOutput);
	void runStage2(ProgramOutput* pProgramOutput, const ProgramInput& input);
	void runStage3(ProgramOutput* pProgramOutput, const ProgramInput& input);

	int scannedElementsCount;
	int n, i, j, m;
	float* A = nullptr;
	float* L = nullptr;
};

#endif
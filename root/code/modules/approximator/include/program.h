#ifndef PROGRAM_H
#define PROGRAM_H

#include <cstring>

enum class InputType
{
	Int,
	Float,
	Any,

	TypesCount
};

struct ProgramInterface
{
	int index;
	int subProgramsCount;
	const char* pName;

	ProgramInterface(const char* pNameString, int subPrCount = 0)
	{
		static int count = 0;

		index = count;
		subProgramsCount = subPrCount;
		pName = pNameString;

		count++;
	}

	~ProgramInterface() {}
};

struct ProgramOutput
{
	InputType requestedInputType;
	bool outputIsError;
	const char* pOutput;

	ProgramOutput()
	{
		requestedInputType = InputType::TypesCount;
		outputIsError = false;
		pOutput = nullptr;
	}
};

struct ProgramInput
{
	int inputInt;
	float inputFloat;

	ProgramInput()
	{
		inputInt = 0;
		inputFloat = 0.0f;
	}
};

class Program
{
public:
	Program() 
	{
		currentStage = 0;
		stageCount = -1;
		memset(outputBuffer, 0, sizeof(outputBuffer));
	};
	virtual ~Program() {}

	void virtual start(ProgramOutput* pProgramOutput) = 0;
	void virtual proceed(ProgramOutput* pProgramOutput, const ProgramInput& input) = 0;
	void virtual reset() = 0;
protected:
	int currentStage;
	int stageCount;
	char outputBuffer[2048];
};

#endif
#ifndef PROGRAM_H
#define PROGRAM_H

enum class InputType
{
	Int,
	Float,

	TypesCount
};

struct ProgramInterface
{
	int index;
	const char* pName;

	ProgramInterface(const char* pNameString)
	{
		static int count = 0;

		index = count;
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
};

struct ProgramInput
{
	int inputInt;
	float inputFloat;
};

class Program
{
public:
	Program() {};
	virtual ~Program() {}

	void virtual start(ProgramOutput* pProgramOutput) = 0;
	void virtual proceed(ProgramOutput* pProgramOutput, const ProgramInput& input) = 0;
	void virtual reset() = 0;
protected:
	int currentStage;
	int stageCount;
	char outputBuffer[1024];
};

#endif
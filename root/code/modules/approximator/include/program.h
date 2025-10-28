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
	const char* pName;
	const char* pCategoryName;

	ProgramInterface(const char* pNameString, const char* pCtgryName = nullptr) : 
		index(-1), 
		pName(pNameString),
		pCategoryName(pCtgryName)
	{
		static int count = 0;
		index = count;
		count++;
	}

	~ProgramInterface() {}
};

struct ProgramOutput
{
	InputType requestedInputType;
	bool outputIsError;
	const char* pOutput;

	ProgramOutput() : 
		requestedInputType(InputType::TypesCount), 
		outputIsError(false), 
		pOutput(nullptr) {}
};

struct ProgramInput
{
	int inputInt;
	float inputFloat;

	ProgramInput() : inputInt(0), inputFloat(0.0f) {}
};

class Program
{
public:
	Program() : m_currentStage(0), m_stageCount(-1)
	{
		memset(m_outputBuffer, 0, sizeof(m_outputBuffer));
	};
	virtual ~Program() {}

	void virtual start(ProgramOutput* pProgramOutput) = 0;
	void virtual proceed(ProgramOutput* pProgramOutput, const ProgramInput& input) = 0;
	void virtual reset() = 0;
	void virtual getCode(ProgramOutput* pProgramOutput) = 0;
protected:
	int m_currentStage;
	int m_stageCount;
	char m_outputBuffer[4096];
};

#endif
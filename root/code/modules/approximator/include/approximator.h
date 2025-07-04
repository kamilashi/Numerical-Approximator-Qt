#ifndef APPROXIMATOR_H
#define APPROXIMATOR_H

struct ProgramItem
{
	int index;
	const char* pName;

	ProgramItem(const char* pNameString)
	{
		static int count = 0;

		index = count;
		pName = pNameString;

		count++;
	}

	~ProgramItem() {}
};

enum class InputType
{
	Int,
	Float,

	TypesCount
};

class Approximator{

public:
	Approximator();
	~Approximator();

	static const int programCount = 5;
	ProgramItem programItems[programCount] = 
	{	ProgramItem("Determinant"),
		ProgramItem("Matrix Inverse"),
		ProgramItem("Muller"),
		ProgramItem("Bracketing Methods"),
		ProgramItem("Open Methods") };

};

#endif
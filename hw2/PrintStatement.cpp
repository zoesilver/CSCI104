// PrintStatement.cpp:
#include "PrintStatement.h"
using namespace std;


PrintStatement::PrintStatement(std::string variableName)
	: m_variableName( variableName )
{}
PrintStatement::~PrintStatement(){

}

void PrintStatement::execute(ProgramState * state, std::ostream &outf)
{
	//print out value from variable using getVal method. 
	outf<<state->getVal(m_variableName)<<endl;
    //increment program counter
    int nextLine = state->getNextLine();
	state->nextLinePlusOne(nextLine);
	}

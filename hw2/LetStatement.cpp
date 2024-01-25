// LetStatement.cpp:
#include "LetStatement.h"
#include <iostream>
using namespace std;

LetStatement::LetStatement(std::string variableName, int value)
	: m_variableName( variableName ), m_value( value )
{

}
LetStatement::~LetStatement(){}


// The LetStatement version of execute() should make two changes to the
// state of the program:
//
//    * set the value of the appropriate variable
//    * increment the program counter
void LetStatement::execute(ProgramState * state, ostream &outf)
{
	state->setVal(m_variableName, m_value);
    //increment program counter
    int nextLine = state->getNextLine();
	state->nextLinePlusOne(nextLine);}

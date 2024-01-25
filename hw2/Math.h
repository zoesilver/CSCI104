#ifndef MATH_INCLUDED
#define MATH_INCLUDED

#include "Statement.h"
#include <iostream>

class Math: public Statement
{
private:
std::string m_type;
std::string m_variableName;
std::string m_p;
	
public:
    Math(std::string type, std::string variableName, std::string p);
	~Math();	
	void execute(ProgramState * state, std::ostream &outf);
};

#endif

#ifndef ifThen_INCLUDED
#define ifThen_INCLUDED

#include "Statement.h"
#include <iostream>

class ifThen: public Statement
{
private:
std::string m_var;
std::string m_oper;
int m_compInt;
int m_lineNum;



	
public:
    ifThen(std::string var, std::string oper, int compInt, int lineNum);
	~ifThen();	
	void execute(ProgramState * state, std::ostream &outf);
};

#endif

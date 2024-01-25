#ifndef GoTo_INCLUDED
#define GoTo_INCLUDED

#include "Statement.h"
#include <iostream>

class GoTo: public Statement
{
private:
std::string m_type;
int m_lineNum;


public:
    GoTo(std::string type, int lineNum);
	~GoTo();	
	void execute(ProgramState * state, std::ostream &outf);
};

#endif

#ifndef END_STATEMENT_INCLUDED
#define END_STATEMENT_INCLUDED

#include "Statement.h"
#include <cstdlib>
#include <iostream>

class EndStatement: public Statement
{
private:
//nothing needed here
	
public:
    EndStatement();
	~EndStatement();	
	void execute(ProgramState * state, std::ostream &outf);
};

#endif

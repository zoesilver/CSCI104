#ifndef PRINTALL_INCLUDED
#define PRINTALL_INCLUDED

#include "Statement.h"
#include <iostream>

class PrintAll: public Statement
{
private:
//nothing needed here
	
public:
    PrintAll();
	~PrintAll();	
	void execute(ProgramState * state, std::ostream &outf);
};

#endif

#ifndef Return_INCLUDED
#define Return_INCLUDED

#include "Statement.h"
#include <iostream>

class Return: public Statement
{
private:
    
public:
    Return();
	~Return();	
	void execute(ProgramState * state, std::ostream &outf);
};

#endif

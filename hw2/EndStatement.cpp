#include "EndStatement.h"
#include <climits>

EndStatement::EndStatement()
{}
EndStatement::~EndStatement(){
    
}

void EndStatement::execute(ProgramState * state, std::ostream &outf)
{
    //end program
	exit(0);
}

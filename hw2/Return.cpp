#include "Return.h"
using namespace std;

Return::Return()
{}
Return::~Return(){
    
}

void Return::execute(ProgramState * state, std::ostream &outf)
{
    //fetch line to return to which was set when GOSUB was exectuted
    int returningTo = state->getReturnTo();
    //if getReturnTo=1000, that means GOSUB was never called, and return cannot execute
    if(returningTo == 1000){
        exit(0);
    }
    //set nextLine to one line past line returning to
	state->setNextLine(returningTo+1);
}
#include "PrintAll.h"
using namespace std;

PrintAll::PrintAll()
{}
PrintAll::~PrintAll(){
    
}

void PrintAll::execute(ProgramState * state, std::ostream &outf)
{
    //for all items in state
    std::map<std::string, int>::iterator it;
    for(it = state->variables.begin(); it != state->variables.end(); ++it){
    std::string variableName = it->first;
    //print variable
    outf<<variableName<<" ";
    //print out each value from variable using getVal method. 
	outf<<state->getVal(variableName)<<endl;
    
}
    //increment program counter
    int nextLine = state->getNextLine();
	state->nextLinePlusOne(nextLine);

    }


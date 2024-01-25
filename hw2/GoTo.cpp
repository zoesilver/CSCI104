#include "GoTo.h"
#include <climits>
#include <iostream>
using namespace std;

GoTo::GoTo(std::string type, int lineNum):m_type (type), m_lineNum(lineNum){}
GoTo::~GoTo(){
  
}
    


void GoTo::execute(ProgramState * state, std::ostream &outf)
{
    if (m_type=="GOTO"){
        state->setNextLine(m_lineNum);

    }
    //type is GOSUB
    else{
        //save value of current line 
        int current = state->getNextLine();
        state->setReturnTo(current);
        //go to desired line
        state->setNextLine(m_lineNum);
    }
	
}


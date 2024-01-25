#include "Math.h"
using namespace std;
using std::string;

Math::Math(std::string type, std::string variableName, std::string p): m_type(type), m_variableName (variableName), m_p(p)
{}
Math::~Math(){
    
}

void Math::execute(ProgramState * state, std::ostream &outf)
{
    int pVal=0;
    int initVal;
    initVal = state->getVal(m_variableName);

    //check if m_p is an int or a variable
    for (size_t i =0; i<m_p.length();i++){
         //if any chars in m_p are not a digit or a negative sign
        if((std::isdigit(m_p[i])==false)&&(m_p[i]!='-')){
            //p must be a var, so get it's int value and save it
            pVal = state->getVal(m_p);
            break;
    }
        else{
            //m_p must be an int, so save it to pVal
            pVal = stoi(m_p);
        }
    }
    //now find what operation to do
    if (m_type=="ADD"){
        //add and set new val
        state->setVal(m_variableName, initVal+pVal);
    }
    else if (m_type=="SUB"){
        //subtract and set
        state->setVal(m_variableName, initVal-pVal);

    }
    else if (m_type=="MULT"){
        //multiply and set
        state->setVal(m_variableName, initVal*pVal);

    }
    else if (m_type=="DIV"){
        //check if dividing by 0
        if (pVal==0){
            cout<<"Divide by 0 exception"<<endl;
            exit(0);
        }
        //divide and set
        state->setVal(m_variableName, initVal/pVal);

    }
    //increment program counter
    int nextLine = state->getNextLine();
	state->nextLinePlusOne(nextLine);
    
}

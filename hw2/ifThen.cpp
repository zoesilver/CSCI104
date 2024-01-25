#include "ifThen.h"
#include <iostream>
using namespace std;

ifThen::ifThen(std::string var, std::string oper, int compInt, int lineNum):
m_var(var), m_oper(oper), m_compInt(compInt), m_lineNum(lineNum)
{}
ifThen::~ifThen(){
    
}

void ifThen::execute(ProgramState * state, std::ostream &outf)
{
    //get value asscociated with variable
    int value = state->getVal(m_var);
    bool truth = false;
    //see if equality is being evaluated
    if ((m_oper == "<=")||(m_oper == ">=")||(m_oper == "=")){
        
        //check if values are equal
        if (value == m_compInt){
            truth = true;
        }
    }
    //check if less than is being evaluated
    if ((m_oper == "<")||(m_oper == "<=")||(m_oper == "<>")){
        if (value<m_compInt){
            truth = true;
        }
    }
    //check if greater than is being evaluated
    if ((m_oper == ">")||(m_oper == ">=")||(m_oper == "<>")){
        if (value>m_compInt){
            truth = true;
        }
    }
    //if the statement evaluates to true
    if (truth == true){

        //set nextLine to the desired line to jump to
	    state->setNextLine(m_lineNum);
    }
    else{
        //proceed to immediate next line 
        int nextLine = state->getNextLine();
        state->nextLinePlusOne(nextLine);
    }
    

}
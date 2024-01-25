#include "ProgramState.h"
#include <string>
#include <iostream>

using namespace std;
ProgramState::ProgramState(int numLines) {
    nextLine=1;
    returnTo = 1000;
     //this will hold all lines to be returned to 
    std::stack<int> returns;

}
int ProgramState::getVal( std::string key){
    //if key has no defined value, set it's value to 0
    if (variables.find(key) == variables.end()) {
        variables[key]=0;
    }
    return variables[key];
}
void ProgramState::setVal(std::string  key, int newVal){
    //updates value with associaited variable, or creates new key-value pair if none exists
    variables[key]=newVal;
    return;
}
int ProgramState::getNextLine(){
    //retrieves nextLine
    int next = nextLine;
    return next;
}

void ProgramState::setNextLine(int line){
    //sets to any specified value
    nextLine = line;

}
int ProgramState::nextLinePlusOne(int line){
    //auto increments by one
    nextLine = line+1;
    return nextLine;
}

int ProgramState::getReturnTo(){
    if (!returns.empty()) {
        //gets most recently added value from returns stack
        int returnTo = returns.top();
        returns.pop();
        return returnTo;}
    //if there has been no gosub, then exit when return is called
    else{
        exit(0);
    }
}

void ProgramState::setReturnTo(int currentLine){
    //adds line to front of returns stack
    returns.push(currentLine);
    return;
}


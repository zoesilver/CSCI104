// ProgramState.h
//
// CS 104
//
// The ProgramState class encapsulates the state of an executing Facile
// program.  The state of a BASIC program consists of three parts:
//
//    * The program counter, which specifies the line number of the
//      statement that will execute next.
//    * A map, as explained by the problem writeup.
//    * A stack of integers, which is used to hold return lines for GOSUB
//      statements.
//
// We've provided you a start on this class, but you'll need to add methods
// to it as you go along.  In particular, you'll need methods to access and
// modify the state; these methods will be called primarily by the various
// execute() methods in the Statement subclasses, to allow the execution of
// a statement to change the state of a program.  For example, executing a
// GOTO statement will cause the program counter to be changed.

#ifndef PROGRAM_STATE_INCLUDED
#define PROGRAM_STATE_INCLUDED
#include <map>
#include <stack>

class ProgramState
{
public:
	ProgramState(int numLines);

	// You'll need to add a variety of methods here.  Rather than trying to
	// think of what you'll need to add ahead of time, add them as you find
	// that you need them.

	std::map<std::string, int> variables;
	//get value w ascociated key
	int getVal(std::string  key);
	//set val
	void setVal(std::string  key, int newVal);
	
	//getNextLine for other classes
	int getNextLine();

	//other classes can change nextLine (ie: for GOTO function)
	void setNextLine(int line);

	//increments nextLine by 1 
	int nextLinePlusOne(int line);

	//get private returnTo value
	int getReturnTo();

	//see if line is in range and has a declared line
	bool validLine(int line);

	//set returnTo value when GOSUB is called;
	void setReturnTo(int currentLine);




private:
	int m_numLines;
	int nextLine;
	int returnTo;
	std::stack<int> returns;




};

#endif

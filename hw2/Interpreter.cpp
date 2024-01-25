// Interpreter.cpp
#include "Statement.h"
#include "LetStatement.h"
// headers for all subclasses
#include "PrintStatement.h"
#include "EndStatement.h"
#include "PrintAll.h"
#include "GoTo.h"
#include "Math.h"
#include "Return.h"
#include "ifThen.h"
//necessary libraries
#include <vector>
#include <string>
#include <sstream> 
#include <fstream>
#include <cstdlib>
#include <iostream>

using namespace std;

// parseProgram() takes a filename as a parameter, opens and reads the
// contents of the file, and returns an vector of pointers to Statement.
void parseProgram(istream& inf, vector<Statement *> & program);

// parseLine() takes a line from the input file and returns a Statement
// pointer of the appropriate type.  This will be a handy method to call
// within your parseProgram() method.
Statement * parseLine(string line);

// interpretProgram() reads a program from the given input stream
// and interprets it, writing any output to the given output stream.
// Note:  you are required to implement this function!
void interpretProgram(istream& inf, ostream& outf);


int main()
{
        cout << "Enter BASIC program file name: ";
        string filename;
        getline(cin, filename);
        ifstream infile(filename.c_str());
        if (!infile)
        {
                cout << "Cannot open " << filename << "!" << endl;
                return 1;
        }
		interpretProgram(infile, cout);
}



void parseProgram(istream &inf, vector<Statement *> & program)
{
	program.push_back(NULL);
	
	string line;
	while( ! inf.eof() )
	{
		getline(inf, line);
		program.push_back( parseLine( line ) );
	}
}


Statement * parseLine(string line)
{
	Statement * statement;
	stringstream ss;
	//the below statements are all used for parsing the lines to be sent to the execute statement
	int line_num;
	string type;
	string var;
	int val;
	string p;
	string oper;
	int compInt;
	string then;





	ss << line;
	ss >> line_num;
	ss >> type;

	if ( type == "LET" )
	{

		ss >> var;
		ss >> val;
		// Note:  Because the project spec states that we can assume the file
		//	  contains a syntactically legal program, we know that
		//	  any line that begins with "LET" will be followed by a space
		//	  and then a variable and then an  integer value.
		statement = new LetStatement(var, val);
	}

	//when PRINT command is speicifed, call print class
	else if (type == "PRINT"){
		ss>>var;
		statement = new PrintStatement(var);
	}

	//when END or . present, call end class
	else if((type == "END")||(type==".")){
		statement = new EndStatement();
	}
	
	//print everything that's been defined
	else if(type=="PRINTALL"){
		statement = new PrintAll();
	}
	//if any math statements, execute math class
	else if((type=="ADD")||(type=="SUB")||(type=="MULT")||(type=="DIV"))
	{
		ss>>var;
		//p will be read in as a string no matter what
		ss>>p;
		statement = new Math(type,var,p);
	}
	//execute goto or gosub. The two will be distinguished inside the class. 
	else if((type=="GOTO")||(type=="GOSUB")){
		ss>>line_num;
		statement = new GoTo(type, line_num);

	}
	//execute return and return to last line after GOSUB was called
	else if(type=="RETURN"){
		statement = new Return();
	}
	//execute ifThen class
	else if (type=="IF"){
		ss>>var;
		ss>>oper;
		ss>>compInt;
		ss>>then;
		ss>>line_num;
		statement = new ifThen(var, oper, compInt, line_num);

	}

	return statement;
}



void interpretProgram(istream& inf, ostream& outf)
{
	vector<Statement *> program;
	ProgramState programState(100); 
	parseProgram( inf, program );
	//always go into while loop
	while(1){
		//if the desired place to jump to has command, error message will be printed
		if (static_cast<int>(program.size())<programState.getNextLine()){
			cout<<"Illegal jump instruction"<<endl;
			exit(0);
		}
		Statement *statement=program[programState.getNextLine()];
		//first item in program is NULL, which needs to be skipped over
		if (statement != nullptr) {
			statement->execute(&programState, outf);
	}

	}

}

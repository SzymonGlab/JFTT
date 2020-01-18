#pragma once
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <climits>
#include <memory>
#include <math.h>
#include <vector>
#include <stack>
#include <map>

using namespace std;

typedef struct
{
	string name;
	// 1 - number
	// 2 - variable
	// 3 - array variable
	// 4 - array start variable
	// 5 - iterator
	// 6 - array_index (Reading arrays elements e.g. a(x))
	int type;
	long long int memory_index;
	long long int size;
	long long int start;
	long long int end;
	bool inicialized;

} Variable;

typedef struct
{
	string name;
	long long int memory_index;
} Iterator;

typedef struct
{
	string name;
	long long int line;
	bool used;
} Error_variable;

// Variables that are used in program, but they are assigned after use.
map<string,Error_variable> notInicializedVariables;

map<string, Variable> variables;
vector<string> asmCode;
vector<long long int> array_index_variables;
stack<long long int> cmd_index_for_jumps;
stack<long long int> cmd_index_for_for_loops;
stack<Iterator> loop_iterators;
long long int memoryCounter = 1;
long long int cmd_index = 0;
int oneIndex;
int minusOneIndex;

///////////////////////////
//		 MEMORY			 //
///////////////////////////

// Returns next free memory address
long long getMemory();

// Frees memory
void freeMemory();

// Retruns variable at given name
Variable getVariable(string name);

///////////////////////////
//		 CONDITIONS		 //
///////////////////////////

// ==
char *__EQ(string var1, string var2);

// !=
char *__NEQ(string var1, string var2);

// <
char *__LE(string var1, string var2);

// >
char *__GE(string var1, string var2);

// <=
char *__LEQ(string var1, string var2);

// >=
char *__GEQ(string var1, string var2);

//If / else
void __end_if();
void __if_else();
void __end_if_else();

///////////////////////////
//		ARITHMETICS		 //
///////////////////////////

// +
char *__add(string var1_address, string var2_address, int yylineno);

// -
char *__sub(string var1_address, string var2_address, int yylineno);

// *
char *__mul(string var1_address, string var2_address, int yylineno);

// /
char *__div(string var1_address, string var2_address, int yylineno);

// %
char *__mod(string var1_address, string var2_address, int yylineno);

///////////////////////////
//		 VARIABLES		 //
///////////////////////////

// Creates new variable in variables map
void __declare_variable(string name, int yylineno);

// Creates new array in variables map. It's normal variable with different type.
void __declare_array(string name, string start, string end, int yylieno);

// Saves variable at name
void __assign_variable(string name, string variable_to_assign, int yylienno);

// Loads number with given value in assembler
char *__create_number(string value);

// Loads variable with given name in assembler
char *__read_variable(string var_name, int yylineno);

// Loads array variable e.g a(1)
char *__read_variable_from_arrayNUM(string array_name, string index, int yylineno);

// Loads array variable e.g a(x)
char *__read_variable_from_arrayVAR(string array_name, string index, int yylineno);

// Deletes from not inicialized variables list
void set_variable_inicialized(Variable var);

//Updates given var as used
void set_variable_used(Variable var, int yylineno);

///////////////////////////
//		 IN / OUT		 //
///////////////////////////

// Reads user input and saves at given var_memory_index
void __read(string var_address);

// Writes var
void __write(string var_address, int yylineno);

///////////////////////////
//		 LOOPS			 //
///////////////////////////

// Creates for loop
void __loop_for(string iterator, string from, string to, bool isTo, int yylineno);

// Ends for loop
void __end_loop_for(bool isTo);

// Creates while loop
void __loop_while(string condition_length);

// Ends while loop
void __end_loop_while();

// Creates do while loop
void __loop_do_while();

// Ends do while loop
void __end_loop_do_while();

///////////////////////////
//		 UTILITIES		 //
///////////////////////////

// Checks if all used variables were inicilaized and adds HALT
void __end(); 

// Adds assembler command to vector
void add_command(string command);

// Generates assembler to create number at mem_index
void create_number(long long int number, long long int mem_index);

// Returns value of a(x) at given adress (in assembler)
void get_value_of_array_with_variable(long long int address);

// Prints all variables in console
void iterateVariables();

// Prints error in console
void error(string a, int yylineno, string msg);

// Parse str to char*
char *parseStringToChar(string str);

// Returns value of variable at given memory_index
long long int get_value_of_variable_from_memory_index(string var_memory_index);

// Returns variable at given memory_index
Variable get_variable_from_mem_index(string memory_index);

// Checks if variable is in map
bool check_variable_exist(string memory_index);

// Saves cmd index for jump
void create_jump();

// Creates variable in map
bool create_variable(string name, int type, long long int memoryIndex, long long int size, long long int start, long long int end, bool inicialized);

// Returns variable type (1,2,3,4,5,6)
int check_variable_type(string mem_index);

// Creates 1 and -1 at address
void createOnes();

// Updates given variable in Variables map
void update_variables(Variable var);

// Checks if both variables were inicialized before
void check_both_variables_inicialized(string var1_memory, string var2_memory, int yylieno);

// Concats 2 char*
char *concat_chars(char *first, char *second);

// COUT str
void debug(string str);

// Prints assembler vector in console
void print_ASM();

// Writes assembler vector to file
void create_ASM(char *fileName);

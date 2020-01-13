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



typedef struct {
	string name;
	// 1 - number
	// 2 - variable
	// 3 - array variable
	// 4 - array start variable
	// 5 - iterator
	// 6 - array_index (Reading arrays elements e.g. a(xarith))
	int type;
	long long int memory_index;
	long long int size;
	long long int start;
	long long int end;
	bool inicialized;
	
} Variable;

typedef struct{
	string name;
	long long int memory_index;
} Iterator;


map<string, Variable> variables;
vector<string> asmCode;
// TABLICA TA SŁUŻY DO SPRAWDZANIA PODCZAS WRITE I ASSIGN CZY MAMY UŻYĆ LOAD CZY LOADI, STORE CZY STOREI
// Załóżmy x = 5 i A(5) znajdowało się pod adresem 19. Po wczytaniu A(x) otrzymamy adres miejsca w którym znajduje się 19, dzięki czemu robiąc STOREI zapiszemy coś pod adresem 19 lub odczytamy robiąc LOADi.
vector<long long int> array_index_variables;
stack<long long int> cmd_index_for_jumps;
stack<long long int> cmd_index_for_for_loops;
stack<Iterator> loop_iterators;
long long int memoryCounter = 1;
long long int cmd_index = 0;
int oneIndex;
int minusOneIndex;
// Variable to check if both variables in arithmetics were iniclialized
bool both_variables_inicialized;


// Memory
long long getMemory();
void freeMemory();
Variable getVariable(string name);

//Utilities
void add_command(string command);
void get_value_of_array_with_variable(long long int address);
void iterateVariables();
void error(string a, int yylineno, string msg);
char *parseStringToChar(string str);
long long int get_value_of_variable_from_memory_index(string var_memory_index);
Variable get_variable_from_mem_index(string memory_index);
bool check_variable_exist(string memory_index);
void add_command(string command);
void create_jump();
bool create_variable(string name, int type, long long int memoryIndex, long long int size, long long int start, long long int end, bool inicialized);
int check_variable_type(string mem_index);
void createOnes();
void update_variables(Variable var);
void check_both_variables_inicialized(string var1_memory, string var2_memory, int yylieno);
void print_ASM();
void create_ASM(char* fileName);
void create_number(long long int number, long long int mem_index);
char* concat_chars(char* first, char* second);
void debug(string str);

// Conditions
// 0 - true 1 - false
char* __EQ(string var1, string var2);
char* __NEQ(string var1, string var2);
char* __LE(string var1, string var2);
char* __GE(string var1, string var2);
char* __LEQ(string var1, string var2);
char* __GEQ(string var1, string var2);

//If / else
void __end_if();
void __if_else();
void __end_if_else();


// Arithmetics
char *__add(string var1_address, string var2_address, int yylineno);
char *__sub(string var1_address, string var2_address, int yylineno);
char *__mul(string var1_address, string var2_address, int yylineno);
char *__div(string var1_address, string var2_address, int yylineno);
char *__mod(string var1_address, string var2_address, int yylineno);

// Variables
void __declare_variable(string name, int yylineno);
void __declare_array(string name, string start, string end, int yylieno);
void __assign_variable(string name, string variable_to_assign, int yylienno);
char *__create_temp_variable(string value);
char *__read_variable(string var_name, int yylineno);
char *__read_variable_from_arrayNUM(string array_name, string index, int yylineno);
char *__read_variable_from_arrayVAR(string array_name, string index, int yylineno);
void __end();

// IN / OUT
void __read(string var_memory_index);
void __write(string var, int yylineno);

// Loops
void __loop_for(string variable, string from, string to, bool isTo, int yylineno);
void __end_loop_for(bool isTo);
void __loop_while(string condition_length);
void __end_loop_while();
void __loop_do_while();
void __end_loop_do_while();
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
	int type;
	long long int memory_index;
	long long int size;
	long long int start;
	long long int end;
	
} Variable;

// Memory
long long getMemory();
void freeMemory();
Variable getVariable(string name);

//Utilities
void add_command(string command);
void print_ASM();
void create_ASM(char* fileName);
void create_number(int number, int mem_index);
char* concat_chars(char* first, char* second);

// Conditions
// 0 - true 1 - false
void __EQ(string var1, string var2);
void __NEQ(string var1, string var2);
void __LE(string var1, string var2);
void __GE(string var1, string var2);
void __LEQ(string var1, string var2);
void __GEQ(string var1, string var2);


// Arithmetics
char* __add(string var1, string var2);
char* __sub(string var1, string var2);
char* __mul(string var1, string var2);
char* __div(string var1, string var2);
char* __mod(string var1, string var2);

// Variables
void __declare_variable(string name, string var_type);
void __declare_array(string name, string start, string end);
void __assign_variable(string name);
char* __create_temp_variable(string name, string value);
char* __read_variable_from_arrayVAR(string array_name, string index);
char* __read_variable_from_arrayNUM(string array_name, string index);
void __end();

// IN / OUT
void __read(string variable, string value);
void __write(string variable);
#include <string.h>
#include <iostream>
#include <unistd.h>
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
	int type;
	int memory_index;
	int size;
	int start;
	int end;
	
} Variable;

// Memory
int getMemory();
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
void __add(string var1, string var2);
void __sub(string var1, string var2);
void __mul(string var1, string var2);
void __div(string var1, string var2);

// Variables
void __declare_variable(string name, string var_type);
void __declare_array(string name, string start, string end);
void __assign_variable(string name);
void __create_temp_variable(string name, string value);
void __read_variable_from_arrayVAR(string array_name, string index);
void __read_variable_from_arrayNUM(string array_name, string index);
void __end();

// IN / OUT

void __read(string variable, string value);
void __write(string variable);
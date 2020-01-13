#include "compilator.hpp"

map<string, Variable> variables;
vector<string> asmCode;
long long int memoryCounter = 1;
long long int cmd_index = 0;
int oneIndex;
int minusOneIndex;

///////////////////////////
//         MEMORY        //
///////////////////////////

long long int getMemory()
{
    return memoryCounter++;
}

void freeMemory()
{
    memoryCounter--;
}

Variable getVariable(string name)
{
    return variables[name];
}

///////////////////////////
//        UTILITIES      //
///////////////////////////

char *concat_chars(char *first, char *second)
{
    long long int newSize = strlen(first) + strlen(second) + 1;

    char *newBuffer = (char *)malloc(newSize);

    strcpy(newBuffer, first);
    strcat(newBuffer, second);
    free(first);
    return newBuffer;
}

char *parseStringToChar(string str)
{
    const char *iStr = str.c_str();
    long long int newSize = strlen(iStr) + 1;
    char *newName = (char *)malloc(newSize);
    strcpy(newName, iStr);
    return newName;
}

void createOnes()
{
    oneIndex = getMemory();
    minusOneIndex = getMemory();
    add_command("SUB 0");
    add_command("INC");
    add_command("STORE " + to_string(oneIndex));
    add_command("DEC");
    add_command("DEC");
    add_command("STORE " + to_string(minusOneIndex));
}

void add_command(string command)
{
    asmCode.push_back(command);
    cmd_index++;
}

void print_ASM()
{
    for (auto i : asmCode)
    {
        cout << i << endl;
    }
}

void create_ASM(const char *fileName)
{
    ofstream code;
    code.open(fileName);
    for (auto i : asmCode)
    {
        code << i << endl;
    }
    code.close();
}

void create_variable(string name, int type, int memoryIndex, int size, int start, int end)
{
    Variable var = {name, type, memoryIndex, size, start, end};
    variables.insert(make_pair(name, var));
}

void create_number(long long int number, long long int mem_index)
{
    bool negative = false;
    long long int temp_var = memoryCounter + 1;
    long long int counter = 0;
    long long int current_number = 2;

    if (number == 0)
    {
        add_command("SUB 0");
        add_command("STORE " + to_string(mem_index));
        return;
    }
    else
    {
        if (number < 0)
        {
            negative = true;
            number = 0 - number;
        }
        add_command("SUB 0");
        add_command("INC");
        add_command("STORE " + to_string(temp_var));

        while (current_number <= number)
        {
            current_number += current_number;
            counter++;
            add_command("SHIFT " + to_string(temp_var));
        }

        current_number = current_number / 2;

        while (current_number < number)
        {
            current_number++;
            add_command("INC");
        }

        add_command("STORE " + to_string(mem_index));

        if (negative)
        {
            add_command("SUB 0");
            add_command("SUB " + to_string(mem_index));
            add_command("STORE " + to_string(mem_index));
        }
        // add_command("# KONIEC TWORZENIA LICZBY " + to_string(number));
    }
}

///////////////////////////
//       VARIABLES       //
///////////////////////////

void __declare_variable(string name, string variable_type)
{
    //DEBUG
    cout << "DECLARE VARIABLE " + name + " at " + to_string(memoryCounter) << endl;

    create_variable(name, stoi(variable_type), getMemory(), 1, 1, 1);
}

void __declare_array(string name, string start, string end)
{
    // DEBUG
    cout << "DECLARE ARRAY " + name + " at " + to_string(memoryCounter) << endl;

    long long int size = stoll(end) - stoll(start);

    if (size <= 0)
    {
        // UJEMNA TABLICA ERROR
    }

    create_variable(name, 1, getMemory(), size, stoll(start), stoll(end));

    for (long long int i = stoll(start); i <= stoll(end); i++)
    {
        create_variable(name + to_string(i), 3, getMemory(), 1, 1, 1);
    }
}

void __assign_variable(string name)
{
    // IF IT CAN PARSE THE NAME OF VARIABLE IT MEANS IT RECIVED THE MEMORY INDEX FROM READING E.G. A(X) AND IT SHOULD STORE CURRENT P0 AT GIVEN INDEX
    try
    {
        stoll(name);
        add_command("STOREI " + name);

        cout << "STORE VARIABLE INDEX AT " + name << endl;
    }
    // ELSE IT RECIVED THE VARIABLE NAME WHERE IT SHOULD STORE THE VALUE OF P0
    catch (const std::exception &e)
    {
        Variable var = variables.at(name);
        add_command("STORE " + to_string(var.memory_index));

        // DEBUG
        cout << "ASSIGN VARIABLE " + name + " AT " + to_string(var.memory_index) << endl;
    }
}

void __create_temp_variable(string name, string value)
{
    long long int memory = getMemory();
    long long int var_value = stoll(value);
    create_variable(name, 2, memory, 1, 1, 1);
    create_number(var_value, memory);

    // DEBUG
    cout << "CREATING TEMP VARIABLE " + name + " at " + to_string(memory) << endl;
}

void __read_variable_from_arrayNUM(string array_name, string index)
{
    Variable var = variables.at(array_name + index);
    add_command("LOAD " + to_string(var.memory_index));

    // DEBUG
    cout << "LOADING " + var.name + " FROM " + to_string(var.memory_index) << endl;
}

char *__read_variable_from_arrayVAR(string array_name, string index)
{
    long long int newVariableMemory = getMemory();
    long long int tempVariable = memoryCounter + 1; //czy tutaj musi być 10? XDDD

    Variable array = variables.at(array_name);
    Variable index_variable = variables.at(index);

    // IF ARRAY IS DECLARED FROM 1 WE MUST ADD ARRAY MEMORY INDEX TO GET ELEMENT MEMORY_INDEX
    if (array.start == 1)
    {
        create_number(array.memory_index, tempVariable);
    }
    // IF ARRAY IS DECLARED FROM DIFFERENT NUMBER THEN 1, WE MUST ADD ARRAY MEMORY INDEX - ARRAY START + 1 TO GET ELEMENT MEMORY_INDEX
    else
    {
        create_number(array.memory_index - array.start + 1, tempVariable);
    }

    add_command("LOAD " + to_string(index_variable.memory_index));
    add_command("ADD " + to_string(tempVariable));
    add_command("STORE " + to_string(newVariableMemory));

    return parseStringToChar(to_string(newVariableMemory));
}

///////////////////////////
//      CONDITIONS       //
///////////////////////////

void __EQ(string var1, string var2)
{
    // DEBUG
    cout << var1 + " == " + var2 << endl;

    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);
    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
    add_command("LOAD " + num2_adress);
    add_command("SUB " + num1_adress);
    add_command("JZERO " + to_string(cmd_index + 3));
    add_command("SUB 0");
    add_command("INC");
}

void __NEQ(string var1, string var2)
{
    // DEBUG
    cout << var1 + " != " + var2 << endl;

    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);
    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
    add_command("LOAD " + num2_adress);
    add_command("SUB " + num1_adress);
    add_command("JZERO " + to_string(cmd_index + 3));
    add_command("SUB 0");
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("INC ");
}

void __GE(string var1, string var2)
{
    // DEBUG
    cout << var1 + " > " + var2 << endl;

    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);
    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
    add_command("LOAD " + num2_adress);
    add_command("SUB " + num1_adress);
    add_command("JNEG " + to_string(cmd_index + 4));
    add_command("SUB 0");
    add_command("INC");
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("SUB 0");
}

void __LE(string var1, string var2)
{
    // DEBUG
    cout << var1 + " < " + var2 << endl;

    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);
    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
    add_command("LOAD " + num2_adress);
    add_command("SUB " + num1_adress);
    add_command("JPOS " + to_string(cmd_index + 4));
    add_command("SUB 0");
    add_command("INC");
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("SUB 0");
}

void __GEQ(string var1, string var2)
{
    // DEBUG
    cout << var1 + " >= " + var2 << endl;

    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);
    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
    add_command("LOAD " + num2_adress);
    add_command("SUB " + num1_adress);
    add_command("JNEG " + to_string(cmd_index + 5));
    add_command("JZERO " + to_string(cmd_index + 4));
    add_command("SUB 0");
    add_command("INC");
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("SUB 0");
}

void __LEQ(string var1, string var2)
{
    // DEBUG
    cout << var1 + " <= " + var2 << endl;

    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);
    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
    add_command("LOAD " + num2_adress);
    add_command("SUB " + num1_adress);
    add_command("JPOS " + to_string(cmd_index + 5));
    add_command("JZERO " + to_string(cmd_index + 4));
    add_command("SUB 0");
    add_command("INC");
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("SUB 0");
}

///////////////////////////
//      IN / OUT         //
///////////////////////////

void __read(string var)
{
    Variable variable = getVariable(var);
    add_command("GET");
    add_command("STORE " + to_string(variable.memory_index));

    // DEBUG
    cout << "WCZYTUJE DO " + variable.name << endl;
}

void __write(string var)
{
    // DEBUG
    cout << "WRITING " + var << endl;

    // IF IT CAN PARSE VAR IT MEANS IT HAS RECIVED THE MEMORY_INDEX OF THE ARRAY ELEMENT
    try
    {
        stoll(var);
        add_command("LOADI " + var);
    }
    // ELSE IT RECIVED VARIABLE NAME
    catch (const std::exception &e)
    {
        Variable variable = getVariable(var);
        add_command("LOAD " + to_string(variable.memory_index));
    }

    add_command("PUT");
}

///////////////////////////
//      ARITHMETICS      //
///////////////////////////

void __add(string var1, string var2)
{
    // DEBUG
    cout << "ADD " + var1 + " " + var2 << endl;

    long long int result = 0;
    Variable variable1;
    Variable variable2;
    variable1 = getVariable(var1);
    variable2 = getVariable(var2);

    add_command("LOAD " + to_string(variable2.memory_index));
    add_command("ADD " + to_string(variable1.memory_index));

    if (variable1.type == 2)
    {
        //remove variable 1
    }

    if (variable2.type == 2)
    {
        //remove variable 2
    }
}

void __sub(string var1, string var2)
{
    // DEBUG
    cout << "SUB " + var1 + " " + var2 << endl;

    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);
    add_command("LOAD " + to_string(variable1.memory_index));
    add_command("SUB " + to_string(variable2.memory_index));
}

void __mul(string var1, string var2)
{
    // DEBUG
    cout << "MUL " + var1 + " " + var2 << endl;

    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);

    if (variable1.type == 1 && variable2.type == 1)
    {
    }

    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);

    string num1_temp = to_string(memoryCounter + 1);
    string num2_temp = to_string(memoryCounter + 2);
    string flag = to_string(memoryCounter + 3);
    string result = to_string(memoryCounter + 4);
    string one = to_string(oneIndex);
    string minus_one = to_string(minusOneIndex);

    add_command("SUB 0");
    add_command("STORE " + result);
    add_command("STORE " + flag);
    add_command("LOAD " + num1_adress);
    add_command("STORE " + num1_temp);
    add_command("LOAD " + num2_adress);
    add_command("STORE " + num2_temp);
    add_command("JPOS " + to_string(cmd_index + 8));
    add_command("SUB 0");
    add_command("SUB " + num2_adress);
    add_command("STORE " + num2_temp);
    add_command("LOAD " + flag);
    add_command("INC");
    add_command("STORE " + flag);
    add_command("LOAD " + num2_temp);
    add_command("JZERO " + to_string(cmd_index + 15));
    add_command("SHIFT " + minus_one);
    add_command("SHIFT " + one);
    add_command("SUB " + num2_temp);
    add_command("JZERO " + to_string(cmd_index + 4));
    add_command("LOAD " + result);
    add_command("ADD " + num1_temp);
    add_command("STORE " + result);
    add_command("LOAD " + num1_temp);
    add_command("SHIFT " + one);
    add_command("STORE " + num1_temp);
    add_command("LOAD " + num2_temp);
    add_command("SHIFT " + minus_one);
    add_command("STORE " + num2_temp);
    add_command("JUMP " + to_string(cmd_index - 14));
    add_command("LOAD " + flag);
    add_command("JZERO " + to_string(cmd_index + 5));
    add_command("SUB 0");
    add_command("SUB " + result);
    add_command("STORE " + result);
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("LOAD " + result);
}

void __div(string var1, string var2)
{
    // DEBUG
    cout << "DIV " + var1 + " " + var2 << endl;

    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);

    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);

    string num1_temp = to_string(memoryCounter + 1);
    string num2_temp = to_string(memoryCounter + 2);
    string flag = to_string(memoryCounter + 3);
    string result = to_string(memoryCounter + 4);
    string multiple = to_string(memoryCounter + 5);
    string one = to_string(oneIndex);
    string minus_one = to_string(minusOneIndex);

    add_command("SUB 0");
    add_command("STORE " + flag);
    add_command("STORE " + result);
    add_command("INC");
    add_command("STORE " + multiple);
    add_command("LOAD " + num1_adress);
    add_command("JZERO " + to_string(cmd_index + 50));
    add_command("STORE " + num1_temp);
    add_command("JPOS " + to_string(cmd_index + 7));
    add_command("SUB 0");
    add_command("SUB " + num1_temp);
    add_command("STORE " + num1_temp);
    add_command("LOAD " + flag);
    add_command("INC");
    add_command("STORE " + flag);
    add_command("LOAD " + num2_adress);
    add_command("STORE " + num2_temp);
    add_command("JZERO " + to_string(cmd_index + 39));
    add_command("JPOS " + to_string(cmd_index + 7));
    add_command("LOAD " + flag);
    add_command("DEC");
    add_command("STORE " + flag);
    add_command("SUB 0");
    add_command("SUB " + num2_temp);
    add_command("STORE " + num2_temp);
    add_command("SUB " + num1_temp);
    add_command("JPOS " + to_string(cmd_index + 8));
    add_command("LOAD " + multiple);
    add_command("SHIFT " + one);
    add_command("STORE " + multiple);
    add_command("LOAD " + num2_temp);
    add_command("SHIFT " + one);
    add_command("STORE " + num2_temp);
    add_command("JUMP " + to_string(cmd_index - 8));
    add_command("LOAD " + num2_temp);
    add_command("SUB " + num1_temp);
    add_command("JPOS " + to_string(cmd_index + 7));
    add_command("LOAD " + num1_temp);
    add_command("SUB " + num2_temp);
    add_command("STORE " + num1_temp);
    add_command("LOAD " + result);
    add_command("ADD " + multiple);
    add_command("STORE " + result);
    add_command("LOAD " + num2_temp);
    add_command("SHIFT " + minus_one);
    add_command("STORE " + num2_temp);
    add_command("LOAD " + multiple);
    add_command("SHIFT " + minus_one);
    add_command("STORE " + multiple);
    add_command("JPOS " + to_string(cmd_index - 15));
    add_command("LOAD " + flag);
    add_command("JZERO " + to_string(cmd_index + 5));
    add_command("SUB 0");
    add_command("SUB " + result);
    add_command("STORE " + result);
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("LOAD " + result);
}

void __mod(string var1, string var2)
{
    // DEBUG
    cout << "MOD " + var1 + " " + var2 << endl;

    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);

    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
    string num1_temp = to_string(memoryCounter + 1);
    string num2_temp = to_string(memoryCounter + 2);
    string result = to_string(memoryCounter + 3);
    string multiple = to_string(memoryCounter + 4);
    string one = to_string(memoryCounter + 5);
    string minus_one = to_string(memoryCounter + 6);
    string num1_sign = to_string(memoryCounter + 7);
    string num2_sign = to_string(memoryCounter + 8);
    string remainder = to_string(memoryCounter + 9);

    add_command("SUB 0");
    add_command("STORE " + num1_sign);
    add_command("STORE " + num2_sign);
    add_command("INC");
    add_command("STORE " + multiple);
    add_command("STORE " + one);
    add_command("DEC");
    add_command("DEC");
    add_command("STORE " + minus_one);
    add_command("LOAD " + num1_adress);
    add_command("JZERO " + to_string(cmd_index + 60));
    add_command("STORE " + num1_temp);
    add_command("JPOS " + to_string(cmd_index + 7));
    add_command("SUB 0");
    add_command("SUB " + num1_temp);
    add_command("STORE " + num1_temp);
    add_command("LOAD " + num1_sign);
    add_command("INC");
    add_command("STORE " + num1_sign);
    add_command("LOAD " + num2_adress);
    add_command("STORE " + num2_temp);
    add_command("STORE " + remainder);
    add_command("JZERO " + to_string(cmd_index + 48));
    add_command("JPOS " + to_string(cmd_index + 8));
    add_command("SUB 0");
    add_command("SUB " + num2_temp);
    add_command("STORE " + num2_temp);
    add_command("STORE " + remainder);
    add_command("LOAD " + num2_sign);
    add_command("INC");
    add_command("STORE " + num2_sign);
    add_command("SUB " + num1_temp);
    add_command("JPOS " + to_string(cmd_index + 8));
    add_command("LOAD " + multiple);
    add_command("SHIFT " + one);
    add_command("STORE " + multiple);
    add_command("LOAD " + num2_temp);
    add_command("SHIFT " + one);
    add_command("STORE " + num2_temp);
    add_command("JUMP " + to_string(cmd_index - 8));
    add_command("LOAD " + num2_temp);
    add_command("SUB " + num1_temp);
    add_command("JPOS " + to_string(cmd_index + 4));
    add_command("LOAD " + num1_temp);
    add_command("SUB " + num2_temp);
    add_command("STORE " + num1_temp);
    add_command("LOAD " + num2_temp);
    add_command("SHIFT " + minus_one);
    add_command("STORE " + num2_temp);
    add_command("LOAD " + multiple);
    add_command("SHIFT " + minus_one);
    add_command("STORE " + multiple);
    add_command("JPOS " + to_string(cmd_index - 12));
    add_command("LOAD " + num1_sign);
    add_command("JZERO " + to_string(cmd_index + 10));
    add_command("LOAD " + num2_sign);
    add_command("JZERO " + to_string(cmd_index + 5));
    add_command("SUB 0");
    add_command("SUB " + num1_temp);
    add_command("STORE " + num1_temp);
    add_command("JUMP " + to_string(cmd_index + 10));
    add_command("LOAD " + remainder);
    add_command("SUB " + num1_temp);
    add_command("JUMP " + to_string(cmd_index + 7));
    add_command("LOAD " + num2_sign);
    add_command("JZERO " + to_string(cmd_index + 4));
    add_command("LOAD " + num1_temp);
    add_command("SUB " + remainder);
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("LOAD " + num1_temp);
}

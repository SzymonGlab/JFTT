#include "compilator.hpp"

map<string, Variable> variables;
vector<string> asmCode;
// TABLICA TA SŁUŻY DO SPRAWDZANIA PODCZAS WRITE I ASSIGN CZY MAMY UŻYĆ LOAD CZY LOADI, STORE CZY STOREI
// Załóżmy x = 5 i A(5) znajdowało się pod adresem 19. Po wczytaniu A(x) otrzymamy adres miejsca w którym znajduje się 19, dzięki czemu robiąc STOREI zapiszemy coś pod adresem 19 lub odczytamy robiąc LOADi.
vector<long long int> array_index_variables;
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

int getValueOfVariableFromMemoryIndex(string var_memory_index)
{

    string var_name;
    // FIND VARIABLE NAME AT THIS MEMORY INDEX
    long long int memory_index = stoll(var_memory_index);
    for (const auto &[name, variable] : variables)
    {
        if (variable.memory_index == memory_index)
        {
            return stoi(variable.name);
        }
    }
}

int check_variable_type(string mem_index)
{

    long long int memory_index = stoll(mem_index);
    for (const auto &[name, variable] : variables)
    {
        if (variable.memory_index == memory_index)
        {
            return variable.type;
        }
    }

    // Variable not found
    return -1;
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
    long long int current_multiplier = memoryCounter + 1;
    long long int prev_multiplier = memoryCounter + 2;
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
        add_command("STORE " + to_string(current_multiplier));

        while (current_number <= number)
        {
            current_number += current_number;
            counter++;
            add_command("STORE " + to_string(prev_multiplier));
            add_command("SHIFT " + to_string(current_multiplier));
        }

        current_number = current_number / 2;

        int difference_left = number - current_number;

        // LICZBA KTÓRĄ TWORZYMY JEST POMIĘDZY (1/2,3/4) następnego mnożenia * 2
        // DODAJ INC RÓŻNICĘ

        if (number < 3 * current_number / 2)
        {
            // JEŚLI LICZBA JEST BLIŻEJ 1/2 DODAJEMY
            if (number <= 5 * current_number / 4)
            {
                for (int i = 0; i < number - current_number; i++)
                {
                    add_command("INC");
                }
            }
            // JEŚLI LICZBA JEST BLIŻEJ 3/4 ODEJMUJEMY
            else if (number < 5 * current_number / 4)
            {
                add_command("ADD " + to_string(prev_multiplier));
                for (int i = 0; i < 3 * current_number - number; i++)
                {
                    add_command("DEC");
                }
            }
        }
        // LICZBA KTÓRĄ TWORZYMY JEST POMIĘDZY (3/4, 1) następnego mnożenia * 2
        // DODAJ POPRZEDNI WYNIK KTÓRY MIAŁEŚ PODCZAS MNOŻENIA ŻEBY OTRZYMAĆ 3/4 i INC
        else if (number > 3 * current_number / 2)
        {
            add_command("ADD " + to_string(prev_multiplier));

            // JEŚLI LICZBA JEST BLIŻEJ 3/4 DODAJEMY
            if (number <= 7 * current_number / 4)
            {
                for (int i = 0; i < number - 3 * current_number / 2; i++)
                {
                    add_command("INC");
                }
            }
            // JEŚLI LICZBA JEST BLIŻEJ 1 ODEJMUJEMY
            else
            {
                add_command("ADD " + to_string(prev_multiplier));
                for (int i = 0; i < 2 * current_number - number; i++)
                {
                    add_command("DEC");
                }
            }
        }
        // LICZBA KTÓRĄ TWORZYMY JEST W 3/4 następnego mnożęnia * 2
        // DODAJ POPRZEDNI WYNIK KTÓRY MIAŁEŚ PODCZAS MNOŻENIA
        else
        {
            add_command("ADD " + to_string(prev_multiplier));
        }

        add_command("STORE " + to_string(mem_index));

        if (negative)
        {
            add_command("SUB 0");
            add_command("SUB " + to_string(mem_index));
            add_command("STORE " + to_string(mem_index));
        }
    }
}

///////////////////////////
//       VARIABLES       //
///////////////////////////

void __declare_variable(string name)
{
    //DEBUG
    cout << "DECLARE VARIABLE " + name + " at " + to_string(memoryCounter) << endl;

    create_variable(name, 2, getMemory(), 1, 1, 1);
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

void __assign_variable(string mem_index)
{
    if (find(array_index_variables.begin(), array_index_variables.end(), stoll(mem_index)) != array_index_variables.end())
    {
        cout << "USING STORE I ASSIGN AT " + mem_index << endl;
        add_command("STOREI " + mem_index);
    }
    else
    {
        cout << "USING STORE ASSIGN AT " + mem_index << endl;
        add_command("STORE " + mem_index);
    }
}

char *__create_temp_variable(string value)
{

    // IF WE CREATED THIS VARIABLE (IN DEED CONST FOR E.G. 1090) BEFORE
    if (variables.find(value) != variables.end())
    {
        cout << "READING TEMP VARIABLE " + value + " from " + to_string(variables.at(value).memory_index) << endl;
        return parseStringToChar(to_string(variables.at(value).memory_index));
    }
    // IF VARIABLE (IN DEED CONST FOR E.G. 1090) WASN'T CREATED BEFORE
    else
    {
        long long int memory = getMemory();
        long long int var_value = stoll(value);
        create_variable(value, 1, memory, 1, 1, 1);
        // DEBUG
        cout << "CREATING TEMP VARIABLE " + value + " at " + to_string(memory) << endl;
        create_number(var_value, memory);
        return parseStringToChar(to_string(memory));
    }
}

char *__read_variable(string var_name)
{

    return parseStringToChar(to_string(variables.at(var_name).memory_index));
}

char *__read_variable_from_arrayNUM(string array_name, string index)
{
    Variable var = variables.at(array_name + index);

    // DEBUG
    cout << "LOADING " + var.name + " FROM " + to_string(var.memory_index) << endl;

    return parseStringToChar(to_string(var.memory_index));
}

char *__read_variable_from_arrayVAR(string array_name, string index)
{
    long long int newVariableMemory = getMemory();
    long long int tempVariable = memoryCounter + 4; //czy tutaj musi być 10? XDDD

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

    cout << "CREATING TEMP VARIABLE WITH INDEX AT " + to_string(newVariableMemory) << endl;

    array_index_variables.push_back(newVariableMemory);

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

void __read(string var_memory_index)
{
    add_command("GET");

    if (find(array_index_variables.begin(), array_index_variables.end(), stoll(var_memory_index)) != array_index_variables.end())
    {
        cout << "STORE I AT " + var_memory_index << endl;
        add_command("STOREI " + var_memory_index);
    }
    else
    {
        // DEBUG
        add_command("STORE " + var_memory_index);
        cout << "STORE AT " + var_memory_index << endl;
    }

    // DEBUG
    cout << "WCZYTUJE DO " + var_memory_index << endl;
}

void __write(string var)
{
    // int var_type = check_variable_type(var);

    long long int var_memory_index = stoll(var);

    if (find(array_index_variables.begin(), array_index_variables.end(), var_memory_index) != array_index_variables.end())
    {
        //     // DEBUG
        add_command("LOADI " + var);
        cout << "LOADING I AND PRINTING " + var << endl;
    }
    else
    {
        // DEBUG
        add_command("LOAD " + var);
        cout << "LOADING AND PRINTING " + var << endl;
    }

    add_command("PUT");
}

///////////////////////////
//      ARITHMETICS      //
///////////////////////////

char *__add(string var1_memory_index, string var2_memory_index)
{
    // DEBUG
    cout << "ADD " + var1_memory_index + " " + var2_memory_index << endl;

    add_command("LOAD " + var1_memory_index);
    add_command("ADD " + var2_memory_index);

    return parseStringToChar(var1_memory_index);
}

char *__sub(string var1_memory_index, string var2_memory_index)
{
    // DEBUG
    cout << "SUB " + var1_memory_index + " " + var2_memory_index << endl;

    add_command("LOAD " + var1_memory_index);
    add_command("SUB " + var2_memory_index);

    return parseStringToChar(var1_memory_index);
}

char *__mul(string var1_memory_index, string var2_memory_index)
{

    int var1_type = check_variable_type(var1_memory_index);
    int var2_type = check_variable_type(var2_memory_index);

    if (var1_type == 1 && var2_type == 1)
    {
        long long int var1_value = getValueOfVariableFromMemoryIndex(var1_memory_index);
        long long int var2_value = getValueOfVariableFromMemoryIndex(var2_memory_index);

        // DEBUG
        cout << "CREATE RESULT OF MUL " + to_string(var1_value) + " " + to_string(var2_value) << endl;

        create_number(var1_value * var2_value, memoryCounter + 1);
        add_command("STORE " + to_string(memoryCounter + 1));

        return parseStringToChar(to_string(memoryCounter + 1));
    }

    // DEBUG
    cout << "MUL " + var1_memory_index + " " + var2_memory_index << endl;

    string num1_adress = var1_memory_index;
    string num2_adress = var2_memory_index;

    string num1_temp = to_string(memoryCounter + 1);
    string num2_temp = to_string(memoryCounter + 2);
    string flag = to_string(memoryCounter + 3);
    string result = to_string(memoryCounter + 4); //czy result mnożenia nie powienien być zapisany na stałe? (get_memory())
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

    return parseStringToChar(result);
}

char *__div(string var1_memory_index, string var2_memory_index)
{

    int var1_type = check_variable_type(var1_memory_index);
    int var2_type = check_variable_type(var2_memory_index);

    if (var1_type == 1 && var2_type == 1)
    {
        long long int var1_value = getValueOfVariableFromMemoryIndex(var1_memory_index);
        long long int var2_value = getValueOfVariableFromMemoryIndex(var2_memory_index);

        // DEBUG
        cout << "CREATE RESULT OF DIV " + to_string(var1_value) + " " + to_string(var2_value) << endl;

        create_number(var1_value * var2_value, memoryCounter + 1);
        add_command("STORE " + to_string(memoryCounter + 1));

        return parseStringToChar(to_string(memoryCounter + 1));
    }

    // DEBUG
    cout << "DIV " + var1_memory_index + " " + var2_memory_index << endl;

    string num1_adress = var1_memory_index;
    string num2_adress = var2_memory_index;

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

    return parseStringToChar(result);
}

char *__mod(string var1_memory_index, string var2_memory_index)
{
    // DEBUG
    cout << "MOD " + var1_memory_index + " " + var2_memory_index << endl;

    string num1_adress = var1_memory_index;
    string num2_adress = var2_memory_index;
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

    return parseStringToChar(remainder);
}

#include "compilator.hpp"

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

void get_value_of_array_with_variable(long long int address)
{
    add_command("LOADI " + to_string(address));
    add_command("STORE " + to_string(address));
}

void iterateVariables()
{
    std::map<std::string, Variable>::iterator it = variables.begin();
    while (it != variables.end())
    {
        // Accessing KEY from element pointed by it.
        std::string word = it->first;

        // Accessing VALUE from element pointed by it.
        Variable var = it->second;

        std::cout << word << " :: " << to_string(var.memory_index) << std::endl;

        // Increment the Iterator to point to next entry
        it++;
    }
}

void error(string a, int yylineno, string msg)
{
    usleep(500000);
    cout << "\e[1m\x1B[31m[ ERROR ]\e[0m \e[1m[ LINE " << yylineno << " ] \e[1m\x1B[31m" << msg << " " << a << ".\e[0m\n"
         << endl;
    exit(1);
}

// void display_command(INFORMATION, string information){
//     cout << cmd_index << " - " << cmd_index+ lines_amount << " : " << information << endl;
// }
// char *concat_chars(char *first, char *second)
// {
//     long long int newSize = strlen(first) + strlen(second) + 1;

//     char *newBuffer = (char *)malloc(newSize);

//     strcpy(newBuffer, first);
//     strcat(newBuffer, second);
//     free(first);
//     return newBuffer;
// }

char *parseStringToChar(string str)
{
    const char *iStr = str.c_str();
    long long int newSize = strlen(iStr) + 1;
    char *newName = (char *)malloc(newSize);
    strcpy(newName, iStr);
    return newName;
}

int get_value_of_variable_from_memory_index(string var_memory_index)
{

    string var_name;
    // FIND VARIABLE NAME AT THIS MEMORY INDEX (VARIABLE VALUE IS ITS NAME)
    long long int memory_index = stoll(var_memory_index);
    for (const auto &[name, variable] : variables)
    {
        if (variable.memory_index == memory_index)
        {
            return stoll(variable.name);
        }
    }
}

Variable get_variable_from_mem_index(string memory_index)
{
    long long int mem_index = stoll(memory_index);

    for (const auto &[name, variable] : variables)
    {
        if (variable.memory_index == mem_index)
        {
            return variable;
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

bool check_variable_exist(string memory_index)
{
    long long int mem_index = stoll(memory_index);

    for (const auto &[name, variable] : variables)
    {
        if (variable.memory_index == mem_index)
        {
            return true;
        }
    }

    return false;
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

void create_jump()
{
    cmd_index_for_jumps.push(cmd_index);
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

bool create_variable(string name, int type, long long int memoryIndex, long long int size, long long int start, long long int end, bool inicialized)
{
    if (variables.find(name) != variables.end())
    {
        return false;
    }
    else
    {
        Variable var = {name, type, memoryIndex, size, start, end, inicialized};
        variables.insert(make_pair(name, var));
        return true;
    }
}

void createOnes()
{
    oneIndex = getMemory();
    create_variable("1", 1, oneIndex, 1, 1, 1, true);
    minusOneIndex = getMemory();
    create_variable("-1", 1, minusOneIndex, 1, 1, 1, true);
    add_command("SUB 0");
    add_command("INC");
    add_command("STORE " + to_string(oneIndex));
    add_command("DEC");
    add_command("DEC");
    add_command("STORE " + to_string(minusOneIndex));
}

void update_variables(Variable var)
{
    variables[var.name] = var;
}

// void create_number_new(long long int number, long long int mem_index)
// {
//     long long int current_number = 1;
//     long long int shift = 0;
//     long long int number_to_create = number;
//     bool negative = false;

//     add_command("SUB 0");

//     if (number < 10)
//     {
//         for (int i = 0; i < number; i++)
//         {
//             add_command("INC");
//         }
//         return;
//     }

//     add_command("INC");
//     add_command("STORE " + mem_index);

//     while (number > 0)
//     {
//         shift = 0;
//         current_number = 1;
//         while (number > current_number)
//         {
//             current_number = current_number * 2;
//             shift++;
//         }

//         current_number = current_number / 2;
//         number = number - current_number;
//         long long int shift_memory = getMemory();

//         create_number(shift, shift_memory);

//         add_command("LOAD " + to_string(mem_index));
//         add_command("SHIFT " + to_string(shift_memory));
//         add_command("STORE " + to_string(mem_index));
//     }
// }

void create_number_older(long long int number, long long int mem_index)
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

void create_number_old(long long int number, long long int mem_index)
{
    bool negative = false;
    add_command("SUB 0");
    for (int i = 0; i < number; i++)
    {
        add_command("INC");
    }
    add_command("STORE " + to_string(mem_index));

    if (negative)
    {
        add_command("SUB 0");
        add_command("SUB " + to_string(mem_index));
        add_command("STORE " + to_string(mem_index));
    }
}

void create_number(long long int number, long long int mem_index)
{
    long long int current_number = 1;
    long long int shift = 0;
    long long int number_to_create = number;
    bool negative = false;

    if (number < 0)
    {
        negative = true;
        number = 0 - number;
    }

    // if( number == 1 ){
    //     add_command("LOAD " + to_string(oneIndex));
    // }

    // if( number == -1){
    //     add_command("LOAD " + to_string(minusOneIndex));
    // }

    // add_command("TWORZĘ LICZBĘ "+to_string(number) +"----------------------------");

    // cout << "WCHODZĘ TUTAJ XD" << endl;
    // SPRWDZIĆ CZY ISTNIEJE JUŻ TAKA LICZBA
    // if (variables.find(to_string(number)) == variables.end())
    // {
    //     cout << "JESTEM W IFIE" << endl;
    //     Variable num = variables.at(to_string(number));
    //     add_command("LOAD " + to_string(num.memory_index));
    //     return;
    // }
    // else
    // {
    // cout << "WCHODZĘ TUTAJ XD" << endl;
    add_command("SUB 0");

    if (number < 300)
    {
        for (int i = 0; i < number; i++)
        {
            add_command("INC");
        }
        add_command("STORE " + to_string(mem_index));
        return;
    }
    else
    {
        add_command("INC");
        add_command("STORE " + to_string(mem_index));

        while (number > 1)
        {
            shift = 0;
            current_number = 1;
            while (number > current_number)
            {
                current_number = current_number * 2;
                shift++;
            }
            current_number = current_number / 2;
            number = number - current_number;

            long long int shift_memory = getMemory();
            long long int next_addition_mem = getMemory();
            add_command("SUB 0 ");

            if (shift == 1)
            {
                add_command("LOAD  " + to_string(oneIndex));
            }
            else
            {
                for (int i = 0; i < shift - 1; i++)
                {
                    add_command("INC");
                }
                add_command("STORE " + to_string(shift_memory));
                add_command("LOAD  " + to_string(oneIndex));
                add_command("SHIFT " + to_string(shift_memory));
                add_command("STORE " + to_string(next_addition_mem));
            }

            add_command("ADD " + to_string(mem_index));
            add_command("STORE " + to_string(mem_index));
            freeMemory();
            freeMemory();
        }

        if (negative)
        {
            add_command("SUB 0");
            add_command("SUB " + to_string(mem_index));
            add_command("STORE " + to_string(mem_index));
        }
    }
}

void check_both_variables_inicialized(string var1_memory, string var2_memory, int yylieno)
{

    Variable var1 = get_variable_from_mem_index(var1_memory);
    Variable var2 = get_variable_from_mem_index(var2_memory);

    if (!var1.inicialized || !var2.inicialized)
    {
        if (!var1.inicialized)
        {
            error(var1.name, yylieno, "Variable was not assigned");
        }
        else
        {
            error(var2.name, yylieno, "Variable was not assigned");
        }
    }
    // It is needed in ASSIGN method because, we mmmust know when we assign result of arithmetics and when a new variable.
    both_variables_inicialized = true;
}

///////////////////////////
//       VARIABLES       //
///////////////////////////

void __declare_variable(string name, int yylineno)
{
    //DEBUG
    cout << "DECLARE VARIABLE " + name + " at " + to_string(memoryCounter) << endl;

    bool var_created = create_variable(name, 2, getMemory(), 1, 1, 1, false);

    if (!var_created)
    {
        error(name, yylineno - 1, "Second declaration");
    }
}

void __declare_array(string name, string start, string end, int yylieno)
{
    // DEBUG
    cout << "DECLARE ARRAY " + name + " at " + to_string(memoryCounter) << endl;

    long long int size = stoll(end) - stoll(start);

    if (size < 0)
    {
        error(name, yylieno, "Wrong array range");
    }

    bool var_created = create_variable(name, 4, getMemory(), size, stoll(start), stoll(end), false);

    if (!var_created)
    {
        error(name, yylieno - 1, "Second declaration");
    }

    memoryCounter += size + 1;
    // for (long long int i = stoll(start); i <= stoll(end); i++)
    // {
    //     create_variable(name + to_string(i), 3, getMemory(), 1, 1, 1, false);
    // }
}

void __assign_variable(string mem_index, string variable_to_assign, int yylineno)
{
    // cout << "WCHODZĘ TUTAJ" << endl;
    //CHECK IF U RECIVED THE VARIABLE OR SOME OTHER STH ELSE FOR E.G. RESULT OF MULTIPLICATION
    // if(check_variable_exist(mem_index)){

    // if(!both_variables_inicialized){
    // Variable must exist in variables, becuase it isn't result of
    Variable var_to_assign = get_variable_from_mem_index(variable_to_assign);
    // cout << "WCHODZĘ TUTAJ " + to_string(var_to_assign.memory_index) << endl;

    if (!var_to_assign.inicialized)
    {
        error(var_to_assign.name, yylineno, "Varibales was not assigned");
    }
    // } else {
    // Reset variable for next assign
    // both_variables_inicialized = false;
    // }
    // cout << "XDDDDDDDDDDDDDDDDDDDDDDDDDD" << endl;
    // cout << "MEM_INDEX =  " + mem_index << endl;
    Variable var_assigned = get_variable_from_mem_index(mem_index);
    // cout << "WCHODZĘ TUTAJ " + var_assigned.name << endl;
    var_assigned.inicialized = true;
    update_variables(var_assigned);
    // cout << "WCHODZĘ TUTAJ" << endl;

    if (find(array_index_variables.begin(), array_index_variables.end(), stoll(mem_index)) != array_index_variables.end())
    {

        cout << "ZAPISUJE \"I\" JĄ POD ADRESEM " + mem_index << endl;
        add_command("STOREI " + mem_index);
    }
    else
    {
        cout << "ZAPISUJE P0 POD ADRESEM " + mem_index << endl;
        // cout << var_assigned.name + " = " + var_to_assign.name << endl;
        add_command("STORE " + mem_index);
    }
}

char *__create_temp_variable(string value)
{

    if (stoll(value) == 1)
    {
        cout << "WCZYTUJE 1 " << endl;
        add_command("LOAD " + to_string(oneIndex));
        return parseStringToChar(to_string(oneIndex));
    }

    if (stoll(value) == -1)
    {
        cout << "WCZYTUJE -1 " << endl;
        add_command("LOAD " + to_string(minusOneIndex));
        return parseStringToChar(to_string(minusOneIndex));
    }

    // IF WE CREATED THIS VARIABLE (IN DEED CONST  E.G. 1090) BEFORE
    if (variables.find(value) != variables.end())
    {
        cout << "WCZYTUJE ZAPISANĄ LICZBĘ " + value + " Z ADRESU " + to_string(variables.at(value).memory_index) << endl;
        add_command("LOAD " + to_string(variables.at(value).memory_index));
        return parseStringToChar(to_string(variables.at(value).memory_index));
    }
    // IF VARIABLE (IN DEED CONST E.G. 1090) WASN'T CREATED BEFORE
    else
    {
        long long int memory = getMemory();
        long long int var_value = stoll(value);
        create_variable(value, 1, memory, 1, 1, 1, true);
        // DEBUG
        cout << "TWORZĘ LICZBĘ  " + value + " POD ADRESEM " + to_string(memory) << endl;
        create_number(var_value, memory);
        char *ret = parseStringToChar(to_string(memory));
        // cout << ret << endl;
        return parseStringToChar(to_string(memory));
    }
}

char *__read_variable(string var_name, int yylineno)
{
    // DEBUG
    cout << "WCZYTUJE ZMIENNĄ " + var_name << endl;

    try
    {
        // If it's first element of array e.g. we declare array arr(10:20), type 4 is variable arr.
        if (variables.at(var_name).type == 4)
        {
            error(var_name, yylineno, "Wrong use of array variable");
        }

        else
        {

            char *variable_mem_index = parseStringToChar(to_string(variables.at(var_name).memory_index));
            add_command("LOAD " + to_string(variables.at(var_name).memory_index));
            return variable_mem_index;
        }
    }
    // Variable doesn't exist in map, so it must had been undeclared.
    catch (const std::exception &e)
    {
        error(var_name, yylineno, "Undeclared variable");
    }
}

char *__read_variable_from_arrayNUM(string array_name, string index, int yylineno)
{
    Variable var;
    try
    {
        Variable arr = variables.at(array_name);

        // Check if variables exists
        if (variables.find(array_name + index) != variables.end())
        {
            var = variables.at(array_name + index);
            add_command("LOAD " + to_string(var.memory_index));
        }
        else
        {
            long long int int_index = stoll(index);

            if (arr.start < 0)
            {
                // if (int_index < 0)
                // {
                create_variable(array_name + index, 3, arr.memory_index - arr.start + stoll(index) + 1, 1, 1, 1, true);
                // }
                // else
                // {
                //     create_variable(array_name + index, 3, arr.memory_index - arr.start +stoll(index) + 1, 1, 1, 1, true);
                // }
            }
            else
            {
                create_variable(array_name + index, 3, arr.memory_index + stoll(index) + 1, 1, 1, 1, true);
            }

            var = variables.at(array_name + index);
            // cout << "TWORZĘ ZMIENNĄ " + var.name + " POD ADRESEM " + to_string(var.memory_index)  << endl;
        }
    }
    // If it can't find variable with name "array_name + index" it means such array wasn't declared.
    catch (const std::exception &e)
    {
        error(array_name, yylineno, "Wrong use of variable");
    }

    // DEBUG
    cout << "WCZYTUJĘ ZMIENNĄ " + var.name + " Z ADRESU " + to_string(var.memory_index) << endl;

    return parseStringToChar(to_string(var.memory_index));
}

char *__read_variable_from_arrayVAR(string array_name, string index, int yylineno)
{
    Variable index_variable;
    Variable array;
    try
    {
        index_variable = variables.at(index);
    }
    catch (const std::exception &e)
    {
        error(index, yylineno, "Undeclared variable");
    }
    try
    {
        array = variables.at(array_name);
    }
    catch (const std::exception &e)
    {
        error(array_name, yylineno, "Undeclared variable");
    }

    // cout << "TUTAJ" <<endl;

    long long int newVariableMemory = getMemory();
    long long int tempVariable = getMemory();

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

    // cout << "TUTAJ" <<endl;

    add_command("LOAD " + to_string(index_variable.memory_index));
    add_command("ADD " + to_string(tempVariable));
    add_command("STORE " + to_string(newVariableMemory));

    cout << "TWORZĘ ZMIENNĄ Z INDEKSEM TABLICY " + array.name + " POD ADRSEM " + to_string(newVariableMemory) << endl;

    array_index_variables.push_back(newVariableMemory);
    // It will always create variable with same name, so it will reset (memory will not reset becaue it uses getMemory!!!!)
    // variables["ARRAY_INDEX" ] = {"ARRAY_INDEX", 6, newVariableMemory, 1, 1, 1, true};
    create_variable("ARRAY_INDEX" + to_string(newVariableMemory), 6, newVariableMemory, 1, 1, 1, true);

    return parseStringToChar(to_string(newVariableMemory));
}

///////////////////////////
//      CONDITIONS       //
///////////////////////////

char *__EQ(string var1_address, string var2_address)
{
    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    cout << "SPRAWDZAM  " + var1.name + " == " + var2.name << endl;

    add_command("LOAD " + var1_address);
    add_command("SUB " + var2_address);
    add_command("JZERO " + to_string(cmd_index + 3));
    add_command("SUB 0");
    add_command("INC");
    add_command("JPOS ");
    create_jump();

    return parseStringToChar(to_string(6));
}

char *__NEQ(string var1_address, string var2_address)
{
    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    cout << "SPRAWDZAM  " + var1.name + " != " + var2.name << endl;

    add_command("LOAD " + var2_address);
    add_command("SUB " + var1_address);
    add_command("JZERO " + to_string(cmd_index + 3));
    add_command("SUB 0");
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("INC ");
    add_command("JPOS ");
    create_jump();
    return parseStringToChar(to_string(7));
}

char *__GE(string var1_address, string var2_address)
{
    // DEBUG
    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    cout << "SPRAWDZAM  " + var1.name + " > " + var2.name << endl;

    add_command("LOAD " + var2_address);
    add_command("SUB " + var1_address);
    add_command("JNEG " + to_string(cmd_index + 4));
    add_command("SUB 0");
    add_command("INC");
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("SUB 0");
    add_command("JPOS ");
    create_jump();

    // cout << "------------" << endl;
    // print_ASM();
    // cout << "------------" << endl;

    return parseStringToChar(to_string(8));
}

char *__LE(string var1_address, string var2_address)
{
    // DEBUG
    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    cout << "SPRAWDZAM  " + var1.name + " < " + var2.name << endl;

    add_command("LOAD " + var2_address);
    add_command("SUB " + var1_address);
    add_command("JPOS " + to_string(cmd_index + 4));
    add_command("SUB 0");
    add_command("INC");
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("SUB 0");
    add_command("JPOS ");
    create_jump();

    return parseStringToChar(to_string(8));
}

char *__GEQ(string var1_address, string var2_address)
{
    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    cout << "SPRAWDZAM  " + var1.name + " >= " + var2.name << endl;

    add_command("LOAD " + var2_address);
    add_command("SUB " + var1_address);
    add_command("JNEG " + to_string(cmd_index + 5));
    add_command("JZERO " + to_string(cmd_index + 4));
    add_command("SUB 0");
    add_command("INC");
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("SUB 0");
    add_command("JPOS ");
    create_jump();

    return parseStringToChar(to_string(9));
}

char *__LEQ(string var1_address, string var2_address)
{
    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    cout << "SPRAWDZAM  " + var1.name + " <= " + var2.name << endl;

    add_command("LOAD " + var2_address);
    add_command("SUB " + var1_address);
    add_command("JPOS " + to_string(cmd_index + 5));
    add_command("JZERO " + to_string(cmd_index + 4));
    add_command("SUB 0");
    add_command("INC");
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("SUB 0");
    add_command("JPOS ");
    create_jump();
    return parseStringToChar(to_string(9));
}

///////////////////////////
//      IN / OUT         //
///////////////////////////

void __read(string var_memory_index)
{
    add_command("GET");

    if (find(array_index_variables.begin(), array_index_variables.end(), stoll(var_memory_index)) != array_index_variables.end())
    {
        cout << "ZAPISUJE \"I\" POD ADRESEM: " + var_memory_index << endl;
        add_command("STOREI " + var_memory_index);
    }
    else
    {
        // DEBUG
        add_command("STORE " + var_memory_index);
        cout << "ZAPISUJE POD ADRESEM: " + var_memory_index << endl;
    }

    Variable var = get_variable_from_mem_index(var_memory_index);
    var.inicialized = true;
    update_variables(var);
}

void __write(string var, int yylineno)
{
    if (var == to_string(oneIndex) || var == to_string(minusOneIndex))
    {
        add_command("LOAD " + var);
        cout << "LOADING AND PRINTING 1 OR -1" << endl;
        add_command("PUT");
        return;
    }

    Variable var_to_assign = get_variable_from_mem_index(var);

    if (!var_to_assign.inicialized)
    {
        error(var_to_assign.name, yylineno, "Variable was not inicialized");
    }

    long long int var_memory_index = stoll(var);

    if (find(array_index_variables.begin(), array_index_variables.end(), var_memory_index) != array_index_variables.end())
    {
        // DEBUG
        add_command("LOADI " + var);
        cout << "LOADING I AND PRINTING " + var_to_assign.name << endl;
    }
    else
    {
        // DEBUG
        add_command("LOAD " + var);
        cout << "LOADING AND PRINTING " + var_to_assign.name << endl;
    }

    add_command("PUT");
}

///////////////////////////
//      ARITHMETICS      //
///////////////////////////

char *__add(string var1_address, string var2_address, int yylineno)
{

    check_both_variables_inicialized(var1_address, var2_address, yylineno);
    // DEBUG

    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    cout << var1.name + " PLUS " + var2.name << endl;

    if (var1.type == 6)
    {
        get_value_of_array_with_variable(var1.memory_index);
    }
    if (var2.type == 6)
    {
        get_value_of_array_with_variable(var2.memory_index);
    }

    add_command("LOAD " + var1_address);
    add_command("ADD " + var2_address);

    return parseStringToChar(var1_address);
}

char *__sub(string var1_address, string var2_address, int yylineno)
{

    check_both_variables_inicialized(var1_address, var2_address, yylineno);
    // DEBUG
    // cout << "SUB " + var1_address + " " + var2_address << endl;
    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    if (var1.type == 6)
    {
        get_value_of_array_with_variable(var1.memory_index);
    }
    if (var2.type == 6)
    {
        get_value_of_array_with_variable(var2.memory_index);
    }

    // DEBUG
    cout << var1.name + " MINUS " + var2.name << endl;

    add_command("LOAD " + var1_address);
    add_command("SUB " + var2_address);

    return parseStringToChar(var1_address);
}

char *__mul(string var1_address, string var2_address, int yylineno)
{

    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    cout << var1.name + " TIMES " + var2.name << endl;

    check_both_variables_inicialized(var1_address, var2_address, yylineno);

    if (var1.type == 6)
    {
        get_value_of_array_with_variable(var1.memory_index);
    }
    if (var2.type == 6)
    {
        get_value_of_array_with_variable(var2.memory_index);
    }

    if (var1.type == 1 && var2.type == 1)
    {
        long long int var1_value = get_value_of_variable_from_memory_index(var1_address);
        long long int var2_value = get_value_of_variable_from_memory_index(var2_address);

        // DEBUG
        cout << "CREATE RESULT OF MUL " + to_string(var1_value) + " " + to_string(var2_value) << endl;

        create_number(var1_value * var2_value, getMemory());
        // add_command("STORE " + to_string(memoryCounter + 1));

        return parseStringToChar(var1_address);
    }

    // DEBUG
    // cout << "MUL " + var1_address + " " + var2_address << endl;

    string num1_adress = var1_address;
    string num2_adress = var2_address;

    string num1_temp = to_string(getMemory());
    string num2_temp = to_string(getMemory());
    string flag = to_string(getMemory());
    string result = to_string(getMemory()); //czy result mnożenia nie powienien być zapisany na stałe? (get_memory())
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

    return parseStringToChar(var1_address);
}

char *__div(string var1_address, string var2_address, int yylineno)
{
    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    cout << var1.name + " DIV " + var2.name << endl;

    if (var1.type == 6)
    {
        get_value_of_array_with_variable(var1.memory_index);
    }
    if (var2.type == 6)
    {
        get_value_of_array_with_variable(var2.memory_index);
    }

    check_both_variables_inicialized(var1_address, var2_address, yylineno);

    int var1_type = check_variable_type(var1_address);
    int var2_type = check_variable_type(var2_address);

    if (var1_type == 1 && var2_type == 1)
    {
        long long int var1_value = get_value_of_variable_from_memory_index(var1_address);
        long long int var2_value = get_value_of_variable_from_memory_index(var2_address);

        // DEBUG
        // display_command("CREATE")
        cout << "CREATE RESULT OF DIV " + to_string(var1_value) + " " + to_string(var2_value) << endl;

        create_number(var1_value / var2_value, getMemory());
        // add_command("STORE " + to_string(memoryCounter + 1));

        return parseStringToChar(var1_address);
    }

    // DEBUG
    // display_command(56,"DIV " + var1_address + " " + var2_address);
    // cout << "DIV " + var1_address + " " + var2_address << endl;

    string num1_adress = var1_address;
    string num2_adress = var2_address;

    string num1_temp = to_string(getMemory());
    string num2_temp = to_string(getMemory());
    string flag = to_string(getMemory());
    string result = to_string(getMemory());
    string multiple = to_string(getMemory());
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

    return parseStringToChar(var1_address);
}

char *__mod(string var1_address, string var2_address, int yylineno)
{
    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    cout << var1.name + " MOD " + var2.name << endl;

    if (var1.type == 6)
    {
        get_value_of_array_with_variable(var1.memory_index);
    }
    if (var2.type == 6)
    {
        get_value_of_array_with_variable(var2.memory_index);
    }

    check_both_variables_inicialized(var1_address, var2_address, yylineno);
    // DEBUG
    // display_command(69, "MOD " + var1_address + " " + var2_address);

    string num1_adress = var1_address;
    string num2_adress = var2_address;
    string num1_temp = to_string(getMemory());
    string num2_temp = to_string(getMemory());
    string result = to_string(getMemory());
    string multiple = to_string(getMemory());
    string one = to_string(getMemory());
    string minus_one = to_string(getMemory());
    string num1_sign = to_string(getMemory());
    string num2_sign = to_string(getMemory());
    string remainder = to_string(getMemory());

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

    return parseStringToChar(var1_address);
}

///////////////////////////
//      IF/ELSE IF       //
///////////////////////////

void __end_if()
{
    cout << "IF: DODAJE SKOK NA POCZĄTEK IFA ->  " + asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index) << " W LINI " << cmd_index_for_jumps.top() << endl;
    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index);
    cmd_index_for_jumps.pop();
}

void __if_else()
{
    //display_comma(1, "IF ELSE");
    cout << "ELSE IF: NOW ADDING " + asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index + 1) << endl;
    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index + 1);
    cmd_index_for_jumps.pop();

    add_command("JUMP ");
    create_jump();
}

void __end_if_else()
{
    cout << "END ELSE IF: NOW ADDING " + asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index) << endl;
    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index);
    cmd_index_for_jumps.pop();
}

///////////////////////////
//          LOOPS        //
///////////////////////////

void __loop_for(string variable, string from, string to, bool isTo, int yylineno)
{
    //display_comma(4, "START FOR LOOP");
    check_both_variables_inicialized(from, to, yylineno);

    long long int it_mem = getMemory();
    Iterator it = {variable, it_mem};
    loop_iterators.push(it);
    // memoryCounter ++;
    string max_iterator = to_string(getMemory());

    bool var_created = create_variable(it.name, 5, it.memory_index, 1, 1, 1, 1);
    create_variable("MAXIT" + max_iterator, 5, stoll(max_iterator), 1, 1, 1, true);

    cout << "ZACZYNAM FOR LOOP W LINI " + to_string(cmd_index) << " I TWORZĘ ITERATOR " + it.name + " POD ADRESEM: " + to_string(it.memory_index) << endl;

    if (!var_created)
    {
        error(variable, yylineno, "Iterator declared before");
    }

    add_command("LOAD " + to);
    add_command("STORE " + max_iterator);
    add_command("LOAD " + from);

    
    if (isTo)
    {
        // add_command("INC");
        add_command("STORE " + to_string(it.memory_index));
        create_jump();

        add_command("LOAD " + max_iterator);
        add_command("SUB " + to_string(it.memory_index));
        add_command("JNEG ");
        create_jump();

    }
    else
    {
        // add_command("INC");
        add_command("STORE " + to_string(it.memory_index));
        create_jump();
        add_command("LOAD " + to_string(it.memory_index));
        add_command("SUB " + max_iterator);
        add_command("JNEG ");
        create_jump();
    }
}

void __end_loop_for(bool isTo)
{
    // display_command(1, "END DO WHILE LOOP");

    cout << "KOŃCZĘ FOR LOOP W LINI " << cmd_index << endl;
    // UPDATE ITERATOR DEPENDING ON LOOP TYPE DEC OR INC
    add_command("LOAD " + to_string(loop_iterators.top().memory_index));
    if (isTo)
    {
        add_command("INC");
    }
    else
    {
        add_command("DEC");
    }

    add_command("STORE " + to_string(loop_iterators.top().memory_index));

    // ASSIGN JPOS FROM __LE TO JUMP HERE (LOOP END)
    // cout << "FOR: ADDING VALUE TO JUMP TO LEAVE FOR -> " + asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index + 1) << endl;
    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index + 1);
    cmd_index_for_jumps.pop();

    // ASSIGN JUMP TO LOOP START
    // cout << "FOR: ADDING JUMP TO LOOP START -> JUMP " + to_string(cmd_index_for_jumps.top() - 1) << endl;
    add_command("JUMP " + to_string(cmd_index_for_jumps.top()));
    cmd_index_for_jumps.pop();

    // REMOVE ITERATOR
    variables.erase(loop_iterators.top().name);
    loop_iterators.pop();
}

void __loop_while(string condition_length)
{
    cout << "ZACZYNAM PĘTLE WHILE " << cmd_index << " WARUNEK JEST W LINI " << cmd_index - stoi(condition_length) << endl;
    int cond_length = stoi(condition_length);
    cmd_index_for_jumps.push(cmd_index - cond_length);
}

void __end_loop_while()
{

    add_command("JUMP " + to_string(cmd_index_for_jumps.top()));
    cmd_index_for_jumps.pop();

    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index);
    cmd_index_for_jumps.pop();
    cout << "KOŃCZĘ WHILE" << cmd_index << " DODAJE SKOK NA POCZĄTEK PĘTLI " + asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index) << " W LINI " << cmd_index_for_jumps.top() << endl;
}

void __loop_do_while()
{
    create_jump();
}

void __end_loop_do_while()
{
    // display_command(1, "END DO WHILE LOOP");
    // cout << "DO WHILE: ADDING VALUE TO JUMP TO LEAVE WHILE -> " + asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index) << endl;
    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index);
    cmd_index_for_jumps.pop();
    add_command("JUMP " + to_string(cmd_index_for_jumps.top()));
    cmd_index_for_jumps.pop();
}

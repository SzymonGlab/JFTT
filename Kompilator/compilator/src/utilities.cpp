#include "compilator.hpp"

///////////////////////////
//        UTILITIES      //
///////////////////////////

char *parseStringToChar(string str)
{
    const char *iStr = str.c_str();
    long long int newSize = strlen(iStr) + 1;
    char *newName = (char *)malloc(newSize);
    strcpy(newName, iStr);
    return newName;
}

long long get_value_of_variable_from_memory_index(string var_memory_index)
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

void create_ASM(char *fileName)
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

    add_command("SUB 0");

    if (number < 300)
    {
        for (int i = 0; i < number; i++)
        {
            add_command("INC");
        }
        add_command("STORE " + to_string(mem_index));
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
    }
    if (negative)
        {
            add_command("SUB 0");
            add_command("SUB " + to_string(mem_index));
            add_command("STORE " + to_string(mem_index));
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

 void debug(string str) {
        cout << str << endl;
}


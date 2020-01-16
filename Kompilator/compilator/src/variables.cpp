#include "compilator.hpp"

///////////////////////////
//       VARIABLES       //
///////////////////////////

void __declare_variable(string name, int yylineno)
{
    bool var_created = create_variable(name, 2, getMemory(), 1, 1, 1, false);

    if (!var_created)
    {
        error(name, yylineno - 1, "Second declaration");
    }
}

void __declare_array(string name, string start, string end, int yylieno)
{
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
}

void __assign_variable(string mem_index, string variable_to_assign, int yylineno)
{
    Variable var_to_assign = get_variable_from_mem_index(variable_to_assign);

    if (!var_to_assign.inicialized)
    {
        error(var_to_assign.name, yylineno, "Varibales was not assigned");
    }

    Variable var_assigned = get_variable_from_mem_index(mem_index);
    var_assigned.inicialized = true;

    if(var_to_assign.type == 5){
        error(var_to_assign.name, yylineno, "Wrong use of iterator variable");
    }


    // if (var_to_assign.type == 6){
    //  var_assigned.type = 6;
    // }

    update_variables(var_assigned);

    

    if (var_assigned.type == 6)
    {
        add_command("STOREI " + mem_index);
    }
    else
    {
        add_command("STORE " + mem_index);
    }
}

char *__create_number(string value)
{
    if (stoll(value) == 1)
    {
        add_command("LOAD " + to_string(oneIndex));
        return parseStringToChar(to_string(oneIndex));
    }

    if (stoll(value) == -1)
    {
        add_command("LOAD " + to_string(minusOneIndex));
        return parseStringToChar(to_string(minusOneIndex));
    }

    // IF WE CREATED THIS VARIABLE (IN DEED CONST  E.G. 1090) BEFORE
    if (variables.find(value) != variables.end())
    {
        add_command("LOAD " + to_string(variables.at(value).memory_index));
        return parseStringToChar(to_string(variables.at(value).memory_index));
    }
    // IF VARIABLE (IN DEED CONST E.G. 1090) WASN'T CREATED BEFORE
    else
    {
        long long int memory = getMemory();
        long long int var_value = stoll(value);
        create_variable(value, 1, memory, 1, 1, 1, true);
        create_number(var_value, memory);
        char *ret = parseStringToChar(to_string(memory));
        return parseStringToChar(to_string(memory));
    }
}

char *__read_variable(string var_name, int yylineno)
{
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

    Variable arr = variables.at(array_name);

    if (arr.type != 4)
    {
        error(array_name, yylineno, "Wrong use of variable");
    }

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
            create_variable(array_name + index, 3, arr.memory_index - arr.start + stoll(index) + 1, 1, 1, 1, true);
        }
        else
        {
            create_variable(array_name + index, 3, arr.memory_index + stoll(index) + 1, 1, 1, 1, true);
        }

        var = variables.at(array_name + index);
    }

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

    add_command("LOAD " + to_string(index_variable.memory_index));
    add_command("ADD " + to_string(tempVariable));
    add_command("STORE " + to_string(newVariableMemory));
    add_command("LOADI " + to_string(newVariableMemory));

    array_index_variables.push_back(newVariableMemory);
    create_variable("ARRAY_INDEX" + to_string(newVariableMemory), 6, newVariableMemory, 1, 1, 1, true);

    return parseStringToChar(to_string(newVariableMemory));
}

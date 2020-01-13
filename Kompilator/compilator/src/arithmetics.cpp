#include "compilator.hpp"

///////////////////////////
//      ARITHMETICS      //
///////////////////////////

char *__add(string var1_address, string var2_address, int yylineno)
{
    // DEBUG
    debug("PLUS");

    check_both_variables_inicialized(var1_address, var2_address, yylineno);

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

    add_command("LOAD " + var1_address);
    add_command("ADD " + var2_address);

    return parseStringToChar(var1_address);
}

char *__sub(string var1_address, string var2_address, int yylineno)
{
    // DEBUG
    debug("MINUS");

    check_both_variables_inicialized(var1_address, var2_address, yylineno);

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

    add_command("LOAD " + var1_address);
    add_command("SUB " + var2_address);

    return parseStringToChar(var1_address);
}

char *__mul(string var1_address, string var2_address, int yylineno)
{

    // DEBUG
    debug("MINUS");

    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

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

        create_number(var1_value * var2_value, getMemory());

        return parseStringToChar(var1_address);
    }

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
    // DEBUG
    debug("DIV");

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

    check_both_variables_inicialized(var1_address, var2_address, yylineno);

    int var1_type = check_variable_type(var1_address);
    int var2_type = check_variable_type(var2_address);

    if (var1_type == 1 && var2_type == 1)
    {
        long long int var1_value = get_value_of_variable_from_memory_index(var1_address);
        long long int var2_value = get_value_of_variable_from_memory_index(var2_address);

        create_number(var1_value / var2_value, getMemory());
        return parseStringToChar(var1_address);
    }

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
    // DEBUG
    debug("MOD");

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

    check_both_variables_inicialized(var1_address, var2_address, yylineno);

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
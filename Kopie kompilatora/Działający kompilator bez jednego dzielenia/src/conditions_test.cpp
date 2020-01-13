#include "compilator.hpp"

///////////////////////////
//      IF/ELSE IF       //
///////////////////////////

void __end_if()
{
    // DEBUG
    debug("END IF");

    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index);
    cmd_index_for_jumps.pop();
}

void __if_else()
{
    // DEBUG
    debug("IF ELSE");

    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index + 1);
    cmd_index_for_jumps.pop();

    add_command("JUMP ");
    create_jump();
}

void __end_if_else()
{
    // DEBUG
    debug("END IF ELSE");

    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index);
    cmd_index_for_jumps.pop();
}

///////////////////////////
//      CONDITIONS       //
///////////////////////////

char *__EQ(string var1_address, string var2_address)
{
    Variable var1 = get_variable_from_mem_index(var1_address);
    Variable var2 = get_variable_from_mem_index(var2_address);

    // DEBUG
    debug("==");

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
    debug("!=");

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
    debug(">");

    add_command("LOAD " + var1_address);
    add_command("SUB " + var2_address);
    add_command("JNEG "+ to_string(cmd_index + 3) );
    add_command("JZERO " + to_string(cmd_index +2));
    add_command("JUMP " + to_string(cmd_index +2));
    add_command("JUMP ");
    create_jump();

    return parseStringToChar(to_string(6));
}

char *__LE(string var1_address, string var2_address)
{
    // DEBUG
    debug("<");

    add_command("LOAD " + var1_address);
    add_command("SUB " + var2_address);
    add_command("JPOS "+ to_string(cmd_index + 3) );
    add_command("JZERO " + to_string(cmd_index +2));
    add_command("JUMP " + to_string(cmd_index +2));
    add_command("JUMP ");
    create_jump();

    return parseStringToChar(to_string(6));
}

char *__GEQ(string var1_address, string var2_address)
{
    // DEBUG
    debug(">=");

    add_command("LOAD " + var1_address);
    add_command("SUB " + var2_address);
    add_command("JPOS " );
    create_jump();

    return parseStringToChar(to_string(3));
}

char *__LEQ(string var1_address, string var2_address)
{
    // DEBUG
    debug("<=");

    add_command("LOAD " + var1_address);
    add_command("SUB " + var2_address);
    add_command("JPOS " );
    create_jump();
    return parseStringToChar(to_string(3));
}
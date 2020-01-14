#include "compilator.hpp"

///////////////////////////
//      IN / OUT         //
///////////////////////////

void __read(string var_address)
{
      add_command("GET");

    if (find(array_index_variables.begin(), array_index_variables.end(), stoll(var_address)) != array_index_variables.end())
    {
        add_command("STOREI " + var_address);
    }
    else
    {
        add_command("STORE " + var_address);
    }

    Variable var = get_variable_from_mem_index(var_address);
    var.inicialized = true;
    update_variables(var);
}

void __write(string var_address, int yylineno)
{

    if (var_address == to_string(oneIndex) || var_address == to_string(minusOneIndex))
    {
        add_command("LOAD " + var_address);
        add_command("PUT");
        return;
    }

    Variable var_to_write = get_variable_from_mem_index(var_address);

    if (!var_to_write.inicialized)
    {
        error(var_to_write.name, yylineno, "Variable was not inicialized");
    }

    if (var_to_write.type == 6)
    {
        add_command("LOADI " + var_address);
    }
    else
    {
        add_command("LOAD " + var_address);
    }

    add_command("PUT");
}

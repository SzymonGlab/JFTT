#include "compilator.hpp"

///////////////////////////
//      IN / OUT         //
///////////////////////////

void __read(string var_memory_index)
{
    // DEBUG
    debug("READ");

    add_command("GET");

    if (find(array_index_variables.begin(), array_index_variables.end(), stoll(var_memory_index)) != array_index_variables.end())
    {
        add_command("STOREI " + var_memory_index);
    }
    else
    {
        add_command("STORE " + var_memory_index);
    }

    Variable var = get_variable_from_mem_index(var_memory_index);
    var.inicialized = true;
    update_variables(var);
}

void __write(string var, int yylineno)
{
    // DEBUG
    debug("WRITING");

    if (var == to_string(oneIndex) || var == to_string(minusOneIndex))
    {
        add_command("LOAD " + var);
        add_command("PUT");
        return;
    }

    Variable var_to_write = get_variable_from_mem_index(var);

    if (!var_to_write.inicialized)
    {
        error(var_to_write.name, yylineno, "Variable was not inicialized");
    }

    long long int var_memory_index = stoll(var);

    if (var_to_write.name == "tab")
    {
        cout << "VAR = "<<var<<endl;
        cout << "var_memory_index = " << to_string(var_memory_index) << endl; 
    }

    if (var_to_write.type == 6)
    {
        add_command("LOADI " + to_string(var_memory_index));
    }
    else
    {
        add_command("LOAD " + var);
    }

    add_command("PUT");
}

#include "compilator.hpp"


///////////////////////////
//          LOOPS        //
///////////////////////////

void __loop_for(string iterator, string from, string to, bool isTo, int yylineno)
{
    
    check_both_variables_inicialized(from, to, yylineno);

    Variable var1 = get_variable_from_mem_index(from);
    Variable var2 = get_variable_from_mem_index(to);


    if (var1.type == 6)
    {
        get_value_of_array_with_variable(var1.memory_index);
    }
    if (var2.type == 6)
    {
        get_value_of_array_with_variable(var2.memory_index);
    }


    long long int it_mem = getMemory();
    Iterator it = {iterator, it_mem};
    loop_iterators.push(it);
    string max_iterator = to_string(getMemory());

    bool var_created = create_variable(it.name, 5, it.memory_index, 1, 1, 1, 1);
    create_variable("MAXIT" + max_iterator, 5, stoll(max_iterator), 1, 1, 1, true);

    if (!var_created)
    {
        error(iterator, yylineno, "Iterator declared before");
    }

    add_command("LOAD " + from);
    add_command("STORE " + to_string(it.memory_index));
    add_command("LOAD " + to);
    add_command("STORE " + max_iterator);
    
    create_jump();

    
    if (isTo)
    {
        __LEQ(to_string(it.memory_index),max_iterator);
    }
    else
    {
        __GEQ(to_string(it.memory_index),max_iterator);
    }
}

void __end_loop_for(bool isTo)
{

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
    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index + 1);
    cmd_index_for_jumps.pop();

    // ASSIGN JUMP TO LOOP START
    add_command("JUMP " + to_string(cmd_index_for_jumps.top()));
    cmd_index_for_jumps.pop();

    // REMOVE ITERATOR
    variables.erase(loop_iterators.top().name);
    loop_iterators.pop();
}

void __loop_while(string condition_length)
{
    int cond_length = stoi(condition_length);
    cmd_index_for_jumps.push(cmd_index - cond_length);
}

void __end_loop_while()
{
  
    add_command("JUMP " + to_string(cmd_index_for_jumps.top()));
    cmd_index_for_jumps.pop();

    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index);
    cmd_index_for_jumps.pop();
}

void __loop_do_while()
{
    create_jump();
        
}

void __end_loop_do_while()
{
    asmCode.at(cmd_index_for_jumps.top() - 1) = asmCode.at(cmd_index_for_jumps.top() - 1) + to_string(cmd_index+1);
    cmd_index_for_jumps.pop();
    add_command("JUMP " + to_string(cmd_index_for_jumps.top()));
    cmd_index_for_jumps.pop();
}

#include "compilator.hpp"

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

void error(string a, int yylineno, string msg)
{
    usleep(500000);
    cout << "\e[1m\x1B[31m[ ERROR ]\e[0m \e[1m[ LINE " << yylineno << " ] \e[1m\x1B[31m" << msg << " " << a << ".\e[0m\n"
         << endl;
    exit(1);
}

void __end()
{
    if (notInicializedVariables.size() > 0)
    {

        for (auto const &[key, val] : notInicializedVariables)
        {
            if (val.used)
            {
                error(notInicializedVariables.begin()->first, notInicializedVariables.begin()->second.line, "Variable not inicialized");
            }
        }
    }

    add_command("HALT");
}

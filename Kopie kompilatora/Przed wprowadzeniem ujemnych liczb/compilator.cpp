#include "compilator.hpp"

map<string, Variable> variables; 
vector <string> asmCode;
int memoryCounter = 1;
int cmd_index = 0;

///////////////////////////
//         MEMORY        //
///////////////////////////

int getMemory(){
       return memoryCounter++;
    }

void freeMemory(){
    memoryCounter --;
    }

Variable getVariable(string name){
    return variables[name];
    }


///////////////////////////
//        UTILITIES      //
///////////////////////////


char* concat_chars(char* first, char* second){
    int newSize = strlen(first)  + strlen(second) + 1; 

    char * newBuffer = (char *)malloc(newSize);
   
    strcpy(newBuffer,first);
    strcat(newBuffer,second);
    free(first);
    return newBuffer;
}

void add_command(string command){
    asmCode.push_back(command);
    cmd_index++;
    }

void print_ASM(){
    for(auto i : asmCode){
        cout << i << endl;
    }
    }

void create_ASM(const char* fileName){
  ofstream code;
  code.open(fileName);
  for(auto i : asmCode){
        code << i << endl;
    }
  code.close();
    }


bool check_variable_exist(string name){
    if ( variables.find(name) == variables.end() ) {
        return false;
    } else {
        return true;
    }
}

void create_number(int number, int mem_index){
    bool negative = false;
    int temp_var = getMemory();
    int counter = 0;
    int current_number = 2;

    if(number == 0){
        add_command("SUB 0");
        add_command("STORE " + to_string(mem_index));
        return;
    } else {
        if(number <  0){
            negative = true;
            number  = 0 - number;
        }
        add_command("SUB 0");
        add_command("INC");
        add_command("STORE " + to_string(temp_var));

        while(current_number <= number){
            current_number += current_number;
            counter ++;
            add_command("SHIFT " + to_string(temp_var));
        }
        
        current_number = current_number/2;

        while(current_number < number){
            current_number ++;
            add_command("INC");
        }

        add_command("STORE " + to_string(mem_index));

        if(negative){
            add_command("SUB 0");
            add_command("SUB " + to_string(mem_index));
            add_command("STORE " + to_string(mem_index));
        }
    }
    freeMemory();
    }

///////////////////////////
//       VARIABLES       //
///////////////////////////


void __declare_variable(string name, string variable_type){
    cout << "DECLARE VARIABLE " + name + " at " + to_string(memoryCounter) << endl;
    Variable var = {name,stoi(variable_type),memoryCounter,1,1,1};
    variables.insert(make_pair(name,var));
    memoryCounter++;
    }

void __declare_array(string name, string start, string end){
    cout << "DECLARE ARRAY " + name + " at " + to_string(memoryCounter) << endl;
    int size = stoi(end) - stoi(start);
    if(size <= 0 ){
        // UJEMNA TABLICA ERROR
    }
    Variable var = {name, memoryCounter, size, stoi(start), stoi(end)};
    variables.insert(make_pair(name, var));
    // POTRZEBNY JEST DODATKOWY ELEMENT
    memoryCounter += size+1;
    }

void __assign_variable(string name){
    cout << "ASSIGN VARIABLE " + name<< endl;
    Variable var = variables.at(name);
    add_command("STORE " + to_string(var.memory_index));
    }

void __create_temp_variable(string name, string value){
    int memory = getMemory();
    cout << "CREATIGN TEMP VARIABLE " + name + " at " + to_string(memory)<< endl;
    Variable var = {name,2,memory,1,1,1};
    int var_value = stoi(value);
    variables.insert(make_pair(name,var));
    create_number(var_value, var.memory_index);
    }

void __read_variable_from_arrayNUM(string array_name, string index){
    Variable var = variables.at(array_name);
    Variable new_var = {array_name+index,1,var.memory_index + stoi(index)- var.start + 1,1,1,1};
    cout << "CREATING VARIABLE " + array_name + index + " at " + to_string(var.memory_index + stoi(index) - var.start + 1) << endl;
    variables.insert(make_pair(array_name+index,new_var));
}

void __read_variable_from_arrayVAR(string array_name, string index){
    // Variable array = variables.at(array_name);
    // Variable index = variables.at(index);
    // Variable new_var = {array_name+index,1,array.memory_index + stoi(index) - array.start + 1,1,1,1};


    // cout << "READIGN VARIABLE FROM ARRAY " + array_name + " AT INDEX = " + index << endl;
}

void __end(){
    add_command("PUT");
    add_command("HALT");
    create_ASM("pierwszy_program.txt");
    }

///////////////////////////
//      CONDITIONS       //
///////////////////////////

void __EQ(string var1, string var2){
    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);
    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
    add_command("LOAD " + num2_adress);
    add_command("SUB " + num1_adress);
    add_command("JZERO " + to_string(cmd_index+3));
    add_command("SUB 0");
    add_command("INC");
    
}

void __NEQ(string var1, string var2){
    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);
    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
    add_command("LOAD " + num2_adress);
    add_command("SUB " + num1_adress);
    add_command("JZERO " + to_string(cmd_index+3));
    add_command("SUB 0");
    add_command("JUMP " + to_string(cmd_index+2));
    add_command("INC ");    
}

void __GE(string var1, string var2){
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

void __LE(string var1, string var2){
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

void __GEQ(string var1, string var2){
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

void __LEQ(string var1, string var2){
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

// void __read(string var, string value){
//     Variable variable = getVariable(var);

// }

void __write(string var){
    Variable variable = getVariable(var);
    add_command("LOAD " + to_string(variable.memory_index));
    add_command("PUT");
}


///////////////////////////
//      ARITHMETICS      //
///////////////////////////


void __add(string var1, string var2){
    cout << "ADD " + var1 + " " + var2 << endl;

    int result = 0;
    Variable variable1;
    Variable variable2;
    variable1 = getVariable(var1);
    variable2 = getVariable(var2);

    add_command("LOAD " + to_string(variable2.memory_index));
    add_command("ADD " + to_string(variable1.memory_index));
    
    if(variable1.type == 2){
        //remove variable 1
    }

    if(variable2.type == 2){
        //remove variable 2
    }
    }

void __sub(string var1, string var2){
    cout << "SUB " + var1 + " " + var2 << endl;
    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);
    add_command("LOAD " + to_string(variable1.memory_index));
    add_command("SUB " + to_string(variable2.memory_index));
    }

void __mul(string var1, string var2){
    cout << "MUL " + var1 + " " + var2 << endl;
    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);

    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
    string num1_temp = to_string(getMemory());
    string num2_temp = to_string(getMemory());
    string flag = to_string(getMemory());
    string result = to_string(getMemory());
    string one = to_string(getMemory());
    string minus_one = to_string(getMemory());

    add_command("SUB 0");
    add_command("STORE "+ result);
    add_command("STORE "+ flag);
    add_command("DEC");
    add_command("STORE " + minus_one);
    add_command("INC");
    add_command("INC");
    add_command("STORE " + one);
    add_command("LOAD " + num1_adress);
    add_command("STORE " + num1_temp);
    add_command("LOAD " + num2_adress);
    add_command("STORE " + num2_temp);
    add_command("JPOS " + to_string(cmd_index+8));
    add_command("SUB 0");
    add_command("SUB " + num2_adress);
    add_command("STORE " + num2_temp);
    add_command("LOAD " + flag);
    add_command("INC");
    add_command("STORE " + flag);
    add_command("LOAD " + num2_temp);
    add_command("JZERO " + to_string(cmd_index+15));
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
    add_command("JUMP " + to_string(cmd_index-14));
    add_command("LOAD " + flag);
    add_command("JZERO " + to_string(cmd_index+5));
    add_command("SUB 0");
    add_command("SUB " + result);
    add_command("STORE " + result);
    add_command("JUMP " + to_string(cmd_index + 2));
    add_command("LOAD " + result);

    freeMemory();
    freeMemory();
    freeMemory();
    freeMemory();
    }

void __div(string var1, string var2){
    cout << "DIV " + var1 + " " + var2 << endl;
    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);

    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);

    string num1_temp = to_string(getMemory());
    string num2_temp = to_string(getMemory()); 
    string flag = to_string(getMemory());
    string result = to_string(getMemory());
    string multiple = to_string(getMemory());
    string one = to_string(getMemory());
    string minus_one = to_string(getMemory());

    add_command("SUB 0");
    add_command("STORE "+ flag);
    add_command("STORE " + result);
    add_command("INC");
    add_command("STORE " + multiple);
    add_command("STORE " + one);
    add_command("DEC");
    add_command("DEC");
    add_command("STORE " + minus_one);
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

    freeMemory();
    freeMemory();
    freeMemory();
    freeMemory();
    freeMemory();
    }

void __mod(string var1, string var2){
    cout << "MOD " + var1 + " " + var2 << endl;
    Variable variable1 = getVariable(var1);
    Variable variable2 = getVariable(var2);

    string num1_adress = to_string(variable1.memory_index);
    string num2_adress = to_string(variable2.memory_index);
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
    add_command("STORE "+ num1_sign);
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

    freeMemory();
    freeMemory();
    freeMemory();
    freeMemory();
    freeMemory();
    }


///////////////////////////
//         DEBUG         //
///////////////////////////


// int main(int argc, char const *argv[])
// {
//     __declare_variable("x");
//     __declare_variable("y");
//     __assign_variable("x", -13);
//     __assign_variable("y", 3);
    
//     __add("x","y");
//     add_command("PUT");
//     __sub("x","y");
//     add_command("PUT");
//     __mul("x","y");
//     add_command("PUT");
//     __div("x","y");
//     add_command("PUT");
//     __mod("x","y");
//     __end();
//     print_ASM();
//     return 0;
// }

%{
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

#define stack_size 100

int yylex();
int yywrap();

bool error = false;
int ptr = -1;
int stack[stack_size];
bool stack_err = false; 

void push(int num) {
    ptr++;
    if (ptr < stack_size) {
        stack[ptr] = num;
    } else {
        fprintf(stderr, "Whoops!");
        stack_err = true;
    } 
}

int pop()  {
    if (ptr >= 0) {
        ptr--;
        return stack[ptr + 1];
    } else {
        printf("Błąd zbyt mało argumentów.\n");
        stack_err = true;
        return 0;
    }
}

%}

%x ERROR

%%
-?[0-9]+        push(atoi(yytext));
\+            {
                    push(pop()+pop());
                    
                    if (stack_err) BEGIN(ERROR);
                }
\-             {
                    int a = pop();
                    int b = pop();
                    push(b - a);
                    if (stack_err) BEGIN(ERROR);
                }
\*             {
                    push(pop()*pop());
                    if (stack_err) BEGIN(ERROR);
                }
\/             {
                    int divisor = pop();
                    if (divisor == 0) {
                        printf("Błąd: nie dziel przez 0!\n");
                        BEGIN(ERROR);
                    } else {
                        push(pop() / divisor);
                    }
                    if (stack_err) BEGIN(ERROR);
                }
\%             {
                    int divisor = pop();
                    if (divisor == 0) {
                        printf("Błąd: nie dziel przez 0!\n");
                        BEGIN(ERROR);
                    } else {
                        push(pop() % divisor);
                    }
                    if (stack_err) BEGIN(ERROR);
                }

\^             { push( (int) pow((double) pop(), (double) pop()));}  

\n            {       
                    if(!error){
                        if (ptr == 0) {
                        printf("= %d\n", pop());
                    } else if(ptr > 0 ){
                        printf("Błąd: zła ilość operatorów.\n");
                    }
                    } else {
                        printf("Za mało argumentów");
                        error = false;
                    }
                    ptr = -1;
                    
                }
[^[:blank:]]    {printf("Błąd: zły symbol %s\n", yytext); error = true;};
[[:blank:]]     ;

<ERROR>.*       ;
<ERROR>\n       {
                    ptr = -1;
                    stack_err = false;
                    BEGIN(INITIAL);
                }
%%

int yywrap() {
    return 1;
}

int main() {
    yylex();
    return 0;
}
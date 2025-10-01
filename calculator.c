#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int integerStack[40]; // creating stack for integer operands
int intStackTop = -1; // top for integer operand stack
char operatorStack[15]; // creating stack for operators
int optStackTop = -1; // top for operator stack

void push_intStack(int value) {  // push for integer operand stack
    intStackTop += 1;
    integerStack[intStackTop] = value;
}
void push_optStack(char value) { // push for operator stack
    optStackTop += 1;
    operatorStack[optStackTop] = value;
}

int pop_intStack() { //pop for integer operand stack
    int value = integerStack[intStackTop];
    intStackTop -= 1;
    return value;
}
char pop_optStack() { // pop for operator stack
    char value = operatorStack[optStackTop];
    optStackTop -= 1;
    return value;
}

int priority(char operator) { //function for determining the priority of operator
    if (operator == '+' || operator == '-')
        return 1;
    return 2;
}


int calculate(int operand1, char operator, int operand2) { // function for calculating the operator oprations
    switch (operator) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '/': return (operand2 == 0) ? -9999 : operand1 / operand2;
        case '*': return operand1 * operand2;
        default: return -9997;
    }
}

int evaluation(char exp[]) { // main logic function for converting given expression in to valid and evaluating expression
    char digit[10];
    int Indx;
    char *e = exp;
    while (*e != '\0') {
        if (isdigit(*e)) { //check if character is digit or not left - right
            Indx = 0;
            while (isdigit(*e)) {
                digit[Indx++] = *e;
                e++;
            }
            digit[Indx] = '\0';
            push_intStack(atoi(digit)); // converting string to int and push in the stack
            continue;
        }
        else if (isalpha(*e)) { // validate if expression consist alphabetic character or not left - right
            return -9998; // error flag for invalid character
        }
        else if (*e == '+' || *e == '-' || *e == '*' || *e == '/') { // check the operator from left - right
            while (optStackTop != -1 && priority(operatorStack[optStackTop]) >= priority(*e)) { // execute according to precedence and associativity
                char operator = pop_optStack();
                int operand2 = pop_intStack();
                int operand1 = pop_intStack();
                int result = calculate(operand1, operator, operand2);
                if (result == -9999) return -9999; // error flag for division by zero
                if (result == -9997) return -9997; // error flag for operator or symbol other than +.-,*,/
                push_intStack(result);
            }
            push_optStack(*e);
        }
        else if (isspace(*e)) { // ignore the space
            e++;
            continue;
        }
        else {
            return -9997; // error flag for invalid symbol
        }
        e++;
    }
    while (optStackTop != -1) { // complete the calculation after complete left - right scaning
        int operand2 = pop_intStack();
        char operator = pop_optStack();
        int operand1 = pop_intStack();
        int result = calculate(operand1, operator, operand2);
        if (result == -9999) return -9999;
        if (result == -9997) return -9997;
        push_intStack(result);
    }
    return pop_intStack(); // final result
}
int main(void) {
    char exp[30];
    printf("Enter the expression: ");
    fgets(exp, sizeof(exp), stdin); // using fgets() for multiple words
    int result = evaluation(exp);
    if (result == -9998 || result == -9997)
        printf("Error: contains invalid characters");
    else if (result == -9999)
        printf("Error: Division by zero");
    else
        printf("The result is: %d\n", result);
    return 0;
}
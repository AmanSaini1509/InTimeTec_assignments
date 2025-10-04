#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int integerStack[40]; // creating stack for integer operands
int intStackTop = -1; // top for integer operand stack

char operatorStack[15]; // creating stack for operators
int optStackTop = -1; // top for operator stack

void pushIntStack(int value) {  // push for integer operand stack
    intStackTop += 1;
    integerStack[intStackTop] = value;
}

int popIntStack() { //pop for integer operand stack
    if (intStackTop < 0) return -9996; // error flag for stack underflow
    int value = integerStack[intStackTop];
    intStackTop -= 1;
    return value;
}

void pushOptStack(char value) { // push for operator stack
    optStackTop += 1;
    operatorStack[optStackTop] = value;
}

char popOptStack() { // pop for operator stack
    if (optStackTop < 0) return '#'; // error flag for character stack underflow
    char value = operatorStack[optStackTop];
    optStackTop -= 1;
    return value;
}

int priority(char operator) { //function for determining the priority of operator
    if (operator == '+' || operator == '-')
        return 1;
    return 2;
}

int calculate(int operand1, char operator, int operand2) { // function for calculating the operator operations
    long long result = 0;
    switch (operator) {
        case '+':
            result = (long long)operand1 + (long long)operand2;
            break;
        case '-':
            result = (long long)operand1 - (long long)operand2;
            break;
        case '/':
            if (operand2 == 0) return -9999;
            if (operand1 == INT_MIN || operand2 == -1) return -9995;
            result = (long long)operand1 / (long long)operand2;
            break;
        case '*':
            result = (long long)operand1 * (long long)operand2;
            break;
        default:
            return -9997; // invalid operator
    }
    if (result > INT_MAX || result < INT_MIN) return -9995; // integer overflow or underflow
    return (int)result;
}

int evaluateExpression(char expression[]) { // main logic function for converting given expression in to valid and evaluating expression
    char digitBuffer[10]; //a buffer to convert digit character more than one to proper corresponding number of digits
    int bufferIndex;
    char *ptr = expression;
    while (*ptr != '\0') {
        if (isdigit(*ptr)) { //check if character is digit or not left - right
            bufferIndex = 0;
            while (isdigit(*ptr)) {
                digitBuffer[bufferIndex++] = *ptr;
                ptr++;
            }
            digitBuffer[bufferIndex] = '\0';
            pushIntStack(atoi(digitBuffer)); // converting string to int and push in the stack
            continue;
        }
        else if (isalpha(*ptr)) { // validate if expression consist alphabetic character or not left - right
            return -9998; // error flag for invalid character
        }
        else if (*ptr == '+' || *ptr == '-' || *ptr == '*' || *ptr == '/') { // check the operator from left - right
            while (optStackTop != -1 && priority(operatorStack[optStackTop]) >= priority(*ptr)) { // execute according to precedence and associativity
                char operator = popOptStack();
                int operand2 = popIntStack();
                int operand1 = popIntStack();

                if (operand1 == -9996 || operand2 == -9996) return -9996;  // error underflow (insufficient operands)

                int result = calculate(operand1, operator, operand2);

                if (result == -9995) return -9995;

                if (result == -9999) return -9999; // error flag for division by zero
                if (result == -9997) return -9997; // error flag for operator or symbol other than +.-,*,/
                pushIntStack(result);
            }
            pushOptStack(*ptr);
        }
        else if (isspace(*ptr)) { // ignore the space
            ptr++;
            continue;
        }
        else {
            return -9997; // error flag for invalid symbol
        }
        ptr++;
    }
    while (optStackTop != -1) { // complete the calculation after complete left - right scaning
        int operand2 = popIntStack();
        char operator = popOptStack();
        int operand1 = popIntStack();

        if (operand1 == -9996 || operand2 == -9996) return -9996;  // error underflow (insufficient operands)

        int result = calculate(operand1, operator, operand2);
        if (result == -9995) return -9995;
        if (result == -9999) return -9999;
        if (result == -9997) return -9997;
        pushIntStack(result);
    }
    return popIntStack(); // final result
}

int main(void) {
    char expression[30];
    printf("Enter the expression: ");
    fgets(expression, sizeof(expression), stdin); // using fgets() for multiple words

    int result = evaluateExpression(expression);

    switch (result) {
        case -9999: printf("Error: Division by zero\n"); break;
        case -9998: printf("Error: Contains invalid characters\n"); break;
        case -9997: printf("Error: Invalid symbol\n"); break;
        case -9996: printf("Error: Insufficient operands (stack underflow)\n"); break;
        case -9995: printf("Error: Integer overflow/underflow\n"); break;
        default:    printf("The result is: %d\n", result);
    }
    return 0;
}
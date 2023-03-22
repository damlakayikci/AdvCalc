// C code to convert infix to postfix expression

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPR_SIZE 100

// Function to return precedence of operators
int precedence(char operator) {
    switch (operator) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return -1;
    }
}

// Function to check if the scanned character is an operator
int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}

// convert infix expression to postfix expression
char *infixToPostfix(char *infix) {
    int i, j; // i for the loop, j for keeping track of the postfix expression
    int len = strlen(infix);
    char *postfix = (char *) malloc(sizeof(char) * (len + 2));
    char stack[MAX_EXPR_SIZE];
    int top = -1; // keep track of the element on top of the stack
    int var_name_len = 0; // Initialize variable name length to 0
    int state = 0; // Initialize state to 0
    char var_name[MAX_LENGTH] = ""; // Initialize variable name to empty string

    for (i = 0, j = 0; i < len; i++) {
        if (infix[i] == ' ' || infix[i] == '\t') {
            state = 0;
            continue;
        }
        if (state == 0) { // beginning of new token
            if (isalpha(infix[i])) {
                state = 1;
                var_name[var_name_len++] = infix[i];
            }
            if (isdigit(infix[i])) {
                state = 1;
                var_name[var_name_len++] = infix[i];
            }
        } else {
            if (isalpha(infix[i])) { // If the current character is a letter
                var_name[var_name_len++] = infix[i];
                if (!isalpha(infix[i + 1]) || i != len) { // If the next character is not a letter, end of variable name
                    // check if it's operator name
                    // add name to dictionary
                    state = 0;
                    var_name = "";
                }
            } else if (isdigit(infix[i])) { // If the current character is a digit
                var_name[var_name_len++] = infix[i];
                if (!isdigit(infix[i + 1]) || i != len) { // If the next character is not a digit, end of variable
                    // add name to dictionary
                    state = 0;
                    var_name = "";
                }
            }
        }


        // If the scanned character is operand
        // add it to the postfix expression
        if (isalnum(infix[i])) {
            postfix[j++] = infix[i];
        }

            // if the scanned character is '('
            // push it in the stack
        else if (infix[i] == '(') {
            stack[++top] = infix[i];
        }

            // if the scanned character is ')'
            // pop the stack and add it to the
            // output string until empty or '(' found
        else if (infix[i] == ')') {
            while (top > -1 && stack[top] != '(')
                postfix[j++] = stack[top--];
            if (top > -1 && stack[top] != '(')
                return "Invalid Expression";
            else
                top--;
        }

            // If the scanned character is an operator
            // push it in the stack
        else if (isOperator(infix[i])) {
            while (top > -1 && precedence(stack[top]) >= precedence(infix[i]))
                postfix[j++] = stack[top--];
            stack[++top] = infix[i];
        }
    }


// Pop all remaining elements from the stack
    while (top > -1) {
        if (stack[top] == '(') {
            return "Error!";
        }
        postfix[j++] = stack[top--];
    }
    postfix[j] = '\0';
    return
            postfix;
}

// Driver code
int main() {
    char infix[MAX_EXPR_SIZE] = "a+b*(c^d-e)^(f+g*h)-i";

    // Function call
    char *postfix = infixToPostfix(infix);
    printf("%s\n", postfix);
    free(postfix);
    return 0;
}

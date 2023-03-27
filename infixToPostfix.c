#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"
#include "tokenizer.h"
#include "formatController.h"

#define MAX_LENGTH 256

typedef struct {
    Token items[MAX_LENGTH];
    int top;
} TokenStack;

void push(TokenStack *stack, Token item) {
    stack->top++;
    stack->items[stack->top].name = item.name;
    stack->items[stack->top].type = item.type;
}

Token pop(TokenStack *stack, Token *postfix) {
    Token item = stack->items[stack->top];
    postfix->name = item.name;
    postfix->type = item.type;
    stack->top--;
    return item;
}

Token peek(TokenStack *stack) {
    return stack->items[stack->top];
}

int precedence(char *operator){
    if(strcmp(operator, "^") == 0 || strcmp(operator, "<") == 0 || strcmp(operator, ">") == 0 ||
    strcmp(operator, "$") == 0 || strcmp(operator, "#") == 0)
        return 1;
    else if(strcmp(operator, "+") == 0 || strcmp(operator, "-") == 0)
        return 2;
    else if(strcmp(operator, "*") == 0)
        return 3;
    else if(strcmp(operator, "&") == 0)
        return 4;
    else if(strcmp(operator, "|") == 0)
        return 5;
    else if(strcmp(operator, "!") == 0)
        return 6;
    else
        return -1;
}

// Function to check if the scanned character
// is an operator
int isOperator(char *ch)
{
    return (strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0 || strcmp(ch, "*") == 0 ||
            strcmp(ch, "|") == 0 || strcmp(ch, "<") == 0 || strcmp(ch, ">") == 0 || strcmp(ch, "^") == 0 ||
            strcmp(ch, "$") == 0 || strcmp(ch, "#") == 0 || strcmp(ch, "!") == 0) || strcmp(ch, "&") == 0;

}

// Main functio to convert infix expression
// to postfix expression
Token *infixToPostfix(Token *infix, int infixSize)
{
    int i, j;
    Token *postfix = malloc(sizeof(Token) * (infixSize+1));
    TokenStack stack;
    stack.top = -1;

    for (i = 0, j = 0; i < infixSize; i++) {
        // If the scanned character is operand
        // add it to the postfix expression
        if (infix[i].name != NULL) {
            if (infix[i].type == TOKEN_TYPE_IDENTIFIER) {
                postfix[j].name = infix[i].name;
                postfix[j++].type = infix[i].type;
            }

            else if(infix[i].type == TOKEN_TYPE_NUMBER) {
                postfix[j].name = infix[i].name;
                postfix[j].value = infix[i].value;
                postfix[j++].type = infix[i].type;
            }
                // if the scanned character is '('
                // push it in the stack
            else if (strcmp(infix[i].name, "(") == 0) {
                push(&stack, infix[i]);
            }

                // if the scanned character is ')'
                // pop the stack and add it to the
                // output string until empty or '(' found
            else if (strcmp(infix[i].name, ")") == 0) {
                while (stack.top > -1 && strcmp(peek(&stack).name, "(") != 0) {
                    pop(&stack, &postfix[j++]);
                }
                if (stack.top > -1 && strcmp(peek(&stack).name, "(") != 0) {
                    printf("Invalid Expression");
                    return 0;
                } else
                    stack.top--;
            }

                // If the scanned character is an operator
                // push it in the stack
            else if (isOperator(infix[i].name)) {
                while (stack.top > -1 && precedence(peek(&stack).name) >= precedence(infix[i].name)) {
                    pop(&stack, &postfix[j++]);
                }
                push(&stack, infix[i]);
                //stack[++top]->name = infix[i].name;
                //stack[top]->type = infix[i].type;
            }
        }
    }

    // Pop all remaining elements from the stack
    while (stack.top > -1) {
        if (strcmp(peek(&stack).name, "(") == 0) {
            printf("Invalid Expression");
            return 0;
        }
        pop(&stack, &postfix[j++]);
    }
    //postfix[j].name = '\0';
    return postfix;
}



int main() {
    int num_tokens = 0;
    int num_variables = 0;
    int output_count = 0;
    int index = 0;
    int num_tokens_formatcontroller = 0;
    Token *variables = malloc(sizeof(Token) * 256);
    char input[256];
    printf("Enter input string: ");
    fgets(input, 256, stdin);
    Token *tokens = tokenizer(input, &num_tokens, variables, &num_variables);


    // controller
    for (int i = 0; i < num_tokens; i++) {
        printf("Token %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, tokens[i].name, tokens[i].type,
               tokens[i].value);
    }
    for (int i = 0; i < num_variables; i++) {
        printf("Variable %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, variables[i].name, variables[i].type,
               variables[i].value);
    }
    // end controller
    Token *output = formatController(tokens, num_tokens, 0, &index, &output_count);
    int i = 0;
    if (output == NULL) {
        printf("Error!\n");
        return 0;
    } else {
        while (i < num_tokens) {
            if (output[i].name != NULL) {
                num_tokens_formatcontroller++;
                printf("OUTPUT %d: Name: %s \n", i + 1, output[i].name);
            }else{
                num_tokens_formatcontroller++;
                printf("OUTPUT %d: Name: %s \n", i + 1, "null");
            }
            i++;
        }
    }

    Token *postfix = infixToPostfix(output, num_tokens_formatcontroller);

    for (int j = 0; j < num_tokens_formatcontroller; j++) {
        printf("Postfix %d: Name: %s\t\t Type: %u\t\t Value: %d\n", j + 1, postfix[j].name, postfix[j].type,
               postfix[j].value);
    }
    free(tokens);
    free(postfix);

    return 0;
}

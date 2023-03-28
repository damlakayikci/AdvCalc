// C program to evaluate value of a postfix
// expression having multiple digit operands
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"

#define INT_BITS 32
#define MAX_LENGTH 256
// Stack type
typedef struct {
    Token items[MAX_LENGTH];
    int top;
} TokenStack;

int isOperator(char *ch)
{
    return (strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0 || strcmp(ch, "*") == 0 ||
            strcmp(ch, "|") == 0 || strcmp(ch, "<") == 0 || strcmp(ch, ">") == 0 || strcmp(ch, "^") == 0 ||
            strcmp(ch, "$") == 0 || strcmp(ch, "#") == 0 || strcmp(ch, "!") == 0) || strcmp(ch, "&") == 0;

}

int isEmpty(TokenStack *stack){
    return stack->top == -1 ;
}

Token pop(TokenStack *stack){
    if (!isEmpty(stack))
        return stack->items[stack->top--] ;
    return (Token) NULL;
}

void push(TokenStack *stack, int item) {
    stack->top++;
    stack->items[stack->top].name = "temp";
    stack->items[stack->top].type = TOKEN_TYPE_NUMBER;
    stack->items[stack->top].value = item;
}

int leftRotate(int n, unsigned int d)
{
    /* In n<<d, last d bits are 0. To put first 3 bits of n at
      last, do bitwise or of n<<d with n >>(INT_BITS - d) */
    return (n << d)|(n >> (INT_BITS - d));
}

/*Function to right rotate n by d bits*/
int rightRotate(int n, unsigned int d)
{
    /* In n>>d, first d bits are 0. To put last 3 bits of at
      first, do bitwise or of n>>d with n <<(INT_BITS - d) */
    return (n >> d)|(n << (INT_BITS - d));
}

// The main function that returns value
// of a given postfix expression
int evaluatePostfix(Token* postfix, int postfixSize){
    Token *output = malloc(sizeof(Token) * (postfixSize + 1));
    TokenStack stack;
    stack.top = -1;

    int i = 0;
    // Scan all characters one by one
    for (i = 0; i < postfixSize; ++i){
        if (isOperator(postfix[i].name)){
            if (strcmp(postfix[i].name, "!") == 0){
                if (pop(&stack).type == TOKEN_TYPE_IDENTIFIER){
                    int val1 = pop(&stack).value;
                    push(&stack, ~val1);
                }
                else if (pop(&stack).type == TOKEN_TYPE_NUMBER){
                    int val1 = pop(&stack).value;
                    push(&stack, ~val1);
                }
                else
                    printf("Error: Invalid operand for ! operator");
            }
            else if (pop(&stack).type == TOKEN_TYPE_IDENTIFIER || pop(&stack).type == TOKEN_TYPE_NUMBER){
                int val1 = pop(&stack).value;
                if (pop(&stack).type == TOKEN_TYPE_IDENTIFIER || pop(&stack).type == TOKEN_TYPE_NUMBER){
                    int val2 = pop(&stack).value;

                    switch (postfix[i].name[i])
                    {
                        case '+': push(&stack, val2 + val1); break;
                        case '-': push(&stack, val2 - val1); break;
                        case '*': push(&stack, val2 * val1); break;
                        case '^': push(&stack, val2 ^ val1); break;
                        case '$': push(&stack, leftRotate(val2,val1)); break;
                        case '#': push(&stack, rightRotate(val2,  val1)); break;
                        case '<': push(&stack, val2 << val1); break;
                        case '>': push(&stack, val2 >> val1); break;
                        case '&': push(&stack, val2 & val1); break;
                        case '|': push(&stack, val2 | val1); break;
                    }
                }
                else{
                    printf("Error: Invalid operand for %s operator", postfix[i].name);
                    return 0;
                }
            }
            else{
                printf("Error: Invalid operand for %s operator", postfix[i].name);
                return 0;
            }
        }
        else if (postfix[i].type == TOKEN_TYPE_NUMBER){
            push(&stack, postfix[i].value);
        }
        else if (postfix[i].type == TOKEN_TYPE_IDENTIFIER){
            push(&stack, postfix[i].value);
        }
        else{
            printf("Error: Invalid token");
            return 0;
        }
    }
    return pop(&stack).value;
}

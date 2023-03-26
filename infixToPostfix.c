#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"

#define MAX_LENGTH 256


int isOperatorName(char *ch) {
    return (strcmp(ch, "xor") == 0 || strcmp(ch, "ls") == 0 || strcmp(ch, "rs") == 0 || strcmp(ch, "lr") == 0 ||
            strcmp(ch, "rr") == 0 || strcmp(ch, "not") == 0);
}

int isSymbol(char *ch) {
    return strchr("+-*&|(),=", *ch) != NULL;
}


Token *tokenizer(char *input, int *num_tokens, Token *variables, int *num_variables) {
    // get the length of the input string
    int input_length = strlen(input);
    if (input[0] == '%') {
        printf("\n");
        Token *tokens = malloc( sizeof(Token));
        tokens[0].name = "Comment_line";
        return tokens;
    }

    // allocate memory for the array of tokens
    Token *tokens = malloc(sizeof(Token) * input_length);

    // iterate through the input string, one character at a time
    int i = 0;
    while (i < input_length) {
        // if the current character is a digit, parse it as a number
        if (isdigit(input[i])) {
            // find the end of the number
            int j = i;
            while (isdigit(input[j])) {
                j++;
            }

            // extract the number string from the input
            char *num_str = malloc(sizeof(char) * (j - i + 1));
            strncpy(num_str, &input[i], j - i);
            num_str[j - i] = '\0';

            // convert the number string to an integer
            int num = atoi(num_str);

            // add the number as a new token to the array
            tokens[*num_tokens].value = num;
            tokens[*num_tokens].name = "num_str";
            tokens[*num_tokens].type = TOKEN_TYPE_NUMBER;
            (*num_tokens)++;

            // move the index to the end of the number
            i = j;

            // free the temporary number string memory
            free(num_str);

            // if the current character is a letter, parse it as a string
        } else if (isalpha(input[i])) {
            // find the end of the string
            int j = i;
            while (isalpha(input[j])) {
                j++;
            }
            // extract the string from the input
            char *name = malloc(sizeof(char) * (j - i + 1));
            strncpy(name, &input[i], j - i);
            name[j - i] = '\0';
            if (isOperatorName(name)) {   // if it's an operator, add the string as a new token to the array
                tokens[*num_tokens].name = name;
                if (strcmp(name, "xor") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_XOR;
                    tokens[*num_tokens].name = "^";
                } else if (strcmp(name, "ls") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_LS;
                    tokens[*num_tokens].name = "<";
                } else if (strcmp(name, "rs") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_RS;
                    tokens[*num_tokens].name = ">";
                } else if (strcmp(name, "lr") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_LR;
                    tokens[*num_tokens].name = "$";
                } else if (strcmp(name, "rr") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_RR;
                    tokens[*num_tokens].name = "#";
                } else if (strcmp(name, "not") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_NOT;
                    tokens[*num_tokens].name = "!";
                }
            } else {   // if it's a variable, add the string as a new token to the array
                tokens[*num_tokens].value = 0;
                tokens[*num_tokens].name = name;
                tokens[*num_tokens].type = TOKEN_TYPE_IDENTIFIER;
                variables[*num_variables].value = 0;
                variables[*num_variables].name = name;
                variables[*num_variables].type = TOKEN_TYPE_IDENTIFIER;
                (*num_variables)++;
            }
            (*num_tokens)++;

            // move the index to the end of the string
            i = j;

            // if the current character is an operator, parse it as an operator
        } else if (isSymbol(&input[i])) {
            char *name = malloc(sizeof(char) * 2);
            strncpy(name, &input[i], 1);
            if (input[i] == '+') {
                tokens[*num_tokens].type = TOKEN_TYPE_PLUS;
            } else if (input[i] == '-') {
                tokens[*num_tokens].type = TOKEN_TYPE_MINUS;
            } else if (input[i] == '*') {
                tokens[*num_tokens].type = TOKEN_TYPE_ASTERISK;
            } else if (input[i] == '&') {
                tokens[*num_tokens].type = TOKEN_TYPE_AMPERSAND;
            } else if (input[i] == '|') {
                tokens[*num_tokens].type = TOKEN_TYPE_PIPE;
            } else if (input[i] == '(') {
                tokens[*num_tokens].type = TOKEN_TYPE_OPENPARENTHESIS;
            } else if (input[i] == ')') {
                tokens[*num_tokens].type = TOKEN_TYPE_CLOSEPARENTHESIS;
            } else if (input[i] == ',') {
                tokens[*num_tokens].type = TOKEN_TYPE_COMMA;
            } else if (input[i] == '=') {
                tokens[*num_tokens].type = TOKEN_TYPE_EQUALS;
            }
            tokens[*num_tokens].name = name;
            tokens[*num_tokens].name[1] = '\0';
            tokens[*num_tokens].value = 0;
            (*num_tokens)++;
            i++;
        } else if (isspace(input[i])) {
            i++;
        }  else {
            printf("Error: Invalid character\n");
            return NULL;
        }
    }

    // return the array of tokens
    return tokens;
}

int isFunctionOperator(char *ch) {
    return (strcmp(ch, "^") == 0 || strcmp(ch, "<") == 0 || strcmp(ch, ">") == 0 || strcmp(ch, "$") == 0 ||
            strcmp(ch, "#") == 0);
}

Token *formatController(Token *input, int inputSize, int recursive, int *index, int *output_count) {
    Token *output = malloc(sizeof(Token) * inputSize);
    int recIndex;
    int nonRecIndex;
    if ((inputSize >= 2) && (input[1].type == TOKEN_TYPE_EQUALS) && (input[0].type != TOKEN_TYPE_IDENTIFIER)) {
        printf("Error: Equal sign\n");
        return NULL;
    }
    while ((*index) < inputSize) {       //for each token
        // for repeated tokens return NULL
        if (input[(*index)].type == TOKEN_TYPE_IDENTIFIER && input[(*index) + 1].type == TOKEN_TYPE_IDENTIFIER) {
            printf("Error: Repeated identifier\n");
            return NULL;
        }


        int parenthesisCount = 0;       //count the parenthesis
        if (isFunctionOperator(input[(*index)].name)) {     //if it is a function operator
            if (strcmp(input[(*index) + 1].name, "(") == 0) {   //if next is an open parenthesis
                output[(*index) + 1].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
                output[(*index) + 1].name = "(";
                (*output_count)++;    //increase the output count
                parenthesisCount++;
            } else { // BURASI CALISIYOR
                printf("Error: Expected open parenthesis after function operator\n");
                return NULL;
            }
            if (recursive) {
                recIndex = *index;
            } else {
                nonRecIndex = *index;
            }
            int j = 2;
            int commaCount = 0;
            while (parenthesisCount != 0) {   //while the parenthesis are not closed
                if ((*index) + j == inputSize) {    //if the end of the input is reached
                    printf("Error: Parenthesis not closed\n");
                    return NULL;
                } else if (input[(*index) + j].type ==
                           TOKEN_TYPE_OPENPARENTHESIS) {       //if it is an open parenthesis
                    output[(*index) + j].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
                    output[(*index) + j].name = "(";
                    (*output_count)++;    //increase the output count
                    parenthesisCount++;
                    j++;
                } else if (input[(*index) + j].type ==
                           TOKEN_TYPE_CLOSEPARENTHESIS) {      //if it is a close parenthesis
                    output[(*index) + j].type = TOKEN_TYPE_CLOSEPARENTHESIS;    //add it to the output
                    output[(*index) + j].name = ")";
                    (*output_count)++;   //increase the output count
                    parenthesisCount--;
                    j++;
                } else if (isFunctionOperator(input[(*index) + j].name)) {     //if it is a function operator
                    (*index) += j;
                    Token *inner = formatController(input, inputSize, 1, index, output_count);
                    if (inner == NULL) {
                        printf("Error: Inner function operator\n");
                        return NULL;
                    }
                    (*index) -= j;
//xor(xor(a,b),c)
                    for (int k = 0; k < inputSize; k++) {
                        if (inner[k].name != NULL) {
                            output[k] = inner[k];
                        }
                    }
                } else if (strcmp(input[(*index) + j].name, ",") == 0) {    //if it is a comma
                    if (recursive) {
                        output[(*index) + j].type = input[recIndex].type;   //insert the operator to the output
                        output[(*index) + j].name = input[recIndex].name;
                    } else {
                        output[(*index) + j].type = input[nonRecIndex].type;   //insert the operator to the output
                        output[(*index) + j].name = input[nonRecIndex].name;
                    }
                    commaCount++;
                    (*output_count)++;   //increase the output count
                    //change the stage
                    j++;
                } else {
                    output[(*index) + j].type = input[(*index) +
                                                      j].type;   //if it is not a function operator, add it to the output
                    output[(*index) + j].name = input[(*index) + j].name;
                    (*output_count)++;   //increase the output count
                    j++;
                }
            }
            if (commaCount != 1) {   //if there is not 1 comma inside expression
                printf("Error: Non-expected number of commas\n");
                return NULL;
            }
            (*index) += j;    //skip the tokens that are already processed
            if (recursive == 1) {   //if it is a recursive call, return the output
                return output;
            }

        } else if (strcmp(input[(*index)].name, ",") == 0) {
            printf("Error: Unexpected comma\n");
            return NULL;
        } else {
            output[(*index)].type = input[(*index)].type;   //if it is not a function operator, add it to the output
            output[(*index)].name = input[(*index)].name;
            (*output_count)++;  //increase the output count
            (*index)++;
        }
    }
    return output;
}


int precedence(char *operator){
    if(strcmp(operator, "^") == 0 || strcmp(operator, "<") == 0 || strcmp(operator, ">") == 0 ||
    strcmp(operator, "$") == 0 || strcmp(operator, "#") == 0 || strcmp(operator, "!") == 0)
        return 1;
    else if(strcmp(operator, "+") == 0 || strcmp(operator, "-") == 0)
        return 2;
    else if(strcmp(operator, "*") == 0)
        return 3;
    else if(strcmp(operator, "&") == 0)
        return 4;
    else if(strcmp(operator, "|") == 0)
        return 5;
    else
        return -1;
}

// Function to check if the scanned character
// is an operator
int isOperator(char *ch)
{
    return (strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0 || strcmp(ch, "*") == 0 ||
            strcmp(ch, "|") == 0 || strcmp(ch, "<") == 0 || strcmp(ch, ">") == 0 || strcmp(ch, "^") == 0 ||
            strcmp(ch, "$") == 0 || strcmp(ch, "#") == 0 || strcmp(ch, "!") == 0);

}

// Main functio to convert infix expression
// to postfix expression
Token *infixToPostfix(Token *infix, int infixSize)
{
    int i, j;
    Token *postfix = malloc(sizeof(Token) * (infixSize));
    Token *stack[MAX_LENGTH];
    int top = -1;

    for (i = 0, j = 0; i < infixSize; i++) {
        // If the scanned character is operand
        // add it to the postfix expression
        if (infix[i].type == TOKEN_TYPE_IDENTIFIER) {
            postfix[j].name = infix[i].name;
            postfix[j++].type = infix[i].type;
        }

            // if the scanned character is '('
            // push it in the stack
        else if (strcmp(infix[i].name, "(") == 0) {
            stack[++top]->name = infix[i].name;
            stack[top]->type = infix[i].type;
        }

            // if the scanned character is ')'
            // pop the stack and add it to the
            // output string until empty or '(' found
        else if (strcmp(infix[i].name, ")") == 0) {
            while (top > -1 && strcmp(stack[top]->name, "(") != 0){
                postfix[j].name = stack[top]->name;
                postfix[j++].type = stack[top--]->type;
            }
            if (top > -1 && strcmp(stack[top]->name, "(") != 0){
                printf("Invalid Expression");
                return 0;
            }else
                top--;
        }

            // If the scanned character is an operator
            // push it in the stack
        else if (isOperator(infix[i].name)) {
            while (top > -1 && precedence(stack[top]->name) >= precedence(infix[i].name)) {
                postfix[j].name = stack[top]->name;
                postfix[j++].type = stack[top--]->type;
            }
            stack[++top]->name = infix[i].name;
            stack[top]->type = infix[i].type;
        }
    }

    // Pop all remaining elements from the stack
    while (top > -1) {
        if (strcmp(stack[top]->name, "(") == 0) {
            printf("Invalid Expression");
            return 0;
        }
        postfix[j].name = stack[top]->name;
        postfix[j++].type = stack[top--]->type;
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
            if (output[i].name != NULL)
                printf("%s", output[i].name);
            i++;
        }
        while (i < num_tokens) {
            if (output[i].name != NULL)
                printf("OUTPUT %d: Name: %s", i + 1, output[i].name);
            i++;
        }
    }
    Token *postfix = infixToPostfix(output, num_tokens_formatcontroller);
    i=0;
    while (i < num_tokens) {
        if (postfix[i].name != NULL) {
            num_tokens_formatcontroller++;
            printf("OUTPUT %d: Name: %s", i + 1, output[i].name);
        }
        i++;
    }
    for (int j = 0; j < num_tokens_formatcontroller; j++) {
        printf("Postfix %d: Name: %s\t\t Type: %u\t\t Value: %d\n", j + 1, postfix[j].name, postfix[j].type,
               postfix[j].value);
    }
    free(tokens);
    free(postfix);

    return 0;
}
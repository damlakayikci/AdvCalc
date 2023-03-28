// #include <stdio.h>
// #include <string.h>
// #include <ctype.h>
// #include <stdlib.h>
// #include "token.h"

#define MAX_LENGTH 256


int isOperatorName(char *ch) {
    return (strcmp(ch, "xor") == 0 || strcmp(ch, "ls") == 0 || strcmp(ch, "rs") == 0 || strcmp(ch, "lr") == 0 ||
            strcmp(ch, "rr") == 0 || strcmp(ch, "not") == 0);
}

int isOperatorSymbol(char *ch) {
    return strchr("^<>#$", *ch) != NULL;
}

int isSymbol(char *ch) {
    return strchr("+-*&|(),=", *ch) != NULL;
}

int returnIndex(Token *variables, int num_tokens, char *name) {
    for (int i = 0; i < num_tokens; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

Token *changeParenthesis(Token *tokens, int input_length) {
    int index = 0;
    int p = 0; // parenthesis counter
    // array to keep track of the parenthesis
    int *parenthesis = malloc(sizeof(int) * input_length); // 0 for normal parenthesis, 1 for operator parenthesis
    // int parenthesis[input_length] ;
    // If it's a comment line, do nothing, return the tokens
    if (strcmp(tokens[0].name, "Comment_line") == 0) {
        return tokens;
    }
    while (index < input_length) {
        if (tokens[index].type == TOKEN_TYPE_OPENPARENTHESIS) {
            if (isOperatorSymbol(tokens[index - 1].name)) {
                tokens[index].type = TOKEN_TYPE_OPENPARENTHESISOPERATOR;
                parenthesis[++p] = 1;
            } else {
                parenthesis[++p] = 0;
            }
        } else if (tokens[index].type == TOKEN_TYPE_CLOSEPARENTHESIS) {
            if (parenthesis[p] == 1) {
                tokens[index].type = TOKEN_TYPE_CLOSEPARENTHESISOPERATOR;
            }
            parenthesis[p] = -1;
            p--;
        }
        index++;
    }
    if (p != 0) {
        printf("Error: Parenthesis mismatch\n");
        return NULL;
    }
    return tokens;
}

//  xor  ( ( (a+b) + xor( (v+y), c  )  ), xor( (x+t) , ( u+ not( c)  )  )  )
//       1 2 3  -3      3 4  -4    -3 -2     2 3  -3   3       4 -4 -3 -2 -1

//         1 2  -2        2  -2       -1       1  -1   1            -1        
//  xor  [ ( (a+b) + xor[ (b+y), c  ]  ), xor[ (a+b) , ( a+ not[ c]  )  ]  ]
//       1              2          -2        2                 3 -3    -2 -1

Token *tokenizer(Token *tokens, char *input, int *num_tokens, Token *variables, int *num_variables) {
    // get the length of the input string
    int num_local_tokens = 0;
    int input_length = strlen(input);
    if (input[0] == '%') {
        printf("\n");
        Token *tokens = malloc(sizeof(Token));
        tokens[0].name = "Comment_line";
        return tokens;
    }

    // allocate memory for the array of tokens
    tokens = NULL;
    tokens = malloc(sizeof(Token) * input_length);

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
            tokens[num_local_tokens].value = num;
            tokens[num_local_tokens].name = "num_str";
            tokens[num_local_tokens].type = TOKEN_TYPE_NUMBER;
            (num_local_tokens)++;
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
                tokens[num_local_tokens].name = name;
                if (strcmp(name, "xor") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_XOR;
                    tokens[num_local_tokens].name = "^";
                } else if (strcmp(name, "ls") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_LS;
                    tokens[num_local_tokens].name = "<";
                } else if (strcmp(name, "rs") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_RS;
                    tokens[num_local_tokens].name = ">";
                } else if (strcmp(name, "lr") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_LR;
                    tokens[num_local_tokens].name = "$";
                } else if (strcmp(name, "rr") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_RR;
                    tokens[num_local_tokens].name = "#";
                } else if (strcmp(name, "not") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_NOT;
                    tokens[num_local_tokens].name = "!";
                }
            } else {   // if it's a variable, add the string as a new token to the array
                tokens[num_local_tokens].value = 0;
                tokens[num_local_tokens].name = name;
                tokens[num_local_tokens].type = TOKEN_TYPE_IDENTIFIER;

                // If variable is not on the array, add it to the array of variables
                if (returnIndex(variables, *num_variables, name) == -1) {
                    variables[*num_variables].value = 0;
                    variables[*num_variables].name = name;
                    variables[*num_variables].type = TOKEN_TYPE_IDENTIFIER;
                    (*num_variables)++;
                }
            }
            (num_local_tokens)++;
            (*num_tokens)++;

            // move the index to the end of the string
            i = j;

            // if the current character is an operator, parse it as an operator
        } else if (isSymbol(&input[i])) {
            char *name = malloc(sizeof(char) * 2);
            strncpy(name, &input[i], 1);
            if (input[i] == '+') {
                tokens[num_local_tokens].type = TOKEN_TYPE_PLUS;
            } else if (input[i] == '-') {
                tokens[num_local_tokens].type = TOKEN_TYPE_MINUS;
            } else if (input[i] == '*') {
                tokens[num_local_tokens].type = TOKEN_TYPE_ASTERISK;
            } else if (input[i] == '&') {
                tokens[num_local_tokens].type = TOKEN_TYPE_AMPERSAND;
            } else if (input[i] == '|') {
                tokens[num_local_tokens].type = TOKEN_TYPE_PIPE;
            } else if (input[i] == '(') {
                tokens[num_local_tokens].type = TOKEN_TYPE_OPENPARENTHESIS;
            } else if (input[i] == ')') {
                tokens[num_local_tokens].type = TOKEN_TYPE_CLOSEPARENTHESIS;
            } else if (input[i] == ',') {
                tokens[num_local_tokens].type = TOKEN_TYPE_COMMA;
            } else if (input[i] == '=') {
                tokens[num_local_tokens].type = TOKEN_TYPE_EQUALS;
            }
            tokens[num_local_tokens].name = name;
            tokens[num_local_tokens].name[1] = '\0';
            tokens[num_local_tokens].value = 0;
            (num_local_tokens)++;
            (*num_tokens)++;
            i++;
        } else if (isspace(input[i])) {
            i++;
        } else {
            printf("Error: Invalid character%c\n", input[i]);
            return NULL;
        }
    }
    //tokens = changeParenthesis(tokens, input_length);
    if (tokens == NULL) {
        return NULL;
    }
    // return the array of tokens
    return tokens;
}
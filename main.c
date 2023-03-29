#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"
//#include "tokenizer.h"
#include "formatController.h"
#include "postfixCalculator.h"

#define MAX_LENGTH 256
#define MAX_VARIABLES 128


int main() {
    int num_variables = 0;
    // Token *tokens = malloc(sizeof(Token) * MAX_LENGTH);
    Token *variables = malloc(sizeof(Token) * MAX_VARIABLES);
    char input[256];

    while (1) {
        int num_tokens = 0; //  keep track of the number of tokens
        int index = 0;     //  keep track of the index of the tokens
        int output_count = 0;
        printf("Enter input string: ");
        fgets(input, 256, stdin);
        //  if the input is blank
        if (isspace(*input)) {
            printf("Blank input\n");
            continue;
        } else {
            // tokenize the input
            Token *tokens = tokenizer(input, &num_tokens, variables, &num_variables);
            // if function retruns null, there is an error
            if (tokens == NULL) {
                printf("Error!\n");
                continue;
                // if it's a comment line, print a new line
            } else if (strcmp(tokens[0].name, "Comment_line") == 0) {
                printf("\n");
                continue;
            } else {
                Token *tokenequals = malloc(sizeof(Token) * MAX_LENGTH);
                int tokenequalsindex = 0;

                Token *variable = malloc(sizeof(Token) * MAX_LENGTH);
                int variableindex = 0;
                for (int i = 0; i < num_tokens - 1; ++i) {

                    // CONTROLLER
                    printf("Tokenizer output\t %d: Name: %s\t\t Type: %u\t\t Value: %lld\n", i + 1, tokens[i].name,
                           tokens[i].type,
                           tokens[i].value);
                    printf("Variable %d: Name: %s\t\t Type: %u\t\t Value: %lld\n", i + 1,
                           variables[num_variables - 1].name,
                           variables[num_variables - 1].type,
                           variables[num_variables - 1].value);
                    // END CONTROLLER

                    if (tokens[i].type == TOKEN_TYPE_EQUALS &&
                        (tokens[i - 1].name == variables[num_variables - 1].name)) {
                        tokenequalsindex = 1;
                        variable[0].name = tokens[i - 1].name;
                        variable[0].type = tokens[i - 1].type;
                        variable[0].value = tokens[i - 1].value;
                    }
                    if (tokenequalsindex == 1) {
                        tokenequals[variableindex].name = tokens[i + 1].name;
                        tokenequals[variableindex].type = tokens[i + 1].type;
                        tokenequals[variableindex].value = tokens[i + 1].value;
                        variableindex++;
                    }
                }
                if (tokenequalsindex == 1) {
                    variables[returnIndex(variables, num_variables, variable[0].name)].value = evaluatePostfix(
                            infixToPostfix(
                                    formatController(tokenequals, variableindex, 0, &index, &output_count),
                                    variableindex), variableindex, variables, num_variables);
                    continue;
                }
                // CONTROLLER
                for (int i = 0; i < num_tokens - 1; i++) {
                    printf("Tokenizer output\t %d: Name: %s\t\t Type: %u\t\t Value: %lld\n", i + 1, tokens[i].name,
                           tokens[i].type,
                           tokens[i].value);
                }
                for (int i = 0; i < num_variables; i++) {

                    printf("Variable %d: Name: %s\t\t Type: %u\t\t Value: %lld\n", i + 1, variables[i].name,
                           variables[i].type,
                           variables[i].value);
                }
                // END OF CONTROLLER


                if (tokenequalsindex == 0) {
                    Token *output = formatController(tokens, num_tokens, 0, &index, &output_count);

                    // if formatController returns null, there is an error
                    if (output == NULL) {
                        printf("Error!\n");
                    } else {
                        Token *postfix = infixToPostfix(output, num_tokens);
                        
                        // if infixToPostfix returns null, there is an error
                        if (postfix == NULL) {
                            printf("Error!\n");
                        } else {

                            // CONTROLLER
                            //                    for (int j = 0; j < num_tokens; j++) {
                            //                        printf("Postfix %d: Name: %s\t\t Type: %u\t\t Value: %d\n", j + 1, postfix[j].name, postfix[j].type,
                            //                               postfix[j].value);
                            //                    }
                            // END OF CONTROLLER

                            printf("Result: %lld\n", evaluatePostfix(postfix, num_tokens, variables, num_variables));
                        }
                    }
                }
            }
            free(tokens);
        }
    }
    return 0;
}

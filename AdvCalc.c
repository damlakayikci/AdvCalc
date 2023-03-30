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
        int error = 0; // boolean for errors

        printf("Enter input string: ");
        fgets(input, 256, stdin);
        //  if the input is blank

        int len = strlen(input);
        int is_empty = 1;
        for (int i = 0; i < len; i++) {
            if (!isspace(input[i])) {
                is_empty = 0;
                break;
            }
        }

        if (is_empty) {
            printf("Input is empty.\n");
        } else {
            // tokenize the input
            Token *tokens = tokenizer(input, &num_tokens, variables, &num_variables);
            // if function doesn't return null, there isn't an error
            if (tokens != NULL) {

                // CONTROLLER
                for (int i = 0; i < num_tokens; ++i) {
                    printf("Tokenizer output\t %d: Name: %s\t\t Type: %u\t\t Value: %lld\n", i + 1, tokens[i].name,
                           tokens[i].type,
                           tokens[i].value);
                }
                printf("Num tokens: %d\n", num_tokens);
                // END CONTROLLER

                if (strcmp(tokens[0].name, "Comment_line") == 0) {
                    printf("\n");
                    continue;
                } else {
                    Token *formatted = formatController(tokens, num_tokens, 0, &index, &output_count);
                    if (formatted != NULL) { // if there is no error in formatting

                        // CONTROLLER
                        for (int i = 0; i < index; ++i) {
                            printf("Formatted output\t %d: Name: %s\t\t Type: %u\t\t Value: %lld\n", i + 1,
                                   formatted[i].name,
                                   formatted[i].type,
                                   formatted[i].value);
                        } // END CONTROLLER

                        // if it is an equation
                        if (formatted[1].type == TOKEN_TYPE_EQUALS) {
                            Token *ptr = formatted;  // create a pointer to the first element of the array
                            Token variable = *(ptr);  // access the element at index 0
                            // the expression after the equal sign will be our value, so we take formatted form second element
                            Token *postfix = infixToPostfix(&formatted[2], num_tokens - 2, &error);

                            // CONTROLLER
                            for (int i = 0; i < output_count - 2; ++i) {
                                printf("Postfix with = output\t %d: Name: %s\t\t Type: %u\t\t Value: %lld\n", i + 1,
                                       postfix[i].name,
                                       postfix[i].type,
                                       postfix[i].value);
                            }
                            // END CONTROLLER

                            if (!error) { // if there is no error in converting to postfix
                                long long int result = evaluatePostfix(postfix, num_tokens - 2, variables,
                                                                       num_variables, &error);

                                // CONTROLLER
                                printf("Result: %lld\n", result);
                                // END CONTROLLER

                                if (error) { // if there is an error in evaluating the postfix
                                    printf("Error!\n");
                                    continue;
                                } else {
                                    int var_index = returnIndex(variables, num_variables, variable.name);
                                    variables[var_index].value = result;

                                    // CONTROLLER
                                    for (int i = 0; i < num_variables; ++i) {
                                        printf("Variables\t %d: Name: %s\t\t Type: %u\t\t Value: %lld\n", i + 1,
                                               variables[i].name,
                                               variables[i].type,
                                               variables[i].value);
                                    }
                                    // END CONTROLLER
                                }
                            }
                        }
                            // if there is no equal sign in the input
                        else {
                            Token *postfix = infixToPostfix(formatted, num_tokens, &error);
                            if (!error) { // if there is no error in converting to postfix

                                // CONTROLLER
                                for (int i = 0; i < output_count; ++i) {
                                    printf("Postfix output\t %d: Name: %s\t\t Type: %u\t\t Value: %lld\n", i + 1,
                                           postfix[i].name,
                                           postfix[i].type,
                                           postfix[i].value);
                                }
                                // END CONTROLLER

                                long long int result = evaluatePostfix(postfix, num_tokens, variables,
                                                                       num_variables, &error);
                                if (!error) { // if there isn't an error in evaluating the postfix
                                    printf("Result: %lld\n", result);
                                }
                            }
                        }

                    }
                }

            } else {
                printf("Error!\n");
            }
            free(tokens);
        }
    }
    return 0;
}

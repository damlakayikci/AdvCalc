#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"
#include "tokenizer.h"
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
        int num_tokens = 0;
        int index = 0;
        int output_count = 0;
        printf("Enter input string: ");
        fgets(input, 256, stdin);
        Token *tokens = tokenizer(input, &num_tokens, variables, &num_variables);

        if (tokens == NULL) {
            printf("Error!\n");
            return 0;
        } else {

            // CONTROLLER
            for (int i = 0; i < num_tokens; i++) {
                printf("Tokenizer output\t %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, tokens[i].name,
                       tokens[i].type,
                       tokens[i].value);
            }
            for (int i = 0; i < num_variables; i++) {
                printf("Variable %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, variables[i].name,
                       variables[i].type,
                       variables[i].value);
            }
            // END OF CONTROLLER


            Token *output = formatController(tokens, num_tokens, 0, &index, &output_count);

            if (output == NULL) {
                printf("Error!\n");
                return 0;
            } else {
                if (strcmp(output[0].name, "Comment_line") == 0) {
                    printf("\n");
                    continue;
                }
                // END OF CONTROLLER
                Token *postfix = infixToPostfix(output, num_tokens);
                if (postfix == NULL) {
                    printf("Error!\n");
                    return 0;
                } else {

                    // CONTROLLER
//                    for (int j = 0; j < num_tokens; j++) {
//                        printf("Postfix %d: Name: %s\t\t Type: %u\t\t Value: %d\n", j + 1, postfix[j].name, postfix[j].type,
//                               postfix[j].value);
//                    }
                    // END OF CONTROLLER

                    printf("Result: %d\n", evaluatePostfix(postfix, num_tokens));

                    if (output[1].type == TOKEN_TYPE_EQUALS) {
                        Token variable = output[0];

                    } else {

                    }
                }



                // CONTROLLER
//            else {
//                int i = 0;
//                while (i < num_tokens) {
//                    //if (output[i].name != NULL)
//                    printf("OUTPUT %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, output[i].name, output[i].type,
//                           output[i].value);
//                    i++;
//                }
//            }


            }
        }

        free(tokens);

    }
    return 0;
}

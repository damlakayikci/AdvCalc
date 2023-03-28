#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"
#include "tokenizer.h"
#include "formatController.h"

#define MAX_LENGTH 256
#define MAX_VARIABLES 128


int main() {
    int num_tokens = 0;
    int num_variables = 0;
    int output_count = 0;
    int index = 0;
    Token *variables = malloc(sizeof(Token) * 256);
    char input[256];
    printf("Enter input string: ");
    fgets(input, 256, stdin);
    Token *tokens = tokenizer(input, &num_tokens, variables, &num_variables);


    // CONTROLLER
    for (int i = 0; i < num_tokens; i++) {
        printf("Tokenizer output\t %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, tokens[i].name, tokens[i].type,
               tokens[i].value);
    }
    for (int i = 0; i < num_variables; i++) {
        printf("Variable %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, variables[i].name, variables[i].type,
               variables[i].value);
    }
    // END OF CONTROLLER


    Token *output = formatController(tokens, num_tokens, 0, &index, &output_count);
    
    if (output == NULL) {
        printf("Error!\n");
        return 0;
    } 
    // CONTROLLER
    else {
        int i = 0;
        while (i < num_tokens) {
            //if (output[i].name != NULL)
                printf("OUTPUT %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, output[i].name, output[i].type,
                       output[i].value);
            i++;
        }
    } 
    // END OF CONTROLLER

    free(tokens);
    return 0;
}



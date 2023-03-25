
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

typedef enum {
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_PLUS,
    TOKEN_TYPE_MINUS,
    TOKEN_TYPE_ASTERISK,
    TOKEN_TYPE_AMPERSAND,
    TOKEN_TYPE_PIPE,
    TOKEN_TYPE_XOR,
    TOKEN_TYPE_LS,
    TOKEN_TYPE_RS,
    TOKEN_TYPE_LR,
    TOKEN_TYPE_RR,
    TOKEN_TYPE_NOT,
    TOKEN_TYPE_OPENPARENTHESIS,
    TOKEN_TYPE_CLOSEPARENTHESIS,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_EQUALS,
} TokenType;

typedef struct {
    TokenType type;
    int value;
    char *name;
} Token;

int isOperatorName(char *ch) {
    return (strcmp(ch, "xor") == 0 || strcmp(ch, "ls") == 0 || strcmp(ch, "rs") == 0 || strcmp(ch, "lr") == 0 ||
            strcmp(ch, "rr") == 0 || strcmp(ch, "not") == 0);
}

int isOperator(char *ch) {
    return strchr("+-*&|(),=", *ch) != NULL;
}


Token *tokenizer(char *input) {
    // get the length of the input string
    int input_length = strlen(input);

    // allocate memory for the array of tokens
    Token *tokens = malloc(sizeof(Token) * input_length);

    // keep track of the number of tokens found so far
    int num_tokens = 0;

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
            tokens[num_tokens].value = num;
            tokens[num_tokens].name = "num_str";
            tokens[num_tokens].type = TOKEN_TYPE_NUMBER;
            num_tokens++;

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
                tokens[num_tokens].name = name;
                if (strcmp(name, "xor") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_XOR;
                    tokens[num_tokens].name = "^";
                } else if (strcmp(name, "ls") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_LS;
                    tokens[num_tokens].name = "<";
                } else if (strcmp(name, "rs") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_RS;
                    tokens[num_tokens].name = ">";
                } else if (strcmp(name, "lr") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_LR;
                    tokens[num_tokens].name = "$";
                } else if (strcmp(name, "rr") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_RR;
                    tokens[num_tokens].name = "€";
                } else if (strcmp(name, "not") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_NOT;
                    tokens[num_tokens].name = "!";
                }
            } else {   // if it's a variable, add the string as a new token to the array
                tokens[num_tokens].value = 0;
                tokens[num_tokens].name = name;
                tokens[num_tokens].type = TOKEN_TYPE_IDENTIFIER;
            }
            num_tokens++;

            // move the index to the end of the string
            i = j;

            // if the current character is an operator, parse it as an operator
        } else if (isOperator(&input[i])) {
            char *name = malloc(sizeof(char) * 2);
            strncpy(name, &input[i], 1);
            if (input[i] == '+') {
                tokens[num_tokens].type = TOKEN_TYPE_PLUS;
            } else if (input[i] == '-') {
                tokens[num_tokens].type = TOKEN_TYPE_MINUS;
            } else if (input[i] == '*') {
                tokens[num_tokens].type = TOKEN_TYPE_ASTERISK;
            } else if (input[i] == '&') {
                tokens[num_tokens].type = TOKEN_TYPE_AMPERSAND;
            } else if (input[i] == '|') {
                tokens[num_tokens].type = TOKEN_TYPE_PIPE;
            } else if (input[i] == '(') {
                tokens[num_tokens].type = TOKEN_TYPE_OPENPARENTHESIS;
            } else if (input[i] == ')') {
                tokens[num_tokens].type = TOKEN_TYPE_CLOSEPARENTHESIS;
            } else if (input[i] == ',') {
                tokens[num_tokens].type = TOKEN_TYPE_COMMA;
            } else if (input[i] == '=') {
                tokens[num_tokens].type = TOKEN_TYPE_EQUALS;
            }
            tokens[num_tokens].name = name;
            tokens[num_tokens].name[1] = '\0';
            tokens[num_tokens].value = 0;
            num_tokens++;
            i++;
        } else {
            i++;
        }
    }

    // return the array of tokens
    return tokens;
}

int main() {
    char input[256];
    printf("Enter input string: ");
    scanf("%s", input);

    Token *tokens = tokenizer(input);
    for (int i = 0; i < strlen(input); i++) {

        printf("Token %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, tokens[i].name, tokens[i].type,
               tokens[i].value);

    }

    free(tokens);
    return 0;
}
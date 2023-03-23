#include <stdio.h>
#include <string.h>

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
    int value = 0
    char *name = NULL;
} Token;


int main() {
    char input[MAX_LENGTH];
    int controller = 0;
    int input_length = 0;
    Token *variables[MAX_LENGTH];
    variable_count = 0;
    while (!controller) { //general while loop
        fgets(input, MAX_LENGTH, stdin); // taking input
        tokenizer(input); //tokenize the input
    }
}

Token tokenizer(char *input) { //tokenizer function
    Token token[MAX_LENGTH];
    int i = 0;
    int token_index = 0;
    char *str;
    int strlength = 0;
    input_length = strlen(input);
    for (i = 0; i < MAX_LENGTH; i++) {
        if (input[i] == '\0') {
            break;
        } else if (input[i] == ' ') {
            continue;
        } else if (isAlpha(input[i])) {
            str[strlength] = input[i];
            strlength++;
            if (isAlpha(input[i + 1]) && (input_length != (i + 1))) {
                continue;
            } else {
                if (strcmp(str, "xor") == 0) {
                    token[token_index].type = TOKEN_TYPE_XOR;
                    token[token_index].name = "^";
                } else if (strcmp(str, "not") == 0) {
                    token[token_index].type = TOKEN_TYPE_NOT;
                    token[token_index].name = "!";
                } else if (strcmp(str, "ls") == 0) {
                    token[token_index].type = TOKEN_TYPE_LS;
                    token[token_index].name = "<";
                } else if (strcmp(str, "rs") == 0) {
                    token[token_index].type = TOKEN_TYPE_RS;
                    token[token_index].name = ">";
                } else if (strcmp(str, "lr") == 0) {
                    token[token_index].type = TOKEN_TYPE_LR;
                    token[token_index].name = "$";
                } else if (strcmp(str, "rr") == 0) {
                    token[token_index].type = TOKEN_TYPE_RR;
                    token[token_index].name = "€";
                } else {
                    token[token_index].type = TOKEN_TYPE_IDENTIFIER;
                    token[token_index].name = str;
                    variables[variable_count] = token[token_index];
                    variable_count++;
                }
                token_index++;
                str = NULL;
                strlength = 0;
            }
        } else if (isDigit(input[i])) {
            str[strlength] = input[i];
            strlength++;
            if (isDigit(input[i + 1]) && (input_length != (i + 1))) {
                continue;
            } else {
                token[token_index].type = TOKEN_TYPE_NUMBER;
                token[token_index].name = atoi(str);
                token[token_index].value = atoi(str);
                token_index++;
                str = NULL;
                strlength = 0;
            }
        } else if (input[i] == '+') {
            token[token_index].type = TOKEN_TYPE_PLUS;
            token[token_index].name = "+";
            token_index++;
        } else if (input[i] == '-') {
            token[token_index].type = TOKEN_TYPE_MINUS;
            token[token_index].name = "-";
            token_index++;
        } else if (input[i] == '*') {
            token[token_index].type = TOKEN_TYPE_ASTERISK;
            token[token_index].name = "*";
            token_index++;
        } else if (input[i] == '|') {
            token[token_index].type = TOKEN_TYPE_PIPE;
            token[token_index].name = "|";
            token_index++;
        } else if (input[i] == '&') {
            token[token_index].type = TOKEN_TYPE_AMPERSAND;
            token[token_index].name = "&";
            token_index++;
        } else if (input[i] == '(') {
            token[token_index].type = TOKEN_TYPE_OPENPARENTHESIS;
            token[token_index].name = "(";
            token_index++;
        } else if (input[i] == ')') {
            token[token_index].type = TOKEN_TYPE_CLOSEPARENTHESIS;
            token[token_index].name = ")";
            token_index++;
        } else if (input[i] == '=') {
            token[token_index].type = TOKEN_TYPE_EQUALS;
            token[token_index].name = "=";
            token_index++;
            int result = calculator(infixtopostfix(tokenizer(input[i:]))); //result of the right expression
            if (strcmp(str, variables[variable_count - 1]->name) == 0) { //check if the variable is same as the last one
                variables[variable_count - 1]->value = result;
            } else { //if not, give an error
                printf("Error!");
            }
        }
    }
    return token;
}






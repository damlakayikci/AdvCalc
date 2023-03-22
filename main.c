#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 256

typedef enum {
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_PLUS,
    TOKEN_TYPE_MINUS,
    TOKEN_TYPE_ASTERISK,
    TOKEN_TYPE_SLASH,
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
} Token;

//bakalım ben de editleyebiliyor muyum

int main() {
    char input[MAX_LENGTH];
    int controller = 0;
    Token token[MAX_LENGTH];
    char *str[MAX_LENGTH];

    while (!controller) {
        fgets(input, MAX_LENGTH, stdin);
        int i = 0;
        str = strtok(input, "");
        char var_name[MAX_LENGTH] = ""; // Initialize variable name to empty string
        int var_name_len = 0; // Initialize variable name length to 0
        int state = 0; // Initialize state to 0
        while (str != NULL) {
            switch (state) {
                case 0: // Initial state
                    if (isalpha(str[i])) {
                        var_name[var_name_len++] = str[i];
                        state = 1;
                    } else if (isdigit(str[i])) {
                        state = 2;
                    } else if (str[i] == '+') {
                        state = 3;
                    } else if (str[i] == '-') {
                        state = 4;
                    } else if (str[i] == '*') {
                        state = 5;
                    } else if (str[i] == '/') {
                        state = 6;
                    } else if (str[i] == '&') {
                        state = 7;
                    } else if (str[i] == '|') {
                        state = 8;
                    } else if (str[i] == '^') {
                        state = 9;
                    } else if (str[i] == '<') {
                        state = 10;
                    } else if (str[i] == '>') {
                        state = 11;
                    } else if (str[i] == '!') {
                        state = 12;
                    } else if (str[i] == '(') {
                        state = 13;
                    } else if (str[i] == ')') {
                        state = 14;
                    } else if (str[i] == ',') {
                        state = 15;
                    } else if (str[i] == '=') {
                        state = 16;
                    } else if (str[i] == ' ') {
                        state = 0;
                    } else {
                        state = 17;
                    }
                    break;
                case 1: // Variable name
                    if (isalpha(str[i])) {
                        var_name[var_name_len++] = str[i];
                        state = 1;
                    } else if (isdigit(str[i])) {
                        var_name[var_name_len++] = str[i];
                        state = 1;
                    } else if (str[i] == '+') {
                        state = 3;
                    } else if (str[i] == '-') {
                        state = 4;
                    } else if (str[i] == '*') {
                        state = 5;
                    } else if (str[i] == '/') {
                        state = 6;
                    } else if (str[i] == '&') {
                        state = 7;
                    } else if (str[i]



    }





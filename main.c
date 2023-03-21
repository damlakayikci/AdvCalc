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
}   TokenType;


typedef struct {
    TokenType type;
    int value;
}   Token;

int main() {
    char input[MAX_LENGTH];
    int controller = 0;
    Token token[MAX_LENGTH];
    char *str[MAX_LENGTH];

    while (!controller) {
        fgets(input, MAX_LENGTH, stdin);
        int i = 0;
        str[i] = strtok(input, "");

    }

}

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

/*
Token** tokenizer(char *input, int variable_count, Token* variables) { //tokenizer function
    Token *token= malloc(MAX_LENGTH * sizeof(Token));  // allocate memory for token array
    int i = 0;
    int token_index = 0;
    char str[MAX_LENGTH];
    int strlength = 0;
   // int input_length = strlen(input);

    char *token_str = strtok(input, "");
    while (token_str != NULL && num_tokens < MAX_TOKENS) {
        Token *token = &tokens[num_tokens];
        token->name = malloc(MAX_TOKEN_LEN * sizeof(char));
        strncpy(token->name, token_str, MAX_TOKEN_LEN);
        token->value = num_tokens + 1;
        num_tokens++;
        token_str = strtok(NULL, " \t\n");
    }
    tokens[num_tokens].name = NULL;
    return tokens;
    for (i = 0; i < MAX_LENGTH; i++) {
        if (input[i] == '\0') {
            break;
        } else if (input[i] == ' ') {
            continue;
        } else if (isalpha(input[i])) {
            str[strlength] = input[i];
            strlength++;
            if (isalpha(input[i + 1]) && (input_length != (i + 1))) {
                continue;
            } 
            
        } else if (isdigit(input[i])) {
            str[strlength] = input[i];
            strlength++;
            if (isdigit(input[i + 1]) && (input_length != (i + 1))) {
                continue;
            } else {
                token[token_index]->type = TOKEN_TYPE_NUMBER;
                token[token_index]->name = atoi(str);
                token[token_index]->value = atoi(str);
                token_index++;
                str = NULL;
                strlength = 0;
            }
        } else { //if not, give an error
                printf("Error!");
            }
        
    }
    return *token;
}


int main() {
    char input[MAX_LENGTH] = "a = 1 + 2";
    Token *variables[MAX_LENGTH];
    int variable_count = 0;
}




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int value;
    char* name;
} Token;
*/
Token* tokenizer(char *input) {
    int input_length = strlen(input);
    Token *tokens = malloc(sizeof(Token) * input_length);
    int num_tokens = 0;
    
    int i = 0;
    while (i < input_length) {
        if (isdigit(input[i])) {
            int j = i;
            while (isdigit(input[j])) {
                j++;
            }
            char *num_str = malloc(sizeof(char) * (j - i + 1));
            strncpy(num_str, &input[i], j - i);
            num_str[j-i] = '\0';
            int num = atoi(num_str);
            tokens[num_tokens].value = num;
            tokens[num_tokens].name = NULL;
            num_tokens++;
            i = j;
            free(num_str);
        } else if (isalpha(input[i])) {
            int j = i;
            while (isalpha(input[j])) {
                j++;
            }
            char *name = malloc(sizeof(char) * (j - i + 1));
            strncpy(name, &input[i], j - i);
            name[j-i] = '\0';
            tokens[num_tokens].value = 0;
            tokens[num_tokens].name = name;
            num_tokens++;
            i = j;
        } else {
            i++;
        }
    }
    return tokens;
}

int main() {
    char input[256];
    printf("Enter input string: ");
    scanf("%s", input);
    
    Token *tokens = tokenizer(input);
    for (int i = 0; i < strlen(input); i++) {
        if (tokens[i].name != NULL) {
            printf("Token %d: %s\n", i+1, tokens[i].name);
        } else {
            printf("Token %d: %d\n", i+1, tokens[i].value);
        }
    }
    
    free(tokens);
    return 0;
}
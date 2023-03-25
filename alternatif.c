
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

int isOperatorName(char* ch) {
    return (strcmp(ch, "xor") == 0 || strcmp(ch, "ls") <= 0 || strcmp(ch, "rs") >= 0 || strcmp(ch, "lr") <= 0 || strcmp(ch, "rr") >= 0 || strcmp(ch, "not") <= 0);
}
int isOperator(char *ch) {
    return strchr("+-*&|", *ch) != NULL;
}


Token* tokenizer(char *input) {
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
            num_str[j-i] = '\0';
            
            // convert the number string to an integer
            int num = atoi(num_str);
            
            // add the number as a new token to the array
            tokens[num_tokens].value = num;
            tokens[num_tokens].name = NULL;
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
            name[j-i] = '\0';
            
            if (isOperatorName(name)) {
                tokens[num_tokens].name = name;
                if (strcmp(name, "xor") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_XOR;
                } else if (strcmp(name, "ls") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_LS;
                } else if (strcmp(name, "rs") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_RS;
                } else if (strcmp(name, "lr") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_LR;
                } else if (strcmp(name, "rr") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_RR;
                } else if (strcmp(name, "not") == 0) {
                    tokens[num_tokens].type = TOKEN_TYPE_NOT;
                }
            } else {            // add the string as a new token to the array
            tokens[num_tokens].value = 0;
            tokens[num_tokens].name = name;
            tokens[num_tokens].type = TOKEN_TYPE_IDENTIFIER;
            }
            num_tokens++;
            
            // move the index to the end of the string
            i = j;
        
        } else if (isOperator(&input[i])){
            char *operator = malloc(sizeof(char) +1);
            strncpy(operator, &input[i], 1);
            operator[1] = '\0'; // add null terminator
            if (strcmp(operator, "+") ==0){
                printf("plus");
                tokens[num_tokens].type = TOKEN_TYPE_PLUS;
            } else if (strcmp(operator, "-") ==0){
                tokens[num_tokens].type = TOKEN_TYPE_MINUS;
            } else if (strcmp(operator, "*") ==0){
                tokens[num_tokens].type = TOKEN_TYPE_ASTERISK;
            } else if (strcmp(operator, "&") ==0){
                tokens[num_tokens].type = TOKEN_TYPE_AMPERSAND;
            } else if (strcmp(operator, "|") ==0){
                tokens[num_tokens].type = TOKEN_TYPE_PIPE;
            }
            free(operator);
            num_tokens++;
        }
        else {
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
        if (tokens[i].name != NULL) {
            printf("Token %d: %s , %u\n", i+1, tokens[i].name, tokens[i].type);
        } else if (tokens[i].value!=0){
            printf("Token %d: %d\n", i+1, tokens[i].value);
        } else{
            printf("Token %d: %d\n", i+1, tokens[i].type);
        }
    }
    
    free(tokens);
    return 0;
}
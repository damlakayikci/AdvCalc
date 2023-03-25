
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


Token *tokenizer(char *input, int *num_tokens) {
    // get the length of the input string
    int input_length = strlen(input);

    // allocate memory for the array of tokens
    Token *tokens = malloc(sizeof(Token) * input_length);

    // keep track of the number of tokens found so far
    //int *num_tokens = 0;

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
            tokens[*num_tokens].value = num;
            tokens[*num_tokens].name = "num_str";
            tokens[*num_tokens].type = TOKEN_TYPE_NUMBER;
            (*num_tokens)++;

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
                tokens[*num_tokens].name = name;
                if (strcmp(name, "xor") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_XOR;
                    tokens[*num_tokens].name = "^";
                } else if (strcmp(name, "ls") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_LS;
                    tokens[*num_tokens].name = "<";
                } else if (strcmp(name, "rs") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_RS;
                    tokens[*num_tokens].name = ">";
                } else if (strcmp(name, "lr") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_LR;
                    tokens[*num_tokens].name = "$";
                } else if (strcmp(name, "rr") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_RR;
                    tokens[*num_tokens].name = "#";
                } else if (strcmp(name, "not") == 0) {
                    tokens[*num_tokens].type = TOKEN_TYPE_NOT;
                    tokens[*num_tokens].name = "!";
                }
            } else {   // if it's a variable, add the string as a new token to the array
                tokens[*num_tokens].value = 0;
                tokens[*num_tokens].name = name;
                tokens[*num_tokens].type = TOKEN_TYPE_IDENTIFIER;
            }
            (*num_tokens)++;

            // move the index to the end of the string
            i = j;

            // if the current character is an operator, parse it as an operator
        } else if (isOperator(&input[i])) {
            char *name = malloc(sizeof(char) * 2);
            strncpy(name, &input[i], 1);
            if (input[i] == '+') {
                tokens[*num_tokens].type = TOKEN_TYPE_PLUS;
            } else if (input[i] == '-') {
                tokens[*num_tokens].type = TOKEN_TYPE_MINUS;
            } else if (input[i] == '*') {
                tokens[*num_tokens].type = TOKEN_TYPE_ASTERISK;
            } else if (input[i] == '&') {
                tokens[*num_tokens].type = TOKEN_TYPE_AMPERSAND;
            } else if (input[i] == '|') {
                tokens[*num_tokens].type = TOKEN_TYPE_PIPE;
            } else if (input[i] == '(') {
                tokens[*num_tokens].type = TOKEN_TYPE_OPENPARENTHESIS;
            } else if (input[i] == ')') {
                tokens[*num_tokens].type = TOKEN_TYPE_CLOSEPARENTHESIS;
            } else if (input[i] == ',') {
                tokens[*num_tokens].type = TOKEN_TYPE_COMMA;
            } else if (input[i] == '=') {
                tokens[*num_tokens].type = TOKEN_TYPE_EQUALS;
            }
            tokens[*num_tokens].name = name;
            tokens[*num_tokens].name[1] = '\0';
            tokens[*num_tokens].value = 0;
            (*num_tokens)++;
            i++;
        } else {
            i++;
        }
    }

    // return the array of tokens
    return tokens;
}

int isFunctionOperator(char *ch){
    return (strcmp(ch, "^") == 0 || strcmp(ch, "<") == 0 || strcmp(ch, ">") == 0 || strcmp(ch, "$") == 0 || strcmp(ch, "#") == 0);
}

Token *formatController(Token *input, int inputSize, int recursive){
    int i = 0;
    int output_count = 0;      //count the output tokens
    Token *output = malloc(sizeof(Token) * inputSize);
    while(i < inputSize){       //for each token
        int parenthesisCount = 0;       //count the parenthesis
        if(isFunctionOperator(input[i].name)){     //if it is a function operator
            parenthesisCount++;
            int j = 0;
            while(parenthesisCount != 0){    //while the parenthesis are not closed
                if(i+j == inputSize){    //if the end of the input is reached
                    printf("Error!");
                    j++;
                    break;
                }else if(input[i+j].type == TOKEN_TYPE_OPENPARENTHESIS){       //if it is an open parenthesis
                    output[i+j].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
                    output[i+j].name = "(";
                    output_count++;    //increase the output count
                    parenthesisCount++;
                    j++;
                }else if(input[i+j].type == TOKEN_TYPE_CLOSEPARENTHESIS){      //if it is a close parenthesis
                    output[i+j].type = TOKEN_TYPE_CLOSEPARENTHESIS;    //add it to the output
                    output[i+j].name = ")";
                    output_count++;   //increase the output count
                    parenthesisCount--;
                    j++;
                }else if(isFunctionOperator(input[i+j].name)){     //if it is a function operator
                    Token *anotherFunctionOperator = malloc(sizeof(Token) * inputSize-i-j);   //create a new array for the tokens for the recursive call
                    int k = 0;
                    while(k < inputSize-i-j){       //copy the tokens to another array
                        anotherFunctionOperator[k].type = input[i+j+k].type;
                        anotherFunctionOperator[k].name = input[i+j+k].name;
                        k++;
                    }
                    Token *anotherOutput = formatController(anotherFunctionOperator, inputSize-i-j, 1);   //recursive call
                    int l = 0;
                    while(anotherOutput[l].name != NULL){    //count the number of the tokens in the output of the recursive call
                        l++;
                    }
                    int m = 0;
                    while(m < l){   //copy the tokens to the output
                        output[i+j+m].type = anotherOutput[m].type;
                        output[i+j+m].name = anotherOutput[m].name;
                        output_count++;   //increase the output count
                        m++;
                    }
                    j+=l;   //skip the tokens that are already processed
                }else if(strcmp(input[i+j].name, ",") == 0){    //if it is a comma
                    output[i+j].type = input[i].type;   //insert the operator to the output
                    output[i+j].name = input[i].name;
                    output_count++;   //increase the output count
                    //change the stage
                    j++;
                }
            }
            i+=j;    //skip the tokens that are already processed
            if (recursive == 1) {   //if it is a recursive call, return the output
                return output;
            }
        }else if(strcmp(input[i].name, ",") == 0) {
            printf("Error!");   //if it is a comma, give an error
            break;
        }else{
            output[i].type = input[i].type;   //if it is not a function operator, add it to the output
            output[i].name = input[i].name;
            output_count++;  //increase the output count
            i++;
        }
    }
    return output;
}

int main() {
    int num_tokens = 0;
    char input[256];
    printf("Enter input string: ");
    fgets(input, 256, stdin);
    Token *tokens = tokenizer(input, &num_tokens);
//    for (int i = 0; i < strlen(input); i++) {
//        printf("Token %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, tokens[i].name, tokens[i].type,
//               tokens[i].value);
//    }
//    printf("Number of tokens: %d\n", num_tokens);

    Token *output = formatController(tokens, num_tokens, 0);
    int i = 0;
    while(output[i].name != NULL){
        printf("Token %d: Name: %s\t\t Type: %u\t\t Value: %d\n", i + 1, output[i].name, output[i].type,
               output[i].value);
        i++;
    }

    free(tokens);
    return 0;
}
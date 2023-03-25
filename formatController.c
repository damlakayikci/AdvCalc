
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256


Token formatController(Token input){
    int i = 0;
    Token *output[strlen(input)];
    while(i < strlen(input)){
        if(isOperator(input[i]->name) && i < strlen(input) - 5){
            Token *flag = malloc(sizeof(Token) * strlen(input));
            int flag_count = 0;
            Token *flag2 = malloc(sizeof(Token) * strlen(input));
            int flag_count2 = 0;
            int flag_comma = 0;
            int parenthesis_count = 0;
            int j = i+1;
            while(j < strlen(input)){
                if (input[i+1]->name != "("){
                    printf("Error: Invalid syntax");
                    return 0;
                }
                else if(flag_comma == 0){
                    if(input[j]->name == ","){
                        flag[flag_count].name = input[i]->name;
                        flag[flag_count].type = input[i]->type;
                        flag[flag_count].value = input[i]->value;
                        flag_count++;
                        flag_comma = 1;
                    }
                    else if(input[j]->name == ")"){
                        printf("Error: Invalid syntax");
                        return 0;
                    }
                    else if(isOperator(input[j]->name)){
                        char *anotherOperator[strlen(input)-j];
                        int k = 0;
                        while(k < strlen(input)-j){
                            anotherOperator[k] = input[j+k]->name;
                        }
                        strcat(flag,anotherOperator);
                    }
                    else{
                        flag[flag_count] = input[j]->name;
                        flag_count++;
                    }
                }
                else if(flag_comma == 1){
                    if(input[j]->name == ")"){
                        return
                    }
                    else if(input[j]->name == ","){
                        printf("Error: Invalid syntax");
                        return 0;
                    }
                    else if(isOperator(input[j]->name)){
                        char *anotherOperator[strlen(input)-j];
                        int k = 0;
                        while(k < strlen(input)-j){
                            anotherOperator[k] = input[j+k]->name;
                        }
                        strcat(flag2,anotherOperator);
                    }
                    else{
                        flag2[flag_count2] = input[j]->name;
                        flag_count2++;
                    }

                }
                j++;
            }
        }
        else if(input[i]->name == "!" && i < strlen(input) - 3){

        }
        output[i] = input[i];
        i++;
    }
}

if (isOperator(&input[i])){
    char *operator = malloc(sizeof(char) +1);
    strncpy(operator, &input[i], 1);
    operator[1] = '\0'; // add null terminator
    if (strcmp(operator, "+") ==0){
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
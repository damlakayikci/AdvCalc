// #include <stdio.h>
// #include <string.h>
// #include <ctype.h>
// #include <stdlib.h>
// #include "token.h"

#define MAX_LENGTH 256

int isFunctionOperator(char *ch) {
    return (strcmp(ch, "^") == 0 || strcmp(ch, "<") == 0 || strcmp(ch, ">") == 0 || strcmp(ch, "$") == 0 ||
            strcmp(ch, "#") == 0);
}

int shouldntRepeat(char *ch) {
    return strchr("+-*&|,=", *ch) != NULL;
}
// int isExpression (Token *input, int parenthesisCount, int index){
//     if (input[index].type == TOKEN_TYPE_OPENPARENTHESIS){
//         parenthesisCount++;
//         isExpression(input, parenthesisCount, ++index);
//     }
//     else if (input[index].type == TOKEN_TYPE_CLOSEPARENTHESIS){
//         parenthesisCount--;
//         index++;
//     }
//     if (parenthesisCount == 0){
//         return 1;
//     }
//     return 0;


// }


Token *formatController(Token *input, int inputSize, int recursive, int *index, int *output_count) {
    Token *output = malloc(sizeof(Token) * inputSize);
    int recIndex;
    int nonRecIndex;
    if ((inputSize >= 2) && (input[1].type == TOKEN_TYPE_EQUALS) && (input[0].type != TOKEN_TYPE_IDENTIFIER)) {
        printf("Error: Equal sign\n");
        return NULL;
    }
    if (!recursive) { // Initial check
        for (int d =0; d<inputSize;d++) {       //for each token
            // for repeated tokens return NULL
            if (input[d].type == TOKEN_TYPE_IDENTIFIER && input[d + 1].type == TOKEN_TYPE_IDENTIFIER) {
                printf("Error: Repeated identifier\n");
                return NULL;
            }
            if (input[d].type == TOKEN_TYPE_NUMBER && input[d + 1].type == TOKEN_TYPE_NUMBER) {
                printf("Error: Repeated number\n");
                return NULL;
            }
            if (shouldntRepeat(input[d].name) && shouldntRepeat(input[d+ 1].name)) {
                printf("Error: Repeated symbol\n");
                return NULL;
            }
            // empty paranthesis
            if (input[d].type == TOKEN_TYPE_OPENPARENTHESIS && input[d + 1].type == TOKEN_TYPE_CLOSEPARENTHESIS) {
                printf("Error: Empty parenthesis\n");
                return NULL;
            }
        }
    }
    while ((*index) < inputSize) {       //for each token

        int parenthesisCount = 0;       //count the parenthesis
        if (isFunctionOperator(input[(*index)].name)) {     //if it is a function operator
            if (strcmp(input[(*index) + 1].name, "(") == 0) {   //if next is an open parenthesis
                output[(*index) + 1].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
                output[(*index) + 1].name = "(";
                (*output_count)++;    //increase the output count
                parenthesisCount++;
            } else { // BURASI CALISIYOR
                printf("Error: Expected open parenthesis after function operator\n");
                return NULL;
            }
            if (recursive) {
                recIndex = *index;
            } else {
                nonRecIndex = *index;
            }
            int j = 2;
            int commaCount = 0;
            while (parenthesisCount != 0) {   //while the parenthesis are not closed
                if ((*index) + j == inputSize) {    //if the end of the input is reached
                    printf("Error: Parenthesis not closed\n");
                    return NULL;
                } else if (input[(*index) + j].type ==
                           TOKEN_TYPE_OPENPARENTHESIS) {       //if it is an open parenthesis
                    output[(*index) + j].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
                    output[(*index) + j].name = "(";
                    (*output_count)++;    //increase the output count
                    parenthesisCount++;
                    j++;
                } else if (input[(*index) + j].type ==
                           TOKEN_TYPE_CLOSEPARENTHESIS) {      //if it is a close parenthesis
                    output[(*index) + j].type = TOKEN_TYPE_CLOSEPARENTHESIS;    //add it to the output
                    output[(*index) + j].name = ")";
                    (*output_count)++;   //increase the output count
                    parenthesisCount--;
                    j++;
                } else if (isFunctionOperator(input[(*index) + j].name)) {     //if it is a function operator
                    (*index) += j;
                    Token *inner = formatController(input, inputSize, 1, index, output_count);
                    if (inner == NULL) {
                        printf("Error: Inner function operator\n");
                        return NULL;
                    }
                    (*index) -= j;
//xor(xor(a,b),c)
                    for (int k = 0; k < inputSize; k++) {
                        if (inner[k].name != NULL) {
                            output[k] = inner[k];
                        }
                    }
                } else if (strcmp(input[(*index) + j].name, ",") == 0) {    //if it is a comma
                    if (recursive) {
                        output[(*index) + j].type = input[recIndex].type;   //insert the operator to the output
                        output[(*index) + j].name = input[recIndex].name;
                        output[(*index) + j].value = input[recIndex].value;

                    } else {
                        output[(*index) + j].type = input[nonRecIndex].type;   //insert the operator to the output
                        output[(*index) + j].name = input[nonRecIndex].name;
                        output[(*index) + j].value = input[nonRecIndex].value;

                    }
                    commaCount++;
                    (*output_count)++;   //increase the output count
                    //change the stage
                    j++;
                } else {
                    output[(*index) + j].type = input[(*index) + j].type;
                    output[(*index) + j].name = input[(*index) + j].name;
                    output[(*index) + j].value = input[(*index) + j].value;
                    (*output_count)++;   //increase the output count
                    j++;
                }
            }
            if (commaCount != 1) {   //if there is not 1 comma inside expression
                printf("Error: Non-expected number of commas\n");
                return NULL;
            }
            (*index) += j;    //skip the tokens that are already processed
            if (recursive == 1) {   //if it is a recursive call, return the output
                return output;
            }

        } else if (strcmp(input[(*index)].name, ",") == 0) {
            printf("Error: Unexpected comma\n");
            return NULL;
        } else {
            output[(*index)].type = input[(*index)].type;   //if it is not a function operator, add it to the output
            output[(*index)].name = input[(*index)].name;
            output[(*index)].value = input[(*index)].value;
            (*output_count)++;  //increase the output count
            (*index)++;
        }
    }
    return output;
}
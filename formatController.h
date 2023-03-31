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

//
//Token *check(Token *input, int startIndex, int endIndex) {
//    if (startIndex == endIndex - 1) {
//        return NULL;
//    }
//    // If the expression doesn't start with an identifier or a number, return NULL
////    if (input[startIndex + 1].control != CONTROL_TYPE_EXPRESSION && input[startIndex + 1].control != CONTROL_TYPE_TERM) {
////        return NULL;
////    }
//    int isExpression = 0; // boolean if the last token was an expression or an operator
//    int index = startIndex + 1; // start from index after open parenthesis
//    while (index < endIndex) {
//        if (isExpression) {  // if expression
//            //an operator should come after an expression, if not, return NULL
//            if (input[index].control != CONTROL_TYPE_OPERATOR) {
//                return NULL;
//            }
//            isExpression = 0;
//        } else { // if operator
//            // if last index is an operator, return NULL
//            if (index == endIndex - 1) {
//                return NULL;
//            }
//            //an expression should come after an operator
//            if (input[index].control != CONTROL_TYPE_EXPRESSION && input[index].control != CONTROL_TYPE_TERM) {
//                return NULL;
//            }
//            // if we have an expression, go until the end of it
//            if (input[index].control == CONTROL_TYPE_EXPRESSION) {
//                index += input[index + 1].checkedUntil;
//            }
//            isExpression = 1;
//        }
//        for (int i = startIndex + 1; i < endIndex; i++) {
//            input[i].checkedUntil = endIndex - startIndex;
//        }
//
//        index++;
//    }
//
//
//    return input;
//}
//
//int isExpression(Token *input, int parenthesisCount, int index, int input_length, int lastOpenParenthesisIndex) {
//
//    while (parenthesisCount != 0) {
//        if (input[index].type == TOKEN_TYPE_OPENPARENTHESIS) {
//            parenthesisCount++;
//            isExpression(input, parenthesisCount, ++index, input_length, index);
//        } else if (input[index].type == TOKEN_TYPE_CLOSEPARENTHESIS) {
//            check(input, lastOpenParenthesisIndex, index);
//            parenthesisCount--;
//            index++;
//        }
//    }
//
//    if (input[index].type == TOKEN_TYPE_IDENTIFIER || input[index].type == TOKEN_TYPE_NUMBER) {
//        input[index].control = CONTROL_TYPE_EXPRESSION;
//    } else if (isFunctionOperator(input[index].name)) {
//        input[index].control = CONTROL_TYPE_OPERATOR;
//    }
//
//
//    if (parenthesisCount == 0) {
//        return 1;
//    }
//    return 0;
//}


Token *formatController(Token *input, int inputSize, int recursive, int *index, int *output_count) {
    Token *output = malloc(sizeof(Token) * inputSize);
    int recIndex;
    int nonRecIndex;
    // if there is an equal sign it should be in the second position and there should be an identifier before it
    if ((input[1].type == TOKEN_TYPE_EQUALS) && ((input[0].type != TOKEN_TYPE_IDENTIFIER) || (inputSize <= 2))) {
        printf("Error: Equal sign\n");
        return NULL;
    }
    if (!recursive) { // Initial check for errors
        for (int d = 0; d < inputSize - 1; d++) {       //for each token
            // for repeated tokens return NULL
            if (input[d].type == TOKEN_TYPE_IDENTIFIER &&
                (input[d + 1].type == TOKEN_TYPE_IDENTIFIER || input[d + 1].type == TOKEN_TYPE_NUMBER ||
                 strcmp(input[d + 1].name, "(") == 0)) {
                printf("Error: Repeated identifier\n");
                return NULL;
            }
            if (input[d].type == TOKEN_TYPE_NUMBER &&
                (input[d + 1].type == TOKEN_TYPE_IDENTIFIER || input[d + 1].type == TOKEN_TYPE_NUMBER ||
                 strcmp(input[d + 1].name, "(") == 0)) {
                printf("Error: Repeated number\n");
                return NULL;
            }
            if (shouldntRepeat(input[d].name) && shouldntRepeat(input[d + 1].name)) {
                printf("Error: Repeated symbol\n");
                return NULL;
            }
            // empty parenthesis
            if (strcmp(input[d].name, "(") == 0 && strcmp(input[d + 1].name, ")") == 0) {
                printf("Error: Empty parenthesis\n");
                return NULL;
            }
            if (input[d].type == TOKEN_TYPE_EQUALS && d != 1) { // if equal sign is not in the second index
                printf("Equal sign is in wrong index \n");
                return NULL;
            }
        }
    }
    int parenthesisCount = 0;       //count the parenthesis
    while ((*index) < inputSize) {       //for each token

        if (isFunctionOperator(input[(*index)].name)) {     //if it is a function operator
            // if the end of the input is reached return NULL
            if ((*index) + 1 == inputSize) {
                printf("Error: Expected open parenthesis after function operator\n");
                return NULL;
            }

            if (strcmp(input[(*index) + 1].name, "(") == 0) {   //if next is an open parenthesis
                output[(*index) + 1].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
                output[(*index) + 1].name = "(";
                (*output_count)++;    //increase the output count
                parenthesisCount++;
            } else {
                printf("Error: Expected open parenthesis after function operator\n");
                return NULL;
            }

            // determine the index of the operator
            if (recursive) {
                recIndex = *index;
            } else {
                nonRecIndex = *index;
            }

            int j = 2; // the index of the next token after the open parenthesis
            int commaCount = 0;
            while (parenthesisCount != 0) {   //while the parenthesis are not closed
                if ((*index) + j == inputSize) {    //if the end of the input is reached parenthesis are not closed !
                    printf("Error: Parenthesis not closed\n");
                    return NULL;
                }

                    // increase the parenthesis count if an open parenthesis is found
                else if (strcmp(input[(*index) + j].name, "(") == 0) {       //if it is an open parenthesis
                    output[(*index) + j].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
                    output[(*index) + j].name = "(";
                    (*output_count)++;    //increase the output count
                    parenthesisCount++;
                    j++;
                }
                    // decrease the parenthesis count if a close parenthesis is found
                else if (strcmp(input[(*index) + j].name, ")") == 0) {      //if it is a close parenthesis
                    output[(*index) + j].type = TOKEN_TYPE_CLOSEPARENTHESIS;    //add it to the output
                    output[(*index) + j].name = ")";
                    (*output_count)++;   //increase the output count
                    parenthesisCount--;
                    j++;
                }
                    // if another function operator is found, we go into recursion
                else if (isFunctionOperator(input[(*index) + j].name)) {     //if it is a function operator
                    (*index) += j; //increase the index to for recursion function parameter
                    Token *inner = formatController(input, inputSize, 1, index, output_count);

                    // if there is an error in the inner function return NULL
                    if (inner == NULL) {
                        printf("Error: Inner function operator\n");
                        return NULL;
                    }

                    // re-adjust the index
                    (*index) -= j;

                    // add the new inner function elements to the output
                    for (int k = 0; k < inputSize; k++) {
                        if (inner[k].name !=
                            NULL) { // if the token is empty it's out of inner function scope, pass them
                            output[k] = inner[k];
                        }
                    }

                    //if we have a comma replace it with the operator
                } else if (strcmp(input[(*index) + j].name, ",") == 0) {
                    if (parenthesisCount != 1) { // if the comma is not in the first level of parenthesis
                        return NULL;
                    }
                    if (recursive) {
                        output[(*index) + j].type = input[recIndex].type;
                        output[(*index) + j].name = input[recIndex].name;
                        output[(*index) + j].value = input[recIndex].value;

                    } else {
                        output[(*index) + j].type = input[nonRecIndex].type;
                        output[(*index) + j].name = input[nonRecIndex].name;
                        output[(*index) + j].value = input[nonRecIndex].value;

                    }
                    commaCount++;  //increase the comma count
                    (*output_count)++;   //increase the output count
                    //change the stage
                    j++;
                }

                    // else copy whatever we have to output array
                else {
                    output[(*index) + j].type = input[(*index) + j].type;
                    output[(*index) + j].name = input[(*index) + j].name;
                    output[(*index) + j].value = input[(*index) + j].value;
                    (*output_count)++;   //increase the output count
                    j++;
                }
            }

            //if there is not 1 comma inside expression
            if (commaCount != 1) {
                printf("Error: Non-expected number of commas\n");
                return NULL;
            }
            (*index) += j;    //skip the tokens that are already processed

            //if it is a recursive call, return the output
            if (recursive == 1) {
                return output;
            }

            // if we have comma outside of operator, return NULL
        } else if (strcmp(input[(*index)].name, ",") == 0) {
            printf("Error: Unexpected comma\n");
            return NULL;
        } else if (strcmp(input[(*index)].name, "(") == 0) {   //if it is an open parenthesis
            output[(*index)].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
            output[(*index)].name = "(";
            output[(*index)].value = input[(*index)].value;
            (*output_count)++;    //increase the output count
            (*index)++;
            parenthesisCount++;
        } else if (strcmp(input[(*index)].name, ")") == 0) {      //if it is a close parenthesis
            output[(*index)].type = TOKEN_TYPE_CLOSEPARENTHESIS;    //add it to the output
            output[(*index)].name = ")";
            output[(*index)].value = input[(*index)].value;
            (*output_count)++;   //increase the output count
            (*index)++;
            parenthesisCount--;
        }

            // else copy whatever we have to output array
        else {
            output[(*index)].type = input[(*index)].type;
            output[(*index)].name = input[(*index)].name;
            output[(*index)].value = input[(*index)].value;
            (*output_count)++;  //increase the output count
            (*index)++;
        }
    }
    if (parenthesisCount != 0) {
        printf("Error: Parenthesis not closed\n");
        return NULL;
    }
    return output;
}
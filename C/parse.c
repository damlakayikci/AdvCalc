#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char str[] = "apple+banana - c";
    char *token;

    token = strtok(str, "+-");
    while (token != NULL) {
        // Remove spaces from token
        int i = 0, j = 0;
        while (token[i] != '\0') {
            if (!isspace(token[i])) {
                token[j] = token[i];
                j++;
            }
            i++;
        }
        token[j] = '\0';

        printf("%s\n", token);

    }

    return 0;
}


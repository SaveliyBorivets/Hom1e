#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define ADD_SIZE 20

int main(){
    size_t com_mas_memory = ADD_SIZE;
    char **com_mas = (char **)malloc(sizeof(char *) * com_mas_memory);
    size_t count_com = 0;
    char ch;
    do {
        size_t command_memory = ADD_SIZE;
        char *command = (char *)malloc(sizeof(char) * command_memory);
        size_t count_ch = 0;
        while((ch = getchar()) != '\n'){
            if (count_ch + 3 > command_memory) {
                command_memory += ADD_SIZE;
                command = (char *)realloc(command, sizeof(char) * command_memory);
            }
            command[count_ch] = ch;
            count_ch++;
            command[count_ch] = '\0';
            if (strcmp(command, "Fin.\0") == 0) break;
        }
        if (count_com + 2 > com_mas_memory) {
            com_mas_memory += ADD_SIZE;
            com_mas = (char **)realloc(com_mas, sizeof(char *) * com_mas_memory);
        }
        com_mas[count_com] = command;
        count_com++;
    } while (strcmp(com_mas[count_com - 1], "Fin.\0") != 0);
    regex_t buf;
    char *pattern = "(\\w+)@[a-zA-Z0-9_-]+: ?~ ?# (.*)";
    int rc;
    size_t nmatch = 3;
    regmatch_t match[3];
    if ((rc = regcomp(&buf, pattern, REG_EXTENDED)) == 0) {
        for(int i = 0; i < count_com; i++) {
            if ((rc = regexec(&buf, com_mas[i], nmatch, match, 0)) == 0) {
                for (int j = match[1].rm_so; j < match[1].rm_eo; j++) {
                    printf("%c", com_mas[i][j]);
                }
                printf(" - ");
                for (int j = match[2].rm_so; j < match[2].rm_eo; j++) {
                    printf("%c", com_mas[i][j]);
                }
                printf("\n");
            }
        }
    } else {
        printf("Ошибка: Регулярное выражение некорректно");
    }
    regfree(&buf);
    for(int i = 0; i < count_com; i++) {
        free(com_mas[i]);
    }
    free(com_mas);
    return 0;
}
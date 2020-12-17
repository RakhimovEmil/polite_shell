#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

char* req_reading() {

    size_t capacity = 0;
    char* cur_buf = NULL;
    getline(&cur_buf, &capacity, stdin);
    return cur_buf;

}

char** req_parsing(char* cur_string) {

    size_t capacity = 64, it = 0;
    char *arg = strtok(cur_string, " \t\r\n\a");
    char **args = malloc(sizeof(char *) * capacity);

    while (arg != NULL) {
        args[it] = arg;
        ++it;

        if (it == capacity) {
            capacity += 64;
            args = realloc(args, sizeof(char *) * capacity);
        }

        arg = strtok(NULL, " \t\r\n\a");
    }

    args[it] = NULL;
    return args;
}

int psh_cmd(char **args, size_t it, size_t end_of_cmd, int number_of_cmd) {
    pid_t pid, wpid;
    int cur_status;

    pid = fork();

    if (pid == 0) {

        if (number_of_cmd == 1) {       // could tou call __ please

            printf("Sure:\n");

            if (it < end_of_cmd) {

                char** cmd;
                cmd = malloc(sizeof(char) * (end_of_cmd - it));
                for (size_t i = it; i < end_of_cmd; ++i) {
                    cmd[i - it] = args[i];
                    printf("%s ", args[i]);
                }
                printf("\n");
                execvp(cmd[0], cmd);

                free(cmd);

            } else {
                execvp(args[it], args);
            }

        } else if (number_of_cmd == 2) {
            printf("I will try:\n");

            char* command = "which";
            char** str = malloc(sizeof(char));
            str[0] = command;
            str[1] = args[it];
            str[2] = NULL;

            if (execvp(str[0], str) == -1) {
                printf("Unfortunately I didn't succeed\n");
            }

            free(command);
            free(str);
        } else if (number_of_cmd == 3) {
            printf("sure:\n");

            char* command = "cd";
            char** str = malloc(sizeof(char));
            str[0] = command;
            str[1] = args[it];
            str[2] = NULL;

            if (execvp(str[0], str) == -1) {
                printf("Unfortunately I can't go where\n");
            }

            free(command);
            free(str);
        }

        exit(EXIT_FAILURE);

    } else {

        if (pid < 0) {
            printf("Sorry, smth went wrong when you tried to call\n");
        } else {

            do {
                wpid = waitpid(pid, &cur_status, WUNTRACED);
            } while (!WIFEXITED(cur_status) && !WIFSIGNALED(cur_status));

        }

    }

    return 1;
}

int req_answering(char** args) {
    if (args[0] == NULL) {
        printf("You didn't write anything\n");
        return -1;
    }

    if (strcmp(args[0], "exit") == 0 ) {
        return 0;
    }

    size_t it = 0;
    while (args[it] != NULL) {
        ++it;
    }
    --it;

    if (it > 3                         &&
        strcmp(args[0],  "could") == 0 &&
        strcmp(args[1],  "you") == 0   &&
        strcmp(args[2],  "call") == 0  &&
//                       "smth"
        strcmp(args[it], "please?") == 0) {

        return psh_cmd(args, 3, it, 1);

    } else {

        if (it >= 3                     &&
            strcmp(args[0], "do") == 0  &&
            strcmp(args[1], "you") == 0 &&
            strcmp(args[2], "know") == 0) {

            return psh_cmd(args, 3, 3, 2);

        } else {

            if (it >= 3                       &&
                strcmp(args[0], "lets") == 0  &&
                strcmp(args[1], "go") == 0    &&
                strcmp(args[2], "to") == 0) {

                return psh_cmd(args, 3, 3, 3);

            } else {
                printf("you are so rude\n");
                return -1;
            }

        }

    }

}

int main() {

    char* cur_req;
    char** parts_of_req;
    int cur_status;

    do {
        printf("> ");
        cur_req = req_reading();
        parts_of_req = req_parsing(cur_req);
        cur_status = req_answering(parts_of_req);

        free(cur_req);
        free(parts_of_req);
    } while (cur_status);

    return 0;
}

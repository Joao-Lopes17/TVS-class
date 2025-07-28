#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "./libs/prompt.h"

#define INPUT_MAX_SIZE 1025

#define SIZE_OF(arr) sizeof(arr)/sizeof(arr[0])

const char *INPUT_PROMPT = "$ ";

int main(int argc, const char *argv[]) 
{
    bool shell_active = true;
    char input[INPUT_MAX_SIZE];
    char *path = getcwd(NULL, 0);

    do{
        fprintf(stdout, "%s%s", path, INPUT_PROMPT);

        if(fgets(input, INPUT_MAX_SIZE, stdin) == NULL)
        {   
            if(feof(stdin) != 0) continue;

            int err = ferror(stdin);
            if(err != 0)
            {
                perror("input error");
                continue;
            }
        }
        else *(strchr(input, '\n')) = 0;
        
        PROMPT *prompt = parseStrToPrompt(input);

        printPrompt(prompt);

        //Check if the command is exit
        if (prompt->commands_count > 0 && strcmp(prompt->commands[0].arguments[0], "exit") == 0) {
            shell_active = false;
            freePrompt(prompt);
            continue;
        }

        //Check if command is cd
        if (prompt->commands_count > 0 && strcmp(prompt->commands[0].arguments[0], "cd") == 0) {
            if (prompt->commands[0].arguments_count > 1) {
                if (chdir(prompt->commands[0].arguments[1]) != 0) {
                    perror("cd error");
                }
                free(path); 
                path = getcwd(NULL, 0);
            } else {
                fprintf(stderr, "cd: missing directory\n");
            }
            freePrompt(prompt);
            continue;
        }

        //Pipe creation
        int num_pipes = prompt->commands_count - 1; 
        int pipes[num_pipes][2];

        for (int i = 0; i < num_pipes; i++) {
            if (pipe(pipes[i]) == -1) {
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
        }

        //Process creation
        for (size_t i = 0; i < prompt->commands_count; i++) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork failed");
                break;
            } else if (pid == 0) {
        
            
                //If it isn´t the first command, redirect last pipe's entry
                if (i > 0) {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                }

                //If it isn´t the last command, redirect next pipe's exit
                if (i < prompt->commands_count - 1) {
                    dup2(pipes[i][1], STDOUT_FILENO); 
                }
                //If it's the last command, check for redirect
                if(i == prompt->commands_count - 1) {
                    //Redirect treatment
                    if (prompt->commands[i].redirect != NULL) {
                        int fd = open(prompt->commands[i].redirect, O_WRONLY | O_CREAT, 0644);
                        if (fd < 0) {
                            perror("open failed");
                            exit(EXIT_FAILURE);
                        }
                        dup2(fd, STDOUT_FILENO); 
                        close(fd);
                    }
                }
                //Child close
                for (int j = 0; j < num_pipes; j++) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                //Execute
                execvp(prompt->commands[i].arguments[0], prompt->commands[i].arguments);
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        }

        //Parent close
        for (int i = 0; i < num_pipes; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        //Wait for  every child
        for (size_t i = 0; i < prompt->commands_count; i++) {
            wait(NULL);
        }

        freePrompt(prompt);
    } while (shell_active);
    
    free(path);
    return 0;
}
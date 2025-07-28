#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../prompt.h" 
#include "parsing.h"

void argumentsAction(char *start_ptr, char *end_ptr, size_t curr_idx, void *rtn_ptr)
{
    if(rtn_ptr == NULL) return;

    size_t argument_length = end_ptr - start_ptr + 1;

    char *argument = (char *) malloc(sizeof(char) * (argument_length + 1));
    strncpy(argument, start_ptr, argument_length);
    argument[argument_length] = '\0';

    char **arguments = (char **) rtn_ptr;
    arguments[curr_idx] = argument;
}

void commandsAction(char *start_ptr, char *end_ptr, size_t curr_idx, void *rtn_ptr)
{
    if(rtn_ptr == NULL) return;

    size_t command_length = end_ptr - start_ptr + 1;
    
    char *redirect = strstr(start_ptr, REDIRECT_SYMBOL);
    if(redirect != NULL) 
    {
        if(redirect <= end_ptr)
        {
            command_length = redirect - start_ptr;
            size_t redirect_len = end_ptr - redirect + 1;
            
            char *redirect_start = malloc(sizeof(char) * (redirect_len + 1));
    
            keywordAction(redirect, redirect_len, REDIRECT_SYMBOL, false, redirectAction, (void *) redirect_start);
    
            redirect = redirect_start;
        }
        else redirect = NULL;
    }

    size_t argument_count = keywordAction(start_ptr, command_length, " ", false, NULL, NULL);
    char **arguments = (char **) malloc(sizeof(char *) * argument_count);

    COMMAND *commands = (COMMAND *) rtn_ptr;
    commands[curr_idx].arguments_count = argument_count;
    commands[curr_idx].arguments = arguments;
    commands[curr_idx].redirect = redirect;

    keywordAction(start_ptr, command_length, " ", false, argumentsAction, (void *) arguments);
}

void redirectAction(char *start_ptr, char *end_ptr, size_t curr_idx, void *rtn_ptr)
{
    if(rtn_ptr == NULL) return;
    
    size_t redirect_len = end_ptr - start_ptr + 1;

    char *redirect = (char *) rtn_ptr;
    
    strncpy(redirect, start_ptr, redirect_len);
    redirect[redirect_len] = '\0';
}

PROMPT *parseStrToPrompt(char *input_start)
{
    size_t input_length = strlen(input_start);
    size_t commands_count = keywordAction(input_start, input_length, PIPE_SYMBOL, false, NULL, NULL);
    COMMAND *commands = (COMMAND *) malloc(sizeof(COMMAND) * commands_count);

    PROMPT *prompt = (PROMPT *) malloc(sizeof(PROMPT));
    prompt->commands_count = commands_count;
    prompt->commands = commands;

    keywordAction(input_start, input_length, PIPE_SYMBOL, false, commandsAction, (void *) commands);

    return prompt;
}

void printPrompt(PROMPT *prompt) 
{
    printf("prompt: {\n");
    printf("\tcommand count: %ld\n", prompt->commands_count);
    for(size_t comm_idx = 0; comm_idx < prompt->commands_count;comm_idx++)
    {  
        size_t args_size = prompt->commands[comm_idx].arguments_count;
        printf("\tcommand[%ld].redirect: %s\n", comm_idx, prompt->commands[comm_idx].redirect);
        printf("\tcommand[%ld].arguments_count: %ld\n", comm_idx, args_size);
        for(size_t arg_idx = 0; arg_idx < args_size; arg_idx++)
        {
            printf("\tcommand[%ld].argument[%ld]: %s\n", 
            comm_idx, arg_idx, prompt->commands[comm_idx].arguments[arg_idx]);
        }
    }
    printf("}\n");
    return;
}

void freePrompt(PROMPT *prompt)
{   
    if(prompt == NULL) return;
    if(prompt->commands != NULL)
    {
        for (size_t comm_idx = 0; comm_idx < prompt->commands_count; comm_idx++)
        {
            if(prompt->commands[comm_idx].arguments != NULL)
            {
                for(size_t var_idx = 0; var_idx < prompt->commands[comm_idx].arguments_count; var_idx++) 
                {
                    free(prompt->commands[comm_idx].arguments[var_idx]);
                }
                free(prompt->commands[comm_idx].arguments);
            }
            free(prompt->commands[comm_idx].redirect);
        }
        free(prompt->commands);
    }
    free(prompt);
}
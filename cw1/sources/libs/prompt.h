#ifndef _PROMPT_H
#define _PROMPT_H

#include <stddef.h>

#define PIPE_SYMBOL "|"
#define REDIRECT_SYMBOL ">"

typedef struct command 
{
    char *redirect;
    char **arguments;
    size_t arguments_count;
} COMMAND;

typedef struct prompt 
{
    COMMAND *commands;
    size_t commands_count;
} PROMPT;

PROMPT *parseStrToPrompt(char *input);

void freePrompt(PROMPT *prompt);

void printPrompt(PROMPT *prompt);

//internals - debugging
void redirectAction(char *start_ptr, char *end_ptr, size_t curr_idx, void *rtn_ptr);
void commandsAction(char *start_ptr, char *end_ptr, size_t curr_idx, void *rtn_ptr);
void argumentsAction(char *start_ptr, char *end_ptr, size_t curr_idx, void *rtn_ptr);

#endif
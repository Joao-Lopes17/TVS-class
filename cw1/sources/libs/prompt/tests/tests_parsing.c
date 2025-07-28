#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../parsing.h"

#define SIZE_OF(arr) sizeof(arr)/sizeof(arr[0])

char *inputs[] = {
    "cat text.txt | grep abc > output.txt",
    "cat text.txt || grep abc > output.txt",
    "cat text.txt | grep abc",
    "cat text.txt > output.txt",
    "cat text.txt",
    " cat text.txt  |  grep abc  >  output.txt ",
    " cat text.txt  ||  grep abc  >  output.txt ",
    " cat text.txt  |  grep abc ",
    " cat text.txt  >  output.txt ",
    " cat text.txt ",
    " cat text.txt  |  ldd  >  output.txt ",
    " cat text.txt  ||  ldd  >  output.txt ",
    " cat text.txt  |  ldd ",
    " ldd  >  output.txt ",
    " ldd ",
};
size_t inputs_size = SIZE_OF(inputs);

void argumentsAction(char *start_ptr, char *end_ptr, size_t curr_idx, void *rtn_ptr)
{
    size_t str_len = end_ptr - start_ptr + 1;
    char *str = (char *) malloc(sizeof(char) * (str_len + 1));
    strncpy(str, start_ptr, str_len);
    str[str_len] = '\0';
    ((char **) rtn_ptr)[curr_idx] = str;
}

void commandsAction(char *start_ptr, char *end_ptr, size_t curr_idx, void *rtn_ptr)
{   
    size_t max_length = end_ptr - start_ptr + 1;
    size_t argument_count = keywordAction(start_ptr, max_length, " ", false, NULL, NULL);
    char **arguments = (char **) malloc(sizeof(char *) * argument_count);
    keywordAction(start_ptr, max_length, " ", false, argumentsAction, (void *) arguments);
    
    for (size_t arg_idx = 0; arg_idx < argument_count; arg_idx++)
        fprintf(stdout, "command[%ld] - argument[%ld]: %s\n", curr_idx, arg_idx, arguments[arg_idx]);
    fprintf(stdout, "command[%ld] - %ld arguments\n", curr_idx, argument_count);

    for (size_t arg_idx = 0; arg_idx < argument_count; arg_idx++)
        free(arguments[arg_idx]);
    free(arguments);
}

size_t getMaxLength(char *input) {
    size_t input_len = strlen(input);
    char *redirect = (char *) strstr(input, ">");
    if(redirect != NULL) input_len =  redirect - input;
    return input_len;
}

int parsing_tests() 
{   
    int ret = 0;
    for (size_t idx = 0; idx < inputs_size && !ret; idx++)
    {   
        size_t input_len = getMaxLength(inputs[idx]);
        size_t command_count = keywordAction(inputs[idx], input_len, "|", false, NULL, NULL);
        keywordAction(inputs[idx], input_len, "|", false, commandsAction, NULL);
        fprintf(stdout, "input[%ld] - %ld commands - %s\n\n", idx, command_count, inputs[idx]);
    }
    
    return ret;
}

int main(int argc, const char *argv[]) 
{
    int ret = 0;
    ret |= parsing_tests();
    
    return ret != 0;
}
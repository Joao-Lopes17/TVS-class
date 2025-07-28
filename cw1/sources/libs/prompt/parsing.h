#ifndef _PARSING_H
#define _PARSING_H

#include <stddef.h>
#include <stdbool.h>

size_t keywordAction(char *input_str, size_t max_length, char *separator, bool destructive,
    void (*action)(char *start_ptr, char *end_ptr, size_t curr_idx, void *rtn_ptr), void *rtn_ptr);

#endif
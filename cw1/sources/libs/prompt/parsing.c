#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "parsing.h"

size_t keywordAction(char *input_str, size_t max_length, char *separator, bool destructive,
    void (*action)(char *start_ptr, char *end_ptr, size_t curr_idx, void *rtn_ptr), void *rtn_ptr)
{
    if (input_str == NULL || *input_str == '\0')
        return 0;

    size_t separator_len = strlen(separator);
    bool outOfBounds = false;

    size_t keyword_idx = 0;
    char *keyword_start = input_str;
    char *keyword_end;
    char *next_separator;

    do {
        next_separator = strstr(keyword_start, separator);
        if(next_separator == NULL) 
            next_separator = strchr(keyword_start, '\0'); 

        outOfBounds = next_separator > input_str + max_length;

        keyword_end = (!outOfBounds ? next_separator: input_str + max_length) - 1;

        if(destructive && !outOfBounds && *next_separator != '\0')
            for(char *ptr = next_separator; ptr < next_separator + separator_len; ptr++)
                *ptr = 0;

        while ((keyword_start <= keyword_end) && isblank(*keyword_start)) 
        {
            if(destructive) *keyword_start = '\0';
            keyword_start++;
        }
            
        while ((keyword_start <= keyword_end) && isblank(*keyword_end)) 
        {
            if(destructive) *keyword_end = '\0';
            keyword_end--;
        }
            
        if(keyword_start <= keyword_end) 
        {
            if(action != NULL) {
                action(keyword_start, keyword_end, keyword_idx, rtn_ptr);
            } 
            keyword_idx++;
        }
            
        keyword_start = next_separator + separator_len;
    } while (!outOfBounds && *next_separator != '\0');
    
    return keyword_idx;
}
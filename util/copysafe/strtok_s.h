#pragma once

#include "strpbrk_s.h"

// Non-destructive and thread-safe version of strtok
char* strtok_s(char* str, const char* delim) 
{
    // use thread_local to be thread safe
    static thread_local char* p = NULL;
    
    if (str != NULL) 
    {
        p = str;
    }
    else if (p == NULL) 
    {
        return NULL;
    }
    
    char* start = p;
    char* end = strpbrk_s(start, delim);
    
    if (end != NULL) 
    {
        *end = '\0';
        p = end + 1;
    }
    else 
    {
        p = NULL;
    }

    return start;
}
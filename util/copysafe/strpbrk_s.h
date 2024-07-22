#pragma once

char* strpbrk_s(const char* str, const char* charset) 
{
    const char* s;
    const char* c;

    for (s = str; *s != '\0'; s++) 
    {
        for (c = charset; *c != '\0'; c++) 
        {
            if (*s == *c) 
            {
                return (char*)s;
            }
        }
    }
    return NULL;
}
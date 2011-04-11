#include <stdio.h>
#include "strFuncs.h"



int str_end(const char* s, const char* t)
{
    //assume len(s) <= len(t)
    //printf("--%s\n", (t+(str_len(t)-str_len(s))));
    return(str_cmp((t+(str_len(t)-str_len(s))), s));
}

void str_cat(const char* s, const char* t, char* buf)
{
   str_copy(t,(buf+str_copy(s,buf)));
}

int str_copy(const char* s, char* result)
{
    return str_map(&identity, s, result);
}

int str_len(const char* s)
{
    return str_foldl(&count, s, 0);
}

int str_cmp(const char* s, const char* t)
{
    /*Reached end of both strings simultaneously, so they are equal*/
    if(*s == 0 && *t == 0)
    {
        return 1;
    }  
    else if(*s != *t)//two chars don't match, strings are not equal
    {
        return 0;
    }
    else//strings are equal so far, keep checking
    {
        return (str_cmp(s+1, t+1));
    }
}

/*return the number of chars copied*/
int str_map(char (* func)(char), const char *string, char *result)
{
    int i = 0;
    while(string[i] != '\0')
    {
        result[i] = (*func)(string[i]);
        i++;
    }
    result[i] = 0;
    return i;
}

int str_foldl(int (* func)(int, const char), const char *str, int initial)
{
    if(*str == 0)
    {
        return initial;
    } else
    {
        return str_foldl(func, (str+1), (*func)(initial,*str));
    }
}

int count(int ct, const char c)
{
    //printf("%c", c);
    return ct+1;
}



char identity(char c)
{
    //printf("%c", c);
    return c;
}

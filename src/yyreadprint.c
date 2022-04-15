#include <stdio.h>
#include <stdlib.h>

#define INITIAL_BUFFER_SIZE 128

// Because getline is inconsistent across compilers
// and Bison needs extra terminators after the line...
size_t yyreadline(char **lineptr, size_t *n, FILE *stream, size_t n_terminate)
{
    char *bufptr = NULL;
    char *p;
    size_t size;
    char c;

    if (lineptr == NULL)
    {
        return (size_t) -1;
    }
    if (stream == NULL)
    {
        return (size_t) -1;
    }
    if (n == NULL)
    {
        return (size_t) -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = 0;
    if (bufptr == NULL)
    {
        bufptr = malloc(INITIAL_BUFFER_SIZE);
        if (bufptr == NULL)
        {
            return (size_t) -1;
        }
        size = INITIAL_BUFFER_SIZE;
    }
    p = bufptr;
    while (c != EOF)
    {
        c = (char) fgetc(stream);
        if ((p - bufptr + 1 + n_terminate) > (size))
        {
            unsigned long offset = p - bufptr;
            size = 2 * size;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL)
            {
                return (size_t) -1;
            }
            p = bufptr + offset;
        }
        *p++ = c;
        if (c == '\n')
        {
            break;
        }
    }

    while (n_terminate > 0)
    {
        *p++ = '\0';
        n_terminate--;
    }

    *n = p - bufptr;
    bufptr = realloc(bufptr, *n);
    *lineptr = bufptr;

    return (p - bufptr);
}

void yyprintline(char *line, size_t len, size_t n_extra_terminates)
{
    size_t lastIndex = len - 1 - n_extra_terminates;
    char lastChar = line[lastIndex];


    if (lastChar == EOF)
    {
        line[lastIndex] = '\0';
        if (lastIndex == 0) printf("%sEOF\n", line);
        else printf("%s\n", line);
        line[lastIndex] = EOF;
    }
    else
    {
        printf("%s", line);
    }
}

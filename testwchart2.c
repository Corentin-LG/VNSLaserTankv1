#include <stdio.h>
#include <string.h>

int main()
{

    char *string = "qwerty";
    char *e;
    int index;

    e = strchr(string, 'e');
    index = (int)(e - string);

    return (0);
}
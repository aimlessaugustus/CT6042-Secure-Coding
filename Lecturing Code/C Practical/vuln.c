#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int copy_name(const char *arg)
{
    // Small fixed buffer
    char name[32];
    // UNSAFE: no bounds check if arg > 31 chars (+1 for '\0')
    // strcpy is used to keep it portable and obviously unsafe
    strcpy(name, arg);
    // Do something trivial so compiler keeps 'name' alive
    printf("Hello, %s! (length=%zu)\n", name, strlen(name));
    return 0;
}
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <your-name>\n", argv[0]);
        return 1;
    }
    return copy_name(argv[1]);
}
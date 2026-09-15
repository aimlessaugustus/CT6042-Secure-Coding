#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NAME_CAP 32 // includes space for '\0'
int copy_name_safe(const char *arg)
{
    char name[NAME_CAP];
    // Defensive: check input
    if (!arg)
    {
        fputs("Invalid input\n", stderr);
        return 1;
    }
    // Bounded copy + manual NUL-termination
    // Force NUL-terminate if src >= n
    strncpy(name, arg, NAME_CAP - 1);
    name[NAME_CAP - 1] = '\0';
    // Optional: detect truncation and react
    if (strlen(arg) >= NAME_CAP)
    {
        fputs("[warn] Name was truncated to fit buffer\n", stderr);
    }
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
    return copy_name_safe(argv[1]);
}
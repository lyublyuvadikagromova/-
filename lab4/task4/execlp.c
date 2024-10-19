#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

int my_execlp(const char *file, const char *arg, ...) {
    char *path = getenv("PATH");
    if (path == NULL) {
        perror("Failed to get PATH");
        return -1;
    }

    char *dir = strtok(path, ":");
    char full_path[1024];

    va_list args;
    va_start(args, arg);

    int argc = 1;
    while (va_arg(args, const char *) != NULL) {
        argc++;
    }
    va_end(args);

    const char *argv[argc + 1]; // +1 для NULL
    argv[0] = arg;

    va_start(args, arg);
    for (int i = 1; i < argc; i++) {
        argv[i] = va_arg(args, const char *);
    }
    argv[argc] = NULL;
    va_end(args);

    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, file);
        execv(full_path, (char *const *)argv);
        dir = strtok(NULL, ":");
    }

    perror("Command not found");
    return -1;
}

int main() {
    my_execlp("ls", "ls", "-l", (char *) NULL);

    return 0;
}

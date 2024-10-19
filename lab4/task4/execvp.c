#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int my_execvp(const char *file, char *const argv[]) {
    char *path = getenv("PATH");
    if (path == NULL) {
        perror("Failed to get PATH");
        return -1;
    }

    char *dir = strtok(path, ":");
    char full_path[1024];

    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, file);
        execv(full_path, argv);
        dir = strtok(NULL, ":");
    }

    perror("Command not found");
    return -1;
}

int main() {
    char *args[] = {"ls", "-l", NULL};
    my_execvp("ls", args);

    return 0;
}

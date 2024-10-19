#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void run_experiment(int total_points, double radius) {
    int processes = total_points / 255;
    int remaining_points = total_points % 255;
    int total_inside_circle = 0;

    for (int i = 0; i < processes; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            execl("./child", "child", "255", (char *)NULL);
            perror("execl");
            exit(1);
        } else {
            int status;
            wait(&status);

            if (WIFEXITED(status)) {
                total_inside_circle += WEXITSTATUS(status);
            }
        }
    }

    if (remaining_points > 0) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            char points_str[12];
            snprintf(points_str, sizeof(points_str), "%d", remaining_points);
            execl("./child", "child", points_str, (char *)NULL);
            perror("execl");
            exit(1);
        } else {
            int status;
            wait(&status);

            if (WIFEXITED(status)) {
                total_inside_circle += WEXITSTATUS(status);
            }
        }
    }

    double sp = 4.0 * radius * radius; 
    double sm = ((double)total_inside_circle / total_points) * sp; 
    double pi_estimate = sm / (radius * radius);  

    printf("Points: %d | Estimated pi: %f | Area of circle: %f\n", total_points, pi_estimate, sm);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <min_points> <max_points> <radius>\n", argv[0]);
        return 1;
    }

    int min_points = atoi(argv[1]);
    int max_points = atoi(argv[2]);
    double radius = atof(argv[3]);

    if (min_points <= 0 || max_points <= 0 || radius <= 0 || min_points > max_points) {
        fprintf(stderr, "Invalid arguments. Ensure positive numbers and min_points <= max_points.\n");
        return 1;
    }

    for (int points = min_points; points <= max_points; points *= 10) {
        run_experiment(points, radius);
    }

    return 0;
}

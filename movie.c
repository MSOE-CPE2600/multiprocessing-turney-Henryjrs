/*
 * Filename:
 * Description:
 * Author: Stephen Henry
 * Date:
 * how to compile: gcc 
 * This program spawns child processes to run the existing `mandel` program
 * multiple times with varying scale values to create a zoom effect.
 * The number of concurrent child processes is controlled by the `-p` option.
 * After generating frames, you can use ffmpeg to create a movie:
 *   ffmpeg -i mandel%d.jpg mandel.mpg
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int processes = 1;
    int frames = 50;
    double xcenter = -0.5;
    double ycenter = 0.0;
    double scale = 4.0;
    double zoomFactor = 0.95;

    int opt;
    while ((opt = getopt(argc, argv, "p:n:")) != -1) {
        switch (opt) {
            case 'p':
                processes = atoi(optarg);
                if (processes < 1) {
                    fprintf(stderr, "Number of processes must be >= 1\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'n':
                frames = atoi(optarg);
                if (frames < 1) {
                    fprintf(stderr, "Number of frames must be >= 1\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s -p <processes> [-n <frames>]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    printf("Generating %d frames with up to %d concurrent processes...\n", frames, processes);

    int active = 0;
    for (int i = 0; i < frames; i++) {
        if (active >= processes) {
            wait(NULL);
            active--;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            char filename[64];
            snprintf(filename, sizeof(filename), "mandel%d.jpg", i);

            char scaleStr[32];
            snprintf(scaleStr, sizeof(scaleStr), "%f", scale);

            char xStr[32], yStr[32];
            snprintf(xStr, sizeof(xStr), "%f", xcenter);
            snprintf(yStr, sizeof(yStr), "%f", ycenter);

            execl("./mandel", "mandel",
                  "-x", xStr,
                  "-y", yStr,
                  "-s", scaleStr,
                  "-o", filename,
                  "-W", "1000",
                  "-H", "1000",
                  "-m", "1000",
                  (char *)NULL);

            perror("execl failed");
            exit(EXIT_FAILURE);
        } else {
            active++;
            scale *= zoomFactor;
        }
    }

    while (active > 0) {
        wait(NULL);
        active--;
    }

    return 0;
}

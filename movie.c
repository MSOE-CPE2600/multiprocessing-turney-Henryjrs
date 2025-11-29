/*
 * Filename: movie.c
 * Description: Source file used to have multiprocessing and multithreading mandel files
 * Author: Stephen Henry
 * Date:11/28/2025
 * how to compile: gcc -o movie movie.c
 * The number of concurrent child processes is controlled by the -p option.
 * The number of concurrent child threading is controlled by the -t option.
 * After generating frames, you can use ffmpeg to create a movie:
 *   ffmpeg -i mandel%d.jpg mandel5.mpg
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>

//Handles the mandel pics to create the zoomed effect from the processes and threads used...
int main(int argc, char *argv[]) {
    int processes = 1;
    int threads = 1;
    int frames = 50;
    double xcenter = -0.5;
    double ycenter = 0.0;
    double scale = 4.0;
    double zoomFactor = 0.95;

    // threads are now integrated into the loop along with processes
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            threads = atoi(argv[++i]);
            if (threads < 1) threads = 1;
            if (threads > 20) threads = 20;
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            processes = atoi(argv[++i]);
            if (processes < 1) processes = 1;
            if (processes > 20) processes = 20;
        }
    }

    printf("Generating %d frames with %d processes and %d threads each...\n", frames, processes, threads);

// I totally got this Lecture 25
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

/* man exec(3) explains: The  first argument should point to the filename associated with the file being executed.
 *The list of arguments must be terminated by a null pointer, and, since these are  variadic  functions,
 *this pointer must be cast (char *) NULL.
 *thread help line is added 
 */
        if (pid == 0) {
            char filename[64];
            snprintf(filename, sizeof(filename), "mandel%d.jpg", i);

            char scaleStr[32], xStr[32], yStr[32], threadsStr[16];
            snprintf(scaleStr, sizeof(scaleStr), "%f", scale);
            snprintf(xStr, sizeof(xStr), "%f", xcenter);
            snprintf(yStr, sizeof(yStr), "%f", ycenter);
            snprintf(threadsStr, sizeof(threadsStr), "%d", threads);

            execl("./mandel", "mandel",
                  "-x", xStr,
                  "-y", yStr,
                  "-s", scaleStr,
                  "-o", filename,
                  "-W", "1000",
                  "-H", "1000",
                  "-m", "1000",
                  "-t", threadsStr,
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


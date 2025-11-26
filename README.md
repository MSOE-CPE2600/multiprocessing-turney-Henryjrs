# System Programming Lab 11 Multiprocessing
/*
 * Filename: System Programming Lab 11 Multiprocessing
 * Description: README (Lab report) for Multiprocessing
 * Author: Stephen Henry
 * Date:11/25/2025
 * how to compile: gcc -o mandel mandel.c movie.c -o movie
 */


With this lab we used up to 50 processes to retrieve 50 images of a Mandelbrat. By running various amounts of processes
affects the time it takes to process it all and but not the value of the image. Reading through the man pages for ffmpeg 
it is the -framerate ## that alters the speed of the movie, so runtime is the focus with the processes. 

Readings were taken of time it takes to run with 1,2,5,10,20 processes. The Real, User, and Sys times were produced.
Real time gets higher the less processes used. User time became faster the less processes used (except) with the
2 processes running. I received a faster time than when 5 and 1 process was running. Sys time gets faster with the 
lower processes. 2 processes is still the anomly in my readings though.

Readings filed in excel chart and AI generated line chart.
13 processes are running in the selected movie that was demo'd and also included in submission.

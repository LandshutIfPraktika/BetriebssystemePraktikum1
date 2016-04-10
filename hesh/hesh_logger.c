//
// Created by s-gheldd on 4/10/16.
//


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <memory.h>
#include <errno.h>
#include "hesh.h"

void exit_time();

void exit_time_parse();

char *time_string(char *buffer);

void logger_session_start();

FILE *log_file;


int logger_log_line(char *line) {
    char buffer[HESH_LINE_BUFF_SIZE];
    fprintf(log_file, "%s: %d \t %s\n", time_string(buffer), errno, line);
    fflush(log_file);
    errno = 0;


    return 0;
}

char *time_string(char *buffer) {
    time_t current_time;
    struct tm *time_struct;
    current_time = time(NULL);
    if (current_time == -1) {
        exit_time();
    }
    time_struct = localtime(&current_time);
    if (time_struct == NULL) {
        exit_time_parse();
    }

    sprintf(buffer, "%d-%02d-%02d--%02d:%02d:%02d:%04d", time_struct->tm_year + 1900, time_struct->tm_mon,
            time_struct->tm_mday, time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec,
            clock() / (CLOCKS_PER_SEC / 1000));
    return buffer;
}

int logger_stop() {

    logger_log_line("hesh session stopped");
    fprintf(log_file, "\n\n");
    fclose(log_file);


    return 0;
}

int logger_start() {
    time_t current_time;
    struct tm *time_struct;
    char file_name[HESH_LINE_BUFF_SIZE];


    current_time = time(NULL);
    if (current_time == -1) {
        exit_time();
    }


    time_struct = localtime(&current_time);
    if (time_string == NULL) {
        exit_time_parse();
    }

    sprintf(file_name, "%s/.hesh/log/hesh-log-%d-%02d-%02d.txt", getenv("HOME"), time_struct->tm_year + 1900,
            time_struct->tm_mon,
            time_struct->tm_mday);

    log_file = fopen(file_name, "a");
    if (log_file == NULL) {
        char directory_name_hesh[HESH_LINE_BUFF_SIZE];
        char directory_name_log[HESH_LINE_BUFF_SIZE];
        sprintf(directory_name_hesh, "%s/.hesh", getenv("HOME"));
        sprintf(directory_name_log, "%s/log", directory_name_hesh);
        if (mkdir(directory_name_hesh, 0777) == -1 || mkdir(directory_name_log, 0777) == -1) {
            perror("log file creation");
            exit(EXIT_FAILURE);
        } else {
            log_file = fopen(file_name, "a");
        }
    }
    logger_session_start();
    logger_log_line("hesh session started");
    return 0;
}

void logger_session_start() {
    int i;
    char *stars = "**********";
    char *empty = "     ";
    char *hesh = "*H*E*S*H*";
    for (i = 0; i < 8; i++) {
        fprintf(log_file, stars);
    }
    fprintf(log_file, "\n");

    fprintf(log_file, stars);
    for (i = 0; i < 5; i++) {
        fprintf(log_file, empty);
    }
    fprintf(log_file, hesh);
    for (i = 0; i < 5; i++) {
        fprintf(log_file, empty);
    }
    fprintf(log_file, " ");
    fprintf(log_file, "%s\n", stars);
    for (i = 0; i < 8; i++) {
        fprintf(log_file, stars);
    }
    fprintf(log_file, "\n\n");


}

void exit_time_parse() {
    fprintf(stderr, "Failure to convert the current time.\n");
    exit(EXIT_FAILURE);
}

void exit_time() {
    fprintf(stderr, "Failure to obtain the current time.\n");
    exit(EXIT_FAILURE);
}




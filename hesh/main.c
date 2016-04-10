//
// Created by s-gheldd on 09.04.16.
//

#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <memory.h>
#include "hesh.h"
#include "hesh_logger.h"

#define HESH_LINE "hesh %s > "

int hesh_loop();

int main(int arc, char **argv) {
    int return_value;

    logger_start();
    return_value = hesh_loop();

    logger_stop();
    return return_value;
}

int hesh_loop() {
    int status;
    char *line;
    char **args;
    char cwd_buffer[HESH_LINE_BUFF_SIZE];


    if (chdir(getenv("HOME"))){
        perror("could not change to HOME dir");
        exit(EXIT_FAILURE);
    };

    do {
        printf(HESH_LINE, getcwd(cwd_buffer, HESH_LINE_BUFF_SIZE));
        line = hesh_read_line();
        {
            char * linecpy = malloc(sizeof(line));
            strcpy(linecpy,line);
            args = hesh_parse_line(line);
            status = hesh_execute_line(args, linecpy);
            free(linecpy);
        }
        free(line);
        free(args);
    } while (status);
    return 0;
}


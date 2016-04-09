//
// Created by s-gheldd on 09.04.16.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define HESH_LINE_BUFF_SIZE 1024
#define HESH_TOKEN_BUF_SIZE 64
#define HESH_TOKEN_DELIMITERS "\n\t\r\a "
#define HESH_EXIT_STRING "exit"
#define LINE_STRING "line"
#define TOKEN_STRING "token"
#define HESH_LINE "hesh > "

int hesh_loop();

char *hesh_read_line();

void exit_buffer_alloc(char *argument);

char **hesh_parse_line(char *line);

int hesh_execute_line(char **tokens);

int main(int arc, char **argv) {
    int return_value;

    return_value = hesh_loop();


    return return_value;
}

int hesh_loop() {
    int status;
    char *line;
    char **args;

    do {
        printf(HESH_LINE);
        line = hesh_read_line();
        args = hesh_parse_line(line);
        status = hesh_execute_line(args);

        free(line);
    } while (status);
    return 0;
}

int hesh_execute_line(char **tokens) {
    char *token;
    int pos = 0;
    while ((token =*(tokens + pos++) )!= NULL) {
        if (!strcmp(HESH_EXIT_STRING,token)) {
            return 0;
        }
    }
    return 1;
}

char **hesh_parse_line(char *line) {
    int buff_size = HESH_TOKEN_BUF_SIZE;
    char **tokens;
    char *token;
    int pos = 0;

    tokens = malloc(buff_size * sizeof(char *));
    if (!tokens) {
        exit_buffer_alloc(TOKEN_STRING);
    }

    token = strtok(line, HESH_TOKEN_DELIMITERS);
    while (token != NULL) {
        *(tokens + pos++) = token;

        if (pos >= buff_size) {
            buff_size += HESH_LINE_BUFF_SIZE;
            tokens = realloc(tokens, buff_size * sizeof(char *));
            if (!tokens) {
                exit_buffer_alloc(TOKEN_STRING);
            }
        }
        token = strtok(NULL, HESH_TOKEN_DELIMITERS);
    }
    *(tokens + pos) = NULL;

    return tokens;
}

char *hesh_read_line() {
    int buff_size = HESH_LINE_BUFF_SIZE;
    char *buffer;
    char c;
    int pos = 0;


    buffer = malloc(sizeof(char) * buff_size);
    if (!buffer) {
        exit_buffer_alloc(LINE_STRING);
    }

    while (1) {
        c = getc(stdin);

        if (c == EOF || c == '\n') {
            *(buffer + pos) = '\0';
            return buffer;
        } else {
            *(buffer + pos++) = c;
        }

        if (pos >= buff_size) {
            buff_size += HESH_LINE_BUFF_SIZE;
            buffer = realloc(buffer, HESH_LINE_BUFF_SIZE);
            if (!buffer) {
                exit_buffer_alloc(LINE_STRING);
            }
        }

    }
    return NULL;
}

void exit_buffer_alloc(char *argument) {
    fprintf(stderr, "could not allocate enough memory for %s buffer\n", argument);
    exit(EXIT_FAILURE);
}
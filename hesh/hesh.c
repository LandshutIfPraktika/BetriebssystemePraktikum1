//
// Created by s-gheldd on 4/10/16.
//

#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include "hesh.h"


int hesh_execute_line(char **tokens) {
    char *token;
    int pos = 0;
    while ((token = *(tokens + pos++)) != NULL) {
        if (!strcmp(HESH_EXIT_STRING, token)) {
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
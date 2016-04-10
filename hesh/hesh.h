//
// Created by s-gheldd on 4/10/16.
//

#ifndef HESH_HESH_H
#define HESH_HESH_H


#define HESH_LINE_BUFF_SIZE 1024
#define HESH_TOKEN_BUF_SIZE 64
#define HESH_TOKEN_DELIMITERS "\n\t\r\a "
#define HESH_EXIT_STRING "exit"
#define LINE_STRING "line"
#define TOKEN_STRING "token"

char **hesh_parse_line(char *line);

char *hesh_read_line();

void exit_buffer_alloc(char *argument);

int hesh_execute_line(char **tokens);


#endif //HESH_HESH_H

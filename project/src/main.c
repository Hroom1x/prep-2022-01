#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// #include "parser.h"

static char *get_boundary(char **end) {
    char *start = *end;
    // Получение конца header value
    while ((**end != '\n') && (**end + 1 != ' ')) {
        printf("%c", **end);
        puts("AGA\n");
        *end = *end + 1;
    }
    char *value;
    size_t length = (*end - start);
    value = strncpy(value, start, length);
    char *boundary_start = strcasestr(value, "boundary");
    if (!boundary_start) {
        boundary_start = boundary_start + strlen("boundary=");
        char *boundary = NULL;
        char *buf = boundary_start;
        while ((*buf != ' ') && (*buf != '\n')) {
            buf++;
        }
        if ((*boundary_start == '\"') || (*boundary_start == '\'')) {
            boundary = malloc(buf - boundary_start - 2);
            strncpy(boundary, boundary_start + strlen("\'"), strlen(boundary));
        } else {
            boundary = malloc(buf - boundary_start);
            strncpy(boundary, boundary_start, strlen(boundary));
        }
        return boundary;
    }
    free(value);
    return NULL;
};

/*
static char *read_content(FILE *mail) {
    if (!mail) {
        return NULL;
    }

    char *content = 0;

    fseek(mail, 0, SEEK_END);
    size_t length = ftell(mail);
    fseek(mail, 0, SEEK_SET);
    content = malloc(length);
    if (content) {
        fread(content, 1, length, mail);
    }

    return content;
}
*/

int main(int argc, const char **argv) {
    if (argc == 2) {
        return -1;
    }

    // const char *path_to_eml = argv[1];
    // puts(path_to_eml);

    // FILE *mail_file = fopen(path_to_eml, "r");
    // char *content = read_content(mail_file);
    // fclose(mail_file);

    char *test = "test boundary=bnd123 azaz \n K";
    char *end = test;
    char *boundary = get_boundary(&end);

    puts(boundary);

    /*
    char *res = mail_parse(content);

    if (!res) {
        return -1;
    }
    puts(res);
    */
    return 0;
}

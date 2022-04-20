// #define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"

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

int main(int argc, const char **argv) {
    if (argc != 2) {
        return -1;
    }

    const char *path_to_eml = argv[1];
    FILE *mail_file = fopen(path_to_eml, "r");
    // FILE *mail_file = fopen("../vkhw/btests/emails/message-external-body.eml", "r");
    char *content = read_content(mail_file);
    fclose(mail_file);

    char *res = mail_parse(content);

    if (!res) {
        return -1;
    }

    return 0;
}

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"

static char *read_content(FILE *mail) {
    if (!mail) {
        return NULL;
    }

    char *content = calloc(1, sizeof(char));
    char *buf = calloc(1, sizeof(char));
    size_t length = sizeof(char);
    while (fread(buf, 1, 1, mail) == 1) {
        length += sizeof(char);
        void *temp = realloc(content, length);
        if (temp == NULL) {
            free(content);
            free(temp);
            return NULL;
        }
        else {
            content = temp;
            content[length - 2] = *buf;
        }
    }
    content[length - 1] = '\0';
    return content;
}

int main(int argc, const char **argv) {
    if (argc != 2) {
        return -1;
    }

    const char *path_to_eml = argv[1];
    FILE *mail_file = fopen(path_to_eml, "r");
    // FILE *mail_file = fopen("../vkhw/btests/emails/on-bobcat.eml", "rb");
    char *content = read_content(mail_file);
    fclose(mail_file);

    char *res = mail_parse(content);

    if (!res) {
        return -1;
    }

    return 0;
}

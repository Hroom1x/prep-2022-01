#define _GNU_SOURCE

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
    if (argc == 2) {
        return -1;
    }

    // "From: \"Administrator\" <administrator@example.com>\nTo: \"Bob Sample\" <bob@example.com>\nSubject: Welcome\nMIME-Version: 1.0\nContent-Type: multipart/alternative; boundary=bd1\n\n--bd1\nContent-Type: text/plain; charset=\"UTF-8\"\nContent-Transfer-Encoding: 8bit\n\nSasha\n\n--bd1\nContent-Type: multipart/related; boundary=\"bd2\"\n\n--bd2\nContent-Type: text/html; charset=\"UTF-8\"\nContent-Transfer-Encoding: 8bit\n\nHi\n\n--bd2--\n--bd1--";
    // char *mail = "Content-Type: multipart/alternative; boundary=bd1\n\n--bd1\nContent-Type: text/plain; charset=\"UTF-8\"\nSasha\n\n--bd1\nContent-Type: multipart/related; boundary=\"bd2\"\n\n--bd2\nContent-Type: text/html; charset=\"UTF-8\"\nContent-Transfer-Encoding: 8bit\n\nHi\n\n--bd2--\n--bd1--";
    // mail_parse(mail);

    const char *path_to_eml = argv[1];
    puts(path_to_eml);

    // FILE *mail_file = fopen("../vkhw/btests/emails/multipart.eml", "r");
    FILE *mail_file = fopen("../vkhw/btests/emails/multipart.eml", "r");
    char *content = read_content(mail_file);
    fclose(mail_file);

    char *res = mail_parse(content);

    if (!res) {
        return -1;
    }

    return 0;
}

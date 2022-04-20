#define _GNU_SOURCE

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "parser.h"

#define FORMAT_STRING_MAX_SIZE 610
#define FROM_BUF 200
#define TO_BUF 200
#define DATE_BUF 200
#define PART_BUF 10

/*
 * H - Header
 * B - Boundary
 * P - Part
 * V - Value
 */

typedef enum {
    L_HBEGIN,
    L_HVALUE,
    L_HEND,
    L_BOUNDARY,
    L_COUNT,
    L_ERR
} lexem_t;

typedef enum {
    S_HBEGIN,
    S_HVALUE,
    S_HEND,
    S_PART,
    S_MPART,
    S_END,
    S_COUNT,
    S_ERR
} state_t;

typedef struct {
    char *from;
    char *to;
    char *date;
    char *boundary;
    int part;
} info_t;

typedef bool (*action_t)(const char *content, const char **end);
// typedef bool (*action_t)(const char *s, const char **end, cjson_callback_t callback)

typedef struct {
    state_t state;
    action_t action;
} rule_t;

static char *get_boundary(char *value);
static char *get_value(char **end);
static char *get_line(char **end);
static lexem_t get_lexem(const state_t *state, char *content, char **end, info_t *msg_info);

// Таблица переходов - матрица размерности
// "число состояний" x "число лексем"

static rule_t transitions[S_COUNT][L_COUNT] = {
        //                L_HBEGIN                                    L_HVALUE                                 L_HEND                                  L_BOUNDARY
        /* S_HBEGIN  */  {{S_HBEGIN, get_lexem},  {S_HVALUE, get_lexem},   {S_HEND, get_lexem},    {S_ERR, NULL}},
        /* S_HVALUE  */  {{S_ERR, NULL},                 {S_HVALUE, get_lexem},   {S_HEND, get_lexem},    {S_ERR, NULL}},
        /* S_HEND    */  {{S_HBEGIN, get_lexem},  {S_HVALUE, get_lexem},   {S_HEND, get_lexem},    {S_MPART, get_lexem}},
        /* S_PART    */  {{S_PART, get_lexem},    {S_PART, get_lexem},     {S_PART, get_lexem},    {S_ERR, NULL}},
        /* S_MPART   */  {{S_MPART, get_lexem},   {S_MPART, get_lexem},    {S_MPART, get_lexem},   {S_MPART, get_lexem}},
        /* S_END     */  {{S_ERR, NULL},                 {S_ERR, NULL},                 {S_ERR, NULL},                 {S_ERR, NULL}}
};

static char *get_value(char **end) {
    size_t length = 0;
    char *start = *end;
    while (true) {
        if ((**end == '\n') && (*(*end + 1) != ' ')) {
            break;
        }
        length++;
        *end = *end + 1;
    }
    char *value = calloc(length + 1, sizeof(char));
    strncpy(value, start, length);
    *end = *end + 1;
    if (!value) {
        free(value);
        free(start);
        return NULL;
    } else
    return value;
}

static char *get_line(char **end) {
    size_t length = 0;
    char *start = *end;
    while (**end != '\n') {
        length++;
        *end = *end + 1;
    }
    if (length == 0) {
        *end = *end + 1;
        return NULL;
    }
    char *value = calloc(length + 1, sizeof(char));
    value = strncpy(value, start, length);
    *end = *end + 1;
    if (!value) {
        free(value);
        return NULL;
    }
    return value;
}

static lexem_t get_lexem(const state_t *state, char *content, char **end, info_t *msg_info) {
    if ((*state != S_PART) && (*state != S_MPART)) {
        if ((*content == '-') && (*(content + 1) == '-')) {
            *end = content + strlen(msg_info->boundary) + 3;
            msg_info->part++;
            return L_BOUNDARY;
        } else if (*content == '\n') {
            *end = *end + 1;
            return L_HEND;
        } else if (*content && (*content != ' ') && (*content != '\n')) {
            *end = content;
            while (**end != ':') {
                *end = *end + 1;
            }
            *end = *end + 1;
            char *header = malloc(*end - content - 1);
            header = strncpy(header, content, *end - content - 1);
            if (strcasestr(header, "Content-Type") != NULL) {
                char *value = get_value((char **) end);
                msg_info->boundary = get_boundary(value);
                free(value);
            } else if (strcasestr(header, "From") != NULL) {
                msg_info->from = get_value((char **) end);
            } else if (strcasestr(header, "To") != NULL) {
                msg_info->to = get_value((char **) end);
            } else if (strcasestr(header, "Date") != NULL) {
                msg_info->date = get_value((char **) end);
            } else {
                get_value((char **) end);
            }
            free(header);
            return L_HEND;
        }
    } else {  // Part_counting
        char *line = get_line((char **) end);
        if (!line) {
            return L_HEND;
        }
        if ((*line == '-') && (*(line + 1) == '-')) {
            if (strcmp(msg_info->boundary, line + 2) == 0) {
                msg_info->part++;
                free(line);
                return L_BOUNDARY;
            }
        }
        free(line);
        return L_HEND;
    }
    return L_ERR;
};

static char *get_boundary(char *value) {
    char *boundary_start = strcasestr(value, "boundary") + strlen("boundary=");
    if (!boundary_start) {
        return NULL;
    }
    char *boundary_end = boundary_start;
    size_t length = 0;
    while ((*boundary_end != ' ') && (*boundary_end != '\n') && (*boundary_end != ';') && (*boundary_end != '\0')) {
        boundary_end++;
        length++;
    }
    char *boundary;
    if ((*boundary_start == '\'') || (*boundary_start == '\"')) {
        boundary = malloc(length - 2);
        boundary = strncpy(boundary, boundary_start + 1, length - 2);
    } else {
        boundary = malloc(length);
        boundary = strncpy(boundary, boundary_start, length);
    }
    return boundary;
};

char *mail_parse(char *content) {
    if (!content) {
        return NULL;
    }
    info_t msg_info = {
            "",
            "",
            "",
            "",
            0
    };
    state_t state = S_HBEGIN;
    while (*content) {
        char *end;
        lexem_t lexem = get_lexem(&state, content, &end, &msg_info);
        if (lexem == L_ERR) {
            return NULL;
        }
        rule_t rule = transitions[state][lexem];
        if (rule.state == S_ERR) {
            return NULL;
        }
        if (!rule.action) {
            if (!rule.action(content, &end)) {
                return NULL;
            }
        }
        if (rule.state == S_END) {
            // char format_string[FORMAT_STRING_MAX_SIZE];
            // snprintf(format_string, FORMAT_STRING_MAX_SIZE,
            //          "%%%",
            //          FROM_BUF, TO_BUF, DATE_BUF, PART_BUF);
            printf("%s|%s|%s|%d", msg_info.from, msg_info.to, msg_info.date, msg_info.part);
            return NULL;
        }
        state = rule.state;
        content = end;
    }
    return NULL;
}

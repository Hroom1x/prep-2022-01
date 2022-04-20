#define _GNU_SOURCE

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "parser.h"

#define FORMAT_STRING_MAX_SIZE 1000

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

typedef lexem_t (*action_t)(const state_t *state, char *content, char **end, info_t *msg_info);
// typedef bool (*action_t)(const char *s, const char **end, cjson_callback_t callback)

typedef struct {
    state_t state;
    action_t action;
} rule_t;

static char *get_boundary(char *value);
static void skip_value(char **end);
static char *get_value(char **end);
static char *get_line(char **end);
static lexem_t get_lexem(const state_t *state, char *content, char **end, info_t *msg_info);

// Таблица переходов - матрица размерности
// "число состояний" x "число лексем"

static rule_t transitions[S_COUNT][L_COUNT] = {
        //                L_HBEGIN                                               L_HVALUE                                           L_HEND                                          L_BOUNDARY
        /* S_HBEGIN  */  {{S_HBEGIN, (action_t) get_lexem},  {S_HVALUE, (action_t) get_lexem},  {S_HEND, (action_t) get_lexem},  {S_ERR, NULL}},
        /* S_HVALUE  */  {{S_ERR, NULL},                            {S_HVALUE, (action_t) get_lexem},  {S_HEND, (action_t) get_lexem},  {S_ERR, NULL}},
        /* S_HEND    */  {{S_HBEGIN, (action_t) get_lexem},  {S_HVALUE, (action_t) get_lexem},  {S_HEND, (action_t) get_lexem},  {S_MPART, (action_t) get_lexem}},
        /* S_PART    */  {{S_PART, (action_t) get_lexem},    {S_PART, (action_t) get_lexem},    {S_PART, (action_t) get_lexem},  {S_ERR, NULL}},
        /* S_MPART   */  {{S_MPART, (action_t) get_lexem},   {S_MPART, (action_t) get_lexem},   {S_MPART, (action_t) get_lexem}, {S_MPART, (action_t) get_lexem}},
        /* S_END     */  {{S_ERR, NULL},                            {S_ERR, NULL},                           {S_ERR, NULL},                          {S_ERR, NULL}}
};

static void skip_value(char **end) {
    while (**end == ' ') {
        *end = *end + 1;
    }
    while (true) {
        if ((**end == '\n') && (*(*end + 1) != ' ')) {
            break;
        }
        *end = *end + 1;
    }
    *end = *end + 1;
}

static char *get_value(char **end) {
    size_t length = 0;
    while (**end == ' ') {
        *end = *end + 1;
    }
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
    return value;
}

static char *get_line(char **end) {
    if (!*end) {
        return NULL;
    }
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
            msg_info->part += 1;
            return L_BOUNDARY;
        } else if (*content == '\n') {
            *end = *end + 1;
            return L_HEND;
        } else if (*content == ' ') {
            skip_value((char **) end);
            return L_HEND;
        } else if (*content) {
            *end = content;
            while (**end != ':') {
                *end = *end + 1;
            }
            *end = *end + 1;
            char *header = malloc(*end - content - 1);
            header = strncpy(header, content, *end - content - 1);
            if (strcmp(header, "Content-Type") == 0) {
                char *value = get_value((char **) end);
                msg_info->boundary = get_boundary(value);
                free(value);
            } else if (strcmp(header, "From") == 0) {
                msg_info->from = get_value((char **) end);
            } else if (strcmp(header, "To") == 0) {
                msg_info->to = get_value((char **) end);
            } else if (strcmp(header, "Date") == 0) {
                msg_info->date = get_value((char **) end);
            } else {
                skip_value((char **) end);
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
        boundary = calloc(length - 2, sizeof(char));
        boundary = strncpy(boundary, boundary_start + 1, length - 2);
    } else {
        boundary = calloc(length, sizeof(char));
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
            return NULL;
        }
        state = rule.state;
        content = end;
        if (strlen(content) == 0) {
            char output[FORMAT_STRING_MAX_SIZE];
            snprintf(output, FORMAT_STRING_MAX_SIZE,
                     "%s|%s|%s|%d", msg_info.from, msg_info.to, msg_info.date, msg_info.part);
            printf("%s|%s|%s|%d", msg_info.from, msg_info.to, msg_info.date, msg_info.part);
            return "OK";
        }
    }
    return NULL;
}

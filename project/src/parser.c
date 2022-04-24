#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "parser.h"

#define FORMAT_STRING_MAX_SIZE 3000000

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
    bool empty;
    int part;
} info_t;

typedef lexem_t (*action_t)(const state_t *state, char *content, char **end, info_t *msg_info);

typedef struct {
    state_t state;
    action_t action;
} rule_t;

static char *get_boundary(char *value);
static void skip_value(char **end);
static char *get_value(char **end);
static char *get_line(char **end);
static char *get_header(char **end);
static char *get_result(info_t *msg_info);
static void free_msg_info(info_t *info);
static lexem_t get_lexem(const state_t *state, char *content, char **end, info_t *msg_info);

// Таблица переходов - матрица размерности
// "число состояний" x "число лексем"

static rule_t transitions[S_COUNT][L_COUNT] = {
        //                L_HBEGIN                                               L_HVALUE                                           L_HEND                                          L_BOUNDARY
        /* S_HBEGIN  */  {{S_HBEGIN, (action_t) get_lexem},  {S_HVALUE, (action_t) get_lexem},  {S_HEND, (action_t) get_lexem},  {S_ERR, NULL}},
        /* S_HVALUE  */  {{S_ERR, NULL},                            {S_HVALUE, (action_t) get_lexem},  {S_PART, (action_t) get_lexem},  {S_ERR, NULL}},
        /* S_HEND    */  {{S_HBEGIN, (action_t) get_lexem},  {S_HVALUE, (action_t) get_lexem},  {S_PART, (action_t) get_lexem},  {S_MPART, (action_t) get_lexem}},
        /* S_PART    */  {{S_PART, (action_t) get_lexem},    {S_PART, (action_t) get_lexem},    {S_PART, (action_t) get_lexem},  {S_MPART, (action_t) get_lexem}},
        /* S_MPART   */  {{S_MPART, (action_t) get_lexem},   {S_MPART, (action_t) get_lexem},   {S_MPART, (action_t) get_lexem}, {S_MPART, (action_t) get_lexem}},
        /* S_END     */  {{S_ERR, NULL},                            {S_ERR, NULL},                           {S_ERR, NULL},                          {S_ERR, NULL}}
};

static void skip_value(char **end) {
    while ((**end == ' ') || (**end == '\t')) {
        while ((**end == ' ') || (**end == '\t')) {
            *end = *end + 1;
        }
        while (**end != '\n') {
            if (**end == '\r') {
                *end = *end + 1;
                continue;
            }
            *end = *end + 1;
        }
        *end = *end + 1;
    }
}

static char *get_value(char **end) {
    char *value = calloc(1, sizeof(char));
    size_t length = 0;
    do {
        while ((**end == ' ') || (**end == '\t')) {
            *end = *end + 1;
        }
        while (**end != '\n') {
            if (**end == '\r') {
                *end = *end + 1;
                continue;
            }
            length += sizeof(char);
            void *temp = realloc(value, length);
            if (temp == NULL) {
                free(temp);
                free(value);
                return NULL;
            }
            else {
                value = temp;
            }
            value[length - 1] = **end;
            *end = *end + 1;
        }
        *end = *end + 1;
        length += sizeof(char);
        value = realloc(value, length);
        value[length - 1] = ' ';
    } while ((**end == ' ') || (**end == '\t'));
    // if (!value) {
    //     return NULL;
    // }
    value[length - 1] = '\0';
    return value;
}

static char *get_line(char **end) {
    char *value = calloc(1, sizeof(char));
    size_t length = 0;
    while (**end != '\n' && **end != '\0') {
        if (**end == '\r') {
            *end = *end + 1;
            continue;
        }
        length += sizeof(char);
        void *temp = realloc(value, length);
        if (temp == NULL) {
            free(value);
            free(temp);
            return NULL;
        }
        else {
            value = temp;
        }
        value[length - 1] = **end;
        *end = *end + 1;
    }
    *end = *end + 1;
    length += sizeof(char);
    value = realloc(value, length);
    // if (!value || length == 1) {
    //     free(value);
    //     return NULL;
    // }
    value[length - 1] = '\0';
    return value;
}

static char *get_header(char **end) {
    if (!*end) {
        return NULL;
    }
    size_t length = 0;
    char *start = *end;
    while (**end != ':' && **end != '\n') {
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

static char *get_result(info_t *msg_info) {
    static char result[FORMAT_STRING_MAX_SIZE];
    char *from = "";
    char *to = "";
    char *date = "";
    if (msg_info->from != NULL) {
        from = msg_info->from;
    }
    if (msg_info->to != NULL) {
        to = msg_info->to;
    }
    if (msg_info->date != NULL) {
        date = msg_info->date;
    }
    snprintf(result, FORMAT_STRING_MAX_SIZE,
             "%s|%s|%s|%d", from, to, date, msg_info->part);
    free_msg_info(msg_info);
    return result;
}

static void free_msg_info(info_t *info) {
    free(info->from);
    free(info->to);
    free(info->date);
    free(info->boundary);
    // if (info->from != NULL) {
    //     free(info->from);
    // }
    // if (info->to != NULL) {
    //     free(info->to);
    // }
    // if (info->date != NULL) {
    //     free(info->date);
    // }
    // if (info->boundary != NULL) {
    //     free(info->boundary);
    // }
}

static lexem_t get_lexem(const state_t *state, char *content, char **end, info_t *msg_info) {
    if ((*state != S_PART) && (*state != S_MPART)) {
        if ((*content == '-') && (*(content + 1) == '-')) {
            *end = content + strlen(msg_info->boundary) + 3;
            msg_info->part += 1;
            return L_BOUNDARY;
        } else if (*content == '\r') {
            *end = *end + 1;
            if (**end == '\n') {
                *end = *end + 1;
            }
            return L_HEND;
        } else if (*content == '\n') {
            *end = *end + 1;
            if (**end == '\r') {
                *end = *end + 1;
            }
            return L_HEND;
        } else if ((*content == ' ') || (*content == '\t')) {
            skip_value((char **) end);
            return L_HVALUE;
        } else if (*content) {
            *end = content;
            char *header = get_header((char **) end);
            if (strcmp(header, "Content-Type") == 0) {
                char *value = get_value((char **) end);
                char *boundary = get_boundary(value);
                if (boundary != NULL) {
                    msg_info->boundary = boundary;
                }
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
            return L_HVALUE;
        }
    } else if (*state == S_MPART) {
        char *line = get_line((char **) end);
        if (!line) {
            return L_HEND;
        }
        if ((*line == '-') && (*(line + 1) == '-')) {
            if (strcmp(msg_info->boundary, line + 2) == 0) {
                msg_info->empty = false;
                msg_info->part++;
                free(line);
                return L_BOUNDARY;
            }
        }
        free(line);
        return L_HEND;
    } else if (*state == S_PART) {
        char *line = get_line((char **) end);
        if (!line) {
            return L_HEND;
        }
        msg_info->empty = false;
        free(line);
        return L_HEND;
    }
    return L_ERR;
};

static char *get_boundary(char *value) {
    char *boundary = calloc(1, sizeof(char));
    size_t length = 0;
    char *end = strcasestr(value, " boundary=");
    if (!end) {
        end = strcasestr(value, ";boundary=");
    }
    if (!end) {
        free(boundary);
        return NULL;
    }
    end += strlen(" boundary=");
    while ((*end != ' ') && (*end != '\n') && (*end != '\r') && (*end != ';') && (*end != '\0')) {
        if (*end == '\'' || *end == '\"') {
            end += 1;
            continue;
        }
        length += sizeof(char);
        void *temp = realloc(boundary, length);
        if (temp == NULL) {
            free(boundary);
            free(temp);
            return NULL;
        }
        else {
            boundary = temp;
        }
        boundary[length - 1] = *end;
        end += 1;
    }
    length += sizeof(char);
    boundary = realloc(boundary, length);
    // if (!boundary || length == 1) {
    //     free(boundary);
    //     return NULL;
    // }
    boundary[length - 1] = '\0';
    return boundary;
};

char *mail_parse(char *content) {
    if (!content) {
        return NULL;
    }
    info_t msg_info = {
            NULL,
            NULL,
            NULL,
            NULL,
            true,
            0
    };
    state_t state = S_HBEGIN;
    while (*content) {
        char *end = content;
        lexem_t lexem = get_lexem(&state, content, &end, &msg_info);
        if (lexem == L_ERR) {
            free_msg_info(&msg_info);
            return NULL;
        }
        rule_t rule = transitions[state][lexem];
        if (rule.state == S_ERR) {
            free_msg_info(&msg_info);
            return NULL;
        }
        if (!rule.action) {
            free_msg_info(&msg_info);
            return NULL;
        }
        state = rule.state;
        content = end;
        if (msg_info.boundary != NULL && state == S_PART) {
            state = S_MPART;
        }
        if (strlen(content) == 0) {
            if (state == S_PART) {
                msg_info.part++;
            }
            if (msg_info.empty) {
                msg_info.part = 0;
            }
            // printf("%s", get_result(&msg_info));
            return get_result(&msg_info);
        }
    }
    free_msg_info(&msg_info);
    return NULL;
}

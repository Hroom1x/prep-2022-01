#define _GNU_SOURCE

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "parser.h"

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

typedef bool (*action_t)(const char *content, const char **end);
// typedef bool (*action_t)(const char *s, const char **end, cjson_callback_t callback)

typedef struct {
    state_t state;
    action_t action;
} rule_t;

static char *get_boundary(char **end);
static void print_value(const char *content, const char **end);
static lexem_t get_lexem(const state_t *state, const char *boundary, char *content, char **end);

// Таблица переходов - матрица размерности
// "число состояний" x "число лексем"

static rule_t transitions[S_COUNT][L_COUNT] = {
        //                L_HBEGIN                                    L_HVALUE                                 L_HEND                                  L_BOUNDARY
        /* S_HBEGIN  */  {{S_HBEGIN, get_lexem},  {S_HVALUE, get_lexem},   {S_HEND, get_lexem},    {S_ERR, NULL}},
        /* S_HVALUE  */  {{S_ERR, NULL},                 {S_HVALUE, get_lexem},   {S_HEND, get_lexem},    {S_ERR, NULL}},
        /* S_HEND    */  {{S_HBEGIN, get_lexem},  {S_HVALUE, get_lexem},   {S_HEND, get_lexem},    {S_HEND, get_lexem}},
        /* S_PART    */  {{S_PART, get_lexem},    {S_PART, get_lexem},     {S_PART, get_lexem},    {S_ERR, NULL}},
        /* S_MPART   */  {{S_MPART, get_lexem},   {S_MPART, get_lexem},    {S_MPART, get_lexem},   {S_MPART, get_lexem}},
        /* S_END     */  {{S_ERR, NULL},                 {S_ERR, NULL},                 {S_ERR, NULL},                 {S_ERR, NULL}}
};

static void print_value(const char *content, const char **end) {
    while (true) {
        if ((**end == '\n') && (*(*end + 1) != ' ')) {
            break;
        }
        printf("%c", **end);
        *end = *end + 1;
    }
    *end = *end + 1;
}

static lexem_t get_lexem(const state_t *state, const char *boundary, char *content, char **end) {
    if (*state != S_PART) {
        if ((*content == '-') && (*(content + 1) == '-')) {
            *end = content + strlen(boundary) + 3;
            // *state = S_PART;  // May be MPART
            return L_BOUNDARY;
        } else if (*content && (*content != ' ') && (*content != '\n')) {
            // *state = S_HBEGIN;
            *end = content;
            while (**end != ':') {
                *end = *end + 1;
            }
            *end = *end + 1;
            // *state = S_HVALUE;
            char *header = malloc(*end - content);
            header = strncpy(header, content, *end - content - 1);
            if (strcasestr(header, "Content-Type") != NULL) {
                boundary = get_boundary(*end);
                // *state = S_HEND;
            }
            // TODO(ME): скипать значение заголовка, если оно нам не нравится
            if (strcasestr(header, "From") != NULL) {
                print_value(content, (const char **) end);
                // *state = S_HEND;
            }
            free(header);
            return L_HEND;
        }
    } else {  // Part_counting
        puts("123");
    }
    return L_ERR;
};

static char *get_boundary(char **end) {
    char *start = *end;
    // Получение конца header value
    size_t length = 0;
    while ((**end != '\n') && (*(*end + 1) != ' ')) {
        length++;
        *end = *end + 1;
    }
    char *value = malloc(length);
    value = strncpy(value, start, length);
    char *boundary_start = strcasestr(value, "boundary");
    if (boundary_start) {
        boundary_start = 9 + boundary_start;
        char *boundary = NULL;
        char *buf = boundary_start;
        length = 0;
        while ((*buf != ' ') && (*buf != '\n')) {
            length++;
            buf++;
        }
        if ((*boundary_start == '\"') || (*boundary_start == '\'')) {
            boundary = malloc(length - 2);
            boundary = strncpy(boundary, boundary_start, length);
        } else {
            boundary = malloc(length);
            boundary = strncpy(boundary, boundary_start, length);
        }
        free(value);
        return boundary;
    }
    free(value);
    return NULL;
};

char *mail_parse(char *content) {
    if (!content) {
        return NULL;
    }
    state_t state = S_HBEGIN;
    char *boundary;
    while (*content) {
        char *end;
        lexem_t lexem = get_lexem(&state, boundary, content, &end);
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
            puts("\n\n==== END ====\n\n");
            return content;
        }
        state = rule.state;
        content = end;
    }
    return NULL;
}

/*

typedef bool (*action_t)(const char *s, const char **end, cjson_callback_t callback);

typedef struct {
    state_t state;
    action_t action;
} rule_t;

// Таблица переходов - матрица размерности
// "число состояний" x "число лексем"

static lexem_t get_lexem(const char *s, const char **end);
static lexem_t get_string(const char *s, const char **end);
static lexem_t get_value(const char *s, const char **end);

static bool extract_json(const char *s, const char **end, cjson_callback_t callback);

static rule_t transitions[S_COUNT][L_COUNT] = {
//             L_OBEGIN                         L_OEND               L_ABEGIN                        L_AEND               L_COLON              L_COMMA               L_STR                            L_NUM                             L_BOOL                            L_NULL
 S_BEGIN   {{ S_OBEGIN, on_obegin},         { S_ERR, NULL },     { S_ABEGIN, on_abegin },        { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_END, on_str },               { S_END, on_num },                { S_END, on_bool },               { S_END, on_null }},
 S_OBEGIN  {{ S_ERR, NULL },                { S_END, on_oend },  { S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_OKEY, on_str },              { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }},
 S_OKEY    {{ S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },                { S_ERR, NULL },     { S_OCOLON, NULL },  { S_ERR, NULL },      { S_ERR, NULL },                 { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }},
 S_OCOLON  {{ S_OVALUE, extract_json },     { S_ERR, NULL },     { S_OVALUE, extract_json },     { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_OVALUE, extract_json },      { S_OVALUE, extract_json },       { S_OVALUE, extract_json },       { S_OVALUE, extract_json }},
 S_OVALUE  {{ S_ERR, NULL },                { S_END, on_oend },  { S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },     { S_OCOMMA, NULL },   { S_ERR, NULL },                 { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }},
 S_OCOMMA  {{ S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_OKEY, on_str },              { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }},
 S_ABEGIN  {{ S_AVALUE, extract_json },     { S_ERR, NULL },     { S_AVALUE, extract_json },     { S_END, on_aend },  { S_ERR, NULL },     { S_ERR, NULL },      { S_AVALUE, extract_json },      { S_AVALUE, extract_json },       { S_AVALUE, extract_json },       { S_AVALUE, extract_json }},
 S_AVALUE  {{ S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },                { S_END, on_aend },  { S_ERR, NULL },     { S_ACOMMA, NULL },   { S_ERR, NULL },                 { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }},
 S_ACOMMA  {{ S_AVALUE, extract_json },     { S_ERR, NULL },     { S_AVALUE, extract_json },     { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_AVALUE, extract_json },      { S_AVALUE, extract_json },       { S_AVALUE, extract_json },       { S_AVALUE, extract_json }},
 S_END     {{ S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },                { S_ERR, NULL },     { S_ERR, NULL },     { S_ERR, NULL },      { S_ERR, NULL },                 { S_ERR, NULL },                  { S_ERR, NULL },                  { S_ERR, NULL }}
};

static bool is_hex(char sym) {
    return (sym >= '0' && sym <= '9') || (sym >= 'A' && sym <= 'F') || (sym >= 'a' && sym <= 'f');
}

static lexem_t get_string(const char *s, const char **end) {
    ++s;
    while (*s != '\0' && *s != '"') {
        if (*s == '\\') {
            char next = *(s + 1);
            if (next == '"' || next == '\\' || next == '/' || next == 'b' || next == 'f' || next == 'n' || next == 'r' || next == 't') {
                ++s;
            } else if (next == 'u' && is_hex(*(s + 2)) && is_hex(*(s + 3)) && is_hex(*(s + 4)) && is_hex(*(s + 5))) {
                s += 4;
            } else {
                return L_ERR;
            }
        }
        ++s;
    }
    if (*s == '\0') {
        return L_ERR;
    }
    *end = s + 1;
    return L_STR;
}

static lexem_t get_value(const char *s, const char **end) {
    if (strncmp(s, "true", sizeof("true") - 1) == 0) {
        *end = s + sizeof("true") - 1;
        return L_BOOL;
    }

    if (strncmp(s, "false", sizeof("false") - 1) == 0) {
        *end = s + sizeof("false") - 1;
        return L_BOOL;
    }

    if (strncmp(s, "null", sizeof("null") - 1) == 0) {
        *end = s + sizeof("null") - 1;
        return L_NULL;
    }

    char *tmp;
    strtod(s, &tmp);
    if (tmp == s) {
        return L_ERR;
    }
    *end = tmp;
    return L_NUM;
}

// Лексемы: { } [ ] : , STR NUM BOOL NULL
static lexem_t get_lexem(const char *s, const char **end) {
    if (!s || !end) { return L_ERR; }

    *end = s + 1;
    switch (*s) {
        case '{': return L_OBEGIN;
        case '}': return L_OEND;
        case '[': return L_ABEGIN;
        case ']': return L_AEND;
        case ':': return L_COLON;
        case ',': return L_COMMA;
        case '"': return get_string(s, end);
        default: return get_value(s, end);
    }

    return L_ERR;
}

static bool extract_json(const char *s, const char **end, cjson_callback_t callback) {
    state_t state = S_BEGIN;
    while (*s) {
        while (isspace(*s)) {
            ++s;
        }
        lexem_t lexem = get_lexem(s, end);
        if (lexem == L_ERR) {
            return false;
        }
        rule_t rule = transitions[state][lexem];
        if (rule.state == S_ERR) {
            return false;
        }
        if (rule.action != NULL) {
            if (!rule.action(s, end, callback)) {
                return false;
            }
        }
        state = rule.state;
        if (rule.state == S_END) {
            break;
        }
        s = *end;
    }
    return state == S_END;
}

{
    "key1": {
        "key2": [1, false, "str"],
        "key3": true
    },
    "key4": "hello"
}

bool cjson_parse(const char *s, cjson_callback_t callback) {
    if (!s) {
        return false;
    }
    const char *end;
    if (!extract_json(s, &end, callback)) {
        return false;
    }
    while (isspace(*end)) {
        ++end;
    }
    return *end == '\0';
}

*/

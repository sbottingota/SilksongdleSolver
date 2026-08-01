#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

static inline uint64_t parse_bitfield(char *token) {
    char *end;
    errno = 0;

    uintmax_t value = strtoumax(token, &end, 16);

    // god I hate c string parsing
    if (end == token || *end != '\0' || errno == ERANGE || value > UINT64_MAX) {
        fprintf(stderr, "Invalid bitfield '%s'.\n", token);
        exit(EXIT_FAILURE);
    }

    return value;
}

static inline int parse_int(char *token) {
    char *end;
    errno = 0;

    long value = strtol(token, &end, 10);

    // have I mentioned how much I hate c string parsing
    if (end == token || *end != '\0' || errno == ERANGE || value < INT_MIN || value > INT_MAX) {
        fprintf(stderr, "Invalid number '%s'.\n", token);
        exit(EXIT_FAILURE);
    }

    return value;
}

struct Guess parse_line(char *line) {
    struct Guess guess;
    char *token = strtok(line, ",");

    for (int i = 0; i < N_LINE_TOKENS; ++i) {
        if (token == NULL) {
            fprintf(stderr, "Parser found line which was too short: note that lines require %d comma-separated tokens.\n", N_LINE_TOKENS);
            exit(EXIT_FAILURE);
        }

        // trim trailing newline if applicable
        size_t token_len = strlen(token);
        if (token[token_len - 1] == '\n') {
            token[token_len - 1] = '\0';
        }

        switch (i) {
            case 0: // name
            if (strlen(token) >= GUESS_NAME_LENGTH) {
                fprintf(stderr, "Guess name '%s' exceeds maximum length of %d characters.\n", token, GUESS_NAME_LENGTH);
                exit(EXIT_FAILURE);
            }
            strcpy(guess.name, token);
            break;

            case 1: // type
            guess.type = parse_bitfield(token);
            break;

            case 2: // location
            guess.location = parse_bitfield(token);
            break;

            case 3: // color
            guess.color = parse_bitfield(token);
            break;

            case 4: // health
            guess.health = parse_int(token);
            break;

            case 5: // kill_count
            guess.kill_count = parse_int(token);
            break;

            default:
            fprintf(stderr, "Mismatch between expected number of tokens (set in N_LINE_TOKENS) and parsing cases in parse_line().\n");
            fprintf(stderr, "N_LINE_TOKENS = %d.\n", N_LINE_TOKENS);
            exit(EXIT_FAILURE);
        }

        token = strtok(NULL, ",");
    }

    if (token == NULL) {
        return guess;

    } else {
        fprintf(stderr, "Parser found line which was too long: note that lines require %d comma separated tokens.\n", N_LINE_TOKENS);
        exit(EXIT_FAILURE);
    }
}

struct GuessListNode *parse_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }

    struct GuessListNode *head = NULL, *tail = NULL;

    char line[BUFFER_SIZE];

    while (fgets(line, BUFFER_SIZE, fp)) {
        struct GuessListNode *next_node = malloc(sizeof(struct GuessListNode));
        if (head == NULL) {
            head = tail = next_node;
        }

        next_node->guess = parse_line(line);
        tail->next = next_node;
        tail = next_node;
    }

    fclose(fp);

    tail->next = NULL;
    return head;
}

void free_guess_list(struct GuessListNode *node) {
    while (node != NULL) {
        struct GuessListNode *next = node->next;
        free(node);
        node = next;
    }
}


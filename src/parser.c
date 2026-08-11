#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

#include "helper.h"

#define PRIMARY_SEPARATOR ","
#define SECONDARY_SEPARATOR ";"

typedef uint64_t (*ParserPtr)(const char *);

static uint64_t parse_type(const char *type) {
    for (size_t i = 0; i < sizeof type_names / sizeof type_names[0]; ++i) {
        if (strcmp(type_names[i].name, type) == 0) {
            return type_names[i].value;
        }
    }

    fprintf(stderr, "Unknown type '%s'.\n", type);
    exit(EXIT_FAILURE);
}

static uint64_t parse_location(const char *location) {
    for (size_t i = 0; i < sizeof location_names / sizeof location_names[0]; ++i) {
        if (strcmp(location_names[i].name, location) == 0) {
            return location_names[i].value;
        }
    }

    fprintf(stderr, "Unknown location '%s'.\n", location);
    exit(EXIT_FAILURE);
}

static uint64_t parse_color(const char *color) {
    for (size_t i = 0; i < sizeof color_names / sizeof color_names[0]; ++i) {
        if (strcmp(color_names[i].name, color) == 0) {
            return color_names[i].value;
        }
    }

    fprintf(stderr, "Unknown color '%s'.\n", color);
    exit(EXIT_FAILURE);
}

static uint64_t parse_bitfield(char *field, ParserPtr parser) {
    uint64_t bitfield = 0;

    char *token;
    char *rest = field;

    while ((token = strtok_r(rest, SECONDARY_SEPARATOR, &rest))) {
        bitfield |= parser(token);
    }

    return bitfield;
}

static int parse_int(char *token) {
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

static struct Guess parse_line(char *line) {
    struct Guess guess;
    char *rest = line;
    char *token = strtok_r(rest, PRIMARY_SEPARATOR, &rest);

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
            guess.type = parse_bitfield(token, parse_type);
            break;

            case 2: // location
            guess.location = parse_bitfield(token, parse_location);
            break;

            case 3: // color
            guess.color = parse_bitfield(token, parse_color);
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

        token = strtok_r(rest, PRIMARY_SEPARATOR, &rest);
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

void free_list_node(struct GuessListNode **head, struct GuessListNode *node, struct GuessListNode *prev) {
    // reattach adjacent nodes in list
    if (prev != NULL) {
        prev->next = node->next;
    } else {
        *head = node->next;
    }

    free(node);
}

static enum ResultInfo char_to_result_info(char c) {
    switch (c) {
        case 'g':
            return GREEN_RESULT;
        case 'o':
            return ORANGE_RESULT;
        case 'r':
            return RED_RESULT;
        case 'h':
            return HIGHER_RESULT;
        case 'l':
            return LOWER_RESULT;
        default:
            fprintf(stderr, "Invalid guess result '%c'.\n", c);
            exit(EXIT_FAILURE);
    }
}

struct GuessResult parse_guess(char *guess_str, struct Guess guess) {
    // trim trailing newline
    size_t guess_str_len = strlen(guess_str);
    if (guess_str[guess_str_len - 1] == '\n') {
        guess_str[guess_str_len - 1] = '\0';
    }

    if (strlen(guess_str) != 5) {
        fprintf(stderr, "Guess strings must be 5 characters long but was of length %lu.\n", strlen(guess_str));
        fprintf(stderr, "String: '%s'\n", guess_str);
        exit(EXIT_FAILURE);
    }

    struct GuessResult result;

    result.type = guess.type;
    result.type_info = char_to_result_info(guess_str[0]);

    result.location = guess.location;
    result.location_info = char_to_result_info(guess_str[1]);

    result.color = guess.color;
    result.color_info = char_to_result_info(guess_str[2]);

    result.health = guess.health;
    result.health_info = char_to_result_info(guess_str[3]);

    result.kill_count = guess.kill_count;
    result.kill_count_info = char_to_result_info(guess_str[4]);

    return result;
}

// debug functions

static void print_bitfield(uint64_t value, const struct NamedBitfield *names, size_t n_names) {
    printf("[");

    bool first = true;
    for (size_t i = 0; i < n_names; ++i) {

        if ((value & names[i].value) != 0) {
            if (!first) {
                printf(", ");
            }

            printf("%s", names[i].name);

            first = false;
        }
    }

    printf("]");
}

static void print_combos(const uint64_t *combo_array, size_t n_combos, const struct NamedBitfield *names, size_t n_names) {
    printf("{");
    for (size_t i = 0; i < n_combos && combo_array[i] != 0; ++i) {
        if (i != 0) {
            printf(", ");
        }

        print_bitfield(combo_array[i], names, n_names);
    }
    printf("}");
}

void print_guess_info(const struct GuessInfo *info) {
    if (info->is_type_correct) {
        printf("type: ");
        print_bitfield(info->possible_types, type_names, ARRAY_LENGTH(type_names));
        printf("\n");

    } else {
        printf("possible types: ");
        print_bitfield(info->possible_types, type_names, ARRAY_LENGTH(type_names));
        printf("\n");

        printf("necessary type combos: ");
        print_combos(info->necessary_type_combos, ARRAY_LENGTH(info->necessary_type_combos),
            type_names, ARRAY_LENGTH(type_names));
        printf("\n");
    }

    printf("\n");

    if (info->is_location_correct) {
        printf("location: ");
        print_bitfield(info->possible_locations, location_names, ARRAY_LENGTH(location_names));
        printf("\n");

    } else {
        printf("possible locations: ");
        print_bitfield(info->possible_locations, location_names, ARRAY_LENGTH(location_names));
        printf("\n");

        printf("necessary location combos: ");
        print_combos(info->necessary_location_combos, ARRAY_LENGTH(info->necessary_location_combos),
            location_names, ARRAY_LENGTH(location_names));
        printf("\n");
    }

    printf("\n");

    if (info->is_color_correct) {
        printf("color: ");
        print_bitfield(info->possible_colors, color_names, ARRAY_LENGTH(color_names));
        printf("\n");

    } else {
        printf("possible colors: ");
        print_bitfield(info->possible_colors, color_names, ARRAY_LENGTH(color_names));
        printf("\n");

        printf("necessary color combos: ");
        print_combos(info->necessary_color_combos, ARRAY_LENGTH(info->necessary_color_combos),
            color_names, ARRAY_LENGTH(color_names));
        printf("\n");
    }

    printf("\n");

    if (info->is_health_correct) {
        printf("health: %d\n", info->health);
    } else {
        printf("min health: %d, max health: %d\n", info->min_health, info->max_health);
    }

    printf("\n");

    if (info->is_kill_count_correct) {
        printf("kill_count: %d\n", info->kill_count);
    } else {
        printf("min kill_count: %d, max kill_count: %d\n", info->min_kill_count, info->max_kill_count);
    }

    printf("\n");
}

void print_guesses(const struct GuessListNode *guess_list) {
    printf("[");

    for (const struct GuessListNode *node = guess_list; node != NULL; node = node->next) {
        // print separator if this node isn't the first one
        if (guess_list != node) {
            printf(", ");
        }

        printf("%s", node->guess.name);
    }

    printf("]");
}


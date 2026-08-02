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

uint64_t parse_type(const char *type) {
    if (strcmp(type, "boss") == 0) return BOSS;
    if (strcmp(type, "enemy") == 0) return ENEMY;
    if (strcmp(type, "merchant") == 0) return MERCHANT;
    if (strcmp(type, "npc") == 0) return NPC;
    if (strcmp(type, "player") == 0) return PLAYER;

    fprintf(stderr, "Unknown type '%s'.\n", type);
    exit(EXIT_FAILURE);
}

uint64_t parse_location(const char *location) {
    if (strcmp(location, "all") == 0) return ALL;
    if (strcmp(location, "bellhart") == 0) return BELLHART;
    if (strcmp(location, "bellways") == 0) return BELLWAYS;
    if (strcmp(location, "bilewater") == 0) return BILEWATER;
    if (strcmp(location, "blasted steps") == 0) return BLASTED_STEPS;
    if (strcmp(location, "bone bottom") == 0) return BONE_BOTTOM;
    if (strcmp(location, "bonegrave") == 0) return BONEGRAVE;
    if (strcmp(location, "chapel of the beast") == 0) return CHAPEL_OF_THE_BEAST;
    if (strcmp(location, "choral chambers") == 0) return CHORAL_CHAMBERS;
    if (strcmp(location, "cogwork core") == 0) return COGWORK_CORE;
    if (strcmp(location, "coral tower") == 0) return CORAL_TOWER;
    if (strcmp(location, "deep docks") == 0) return DEEP_DOCKS;
    if (strcmp(location, "exhaust organ") == 0) return EXHAUST_ORGAN;
    if (strcmp(location, "far fields") == 0) return FAR_FIELDS;
    if (strcmp(location, "grand gate") == 0) return GRAND_GATE;
    if (strcmp(location, "greymoor") == 0) return GREYMOOR;
    if (strcmp(location, "halfway home") == 0) return HALFWAY_HOME;
    if (strcmp(location, "high halls") == 0) return HIGH_HALLS;
    if (strcmp(location, "hunters march") == 0) return HUNTERS_MARCH;
    if (strcmp(location, "lost verdania") == 0) return LOST_VERDANIA;
    if (strcmp(location, "memorium") == 0) return MEMORIUM;
    if (strcmp(location, "moss grotto") == 0) return MOSS_GROTTO;
    if (strcmp(location, "mosshome") == 0) return MOSSHOME;
    if (strcmp(location, "mount fay") == 0) return MOUNT_FAY;
    if (strcmp(location, "putrified ducts") == 0) return PUTRIFIED_DUCTS;
    if (strcmp(location, "red memory") == 0) return RED_MEMORY;
    if (strcmp(location, "ruined chapel") == 0) return RUINED_CHAPEL;
    if (strcmp(location, "sands of karak") == 0) return SANDS_OF_KARAK;
    if (strcmp(location, "shellwood") == 0) return SHELLWOOD;
    if (strcmp(location, "sinners road") == 0) return SINNERS_ROAD;
    if (strcmp(location, "songclave") == 0) return SONGCLAVE;
    if (strcmp(location, "the abyss") == 0) return THE_ABYSS;
    if (strcmp(location, "the cradle") == 0) return THE_CRADLE;
    if (strcmp(location, "the marrow") == 0) return THE_MARROW;
    if (strcmp(location, "the mist") == 0) return THE_MIST;
    if (strcmp(location, "the slab") == 0) return THE_SLAB;
    if (strcmp(location, "underworks") == 0) return UNDERWORKS;
    if (strcmp(location, "voltnest") == 0) return VOLTNEST;
    if (strcmp(location, "weavenest atla") == 0) return WEAVENEST_ATLA;
    if (strcmp(location, "whispering vaults") == 0) return WHISPERING_VAULTS;
    if (strcmp(location, "whiteward") == 0) return WHITEWARD;
    if (strcmp(location, "wisp thicket") == 0) return WISP_THICKET;
    if (strcmp(location, "wormways") == 0) return WORMWAYS;

    fprintf(stderr, "Unknown location '%s'.\n", location);
    exit(EXIT_FAILURE);
}

uint64_t parse_color(const char *color) {
    if (strcmp(color, "???") == 0) return QUESTION_MARK;
    if (strcmp(color, "beige") == 0) return BEIGE;
    if (strcmp(color, "black") == 0) return BLACK;
    if (strcmp(color, "blue") == 0) return BLUE;
    if (strcmp(color, "brown") == 0) return BROWN;
    if (strcmp(color, "gold") == 0) return GOLD;
    if (strcmp(color, "gray") == 0) return GRAY;
    if (strcmp(color, "green") == 0) return GREEN;
    if (strcmp(color, "orange") == 0) return ORANGE;
    if (strcmp(color, "pink") == 0) return PINK;
    if (strcmp(color, "purple") == 0) return PURPLE;
    if (strcmp(color, "red") == 0) return RED;
    if (strcmp(color, "white") == 0) return WHITE;
    if (strcmp(color, "yellow") == 0) return YELLOW;

    fprintf(stderr, "Unknown color '%s'.\n", color);
    exit(EXIT_FAILURE);
}

uint64_t parse_bitfield(char *field, ParserPtr parser) {
    uint64_t bitfield = 0;

    char *token;
    char *rest = field;

    while ((token = strtok_r(rest, SECONDARY_SEPARATOR, &rest))) {
        bitfield |= parser(token);
    }

    return bitfield;
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

enum ResultInfo char_to_result_info(char c) {
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



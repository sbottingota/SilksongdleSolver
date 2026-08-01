#include "logic.h"

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

void modify_guess_info(struct GuessInfo *info, struct GuessResult result) {
    switch (result.type_info) {
        case GREEN_RESULT:
        info->possible_types = result.type;
        info->is_type_correct = true;
        break;

        case ORANGE_RESULT:
        info->possible_types |= result.type;
        break;

        case RED_RESULT:
        info->possible_types &= ~result.type;
        break;

        default:
        fprintf(stderr, "Invalid 'type' field in result.\n");
        exit(1);
    }

    switch (result.location_info) {
        case GREEN_RESULT:
        info->possible_locations = result.location;
        info->is_location_correct = true;
        break;

        case ORANGE_RESULT:
        info->possible_locations |= result.location;
        break;

        case RED_RESULT:
        info->possible_locations &= ~result.location;
        break;

        default:
        fprintf(stderr, "Invalid 'location' field in result.\n");
        exit(1);
    }

    switch (result.color_info) {
        case GREEN_RESULT:
        info->possible_colors = result.color;
        info->is_color_correct = true;
        break;

        case ORANGE_RESULT:
        info->possible_colors |= result.color;
        break;

        case RED_RESULT:
        info->possible_colors &= ~result.color;
        break;

        default:
        fprintf(stderr, "Invalid 'color' field in result.\n");
        exit(1);
    }

    switch (result.health_info) {
        case GREEN_RESULT:
        info->health = result.health;
        info->is_health_correct = true;
        break;

        case HIGHER_RESULT:
        info->min_health = result.health + 1;
        break;

        case LOWER_RESULT:
        info->max_health = result.health - 1;
        break;

        case RED_RESULT:
        info->health = -1;
        info->is_health_correct = true;
        break;

        default:
        fprintf(stderr, "Invalid 'health' field in result.\n");
        exit(1);
    }

    switch (result.kill_count_info) {
        case GREEN_RESULT:
        info->kill_count = result.kill_count;
        info->is_kill_count_correct = true;
        break;

        case HIGHER_RESULT:
        info->min_kill_count = result.kill_count + 1;
        break;

        case LOWER_RESULT:
        info->max_kill_count = result.kill_count - 1;
        break;

        case RED_RESULT:
        info->kill_count = -1;
        info->is_kill_count_correct = true;
        break;

        default:
        fprintf(stderr, "Invalid 'kill count' field in result.\n");
        exit(1);
    }
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
            exit(1);
    }
}

struct GuessResult parse_guess(const char *guess_str, struct Guess guess) {
    if (strlen(guess_str) != 5) {
        fprintf(stderr, "Guess strings must be 5 characters long but was of length %lu.\n", strlen(guess_str));
        fprintf(stderr, "String: '%s'\n", guess_str);
        exit(1);
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

void add_combo(uint64_t combo, uint64_t *combo_array) {
    for (int i = 0; i < COMBO_ARRAY_LENGTH; ++i) {
        if (combo_array[i] == 0) {
            combo_array[i] = combo;
            return;

        } else if (combo_array[i] == combo) return; // combination is already in array
    }

    fprintf(stderr, "Ran out of space in combo array when trying to add '%lu'.\n", combo);
    exit(1);
}

struct GuessInfo get_blank_guess_info(void) {
    struct GuessInfo info;

    info.possible_types = ALL_TYPES;
    info.necessary_type_combos[0] = 0;
    info.is_type_correct = false;

    info.possible_locations = ALL_LOCATIONS;
    info.necessary_location_combos[0] = 0;
    info.is_location_correct = false;

    info.possible_colors = ALL_COLORS;
    info.necessary_color_combos[0] = 0;
    info.is_color_correct = false;

    info.min_health = -1;
    info.max_health = INT_MAX;
    info.has_health = true;
    info.is_health_correct = false;

    info.min_kill_count = -1;
    info.max_kill_count = INT_MAX;
    info.has_kill_count = true;
    info.is_kill_count_correct = false;

    return info;
}


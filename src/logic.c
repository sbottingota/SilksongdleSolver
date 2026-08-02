#include "logic.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }
}

void add_combo(uint64_t combo, uint64_t *combo_array) {
    for (int i = 0; i < COMBO_ARRAY_LENGTH; ++i) {
        if (combo_array[i] == 0) {
            combo_array[i] = combo;
            return;

        } else if (combo_array[i] == combo) return; // combination is already in array
    }

    fprintf(stderr, "Ran out of space in combo array when trying to add '%lu'.\n", combo);
    exit(EXIT_FAILURE);
}

bool is_guess_valid(struct Guess guess, struct GuessInfo guess_info) {
    if (guess_info.is_type_correct) {
        if (guess.type != guess_info.possible_types) return false;
    } else {
        if ((guess.type & guess_info.possible_types) == 0) return false;
    }

    if (guess_info.is_location_correct) {
        if (guess.location != guess_info.possible_locations) return false;
    } else {
        if ((guess.location & guess_info.possible_locations) == 0) return false;
    }

    if (guess_info.is_color_correct) {
        if (guess.color != guess_info.possible_colors) return false;
    } else {
        if ((guess.color & guess_info.possible_colors) == 0) return false;
    }

    for (int i = 0; i < COMBO_ARRAY_LENGTH; ++i) {
        if (!guess_info.is_type_correct && guess_info.necessary_type_combos[i] != 0
            && (guess.type & guess_info.necessary_type_combos[i]) == 0) return false;

        if (!guess_info.is_location_correct && guess_info.necessary_location_combos[i] != 0
            && (guess.location & guess_info.necessary_location_combos[i]) == 0) return false;

        if (!guess_info.is_color_correct && guess_info.necessary_color_combos[i] != 0
            && (guess.color & guess_info.necessary_color_combos[i]) == 0) return false;
    }

    if (guess_info.is_health_correct) {
        if (guess.health != guess_info.health) return false;
    } else {
        if (!(guess_info.min_health < guess.health && guess.health < guess_info.max_health)) return false;
    }

    if (guess_info.is_kill_count_correct) {
        if (guess.kill_count != guess_info.kill_count) return false;
    } else {
        if (!(guess_info.min_kill_count < guess.kill_count && guess.kill_count < guess_info.max_kill_count)) return false;
    }

    return true;
}

static inline enum ResultInfo get_bitfield_result_info(uint64_t guess_bitfield, uint64_t answer_bitfield) {
    if (guess_bitfield == answer_bitfield) {
        return GREEN_RESULT;
    } else if ((guess_bitfield & answer_bitfield) != 0) {
        return ORANGE_RESULT;
    } else {
        return RED_RESULT;
    }
}

static inline enum ResultInfo get_number_result_info(int guess_number, int answer_number) {
    if (guess_number == answer_number) {
        return GREEN_RESULT;
    } else if (guess_number == -1 || answer_number == -1) { // check for n/a
        return RED_RESULT;
    } else if (guess_number > answer_number) {
        return LOWER_RESULT;
    } else {
        return HIGHER_RESULT;
    }
}

struct GuessResult get_guess_result(struct Guess guess, struct Guess answer) {
    struct GuessResult result;

    result.type = guess.type;
    result.type_info = get_bitfield_result_info(guess.type, answer.type);

    result.location = guess.location;
    result.location_info = get_bitfield_result_info(guess.location, answer.location);

    result.color = guess.color;
    result.color_info = get_bitfield_result_info(guess.color, answer.color);

    result.health = guess.health;
    result.health_info = get_number_result_info(guess.health, answer.health);

    result.kill_count = guess.kill_count;
    result.kill_count_info = get_number_result_info(guess.kill_count, answer.kill_count);

    return result;
}

double calculate_result_entropy(struct GuessInfo info, struct GuessResult result, struct GuessListNode *search_space) {
    double search_space_size = 0;
    double n_valid_states = 0;

    modify_guess_info(&info, result);

    for (struct GuessListNode *node = search_space; node != NULL; node = node->next) {
        ++search_space_size;

        if (is_guess_valid(node->guess, info)) {
            ++n_valid_states;
        }
    }

    return -log2(n_valid_states / search_space_size);
}

double calculate_expected_entropy(struct Guess guess, struct GuessInfo info, struct GuessListNode *search_space) {
    double entropy_sum = 0;
    double search_space_size = 0;

    for (struct GuessListNode *node = search_space; node != NULL; node = node->next) {
        struct GuessResult guess_result = get_guess_result(guess, node->guess);
        entropy_sum += calculate_result_entropy(info, guess_result, search_space);

        ++search_space_size;
    }

    return entropy_sum / search_space_size;
}

struct Guess calculate_best_guess(struct GuessInfo info, struct GuessListNode *search_space) {
    struct Guess best_guess;
    double best_expected_entropy = 0;

    for (struct GuessListNode *node = search_space; node != NULL; node = node->next) {
        double expected_entropy = calculate_expected_entropy(node->guess, info, search_space);

        if (expected_entropy > best_expected_entropy) {
            best_guess = node->guess;
            best_expected_entropy = expected_entropy;
        }
    }

    // ensure that the function *has* actually found a guess
    if (best_expected_entropy > 0) {
        return best_guess;

    } else {
        fprintf(stderr, "No guess found in calculate_best_guess(). Perhaps search space is empty?\n");
        exit(EXIT_FAILURE);
    }
}

void cull_search_space(struct GuessListNode **search_space, struct GuessInfo info) {
    struct GuessListNode *prev = NULL;
    struct GuessListNode *next;

    for (struct GuessListNode *node = *search_space; node != NULL; node = next) {
        next = node->next;

        if (!is_guess_valid(node->guess, info)) {
            free_list_node(search_space, node, prev);

        } else {
            prev = node;
        }
    }
}


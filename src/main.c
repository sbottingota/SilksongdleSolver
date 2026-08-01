#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

enum Type {
    BOSS = 0x1,
    ENEMY = 0x2,
    MERCHANT = 0x4,
    NPC = 0x8,
    PLAYER = 0x10,

    ALL_TYPES = 0x1F
};

enum Color {
    QUESTION_MARK = 0x1,
    BEIGE = 0x2,
    BLACK = 0x4,
    BLUE = 0x8,
    BROWN = 0x10,
    GOLD = 0x20,
    GRAY = 0x40,
    GREEN = 0x80,
    ORANGE = 0x100,
    PINK = 0x200,
    PURPLE = 0x400,
    RED = 0x800,
    WHITE = 0x1000,
    YELLOW = 0x2000,
    
    ALL_COLORS = 0x3FFF
};

enum Location {
    ALL_LOCATIONS = 0xFFFFFF
};

enum ResultInfo {
    GREEN_RESULT,
    ORANGE_RESULT,
    RED_RESULT,

    HIGHER_RESULT,
    LOWER_RESULT,
};

struct Guess {
    char name[16];

    // bitfields
    uint8_t type;
    uint64_t location;
    uint16_t color;

    // actual numbers; use -1 for n/a
    int health;
    int kill_count;
};

struct GuessInfo {
    uint8_t possible_types;
    uint8_t impossible_types;
    bool is_type_correct;

    uint64_t possible_locations;
    uint64_t impossible_locations;
    bool is_location_correct;

    uint16_t possible_colors;
    uint16_t impossible_colors;
    bool is_color_correct;

    union {
        struct {
            int min_health, max_health;
            bool has_health;
        };
        int health;
    };
    bool is_health_correct;

    union {
        struct {
            int min_kill_count, max_kill_count;
            bool has_kill_count;
        };
        int kill_count;
    };
    bool is_kill_count_correct;
};

struct GuessResult {
    uint8_t type;
    enum ResultInfo type_info;

    uint64_t location;
    enum ResultInfo location_info;

    uint16_t color;
    enum ResultInfo color_info;

    int health;
    enum ResultInfo health_info;

    int kill_count;
    enum ResultInfo kill_count_info;
};

struct Guess all_guesses[100];

struct GuessInfo get_blank_guess_info(void) {
    struct GuessInfo info;

    info.possible_types = ALL_TYPES;
    info.impossible_types = 0;
    info.is_type_correct = false;

    info.possible_locations = ALL_LOCATIONS;
    info.impossible_locations = 0;
    info.is_location_correct = false;

    info.possible_colors = ALL_COLORS;
    info.impossible_colors = 0;
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

void init_all_guesses(void) {
    strcpy(all_guesses[0].name, "aknid");
    all_guesses[0].type = ENEMY;
    all_guesses[0].location = 0;
    all_guesses[0].color = WHITE | GREEN;
    all_guesses[0].health = 15;
    all_guesses[0].kill_count = 15;
    
    strcpy(all_guesses[1].name, "alchemist zylotol");
    all_guesses[1].type = NPC;
    all_guesses[1].location = 0;
    all_guesses[1].color = WHITE | BLUE;
    all_guesses[1].health = -1;
    all_guesses[1].kill_count = -1;

    strcpy(all_guesses[2].name, "alita");
    all_guesses[2].type = ENEMY;
    all_guesses[2].location = 0;
    all_guesses[2].color = RED | BLACK;
    all_guesses[2].health = 80;
    all_guesses[2].kill_count = 6;
}

void modify_guess_info(struct GuessInfo *info, struct GuessResult result) {
    switch (result.type_info) {
        case GREEN_RESULT:
        info->possible_types = result.type;
        info->impossible_types = ALL_TYPES & ~result.type;
        info->is_type_correct = true;
        break;

        case ORANGE_RESULT:
        info->possible_types |= result.type;
        break;

        case RED_RESULT:
        info->impossible_types |= result.type;
        info->possible_types &= ~result.type;
        break;

        default:
        fprintf(stderr, "Invalid 'type' field in result.\n");
    }

    switch (result.location_info) {
        case GREEN_RESULT:
        info->possible_locations = result.location;
        info->impossible_locations = ALL_LOCATIONS & ~result.location;
        info->is_location_correct = true;
        break;

        case ORANGE_RESULT:
        info->possible_locations |= result.location;
        break;

        case RED_RESULT:
        info->impossible_locations |= result.location;
        info->possible_locations &= ~result.location;
        break;

        default:
        fprintf(stderr, "Invalid 'location' field in result.\n");
    }

    switch (result.color_info) {
        case GREEN_RESULT:
        info->possible_colors = result.color;
        info->impossible_colors = ALL_LOCATIONS & ~result.color;
        info->is_color_correct = true;
        break;

        case ORANGE_RESULT:
        info->possible_colors |= result.color;
        break;

        case RED_RESULT:
        info->impossible_colors |= result.color;
        info->possible_colors &= ~result.color;
        break;

        default:
        fprintf(stderr, "Invalid 'color' field in result.\n");
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
    }
}

int main() {
    init_all_guesses();

    struct GuessInfo info = get_blank_guess_info();

    printf("Init complete!\n");
}


#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>
#include <stdbool.h>

#define GUESS_NAME_LENGTH 32
#define COMBO_ARRAY_LENGTH 8

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
    ALL = 0x1,
    BELLHART = 0x2,
    BELLWAYS = 0x4,
    BILEWATER = 0x8,
    BLASTED_STEPS = 0x10,
    BONE_BOTTOM = 0x20,
    BONEGRAVE = 0x40,
    CHAPEL_OF_THE_BEAST = 0x80,
    CHORAL_CHAMBERS = 0x100,
    COGWORK_CORE = 0x200,
    CORAL_TOWER = 0x400,
    DEEP_DOCKS = 0x800,
    EXHAUST_ORGAN = 0x1000,
    FAR_FIELDS = 0x2000,
    GRAND_GATE = 0x4000,
    GREYMOOR = 0x8000,
    HALFWAY_HOME = 0x10'000,
    HIGH_HALLS = 0x20'000,
    HUNTERS_MARCH = 0x40'000,
    LOST_VERDANIA = 0x80'000,
    MEMORIUM = 0x100'000,
    MOSS_GROTTO = 0x200'000,
    MOSSHOME = 0x400'000,
    MOUNT_FAY = 0x800'000,
    PUTRIFIED_DUCTS = 0x1'000'000,
    RED_MEMORY = 0x2'000'000,
    RUINED_CHAPEL = 0x4'000'000,
    SANDS_OF_KARAK = 0x8'000'000,
    SHELLWOOD = 0x10'000'000,
    SINNERS_ROAD = 0x20'000'000,
    SONGCLAVE = 0x40'000'000,
    THE_ABYSS = 0x80'000'000,
    THE_CRADLE = 0x100'000'000,
    THE_MARROW = 0x200'000'000,
    THE_MIST = 0x400'000'000,
    THE_SLAB = 0x800'000'000,
    UNDERWORKS = 0x1'000'000'000,
    VOLTNEST = 0x2'000'000'000,
    WEAVENEST_ATLA = 0x4'000'000'000,
    WHISPERING_VAULTS = 0x8'000'000'000,
    WHITEWARD = 0x10'000'000'000,
    WISP_THICKET = 0x20'000'000'000,
    WORMWAYS = 0x40'000'000'000,

    ALL_LOCATIONS = 0x7F'FFF'FFF'FFF
};

enum ResultInfo {
    GREEN_RESULT,
    ORANGE_RESULT,
    RED_RESULT,

    HIGHER_RESULT,
    LOWER_RESULT,
};

struct Guess {
    char name[GUESS_NAME_LENGTH];

    // bitfields
    uint64_t type;
    uint64_t location;
    uint64_t color;

    // actual numbers; use -1 for n/a
    int health;
    int kill_count;
};

// N.B. necessary_xxx_combos arrays use 0 as a null terminator
// these represent a situation where at least one (but not all) of the cases must be true
struct GuessInfo {
    uint64_t possible_types;
    uint64_t necessary_type_combos[COMBO_ARRAY_LENGTH];
    bool is_type_correct;

    uint64_t possible_locations;
    uint64_t necessary_location_combos[COMBO_ARRAY_LENGTH];
    bool is_location_correct;

    uint64_t possible_colors;
    uint64_t necessary_color_combos[COMBO_ARRAY_LENGTH];
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
    uint64_t type;
    enum ResultInfo type_info;

    uint64_t location;
    enum ResultInfo location_info;

    uint64_t color;
    enum ResultInfo color_info;

    int health;
    enum ResultInfo health_info;

    int kill_count;
    enum ResultInfo kill_count_info;
};

#endif

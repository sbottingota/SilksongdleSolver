#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>
#include <stdbool.h>

#define GUESS_NAME_LENGTH 32
#define COMBO_ARRAY_LENGTH 8

#define ARRAY_LENGTH(array) (sizeof (array) / sizeof (array)[0])

struct NamedBitfield {
    uint64_t value;
    const char *name;
};

enum Type {
    BOSS = 1UL<<0,
    ENEMY = 1UL<<1,
    MERCHANT = 1UL<<2,
    NPC = 1UL<<3,
    PLAYER = 1UL<<4,

    ALL_TYPES = (1UL<<5) - 1
};

static const struct NamedBitfield type_names[] = {
    {BOSS, "boss"},
    {ENEMY, "enemy"},
    {MERCHANT, "merchant"},
    {NPC, "npc"},
    {PLAYER, "player"}
};

enum Color {
    QUESTION_MARK = 1UL<<0,
    BEIGE = 1UL<<1,
    BLACK = 1UL<<2,
    BLUE = 1UL<<3,
    BROWN = 1UL<<4,
    GOLD = 1UL<<5,
    GRAY = 1UL<<6,
    GREEN = 1UL<<7,
    ORANGE = 1UL<<8,
    PINK = 1UL<<9,
    PURPLE = 1UL<<10,
    RED = 1UL<<11,
    WHITE = 1UL<<12,
    YELLOW = 1UL<<13,
    
    ALL_COLORS = (1UL<<14) - 1
};

static const struct NamedBitfield color_names[] = {
    {QUESTION_MARK, "???"},
    {BEIGE, "beige"},
    {BLACK, "black"},
    {BLUE, "blue"},
    {BROWN, "brown"},
    {GOLD, "gold"},
    {GRAY, "gray"},
    {GREEN, "green"},
    {ORANGE, "orange"},
    {PINK, "pink"},
    {PURPLE, "purple"},
    {RED, "red"},
    {WHITE, "white"},
    {YELLOW, "yellow"}
};

enum Location {
    ALL = 1UL<<0,
    BELLHART = 1UL<<1,
    BELLWAYS = 1UL<<2,
    BILEWATER = 1UL<<3,
    BLASTED_STEPS = 1UL<<4,
    BONE_BOTTOM = 1UL<<5,
    BONEGRAVE = 1UL<<6,
    CHAPEL_OF_THE_BEAST = 1UL<<7,
    CHORAL_CHAMBERS = 1UL<<8,
    COGWORK_CORE = 1UL<<9,
    CORAL_TOWER = 1UL<<10,
    DEEP_DOCKS = 1UL<<11,
    EXHAUST_ORGAN = 1UL<<12,
    FAR_FIELDS = 1UL<<13,
    GRAND_GATE = 1UL<<14,
    GREYMOOR = 1UL<<15,
    HALFWAY_HOME = 1UL<<16,
    HIGH_HALLS = 1UL<<17,
    HUNTERS_MARCH = 1UL<<18,
    LOST_VERDANIA = 1UL<<19,
    MEMORIUM = 1UL<<20,
    MOSS_GROTTO = 1UL<<21,
    MOSSHOME = 1UL<<22,
    MOUNT_FAY = 1UL<<23,
    PUTRIFIED_DUCTS = 1UL<<24,
    RED_MEMORY = 1UL<<25,
    RUINED_CHAPEL = 1UL<<26,
    SANDS_OF_KARAK = 1UL<<27,
    SHELLWOOD = 1UL<<28,
    SINNERS_ROAD = 1UL<<29,
    SONGCLAVE = 1UL<<30,
    THE_ABYSS = 1UL<<31,
    THE_CRADLE = 1UL<<32,
    THE_MARROW = 1UL<<33,
    THE_MIST = 1UL<<34,
    THE_SLAB = 1UL<<35,
    UNDERWORKS = 1UL<<36,
    VOLTNEST = 1UL<<37,
    WEAVENEST_ATLA = 1UL<<38,
    WHISPERING_VAULTS = 1UL<<39,
    WHITEWARD = 1UL<<40,
    WISP_THICKET = 1UL<<41,
    WORMWAYS = 1UL<<42,

    ALL_LOCATIONS = (1UL<<43) - 1
};

static const struct NamedBitfield location_names[] = {
    {ALL, "all"},
    {BELLHART, "bellhart"},
    {BELLWAYS, "bellways"},
    {BILEWATER, "bilewater"},
    {BLASTED_STEPS, "blasted steps"},
    {BONE_BOTTOM, "bone bottom"},
    {BONEGRAVE, "bonegrave"},
    {CHAPEL_OF_THE_BEAST, "chapel of the beast"},
    {CHORAL_CHAMBERS, "choral chambers"},
    {COGWORK_CORE, "cogwork core"},
    {CORAL_TOWER, "coral tower"},
    {DEEP_DOCKS, "deep docks"},
    {EXHAUST_ORGAN, "exhaust organ"},
    {FAR_FIELDS, "far fields"},
    {GRAND_GATE, "grand gate"},
    {GREYMOOR, "greymoor"},
    {HALFWAY_HOME, "halfway home"},
    {HIGH_HALLS, "high halls"},
    {HUNTERS_MARCH, "hunters march"},
    {LOST_VERDANIA, "lost verdania"},
    {MEMORIUM, "memorium"},
    {MOSS_GROTTO, "moss grotto"},
    {MOSSHOME, "mosshome"},
    {MOUNT_FAY, "mount fay"},
    {PUTRIFIED_DUCTS, "putrified ducts"},
    {RED_MEMORY, "red memory"},
    {RUINED_CHAPEL, "ruined chapel"},
    {SANDS_OF_KARAK, "sands of karak"},
    {SHELLWOOD, "shellwood"},
    {SINNERS_ROAD, "sinners road"},
    {SONGCLAVE, "songclave"},
    {THE_ABYSS, "the abyss"},
    {THE_CRADLE, "the cradle"},
    {THE_MARROW, "the marrow"},
    {THE_MIST, "the mist"},
    {THE_SLAB, "the slab"},
    {UNDERWORKS, "underworks"},
    {VOLTNEST, "voltnest"},
    {WEAVENEST_ATLA, "weavenest atla"},
    {WHISPERING_VAULTS, "whispering vaults"},
    {WHITEWARD, "whiteward"},
    {WISP_THICKET, "wisp thicket"},
    {WORMWAYS, "wormways"}
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

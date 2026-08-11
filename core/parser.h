#ifndef PARSER_H
#define PARSER_H

#include "definitions.h"

#define BUFFER_SIZE 256
#define N_LINE_TOKENS 6

struct GuessListNode {
    struct Guess guess;
    struct GuessListNode *next;
};

struct GuessListNode *parse_file(char *filename);

void free_guess_list(struct GuessListNode *head);

// frees given node, and reattaches next node to previous node
// set prev to NULL if node is at the head of the list
void free_list_node(struct GuessListNode **head, struct GuessListNode *node, struct GuessListNode *prev);

struct GuessListNode *copy_guess_list(const struct GuessListNode *guess_list);

// N.B. this may trim trailing newlines from your string
struct GuessResult parse_guess(char *guess_str, struct Guess guess);

void print_guess_info(const struct GuessInfo *info);
void print_guesses(const struct GuessListNode *guess_list);

#endif

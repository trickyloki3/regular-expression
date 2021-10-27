#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "panic.c"

#include "lex.h"

#include "nfa.c"
#include "dfa.c"

int lex_create(struct lex * lex, int node_size, int edge_size, int span_size, int token_size, int key_size, int set_size) {
    lex->root = 0;

    lex->node = malloc(node_size * sizeof(*lex->node));
    if(lex->node == NULL)
        goto node_fail;

    lex->node_part = 1;
    lex->node_next = 1;
    lex->node_size = node_size;

    lex->edge = malloc(edge_size * sizeof(*lex->edge));
    if(lex->edge == NULL)
        goto edge_fail;

    lex->edge_part = 1;
    lex->edge_next = 1;
    lex->edge_size = edge_size;

    lex->span = malloc(span_size * sizeof(*lex->span));
    if(lex->span == NULL)
        goto span_fail;

    lex->span_part = 1;
    lex->span_next = 1;
    lex->span_size = span_size;

    lex->token = malloc(token_size * sizeof(*lex->token));
    if(lex->token == NULL)
        goto token_fail;

    lex->bound = lex->token + token_size;

    lex->key = malloc(key_size * sizeof(*lex->key));
    if(lex->key == NULL)
        goto key_fail;

    lex->key_part = 0;
    lex->key_next = 0;
    lex->key_prev = key_size;
    lex->key_size = key_size;

    lex->set = malloc(set_size * sizeof(*lex->set));
    if(lex->set == NULL)
        goto set_fail;

    lex->set_next = 0;
    lex->set_prev = set_size;
    lex->set_size = set_size;

    return 0;

set_fail:
    free(lex->key);
key_fail:
    free(lex->token);
token_fail:
    free(lex->span);
span_fail:
    free(lex->edge);
edge_fail:
    free(lex->node);
node_fail:
    return 1;
}

void lex_delete(struct lex * lex) {
    free(lex->set);
    free(lex->key);
    free(lex->token);
    free(lex->span);
    free(lex->edge);
    free(lex->node);
}

void lex_clear(struct lex * lex) {
    lex->root = 0;
    lex->node_part = 1;
    lex->node_next = 1;
    lex->edge_part = 1;
    lex->edge_next = 1;
    lex->span_part = 1;
    lex->span_next = 1;
    lex->key_part = 0;
    lex->key_next = 0;
    lex->key_prev = lex->key_size;
    lex->set_next = 0;
    lex->set_prev = lex->set_size;
}

int lex_compile(struct lex * lex, char * s, int flag) {
    lex->node_part = lex->node_next;
    lex->edge_part = lex->edge_next;
    lex->span_part = lex->span_next;

    if(nfa_construct(lex, s, flag))
        return panic("failed to construct nfa");

    return 0;
}

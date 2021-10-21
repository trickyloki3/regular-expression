#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "panic.c"

#include "lex.h"

int lex_create(struct lex * lex, int node_size, int edge_size, int span_size) {
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

    return 0;

span_fail:
    free(lex->edge);
edge_fail:
    free(lex->node);
node_fail:
    return 1;
}

void lex_delete(struct lex * lex) {
    free(lex->span);
    free(lex->edge);
    free(lex->node);
}

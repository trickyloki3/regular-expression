#ifndef lex_h
#define lex_h

struct node {
    int edge;
    int span;
    int flag;
};

struct edge {
    int node;
    int next;
};

struct span {
    int min;
    int max;
    int node;
    int next;
};

struct lex {
    struct node * node;
    int node_part;
    int node_next;
    int node_size;
    struct edge * edge;
    int edge_part;
    int edge_next;
    int edge_size;
    struct span * span;
    int span_part;
    int span_next;
    int span_size;
};

int lex_create(struct lex *, int, int, int);
void lex_delete(struct lex *);

#endif
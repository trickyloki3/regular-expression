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
    int a;
    int b;
    int node;
    int next;
};

struct token {
    int head;
    int tail;
    int state;
};

struct set {
    int key_part;
    int key_next;
    int node;
};

struct line {
    int a;
    int b;
    int node;
};

struct lex {
    int root;
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
    struct token * token;
    struct token * bound;
    struct token * stack;
    int * key;
    int key_part;
    int key_next;
    int key_prev;
    int key_size;
    struct set * set;
    int set_next;
    int set_prev;
    int set_size;
    struct line * line;
    int line_next;
    int line_size;
};

int lex_create(struct lex *, int, int, int, int, int, int, int);
void lex_delete(struct lex *);
void lex_clear(struct lex *);
int lex_compile(struct lex *, char *, int);

#endif

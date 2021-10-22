static int node_push(struct lex *, int *);
static int edge_push(struct lex *, int, int);
static int span_push(struct lex *, int, int, int, int);

static int node_push(struct lex * lex, int * node) {
    int i;

    if(lex->node_next >= lex->node_size)
        return panic("out of memory");

    i = lex->node_next++;

    lex->node[i].edge = 0;
    lex->node[i].span = 0;
    lex->node[i].flag = 0;

    *node = i;

    return 0;
}

static int edge_push(struct lex * lex, int x, int y) {
    int i;

    if(lex->edge_next >= lex->edge_size)
        return panic("out of memory");

    i = lex->edge_next++;

    lex->edge[i].node = y;
    lex->edge[i].next = lex->node[x].edge;
    lex->node[x].edge = i;

    return 0;
}

static int span_push(struct lex * lex, int a, int b, int x, int y) {
    int i;

    if(lex->span_next >= lex->span_size)
        return panic("out of memory");

    i = lex->span_next++;

    lex->span[i].a = a;
    lex->span[i].b = b;
    lex->span[i].node = y;
    lex->span[i].next = lex->node[x].span;
    lex->node[x].span = i;

    return 0;
}

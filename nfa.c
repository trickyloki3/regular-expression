enum {
    NUL,
    VAR,
    EXP,
    FIN,
    X28,
    X29,
    X2A,
    X2B,
    X3F,
    X7C
};

static int node_push(struct lex *, int *);
static int edge_push(struct lex *, int, int);
static int span_push(struct lex *, int, int, int, int);

static void node_cat(struct lex *, int, int);
static void node_sub(struct lex *, int, int);

static int token_parse(struct lex *, char *);
static int token_shift(struct lex *, int, int, int);

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

static void node_cat(struct lex * lex, int x, int y) {
    int i;

    i = lex->node[y].edge;
    if(i) {
        while(lex->edge[i].next)
            i = lex->edge[i].next;

        lex->edge[i].next = lex->node[x].edge;
        lex->node[x].edge = lex->node[y].edge;
        lex->node[y].edge = 0;
    }

    i = lex->node[y].span;
    if(i) {
        while(lex->span[i].next)
            i = lex->span[i].next;

        lex->span[i].next = lex->node[x].span;
        lex->node[x].span = lex->node[y].span;
        lex->node[y].span = 0;
    }
}

static void node_sub(struct lex * lex, int x, int y) {
    int i;

    for(i = lex->edge_part; i < lex->edge_next; i++)
        if(lex->edge[i].node == x)
            lex->edge[i].node = y;

    for(i = lex->span_part; i < lex->span_next; i++)
        if(lex->span[i].node == x)
            lex->span[i].node = y;
}

static int token_parse(struct lex * lex, char * s) {
    int a;
    int b;

loop:
    switch(*s) {
        case '(':
            if(token_shift(lex, 0, 0, X28))
                return panic("invalid syntax");
            s++;
            goto loop;
        case ')':
            if(token_shift(lex, 0, 0, X29))
                return panic("invalid syntax");
            s++;
            goto loop;
        case '*':
            if(token_shift(lex, 0, 0, X2A))
                return panic("invalid syntax");
            s++;
            goto loop;
        case '+':
            if(token_shift(lex, 0, 0, X2B))
                return panic("invalid syntax");
            s++;
            goto loop;
        case '?':
            if(token_shift(lex, 0, 0, X3F))
                return panic("invalid syntax");
            s++;
            goto loop;
        case '|':
            if(token_shift(lex, 0, 0, X7C))
                return panic("invalid syntax");
            s++;
            goto loop;
        case '[':

    span:
            a = 0;
            b = 0;

    a:
            s++;
            switch(*s) {
                case '0': a = (a * 16) + 0x0; goto a;
                case '1': a = (a * 16) + 0x1; goto a;
                case '2': a = (a * 16) + 0x2; goto a;
                case '3': a = (a * 16) + 0x3; goto a;
                case '4': a = (a * 16) + 0x4; goto a;
                case '5': a = (a * 16) + 0x5; goto a;
                case '6': a = (a * 16) + 0x6; goto a;
                case '7': a = (a * 16) + 0x7; goto a;
                case '8': a = (a * 16) + 0x8; goto a;
                case '9': a = (a * 16) + 0x9; goto a;
                case 'A': a = (a * 16) + 0xA; goto a;
                case 'B': a = (a * 16) + 0xB; goto a;
                case 'C': a = (a * 16) + 0xC; goto a;
                case 'D': a = (a * 16) + 0xD; goto a;
                case 'E': a = (a * 16) + 0xE; goto a;
                case 'F': a = (a * 16) + 0xF; goto a;
            }

            if(*s == '-') {
    b:
                s++;
                switch(*s) {
                    case '0': b = (b * 16) + 0x0; goto b;
                    case '1': b = (b * 16) + 0x1; goto b;
                    case '2': b = (b * 16) + 0x2; goto b;
                    case '3': b = (b * 16) + 0x3; goto b;
                    case '4': b = (b * 16) + 0x4; goto b;
                    case '5': b = (b * 16) + 0x5; goto b;
                    case '6': b = (b * 16) + 0x6; goto b;
                    case '7': b = (b * 16) + 0x7; goto b;
                    case '8': b = (b * 16) + 0x8; goto b;
                    case '9': b = (b * 16) + 0x9; goto b;
                    case 'A': b = (b * 16) + 0xA; goto b;
                    case 'B': b = (b * 16) + 0xB; goto b;
                    case 'C': b = (b * 16) + 0xC; goto b;
                    case 'D': b = (b * 16) + 0xD; goto b;
                    case 'E': b = (b * 16) + 0xE; goto b;
                    case 'F': b = (b * 16) + 0xF; goto b;
                }
            } else {
                b = a;
            }

            if(*s == ',')
                goto span;

            if(*s != ']')
                return panic("invalid character");

            if(token_shift(lex, 0, 0, VAR))
                return panic("invalid syntax");

            s++;
            goto loop;
    }

    if(*s != 0)
        return panic("invalid character");

    if(token_shift(lex, 0, 0, NUL))
        return panic("invalid syntax");

    return 0;
}

static int token_shift(struct lex * lex, int head, int tail, int token) {
    return 0;
}

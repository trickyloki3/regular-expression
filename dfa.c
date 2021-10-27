static int set_push(struct lex *, int);
static int set_join(struct lex *, int);

static int set_push(struct lex * lex, int node) {
    int l;
    int r;
    int m;

    l = lex->key_part;
    r = lex->key_next - 1;
    while(l <= r) {
        m = (l + r) / 2;
        if(lex->key[m] < node) {
            l = m + 1;
        } else if(lex->key[m] > node) {
            r = m - 1;
        } else {
            return 0;
        }
    }

    if(lex->key_prev - lex->key_next < 2)
        return panic("out of memory");

    memmove(lex->key + l + 1, lex->key + l, (lex->key_next - l) * sizeof(*lex->key));

    lex->key[l] = node;

    lex->key_next++;

    lex->key[--lex->key_prev] = lex->key[l];

    return 0;
}

static int set_join(struct lex * lex, int node) {
    int n;
    int e;

    if(set_push(lex, node))
        return panic("failed to push set");

    while(lex->key_prev < lex->key_size) {
        n = lex->key[lex->key_prev++];
        e = lex->node[n].edge;
        while(e) {
            if(set_push(lex, lex->edge[e].node))
                return panic("failed to push set");
            e = lex->edge[e].next;
        }
    }

    return 0;
}

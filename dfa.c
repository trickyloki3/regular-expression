static int set_push(struct lex *, int);
static int set_join(struct lex *, int);
static int set_compare(int *, int, int, int, int);
static int set_closure(struct lex *, int *);

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

static int set_compare(int * k, int i, int j, int x, int y) {
    while(i < j && x < y) {
        if(k[i] < k[x])
            return -1;

        if(k[i] > k[x])
            return 1;

        i++;
        x++;
    }

    if(i < j)
        return 1;

    if(x < y)
        return -1;

    return 0;
}

static int set_closure(struct lex * lex, int * node) {
    int l;
    int r;
    int m;
    int c;

    l = 0;
    r = lex->set_next - 1;
    while(l <= r) {
        m = (l + r) / 2;
        c = set_compare(lex->key, lex->set[m].key_part, lex->set[m].key_next, lex->key_part, lex->key_next);
        if(c < 0) {
            l = m + 1;
        } else if(c > 0) {
            r = m - 1;
        } else {
            /* remove the key */
            lex->key_next = lex->key_part;
            *node = lex->set[m].node;

            return 0;
        }
    }

    if(lex->set_prev - lex->set_next < 2)
        return panic("out of memory");

    memmove(lex->set + l + 1, lex->set + l, (lex->set_next - l) * sizeof(*lex->set));

    lex->set[l].key_part = lex->key_part;
    lex->set[l].key_next = lex->key_next;
    if(node_push(lex, &lex->set[l].node))
        return panic("failed to push node");

    lex->set_next++;

    lex->set[--lex->set_prev] = lex->set[l];

    /* commit the key */
    lex->key_part = lex->key_next;
    *node = lex->set[l].node;

    return 0;
}

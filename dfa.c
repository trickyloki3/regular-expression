static int set_push(struct lex *, int);
static int set_join(struct lex *, int);
static int set_compare(int *, int, int, int, int);
static int set_closure(struct lex *, int *);

static void line_sort(struct line *, int);

static int line_push(struct lex *, int, int, int);
static int line_join(struct lex *, int, int);

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

static void line_sort(struct line * l, int n) {
    int r;
    int p;
    int c;

    struct line line;

    for(r = n / 2 - 1; r >= 0; r--) {
        p = r;
        c = p * 2 + 1;
        while(c < n) {
            if(c + 1 < n && l[c].a < l[c + 1].a)
                c++;

            if(l[p].a > l[c].a)
                break;

            line = l[c];
            l[c] = l[p];
            l[p] = line;

            p = c;
            c = p * 2 + 1;
        }
    }

    for(r = n - 1; r > 0; r--) {
        line = l[0];
        l[0] = l[r];
        l[r] = line;

        p = 0;
        c = p * 2 + 1;
        while(c < r) {
            if(c + 1 < r && l[c].a < l[c + 1].a)
                c++;

            if(l[p].a > l[c].a)
                break;

            line = l[c];
            l[c] = l[p];
            l[p] = line;

            p = c;
            c = p * 2 + 1;
        }
    }
}

static int line_push(struct lex * lex, int a, int b, int node) {
    int i;

    if(lex->line_next >= lex->line_size)
        return panic("out of memory");

    i = lex->line_next++;

    lex->line[i].a = a;
    lex->line[i].b = b;
    lex->line[i].node = node;

    return 0;
}

static int line_join(struct lex * lex, int key_part, int key_next) {
    int n;
    int s;

    lex->line_next = 0;

    while(key_part < key_next) {
        n = lex->key[key_part++];
        s = lex->node[n].span;
        while(s) {
            if(line_push(lex, lex->span[s].a, lex->span[s].b, lex->span[s].node))
                return panic("failed to push line");
            s = lex->span[s].next;
        }
    }

    return 0;
}

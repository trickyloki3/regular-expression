**Regular Expression Syntax**
```
E : e

e : v
  : e v     (concatenation)
  : e *     (zero or more)
  : e +     (one or more)
  : e ?     (zero or one)
  : e | e   (alternation)
  : ( e )
```

shift is index by **state (0 - 13)** and **token (0 - 9)** to get the next state

a **zero state** means token cannot be push to stack

```
static int shift[14][10] = {
        0   v   e   f   (   )   *   +   ?   |
 [ 0] { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
 [ 1] { 0 , 2 , 12, 13, 3 , 0 , 0 , 0 , 0 , 0  },
 [ 2] { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
 [ 3] { 0 , 2 , 4 , 0 , 3 , 0 , 0 , 0 , 0 , 0  },
 [ 4] { 0 , 6 , 0 , 0 , 0 , 5 , 7 , 8 , 9 , 10 },
 [ 5] { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
 [ 6] { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
 [ 7] { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
 [ 8] { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
 [ 9] { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
 [10] { 0 , 2 , 11, 0 , 3 , 0 , 0 , 0 , 0 , 0  },
 [11] { 0 , 6 , 0 , 0 , 0 , 0 , 7 , 8 , 9 , 0  },
 [12] { 0 , 6 , 0 , 0 , 0 , 0 , 7 , 8 , 9 , 10 },
 [13] { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  }
};
```

reduce is index by **state (0 - 13)** to get the rule

a **zero rule** means token(s) cannot be pop from stack

```
static int reduce[14] = {
 [ 0]   0,
 [ 1]   0,
 [ 2]   2, e : v
 [ 3]   0,
 [ 4]   0,
 [ 5]   3, e : ( e )
 [ 6]   4, e : e v
 [ 7]   5, e : e *
 [ 8]   6, e : e +
 [ 9]   7, e : e ?
 [10]   0,
 [11]   8, e : e | e
 [12]   1, E : e
 [13]   0
};
```

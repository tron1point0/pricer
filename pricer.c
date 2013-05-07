#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int key;
    int value;
    void *next;
    void *list;
} node_t;

typedef struct {
    void * head;
    int (*cmp)(int,int);
} list_t;

int * Map(int size) {
    int *map = (int *) malloc(sizeof(int) * size);
    return map;
}

inline int get(int *map, unsigned long key) {
    return map[key];
}

inline int set(int *map, unsigned long key, int val) {
    map[key] = val;
    return map[key];
}

inline node_t * Node (int key, int val) {
    node_t *node = (node_t *) malloc(sizeof(node_t));
    node->key = key;
    node->value = val;
    return node;
}

list_t * List (int (*cmp)(int,int)) {
    list_t *list = (list_t *) malloc(sizeof(list_t));
    list->cmp = cmp;
    list->head = NULL;
    return list;
}

void free_list (list_t *list) {
    node_t *elem = list->head;
    while (elem != NULL) {
        node_t *next = (node_t *) elem->next;
        free(elem);
        elem = next;
    }
    free(list);
}

inline int insert (list_t *list, node_t *prev, node_t *node) {
    node->list = (list_t *) list;
    if (prev == NULL) {
        node->next = (node_t *) list->head;
        list->head = (node_t *) node;
    } else {
        node->next = prev->next;
        prev->next = node;
    }
    return 1;
}

inline void delete (node_t *prev, node_t *node) {
    list_t *list = node->list;
    if (prev == NULL) {
        list->head = node->next;
    } else {
        prev->next = node->next;
    }
    free(node);
}

inline int find (list_t *list, int key, node_t **prev, node_t **node) {
    int (*cmp)(int,int) = list->cmp;
    if (list->head == NULL) {
        *node = NULL;
        *prev = NULL;
        return 0;
    }
    *node = list->head;
    int c = cmp(key,(*node)->key);
    if (c < 0) {
        *prev = NULL;
        *node = NULL;
        return 0;
    } else if (c == 0) {
        *prev = NULL;
        *node = list->head;
        return 1;
    } else {
        *prev = list->head;
        *node = (*prev)->next;
        while (*node != NULL) {
            c = cmp(key,(*node)->key);
            if (c < 0) {
                *node = NULL;
                return 0;
            } else if (c == 0) {
                return 1;
            } else {
                *prev = *node;
                *node = (*node)->next;
            }
        }
        return 0;
    }
}

inline int least (int a, int b) {
    if (a < b)
        return -1;
    if (a == b)
        return 0;
    if (a > b)
        return 1;
}

inline int most (int a, int b) {
    return -1 * least(a,b);
}

inline void add_order (list_t *list, int price, int size) {
    node_t *node;
    node_t *prev;
    if (find(list,price,&prev,&node)) {
        node->value += size;
    } else {
        insert(list,prev,Node(price,size));
    }
}

inline void remove_order (list_t *list, int price, int size) {
    node_t *node;
    node_t *prev;
    if (find(list,price,&prev,&node)) {
        node->value -= size;
        if (node->value <= 0) {
            delete(prev,node);
        }
    }
}

inline unsigned int trade (list_t *list, int target) {
    int remain = target;
    unsigned int total = 0;
    node_t *node = list->head;
    while (node != NULL && remain > 0) {
        if (node->value < remain) {
            total += node->key * node->value;
        } else {
            total += node->key * remain;
        }
        remain -= node->value;
        node = node->next;
    }
    if (remain > 0 || total <= 0) {
        return 0;
    } else {
        return total;
    }
}

inline void print_int (int value) {
    char *buff = (char *) malloc(sizeof(char) * 16);
    int len = 0;
    *buff = '0';
    while (value > 0 && len < 16) {
        len++;
        *(buff++) = '0' + value % 10;
        value /= 10;
    }
    while (len > 0) {
        putchar_unlocked(*--buff);
        len--;
    }
    free(buff);
}

inline void print_char (char c) {
    putchar_unlocked(c);
}

inline void print_string (char* c) {
    while (*c) {
        putchar_unlocked(*c);
        c++;
    }
}

inline void print_total(char* time, char side, unsigned int total) {
    print_string(time);
    print_char(' ');
    print_char(side);
    print_char(' ');
    if (total == 0) {
        print_char('N');
        print_char('A');
    } else {
        unsigned int dollar = total / 100;
        unsigned int cent = total % 100;
        print_int(dollar);
        print_char('.');
        if (cent < 10) print_char('0');
        if (cent == 0) print_char('0');
        print_int(cent);
    }
    print_char('\n');
}

#define next_char(c1) c1 = getchar_unlocked(); if (c1 == EOF) return EOF
#define with_next(match,buff) if (read_until(match,buff) != EOF)

inline int read_until (char match, char *buff) {
    char c;
    next_char(c);
    while(c != match) {
        if (c == EOF) return EOF;
        *buff++ = c;
        next_char(c);
    }
    *buff = '\0';
    return 1;
}

inline int read_int () {
    char c;
    int i = 0;
    next_char(c);
    while (c >= '0' && c <= '9') {
        c -= '0';
        i *= 10;
        i += c;
        next_char(c);
    }
    return i;
}

inline unsigned long pow (int base,unsigned char exp) {
    if (exp == 0) return 1;
    unsigned long r = base;
    while (exp--) r *= base;
    return r;
}

inline unsigned long read_orderId () {
    char c;
    unsigned long i = 0;
    unsigned char digits = 0;
    next_char(c);
    while (c >= 'a' && c <= 'z') {
        i += (c - 'a') * pow(26,digits++);
        next_char(c);
    }
    return i;
}

inline char read_char () {
    char c;
    next_char(c);
    return c;
}

inline int read_token(char *token) {
    with_next(' ',token) {} else { return EOF; }
    return 1;
}

inline int read_prefix (char *time, char *type, unsigned long *orderId) {
    char c;
    if (read_token(time) == EOF) {
        return EOF;
    }
    *type = read_char();
    next_char(c);
    *orderId = read_orderId();
    return 1;
}

inline int read_add (char *side, int *dollar, int *cent, int *size) {
    char s;
    *side = read_char();
    next_char(s);
    *dollar = read_int();
    *cent = read_int();
    *size = read_int();
    return 1;
}

inline int read_rem (int *size) {
    *size = read_int();
    return 1;
}

int main(int argc, char *argv[]) {
    setvbuf(stdin, (char*)NULL, _IOFBF, 32 * 1024 * 1024);
    setvbuf(stdout, (char*)NULL, _IOFBF, 32 * 1024 * 1024);
    list_t *asks = List(&least);
    list_t *bids = List(&most);
    /*
     * Yeah, that's about 11 MB *each*, but it makes random access to an
     * order's data an O(1) operation. To support another character, I'd either
     * need to use ~600 MB each for this table, or a tree.
     */
    int *price_of = Map(16 * 1024 * 1024);
    int *side_of = Map(16 * 1024 * 1024);

    int target = 200;
    if (argc >= 2) {
        target = atoi(argv[1]);
    }

    char *time = malloc(sizeof(char) * 32);
    char type = 'A';
    unsigned long orderId = 0;
    char side = 'B';
    int dollar = 0;
    int cent = 0;
    int price = 0;
    int size = 0;

    unsigned int ask_total = 0;
    unsigned int bid_total = 0;

    while (read_prefix(time,&type,&orderId) != EOF) {
        if (type == 'A') {
            if (read_add(&side,&dollar,&cent,&size)) {
                price = dollar * 100 + cent;
                if (side == 'B') {
                    add_order(bids,price,size);
                    set(price_of,orderId,price);
                    set(side_of,orderId,0);
                    unsigned int total = trade(bids,target);
                    if (bid_total != total) {
                        print_total(time,'S',total);
                        bid_total = total;
                    }
                } else {
                    add_order(asks,price,size);
                    set(price_of,orderId,price);
                    set(side_of,orderId,1);
                    unsigned int total = trade(asks,target);
                    if (ask_total != total) {
                        print_total(time,'B',total);
                        ask_total = total;
                    }
                }
            }
        } else {
            if (read_rem(&size)) {
                int _side = get(side_of,orderId);
                price = get(price_of,orderId);
                if (_side == 0) {
                    remove_order(bids,price,size);
                    unsigned int total = trade(bids,target);
                    if (bid_total != total) {
                        print_total(time,'S',total);
                        bid_total = total;
                    }
                } else {
                    remove_order(asks,price,size);
                    unsigned int total = trade(asks,target);
                    if (ask_total != total) {
                        print_total(time,'B',total);
                        ask_total = total;
                    }
                }
            }
        }
    }

    free(price_of);
    free(side_of);
    free_list(asks);
    free_list(bids);

    return 0;
}

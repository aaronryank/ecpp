#ifndef _ECPP_TOKEN_H
#define _ECPP_TOKEN_H

typedef struct node {
  int type;
  int line;
  char *val;
  struct node *prev, *next;
} token_t;

typedef struct token {
  int type;
  int line;
  char *val;
} token_struct;

extern void print_list(token_t *);
extern void printline(int, token_t *);
extern void push(token_t **, char *, int, int);
extern void add_at_current(token_t **, char *, int, int);
extern void rewind_token(token_t **);
extern token_struct *pop(token_t **);
extern token_struct *remove_last(token_t *);
extern token_struct *remove_by_index(token_t **, int);

#endif /* ecpp's token.h */
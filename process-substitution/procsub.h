#ifndef _PROCSUB_H
#define _PROCSUB_H

typedef struct argument argument;

typedef struct command command;

struct command
{
  char *prog;             // pathname to be exec'ed
  int numargs;            // number of arguments for prog
  struct argument *args;  // arguments for prog; array, size is numargs
};

typedef enum argtype { STRING, SUBST } argtype;

struct argument
{
  enum argtype type;
  union {
    char *str;             // if type == STRING
    struct command cmd;    // if type == SUBST
  } data;
};

int run(struct command *cmd, int *wstatus);

#endif

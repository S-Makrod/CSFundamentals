#include <stdio.h>

#include "procsub.h"

// uniq -c <(sort -r violinists.txt)
command *example_1(void) {
  static argument sort_args[2] = {
    { .type = STRING, .data = {.str = "-r"} },
    { .type = STRING, .data = {.str = "violinists.txt"} }
  };
  static argument uniq_args[2] = {
    { .type = STRING, .data = {.str = "-c"} },
    { .type = SUBST,
      .data = { .cmd = {
          .prog = "sort",
          .numargs = 2,
          .args = sort_args
        }}
    }
  };
  static command c = {
    .prog = "uniq",
    .numargs = 2,
    .args = uniq_args
  };
  return &c;
}

// diff <(cat violinists.txt) <(head -n -1 violinists.txt)
command *example_2(void) {
  static argument cat_args[1] = {
    { .type = STRING, .data = {.str = "violinists.txt"} }
  };
  static argument head_args[3] = {
    { .type = STRING, .data = {.str = "-n"} },
    { .type = STRING, .data = {.str = "-1"} },
    { .type = STRING, .data = {.str = "violinists.txt"} }
  };
  static argument diff_args[2] = {
    { .type = SUBST,
      .data = { .cmd = {
          .prog = "cat",
          .numargs = 1,
          .args = cat_args
        }}
    },
    { .type = SUBST,
      .data = { .cmd = {
          .prog = "head",
          .numargs = 3,
          .args = head_args
        }}
    }
  };
  static command c = {
    .prog = "diff",
    .numargs = 2,
    .args = diff_args
  };
  return &c;
}

// uniq -c <(sort -r <(cat violinists.txt))
command *example_3(void) {
  static argument cat_args[1] = {
    { .type = STRING, .data = {.str = "violinists.txt"} }
  };
  static argument sort_args[2] = {
    { .type = STRING, .data = {.str = "-r"} },
    { .type = SUBST, .data = { .cmd = {
          .prog = "cat",
          .numargs = 1,
          .args = cat_args
        }}}
  };
  static argument uniq_args[2] = {
    { .type = STRING, .data = {.str = "-c"} },
    { .type = SUBST, .data = { .cmd = {
          .prog = "sort",
          .numargs = 2,
          .args = sort_args
        }}}
  };
  static command c = {
    .prog = "uniq",
    .numargs = 2,
    .args = uniq_args
  };
  return &c;
}

// For this to work, you need to compile: gcc funny.c -o funny
// ./funny <(./funny)
command *example_4(void) {
  static argument outer_args[1] = {
    { .type = SUBST,
      .data = { .cmd = {
          .prog = "./funny",
          .numargs = 0,
          .args = NULL
        }}
    }
  };
  static command c = {
    .prog = "./funny",
    .numargs = 1,
    .args = outer_args
  };
  return &c;
}


int main(void)
{
  int ws;

  run(example_1(), &ws);
  run(example_2(), &ws);
  run(example_3(), &ws);

  // The purpose of example 4 is to test that you are getting the wait status of
  // the correct child.  It should be 0.
  run(example_4(), &ws);
  printf("example 4 wait status: %d\n", ws);
  if (ws != 0) {
    printf("You've got the wrong child.\n");
  }

  return 0;
}

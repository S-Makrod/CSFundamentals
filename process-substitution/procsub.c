// Add #include's you need
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "procsub.h"

// for debugging purposes (prints the argument list)
void print_args(char **args, int n) {
    for(int i = 0; i < n; i++) {
        printf("%s, ", *(args+i));
    }
    printf("\n");
}

// free the argument list
void free_args(char **args, int n) {
    char **temp = NULL;
    char **curr = args;

    for(int i = 0; i < n; i++) {
        temp = curr + 1;
        free(*curr);
        curr = temp;
    }

    free(args);
}

// allocate memory for arg to be added to argument list (string case)
void allocate_arg(char **args, char *arg, int n) {
    char *argument = calloc(strlen(arg)+1, sizeof(char));
    strcpy(argument, arg);
    *(args+n) = argument;
}

// allocate memory for the arg to be added to the argument list (sub-process case)
void allocate_command_arg(char **args, int fd, int n) {
    int len = snprintf(NULL, 0, "/dev/fd/%d", fd);
    char *argument = calloc(len+1, sizeof(char));
    sprintf(argument, "/dev/fd/%d", fd);
    *(args+n) = argument;
}

// execute a single command (called by run when parent command has sub-process argument)
int execute_cmd(command cmd) {
    // allocate memory for argument list (needed for execvp)
    char** args = calloc(cmd.numargs+2, sizeof(char *));
    int args_index = 0;
    int pipefd[2];
    pid_t p;

    // init the argument list
    allocate_arg(args, cmd.prog, args_index);
    args[cmd.numargs+1] = NULL;
    args_index++;

    // for each arg in the commands arg list
    for(int i = 0; i < cmd.numargs; i++) {
        // if string case then just add it to the arg list
        if(cmd.args[i].type == STRING) {
            allocate_arg(args, cmd.args[i].data.str, args_index);
            args_index++;
        // if sub-process have more work to do
        } else {
            // make pipe
            pipe(pipefd);

            // fork error
            if((p = fork()) == -1) {
                perror("execute_cmd fork error ");
                return -1;
            // parent process
            } else if(p != 0) {
                // parent does not need to write
                close(pipefd[1]);
                // wait for child to finish
                waitpid(p, NULL, 0);
                // add the read file descriptor to the argument list
                allocate_command_arg(args, pipefd[0], args_index);
                args_index++;
            //child process
            } else {
                // child does not need to read
                close(pipefd[0]);
                // send stdout to pipe and close the write end of pipe
                dup2(pipefd[1], 1);
                close(pipefd[1]);

                // resolve the command (let recursion handle this)
                int res = execute_cmd(cmd.args[i].data.cmd);
                if(res == 127) return 127;
                if(res == -1) return -1;
            }
        }
    }

    // at this point the command argument list has been built so execute the command
    if(execvp(cmd.prog, args) == -1) {
        perror("execute_cmd exec error ");
        return 127;
    }

    return 0; // should not be reached
}

// execute command
int run(struct command *cmd, int *wstatus) {
    pid_t p, p2;
    // allocate memory for argument list (needed for execvp)
    char** args = calloc(cmd->numargs+2, sizeof(char *));
    int args_index = 0;
    int pipefd[2];

    // init arg list
    allocate_arg(args, cmd->prog, args_index);
    args[cmd->numargs+1] = NULL;
    args_index++;

    // error with fork
    if((p = fork()) == -1) {
        free_args(args, args_index);
        perror("run fork error ");
        return -1;
    // parent process
    } else if(p != 0) {
        // free arg list and wait for child to finish
        free_args(args, args_index);
        waitpid(p, wstatus, 0);

        // if child exited
        if(WIFEXITED(*wstatus)) {
            // if the child had a exit status of 127 then run will return the same
            if(WEXITSTATUS(*wstatus) == 127) {
                return 127;
            }

            // if the child returned -1 there was an error, run returns -1
            if(WEXITSTATUS(*wstatus) == -1) {
                return -1;
            }
        }
    //child process
    } else {
        // for each arg in the commands arg list
        for(int i = 0; i < cmd->numargs; i++) {
            // if string case then just add it to the arg list
            if(cmd->args[i].type == STRING) {
                allocate_arg(args, cmd->args[i].data.str, args_index);
                args_index++;
            // if sub-process have more work to do
            } else {
                // make pipe
                pipe(pipefd);

                // fork error
                if((p2 = fork()) == -1) {
                    perror("run for error (p2) ");
                    return -1;
                //parent process
                } else if(p2 != 0) {
                    // parent does not need to write
                    close(pipefd[1]);
                    // wait for child to finish
                    waitpid(p2, NULL, 0);
                    // add the read file descriptor to the argument list
                    allocate_command_arg(args, pipefd[0], args_index);
                    args_index++;
                //child process
                } else {
                    // child does not need to read
                    close(pipefd[0]);
                    // send stdout to pipe and close the write end of pipe
                    dup2(pipefd[1], 1);
                    close(pipefd[1]);

                    // resolve the command with execute_cmd function
                    int res = execute_cmd(cmd->args[i].data.cmd);
                    if(res == 127) return 127;
                    if(res == -1) return -1;
                }
            }
        }

        // at this point the command argument list has been built so execute the command
        if(execvp(cmd->prog, args) == -1) {
            perror("run exec error ");
            return 127;
        }
    }

    return 0;
}

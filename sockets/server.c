#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#define INPUT_LENGTH 28
#define RESPONSE_LENGTH 11

typedef struct record {
    unsigned suncents;       // 4 bytes on x86-64
    unsigned char name_len;  // length of name
    char name[27];           // name; NOT nul-terminated! use name_len above
} record;

int get_suncents(FILE *f, const char *name, unsigned *psuncents) {
    // init values
    int res;
    record temp_record;
    record *r = &temp_record;
    res = fseek(f, 0, SEEK_SET);

    if(res == -1) {
        fseek(f, 0, SEEK_END);
        fprintf(stderr, "Error moving to beginning of file\n");
        return 0;
    }

    // read file
    while(!feof(f)) {
        res = fread(r, sizeof(record), 1, f);

        if(ferror(f)) {
            fseek(f, 0, SEEK_END);
            fprintf(stderr, "Error reading file\n");
            return 0;
        }

        // if record was read
        if(res == 1) {
            // if name not equal length skip
            if(strlen(name) != r->name_len) continue;

            // if name match return
            if(strncmp(r->name, name, r->name_len) == 0) {
                *psuncents = r->suncents;
                return 1;
            }
        }
    }

    // no match, setfile pointer to eof and return
    res = fseek(f, 0, SEEK_END);

    if(res == -1) {
        fprintf(stderr, "Error when setting cursor to end of file\n");
    }

    return 0;
}

// ignore sigpipe
void ignore_sigpipe(void) {
    struct sigaction ignore_sigpipe_please;
    ignore_sigpipe_please.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &ignore_sigpipe_please, NULL);
}

// do not allow zombie child processes
void no_zombie(void) {
    struct sigaction no_zombie_please;
    no_zombie_please.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &no_zombie_please, NULL);
}

int main(int argc, char **argv) {
    // verify correct program call
    if (argc < 3) {
        fprintf(stderr, "Need file pathname and port number.\n");
        return 1;
    }

    // init values
    int sfd;
    struct sockaddr_in a;
    pid_t p;
    FILE *f;

    // verify file is correct
    if((f = fopen(argv[1], "rb")) == NULL) {
        perror("file error");
        return 1;
    }

    // ignore SIGPIPE
    ignore_sigpipe();
    // No zombies allowed
    no_zombie();

    // create socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    // init sockaddr_in a
    memset(&a, 0, sizeof(struct sockaddr_in));
    a.sin_family = AF_INET;
    a.sin_port = htons(atoi(argv[2]));
    a.sin_addr.s_addr = htonl(INADDR_ANY);
    if (-1 == bind(sfd, (struct sockaddr *)&a, sizeof(struct sockaddr_in))) {
        perror("bind");
        return 1;
    }

    // begin listen
    if (-1 == listen(sfd, 2)) {
        perror("listen");
        return 1;
    }

    // until termination by SIGINT or SIGTERM
    while(1) {
        int cfd;
        struct sockaddr_in ca;
        socklen_t sinlen = sizeof(struct sockaddr_in);

        // wait for client
        cfd = accept(sfd, (struct sockaddr *)&ca, &sinlen);

        // fork error
        if((p = fork()) == -1) {
            perror("fork error");
            return 1;
        // parent process close and continue to handle multiple client
        } else if(p != 0) {
            close(cfd);
        //child process
        } else {
            // do not need server fd
            close(sfd);
            char input[INPUT_LENGTH];
            char name[INPUT_LENGTH];
            char res[RESPONSE_LENGTH];
            int len, found, suncents, ret;
            ssize_t read_num = 2;

            // until client terminates
            while(1) {
                len = 0;
                found = 0;

                // read client request
                // determine length of string (newline terminated)
                for(int i = 0; i < INPUT_LENGTH; i++) {
                    len += 1;
                    read_num = read(cfd, &input[i], 1);

                    if(input[i] == '\n') {
                        found = 1;
                        break;
                    }
                    if(read_num < 1) {
                        if(read_num == -1) fprintf(stderr, "read error\n");
                        break;
                    }
                }

                if(read_num < 1) break;

                // if terminated properly
                if(found) {
                    // init name to be properly null terminated
                    strncpy(name, input, len-1);
                    name[len-1] = '\0';

                    //get suncents
                    ret = get_suncents(f, name, &suncents);

                    // write value or none to child
                    if(ret) {
                        len = snprintf(res, RESPONSE_LENGTH, "%d\n", suncents);
                        write(cfd, res, len);
                    } else {
                        write(cfd, "none\n", 5);
                    }
                // child error
                } else {
                    break;
                }
            }

            // close fd and exit
            close(cfd);
            exit(0);
        }
    }

    return 0;
}
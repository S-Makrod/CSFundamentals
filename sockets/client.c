#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define INPUT_LENGTH 28
#define RESPONSE_LENGTH 11

int main(int argc, char **argv) {
    // Verify program called correctly
    if (argc < 3) {
        fprintf(stderr, "Need server IPv4 address and port number.\n");
        return 1;
    }

    // init values
    int cfd, len, inplen, found;
    char *ret1;
    ssize_t ret2 = 2;
    char input[INPUT_LENGTH], res[RESPONSE_LENGTH];
    struct sockaddr_in a;

    // init sockaddr_in a
    memset(&a, 0, sizeof(struct sockaddr_in));
    a.sin_family = AF_INET;
    a.sin_port = htons(atoi(argv[2]));
    if (0 == inet_pton(AF_INET, argv[1], &a.sin_addr)) {
        fprintf(stderr, "That's not an IPv4 address.\n");
        return 1;
    }

    // get socket
    cfd = socket(AF_INET, SOCK_STREAM, 0);

    // conect to server
    if (-1 == connect(cfd, (struct sockaddr *)&a, sizeof(struct sockaddr_in))) {
        perror("connect");
        return 1;
    }

    printf("Ready\n");

    // until termination
    while(1) {
        len = 0;
        found = 0;

        // read from stdin
        ret1 = fgets(input, INPUT_LENGTH, stdin);
        inplen = strlen(input);

        if(inplen == 27 && input[INPUT_LENGTH-2] != '\n') {
            input[27] = '\n';
            inplen = 28;
            getchar();
        }

        // if user enters hard return consider it as signal to end program
        if(strcmp(input, "\n") == 0|| ret1 == NULL) {
            break;
        } else {
            // write to server
            ret2 = write(cfd, input, inplen);
            if(ret2 == -1) {
                fprintf(stderr, "write error\n");
                break;
            }

            // read response and determine length of string (newline terminated)
            for(int i = 0; i < RESPONSE_LENGTH; i++) {
                len += 1;
                ret2 = read(cfd, &res[i], 1);

                if(res[i] == '\n') {
                    found = 1;
                    break;
                }
                if(ret2 < 1) {
                    fprintf(stderr, "read error\n");
                    break;
                }
            }

            if(ret2 < 1 ) break;

            // print results (if no newline server error)
            if(found) {
                printf("%.*s", len, res);
            } else {
                fprintf(stderr, "server error\n");
                ret2 = -1;
                break;
            }
        }
    }

    // print end to stderr (so it does not mess with automarker) and close cfd
    fprintf(stderr, "bye!\n");
    close(cfd);

    // if error with server meaning ret2 is either -1 or 0 then return 1
    // otherwise return 0
    return (ret2 == -1 || ret2 == 0) ? 1 : 0;
}
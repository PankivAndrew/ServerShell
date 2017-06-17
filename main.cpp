#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <zconf.h>
#include <wait.h>

int main() {
    struct sockaddr_in server;
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        sd = errno;
        printf("Error: %s\n", strerror(sd));
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(20003);
    int res = bind(sd, (const sockaddr *) &server, sizeof(server));
    if (res == -1) {
        res = errno;
        printf("Error: %s\n", strerror(res));
    }
    printf("ddd\n");
    listen(sd, 10);
    printf("aaa\n");
    int psd = accept(sd, NULL, NULL);
    if(psd == 0){
        printf("Accept new client!\n");
    }
    printf("ссс\n");
    if(psd == -1){
        psd = errno;
        printf("Error: %s\n",strerror(psd));
    }
    printf("I TRY TO FORK\n");
    pid_t PID;
    switch (PID = fork()){
        case -1:
        {
            printf("Error creating child process");
            exit(1);
        }
        case 0:
        {
            int input_fd = psd;
            int output_fd = dup(psd);
            dup2(input_fd, 0);
            dup2(output_fd, 1);
            execv("/home/paniv/CLionProjects/shell/cmake-build-debug/at_shell",NULL);
        }
        default:
        {
            printf("Process created! Shell opened\n");
            waitpid(PID,NULL,0);
        }


            return 0;
    }

}


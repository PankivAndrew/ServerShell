#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <zconf.h>
#include <wait.h>

int main() {
    struct sockaddr_in server;
    int mypipe1[2],mypipe2[2];
    char buf[1024];
    pipe(mypipe1);
    pipe(mypipe2);
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        sd = errno;
        printf("Error: %s\n", strerror(sd));
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(20010);
    int res = bind(sd, (const sockaddr *) &server, sizeof(server));
    if (res == -1) {
        res = errno;
        printf("Error: %s\n", strerror(res));
    }
    for(;;) {
        printf("ddd\n");
        listen(sd, 10);
        printf("aaa\n");
        int psd = accept(sd, NULL, NULL);
        if (psd == 0) {
            printf("Accept new client!\n");
        }
        printf("ссс\n");
        if (psd == -1) {
            psd = errno;
            printf("Error: %s\n", strerror(psd));
        }

        printf("I TRY TO FORK\n");
        pid_t PID;
        switch (PID = fork()) {
            case -1: {
                printf("Error creating child process");
                exit(1);
            }
            case 0: {
                close(mypipe1[1]);
                close(mypipe2[0]);
                dup2(mypipe1[0], 0);
                dup2(mypipe2[1], 1);
                execv("/home/paniv/CLionProjects/shell/cmake-build-debug/at_shell", NULL);
            }
            default: {
                close(mypipe1[0]);
                close(mypipe2[1]);
                printf("Process created! Shell opened\n");
            }

        }
        for(;;) {
            int rcv = recv(psd,buf,sizeof(buf),0);
            if(rcv == -1){
                rcv = errno;
                printf("Error: %s\n",strerror(rcv));
            }
            write(mypipe1[1],buf,rcv - 1);
            sleep(1);
            while (read(mypipe2[0],buf,sizeof(buf)) != 0)
            {
                int snd = send(psd,buf,sizeof(buf),0);
                if(snd == -1){
                    snd = errno;
                    printf("Error: %s\n",strerror(snd));
                }
            }
        }
    }
    return 0;
}

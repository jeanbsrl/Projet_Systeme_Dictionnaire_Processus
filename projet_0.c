#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){

    int N;
    if(argc != 2 || ( argc == 2 && atoi(argv[1]) < 2 )){
        fprintf(stderr, "erreur usage : ./projet_i int (int >= 2)\n");
        exit(-1);
    }else {
        N = atoi(argv[1]);
    }

    printf("Pere pid = %d\n\n", getpid());
    for(int i = 0; i < N; i++){
        switch(fork()){
        case -1:
            fprintf(stderr, "fork");
            exit(-1);
        case 0:
            printf("Fils pid = %d, son pere a le pid %d\n", getpid(), getppid());
            sleep(1);
            exit(0);
        default:
            break;
        }
    }

    char pid_string[6];
    switch (fork()){
    case -1:
        fprintf(stderr, "fork");
        exit(-1);
    case 0:
        sprintf(pid_string, "%d", getppid());
        printf("\n\n---AFFICHAGE FILS PGREP---\n\n");
        execlp("pgrep", "pgrep", "-P", pid_string, (char*) 0);
    default:
        wait(NULL);
    }

    while(wait(NULL) != -1);

    return 0;
}
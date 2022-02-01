/**
 * authors:
 * FRANCES Tom
 * BOU SERHAL Jean
 * 
 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "request.h"
#include "table.h"
#include "functions.h"

#ifndef DEBUG
#define DEBUG 0
#endif

int main(int argc, char *argv[]){

    if(DEBUG){
        printf("\nDEBUG MODE\n\n");
    }else{
        printf("\n\n");
    }
    
    //Gestion des arguments de la commande
    int N;
    if (argc != 2 || (argc == 2 && atoi(argv[1]) < 2)){
        fprintf(stderr, "erreur usage : ./projet_i int (int >= 2)\n");
        exit(-1);
    }else{
        N = atoi(argv[1]);
    }

    //Redirection du ctrl+C sur SIG_IGNORE
    signal(SIGINT, SIG_IGN);

    //Création des pipes
    int** pipes = (int **)malloc(sizeof(int[2]) * (N+1));
    for (int i = 0; i <= N; i++){
        pipes[i] = malloc(sizeof(int) * 2);
        if(pipe(pipes[i]) == -1){
            if(DEBUG) fprintf(stderr, "erreur creation pipe\n");
            exit(-1);
        }else{
            if(DEBUG) printf("Creation pipe %d ok\n", i);
        }
    }

    //Création et initialisation de la requète
    Request* request = malloc(sizeof(Request));
    init_request(request);

    if(DEBUG) printf("\nController pid = %d\n\n", getpid());

    //Création des nodes
    for (int i = 0; i < N; i++){
        switch (fork()){
        case -1:
            fprintf(stderr, "fork");
            exit(-1);
        case 0:
            //printf("Child PID = %d, father's PID = %d\n", getpid(), getppid());
            close_unused_pipes(pipes, i, N);    //fermeture des pipes inutilisés par le node
            if(i == 0){
                node(request, pipes[N - 1], pipes[i], pipes[N], N, i);
            }else{
                node(request, pipes[i - 1], pipes[i], pipes[N], N, i);
            }
        default:
            break;
        }
    }

    close_unused_pipes(pipes, N, N);    //fermeture des pipes inutilisés par le controller
    wait_childs_ready(request, pipes, N); //attente des fils

    if(DEBUG) {
        // Affichage des fils grâce à la commande pgrep
        switch (fork()){
        case -1:
            fprintf(stderr, "fork");
            exit(-1);
        case 0:
            printf("\n");
            char pid_string_childs[6];
            sprintf(pid_string_childs, "%d", getppid());
            printf("---CHILDS---\n");
            execlp("pgrep", "pgrep", "-P", pid_string_childs, (char*) 0);
        default:
            wait(NULL);
        }
        printf("---------\n\n");
    }

    // lancement de l'application
    controller(request, pipes, N);

    if(DEBUG) {
        printf("\n");
        //Attente de la mort de tous les fils
        int child;
        while((child = wait(NULL)) != -1){
            printf("Child PID %d died peacefully\n", child);
        }

        //On vérifie qu'il n'y a pas de processus zombies
        switch (fork()){
        case -1:
            fprintf(stderr, "fork");
            exit(-1);
        case 0:
            printf("\n");
            char pid_string_zombies[6];
            sprintf(pid_string_zombies, "%d", getppid());
            printf("---CHECK ZOMBIES---\n");
            execlp("pgrep", "pgrep", "-P", pid_string_zombies, (char*) 0);
        default:
            wait(NULL);
        }
        printf("-------------------\n\n");
    }

    printf("\nAu revoir !\n\n");

    return 0;
}
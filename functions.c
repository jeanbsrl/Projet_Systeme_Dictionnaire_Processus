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

//Gère la fermeture de tous les pipes inutilisés pour le node d'indice indice_node
void close_unused_pipes(int** pipes, int indice_node, int N){

    if (indice_node == N){      //le node est le père (controller)
        for(int i = 0; i < N; i++){
            close(pipes[i][0]);     //fermeture de read de tous les pipes fils
        }
        close(pipes[N][1]);     //fermeture de write du pipe controller
    }else{      //le node est un des fils
        if (indice_node == 0){ 
            for (int i = 1; i < N - 1; i++){    //fermeture complete des pipes fils inutilisés
                close(pipes[i][0]);
                close(pipes[i][1]);
            }
            close(pipes[N - 1][1]);     //fermeture de write du reading_pipe fils 
        }else{
            for (int i = 0; i < N; i++){  //fermeture complete des pipes fils inutilisés
                if(i != indice_node && i != indice_node - 1){
                    close(pipes[i][0]);
                    close(pipes[i][1]);
                }
            }
            close(pipes[indice_node - 1][1]);  //fermeture de write du reading_pipe fils 
        }
        close(pipes[indice_node][0]);  //fermeture de read du writing_pipe fils 
        close(pipes[N][0]);     //fermeture de read du pipe controller
    }
}

//Ecrit la requète request dans le pipe pipe
void write_pipe(int pipe[], Request* request){
    write(pipe[1], request, sizeof(Request));
}

//Crée une boucle d'attente tant que la lecture dans le pipe pipe d'une requète ne donne rien
void read_pipe(int pipe[], Request* request){
    while (read(pipe[0], request, sizeof(Request)) <= 0);
}

//Affichage un message d'interprétation de la réponse d'une requète en fonction de la commande 
void interprete_response(Request* request){
    printf("\n");
    if(request->command == 1){
        if(request->response == 1){
            printf("Stockage de la nouvelle donne reussi\n");
        }else if(request->response == 0){
            printf("Cle deja utilisee, stockage refuse\n");
        }
    }else if(request->command == 2){
        if(request->response == 1){
            printf("Affichage de la donnee reussi\n");
        }else if(request->response == 0){
            printf("Cle inconnue, impossible d'afficher la donnee\n");
        }
    }
    printf("\n");
}

//Gère la réception, le traitement et/ou la transmision d'une 
//requète par un node à un autre node ou au controller
void node(Request* request, int reading_pipe[], int writing_pipe[], int controller_pipe[], int N, int node_indice){
    int terminated = 0;
    //création dictionnaire
    PTable_entry dictionnary = init_table();

    //synchronisation (wait_childs_action_done)
    write_pipe(controller_pipe, request);

    //la fonction boucle tant que le node ne reçoit pas une requète avec commande 0
    do{
        read_pipe(reading_pipe, request);   //attente d'une lecture d'une requète
        if(DEBUG){
            printf("\nChild %d received request : \n", getpid()); 
            print_request(request); 
        } 
        switch(request->command){
        case 0:     //réception commande exit, on termine la boucle
            if(DEBUG) printf("Exit request received by child %d\n", getpid());
            terminated = 1;
            break;
        case 1:     //réception commande set
            if(request->key % N == node_indice){    //si c'est au node de traiter la requète
                if(key_exists(dictionnary, request->key)){      //si la clé existe déjà, on n'ajoute pas la valeur au dictionnaire
                    request->response = 0;  
                }else{      //sinon, on l'ajoute
                    store(&dictionnary, request->key, request->data);
                    request->response = 1;  
                }
                if(DEBUG) printf("Request treated, transmitting response to controller\n");
                write_pipe(controller_pipe, request);   //transmission de la réponse au controller (1 si tout ok ou 0 si insertion refusée car clé déjà existante)
            }else{  //si ce n'est pas à ce node de traiter la requète, il la transmet au node suivant
                if(DEBUG) printf("Wrong node to treat request, transmitting to next node\n");
                write_pipe(writing_pipe, request);
            }
            break;
        case 2:     //réception commande lookup
            if(request->key % N == node_indice){    //si c'est au node de traiter la requète
                if(key_exists(dictionnary, request->key)){  // si la clé existe, on affiche le mot
                    printf("    Data key %d : %s\n", request->key, lookup(dictionnary, request->key));
                    request->response = 1;  
                }else{      // sinon on n'affiche rien
                    request->response = 0;  
                }
                if(DEBUG) printf("Request treated, transmitting response to controller\n");
                write_pipe(controller_pipe, request);   //transmission de la réponse au controller (1 si tout ok ou 0 si la clé n'existait pas)
            }else{
                if(DEBUG) printf("Wrong node to treat request, transmitting to next node\n");
                write_pipe(writing_pipe, request);
            }
            break;
        case 3:     //réception commande dump
            write_pipe(writing_pipe, request);
            read_pipe(reading_pipe, request);
            //les nodes d'incides 1..N-1 se mettent en attente d'une lecture depuis le node précédent
            if(node_indice != 0){   
                read_pipe(reading_pipe, request);   // les fils 1 ... N-1 se mettent en attente d'une réponse de la part du fils precedent pour commencer à afficher leur table
            }

            //affichage du dictionnaire
            printf("\nTable du fils %d\n", getpid());
            display(dictionnary);
            printf("\n");
            //transmission de la requète au controller qui attendra d'avoir lu une réponse de chaque fils pour reprendre l'interaction utilisateur
            write_pipe(controller_pipe, request);

            //les node d'indice 0..N-2 transmettent la requète à leur node suivant pour qu'il commence son affichage
            if(node_indice != N-1){     
                if(DEBUG) printf("Display done, transmitting permission to display to next child\n");
                write_pipe(writing_pipe, request);
            }
            break;
        default:
            break;
        }
    }while(!terminated);

    //une fois terminé, le node envoie une réponse -1 au controller, puis meurt
    request->response = -1;
    write_pipe(controller_pipe, request);
    exit(0);
}

//Gère la saisie d'une requète, son envoi aux nodes et la réception et traitement d'une réponse
void controller(Request* request, int** pipes, int N){
    int nb_response;
    int terminated = 0;
    do{
        new_request(request);   //saisie d'une nouvelle requète
        printf("\n");
        switch(request->command){
        case 0:
        case 3:
            //Pour les commandes exit et dump, le controller envoie d'abord la requète à tous les fils,
            for(int i = 0; i < N; i++){
                write_pipe(pipes[i], request);
            }
            //puis il attend de recevoir une réponse de chaque node pour continuer
            nb_response = 0;
            while(nb_response != N){
                read_pipe(pipes[N], request);
                nb_response++;
            }
            //si la commande est exit, on quitte la boucle
            if(request->command == 0){
                terminated = 1;
            }
            break;
        default:
            //Pour les commandes set et lookup, le controller envoie la requète au node 0 (toujours)
            write_pipe(pipes[N-1], request);
            //Il attend ensuite de recevoir une réponse d'un node
            read_pipe(pipes[N], request);    
            //Puis il interprète la réponse
            interprete_response(request);    
            break;    
        }
    }while(!terminated);
}

//Permet la synchronisation au début du programme, le père commencera l'intercation utilisateur 
//une fois que tous les nodes seront bien créés et en attente de lecture d'une requète
void wait_childs_ready(Request* request, int** pipes, int N){
    int nb = 0;
    while(nb < N){
        read_pipe(pipes[N], request);
        nb++;
    }
}
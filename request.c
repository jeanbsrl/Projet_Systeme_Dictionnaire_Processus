/**
 * authors:
 * FRANCES Tom
 * BOU SERHAL Jean
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"

//Initialise une requète aux valeurs -1, -1, "" et -1
void init_request(Request* request){
    request->command = -1;
    request->key = -1;
    strcpy(request->data, "");
    request->response = -1;
}

//Gère la saisie de nouvelles valeurs pour une requète:
//command, key et data
void new_request(Request* request){
    int comm;
    int key;
    char data[30];
    init_request(request);  // on réinitialise la requete

    //saisie commande
    printf("Quelle commande souhaitez-vous utiliser ?\n  0 : Quitter\n  1 : Nouvelle donnee\n  2 : Regarder une donnee\n  3 : Regarder toutes les donnees\n\n");
    printf("Choix commande : ");
    scanf("%d", &comm);
    while(comm < 0 || comm > 3){
        printf("Vous devez saisir une commande entre 0 et 3 : ");
        scanf("%d", &comm);
    }
    request->command = comm;

    switch(request->command){
    case 1:
        //saisie cle
        printf("Choix cle (superieure à 0) : ");
        scanf("%d", &key);
        while(key < 1){
            printf("Votre cle doit etre superieure a 0 : ");
            scanf("%d", &key);
        }
        request->key = key;

        //saisie donnee
        printf("Veuillez saisir votre donnee : ");
        scanf("%s", data);
        while(strlen(data) == 0){
            printf("Veuillez saisir une donnee non vide : ");
            scanf("%s", data);
        }
        strcpy(request->data, data);
        break;
    case 2:
        //saisie cle
        printf("De quelle cle souhaitez vous regarder la donnee ? \n");
        printf("Choix cle (superieure a 0) : ");
        scanf("%d", &(request->key));
        while(request->key < 1){
            printf("Votre cle doit etre superieure a 0 : ");
            scanf("%d", &(request->key));
        }
        break;
    default:    //cas 0 et 3, on ne veut que la commande (déjà saisie avant le switch)
        break;
    }
}

//Affichage du détail d'une requète (debugg)
void print_request(Request* request){
    printf("Commande : %d\n", request->command);
    printf("Cle : %d\n", request->key);
    printf("Valeur : %s\n", request->data);
    printf("Reponse : %d\n\n", request->response);
}
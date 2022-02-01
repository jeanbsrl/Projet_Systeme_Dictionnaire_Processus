/**
 * authors:
 * FRANCES Tom
 * BOU SERHAL Jean
 * 
 */


#ifndef REQUEST_H
#define REQUEST_H

typedef struct Request{
    int command;    // commande (0, 1, 2 ou 3)
    int key;        // cle  (>0)
    char data[30];  // valeur
    int response;   // réponse
} Request;

void init_request(Request* request);
void new_request(Request* request);
void print_request(Request* request);

#endif
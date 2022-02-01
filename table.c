/**
 * authors:
 * FRANCES Tom
 * BOU SERHAL Jean
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

//Crée une table
PTable_entry init_table(){
    PTable_entry table = malloc(sizeof(PTable_entry));
    return table;
}

// Stocke dans table la valeur v sous la cle k
void store(PTable_entry *table, int k, char v[]){
    PTable_entry te = (PTable_entry)malloc(sizeof(Table_entry));
    te->key = k;
    te->val = (char *)malloc(strlen(v) + 1);
    strcpy(te->val, v);
    te->next = *table;
    *table = te;
}

// Retourne la valeur se trouvant dans table sous la cle k, ou NULL si la
// cle n'existe pas
char* lookup(PTable_entry table, int k){
    PTable_entry pe = table;
    while (pe != NULL && pe->key != k){
        pe = pe->next;
    }
    if (pe != NULL)
        return pe->val;
    else
        return NULL;
}

// Affiche le contenu de la table
int display(PTable_entry table){
    PTable_entry pe = table;
    while (pe != NULL){
        if(pe->key != 0){
            printf("    Data key %d : %s\n", pe->key, pe->val);
        }
        pe = pe->next;
    }
    return 1;
}

int key_exists(PTable_entry table, int key){
    PTable_entry current = table;
    int key_exist = 0;
    while(current != NULL && !key_exist){
        if(current->key == key){
            key_exist = 1;
        }else{
            current = current->next;
        }
    }
    return key_exist;
}


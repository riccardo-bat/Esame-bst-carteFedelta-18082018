#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tipo_inf.h"
#include "bst.h"
#include "carta.h"

//prototipi
void load_bst_from_file(bst* carte, char* filename);

int main(){
    bst bst_carte = NULL;
    load_bst_from_file(&bst_carte, "dati.txt");

    //punto 2
    stampa_bst_inorder(bst_carte);
    

    printf("\n\n");
    return 0;
}

/**
 * @brief Acquisizione di un bst da file
 * 
 * @param carte 
 * @param filename 
 */
void load_bst_from_file(bst* carte, char* filename){
    char* mode = "r"; 

    FILE* fp_bst = fopen(filename, mode);
    if(fp_bst == NULL){printf("\nERRORE"); exit(EXIT_FAILURE);}

    //creo il nuovo bst da restituire
    bst tmp_carte = NULL;

    //la prima riga del file è composta dal numero totali di nodi che devono essere letti
    char buffer[100];
    int totClienti = 0;

    if(fgets(buffer, sizeof buffer, fp_bst) == NULL){printf("\nERRORE"); fclose(fp_bst); exit(EXIT_FAILURE);}
    buffer[strcspn(buffer, "\r\n")] = '\0';
    if(sscanf(buffer, "%d", &totClienti) != 1){printf("\nERRORE"); fclose(fp_bst); exit(EXIT_FAILURE);}

    //per ogni cliente si tengono in consideraione 4 righe (1 vuota + 3 con i dati)
    for(int cliente = 1; cliente<=totClienti; cliente++){
        printf("\nLETTURA CLIENTE %d", cliente);

        //prima riga: vuota
        if(fgets(buffer, sizeof buffer, fp_bst) == NULL){printf("\nERRORE"); fclose(fp_bst); exit(EXIT_FAILURE);}
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if(buffer[0] != '\0') {printf("\nERRORE"); fclose(fp_bst); exit(EXIT_FAILURE);}

        //seconda riga: codice carta
        int codice = -1; 
        if(fgets(buffer, sizeof buffer, fp_bst) == NULL){printf("\nERRORE"); fclose(fp_bst); exit(EXIT_FAILURE);}
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if(sscanf(buffer, "%d", &codice) != 1){printf("\nERRORE"); fclose(fp_bst); exit(EXIT_FAILURE);}

        //terza riga: nome e cognome
        char nomeCognome[41]; 
        if(fgets(buffer, sizeof buffer, fp_bst) == NULL){printf("\nERRORE"); fclose(fp_bst); exit(EXIT_FAILURE);}
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if(sscanf(buffer, "%s", nomeCognome) != 1){printf("\nERRORE"); fclose(fp_bst); exit(EXIT_FAILURE);}
    
        //quarta riga: totale dei punti
        long int punti = -1;
        if(fgets(buffer, sizeof buffer, fp_bst) == NULL){printf("\nERRORE"); fclose(fp_bst); exit(EXIT_FAILURE);}
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if(sscanf(buffer, "%li", &punti) != 1){printf("\nERRORE"); fclose(fp_bst); exit(EXIT_FAILURE);}
    
        //una volta letti i dati, inserisco il nodo nel bst
        tipo_inf tmp;
        strcpy(tmp.nomeCognome, nomeCognome);
        tmp.totPunti = punti;

        printf("\nValore da inserire: %d %s %li", codice, nomeCognome, punti);
        bst_insert(&tmp_carte, bst_newNode(codice, tmp));
    }


    if(fclose(fp_bst) == EOF){printf("\nERRORE"); exit(EXIT_FAILURE);}

    //imposto i valori di ritorno 
    (*carte) = tmp_carte;
    return;
}
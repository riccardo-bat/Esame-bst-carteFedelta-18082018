#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tipo_inf.h"
#include "bst.h"
#include "carta.h"

//prototipi
long int int_input(char* msg, int minimum_value);
char* string_input(char* msg, int max_length);
void load_bst_from_file(bst* carte, char* filename);
bool aggiorna(bst carte, tipo_key codiceCarta, long int punti);



int main(){
    bst bst_carte = NULL;
    load_bst_from_file(&bst_carte, "dati.txt");

    //punto 2
    stampa_bst_inorder(bst_carte);

    //punto 3
    printf("\n-----------------------\n");
    char scelta = string_input("\nSi vogliono inserire degli acquisti? Se si, premere qualsiasi carattere, se no digitare Q: ", 1)[0];
    while(scelta != 'Q'){
        //ogni acquisto è determinato dal numero di carta e dai punti accumulati con quell'acquisto
        bool esito = aggiorna(bst_carte, (tipo_key) int_input("\nInserire il numero della carta che ha effettuato l'acquisto: ", 1), int_input("\nInserire i punti accumulati: ", 0));
        if(!esito) printf("ERRORE: la chiave inserita non e' presente\n");

        //richiedo la scelta per un altro eventuale inserimento
        scelta = string_input("\nSi vogliono inserire degli acquisti? Se si, premere qualsiasi carattere, se no digitare Q: ", 1)[0];
    }

    stampa_bst_inorder(bst_carte);
    

    printf("\n\n");
    return 0;
}

long int int_input(char* msg, int minimum_value){
    long int input;
    char buffer[100];

    printf("%s", msg);
    while(fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("\nErrore nella lettura dell'input. Riprova: ");
    }

    char *endptr;
    input = strtol(buffer, &endptr, 10); //per altri formati, strtof, strtod, ...
    if (endptr == buffer || *endptr != '\n' || input < minimum_value) {
        printf("\nInput non valido\n.");
        return int_input(msg, minimum_value);
    }

    return input;
}

char* string_input(char* msg, int max_length){
    printf("%s", msg); 
    char* buffer = malloc(max_length * sizeof(char));
    if(!buffer){printf("\n\nMalloc failed"); exit(EXIT_FAILURE);} 

    while(fgets(buffer, max_length+1, stdin) == NULL){
        printf("\nInput fallito\n");
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    return buffer;
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
        strcpy(nomeCognome, buffer);
    
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

/**
 * @brief Funzione che aggiorna i punti accumulati da un utente dopo un acquisto. Se non trova la carta restituisce FALSE
 * 
 * @param carte 
 * @param codiceCarta 
 * @param punti 
 * @return true 
 * @return false 
 */
bool aggiorna(bst carte, tipo_key codiceCarta, long int punti){
    if(carte == NULL) return false;

    //visita in-order per scorrere l'albero e trovare la carta
    //printf("\nNodo considerato: %d", carte->key);

    //se cerco una chiave < della radice, guardo solo il sottoalbero sx
    if(codiceCarta < carte->key) return aggiorna(carte->left, codiceCarta, punti);

    else if(codiceCarta == carte->key){
        //aggiorno i punti accumulati
        carte->inf.totPunti += punti;
        return true; //ho aggiornato i punti
    }

    //se codiceCarta > radice, guardo solo il sottoalbero dx
    else return aggiorna(carte->right, codiceCarta, punti);

}







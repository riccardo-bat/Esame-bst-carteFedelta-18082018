#include "carta.h"

int compare(tipo_inf a, tipo_inf b){
    int cmp = strcmp(a.nomeCognome, b.nomeCognome);
    if(cmp != 0) return cmp;

    //se hanno lo stesso nome e cognome verifico i punti 
    return (int) (a.totPunti - b.totPunti);
}

void copy(tipo_inf *dest, tipo_inf src){
    *dest = src;
}

void print_tipo_inf(tipo_inf v){
    printf("%s, che ha totalizzato %li punti", v.nomeCognome, v.totPunti);
}
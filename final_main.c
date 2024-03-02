#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Lista delle struct utilizzate
typedef struct Stazione* stazione;
typedef struct Parco* parco;
typedef struct Lista* lista;

struct Stazione{
    int key;
    int max_autonomia;
    stazione dx;
    stazione sx;
    parco parco_macchine;
};

struct Parco{
    int key;
    int ricorrenze;
    parco dx;
    parco sx;
};

struct Lista{
    int stazione;
    int ragg;
    lista best_succ;
    lista next;
    lista prev;
};

//Variabili globali
#define BUFFER 10000

//Prototipi delle funzioni per l'albero delle stazioni
stazione creaNodo(int valore);
stazione ricerca(stazione radice, int valore);
void in_order(stazione T);
stazione successore(stazione radix, stazione nodo);
stazione predecessore(stazione radix, stazione nodo);
stazione BST_delete(stazione T, int key);
stazione delete_root(stazione T);
stazione delete_min(stazione P, stazione T);
int estraiMax(parco parco_macchine);

//Prototipi delle funzioni per l'albero delle autonomie
parco parco_creaNodo(int valore);
parco parco_ricerca(parco radice, int valore);
void parco_in_order(parco T);
parco parco_successore(parco radix, parco nodo);
parco parco_predecessore(parco radix, parco nodo);
parco parco_BST_delete(parco T, int key);
parco parco_delete_root(parco T);
parco parco_delete_min(parco P, parco T);

//Lista delle funzioni core del progetto
stazione aggiungi_stazione(stazione radix, int distanza, parco parco_macchine, int max_autonomia);
parco aggiungi_singola_auto(parco radix, int distanza);
stazione demolisci_stazione(stazione radix, int distanza_demolizione);
void aggiungi_auto(stazione radix, int distanza, int auto_nuova);
void rottama_auto(stazione radix, int distanza, int rottama);
void pianifica_percorso_dx(stazione radix, int partenza, int arrivo);
void array_somme(stazione radix, int partenza, int arrivo);
void pianifica_percorso_sx(stazione radix, int partenza, int arrivo);
void stampa_percorso(lista ls, int arrivo);
void array_differenze(stazione radix, int partenza, int arrivo);

//Lista delle funzioni ausiliarie
void swap(int a, int b);
void swap_pointer(int* a, int* b);
void InserisciBarenghi(stazione radix, stazione x);
stazione minBarenghi(stazione radix);
stazione maxBarenghi(stazione radix);
int LEFT(int n){return (2 * n);};
int RIGHT(int n){return (2 * n) + 1;};

//Funzioni ausiliarie liste
void stampa_lista(lista testa);
lista agg_coda_best(lista testa, int num, int ragg);
lista agg_coda_next(lista testa, int num, int ragg);
void libera_lista(lista testa);

//MAIN
int main() {
    char buffer[BUFFER]; // Creo un buffer per memorizzare la riga di testo
    stazione radice = NULL;


    // Continua a leggere righe di testo finché non si raggiunge la fine del file (EOF)
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == 'a' && buffer[16] == 'e'){
            //aggiungi-stazione
            int distanza;
            int numero_auto = 0;
            int auto_estratte = 0;
            int i = 0;
            //Salvo i valori dalle stringhe
            sscanf(buffer, "aggiungi-stazione %d %d", &distanza, &numero_auto);
            int num_spazi = 0;
            while (num_spazi < 3){
                if (buffer[i] == ' ') {
                    num_spazi++;
                }
                i++;
            }
            const char* ptr = &buffer[i]; //Puntatore per tenere traccia della posizione corrente nella stringa
            int autonomia;
            int max_autonomia = 0;
            parco parco_macchine = NULL;
            if (numero_auto != 0){
                //Utilizza sscanf e un ciclo while per estrarre gli interi dalla stringa
                //parco_macchine -> max_autonomia = 0;
                while ((auto_estratte < numero_auto) && (sscanf(ptr, "%d", &autonomia) == 1)){
                    parco_macchine = aggiungi_singola_auto(parco_macchine, autonomia);
                    //parco_macchine -> max_autonomia = 0;
                    if (autonomia > max_autonomia){
                        max_autonomia = autonomia;
                    }
                    ptr = strchr(ptr, ' '); // Trova il prossimo spazio nella stringa
                    if (ptr == NULL) {
                        break; //Se non ci sono più spazi, interrompi il ciclo
                    }
                    ptr++; //Salta lo spazio e continua con la prossima iterazione
                    auto_estratte++;
                }
            }
            else if(numero_auto == 0){
                parco_macchine = aggiungi_singola_auto(parco_macchine, 0);
            }
            radice = aggiungi_stazione(radice, distanza, parco_macchine, max_autonomia);
        }
        //stazione global_radix = radice;
        if (buffer[0] == 'd' && buffer[1] == 'e'){
            //demolisci-stazione
            int dist_demol;
            sscanf(buffer, "demolisci-stazione %d", &dist_demol);
            demolisci_stazione(radice, dist_demol);
        }
        if (buffer[0] == 'a' && buffer[12] == 'o'){
            //aggiungi-auto
            int distanza;
            int auto_nuova;
            sscanf(buffer, "aggiungi-auto %d %d", &distanza, &auto_nuova);
            aggiungi_auto(radice, distanza, auto_nuova);
        }
        if (buffer[0] == 'r' && buffer[1] == 'o'){
            //rottama-auto
            int distanza;
            int rottama;
            sscanf(buffer, "rottama-auto %d %d", &distanza, &rottama);
            rottama_auto(radice, distanza, rottama);
        }
        if (buffer[0] == 'p' && buffer[1] == 'i'){
            //pianifica_percorso
            int partenza, arrivo;
            //Salvo i valori dalle stringhe
            sscanf(buffer, "pianifica-percorso %d %d", &partenza, &arrivo);
            if (partenza < arrivo){
                //PIANIFICA-PERCORSO DA SINISTRA A DESTRA
                pianifica_percorso_dx(radice, partenza, arrivo);
            }
            else if (partenza > arrivo){
                //PIANIFICA-PERCORSO DA DESTRA A SINISTRA
                pianifica_percorso_sx(radice, partenza, arrivo);
            }
        }
    }

    return 0;
}

//Implementazione delle funzioni
stazione minBarenghi(stazione radix){
    stazione curr = radix;
    while (curr -> sx != NULL){
        curr = curr -> sx;
    }
    return curr;
}

stazione maxBarenghi(stazione radix){
    stazione curr = radix;
    while (curr -> dx != NULL){
        curr = curr -> dx;
    }
    return curr;
}

/*++++++++++++++++++++++ PROGRAMMA PER LA MAX-HEAP COME STRUCT (non più utilizzato) ***************************/
void swap_pointer(int* a, int* b)
{
    int temp = *b;
    *b = *a;
    *a = temp;
}

void swap(int a, int b)
{
    int temp = b;
    b = a;
    a = temp;
}

/******************** FUNZIONI ALBERO **************/
stazione creaNodo(int valore) {
    stazione nuovoNodo = (stazione)malloc(sizeof(struct Stazione));
    nuovoNodo->key = valore;
    nuovoNodo->sx = NULL;
    nuovoNodo->dx = NULL;
    return nuovoNodo;
}
parco parco_creaNodo(int valore) {
    parco nuovoNodo = (parco)malloc(sizeof(struct Parco));
    nuovoNodo->key = valore;
    nuovoNodo->sx = NULL;
    nuovoNodo->dx = NULL;
    return nuovoNodo;
}



stazione aggiungi_stazione(stazione T, int key, parco parco_macchine, int max_autonomia){ //ex BST_insert
    if (T != NULL){
        if (key > T -> key){
            T -> dx = aggiungi_stazione(T -> dx, key, parco_macchine, max_autonomia);
        }
        else if (key < T -> key){
            T -> sx = aggiungi_stazione(T -> sx, key, parco_macchine, max_autonomia);
        }
        else if(key == T -> key){
            //free(T -> parco_auto);
            printf("non aggiunta\n");
        }
    }
    else{
        T = (stazione)malloc(sizeof(struct Stazione));
        T -> sx = NULL;
        T -> dx = NULL;
        T -> key = key;
        T -> parco_macchine = parco_macchine;
        T -> max_autonomia = max_autonomia;
        printf("aggiunta\n");
    }
    return T;
}



parco aggiungi_singola_auto(parco T, int key){
    if (T != NULL){
        if (key > T -> key){
            T -> dx = aggiungi_singola_auto(T -> dx, key);
        }
        else if (key < T -> key){
            T -> sx = aggiungi_singola_auto(T -> sx, key);
        }
        else if (key == T -> key){
            T -> ricorrenze++;
        }
    }
    else{
        T = (parco)malloc(sizeof(struct Parco));
        T -> sx = NULL;
        T -> dx = NULL;
        T -> key = key;
        T -> ricorrenze = 1;
    }
    return T;
}



stazione ricerca(stazione radix, int valore) {
    if (radix == NULL || radix->key == valore) {
        return radix;
    }
    if (valore < radix->key) {
        return ricerca(radix->sx, valore);
    }
    return ricerca(radix->dx, valore);
}
parco parco_ricerca(parco radix, int valore) {
    if (radix == NULL || radix->key == valore) {
        return radix;
    }
    if (valore < radix->key) {
        return parco_ricerca(radix->sx, valore);
    }
    return parco_ricerca(radix->dx, valore);
}



void in_order(stazione T){
    if (T != NULL){
        in_order(T->sx);
        printf("%d | ", T -> key);
        in_order(T -> dx);
    }
}
void parco_in_order(parco T){
    if (T != NULL){
        parco_in_order(T -> sx);
        printf("%d | ", T -> key);
        parco_in_order(T -> dx);
    }
}



stazione BST_delete(stazione T, int key){
    if (T == NULL){
        return NULL;
    }
    if (T->key > key){
        T -> sx = BST_delete(T->sx, key);
    }
    else if (T->key < key){
        T -> dx = BST_delete(T->dx, key);
    }
    else{
        T = delete_root(T);
    }
    return T;
}
parco parco_BST_delete(parco T, int key){
    if (T == NULL){
        return NULL;
    }
    if (T->key > key){
        T -> sx = parco_BST_delete(T->sx, key);
    }
    else if (T->key < key){
        T -> dx = parco_BST_delete(T->dx, key);
    }
    else{
        T = parco_delete_root(T);
    }
    return T;
}



stazione delete_root(stazione T){
    if (T == NULL){
        return NULL;
    }
    if (T->dx != NULL && T->sx != NULL){
        stazione min = delete_min(T, T->dx);
        T -> key = min -> key;
        free(min);

        return T;
    }

    stazione new_root;
    if (T->sx == NULL){
        new_root = T->dx;
    }
    else{
        new_root = T->sx;
    }

    free(T);

    return new_root;
}
parco parco_delete_root(parco T){
    if (T == NULL){
        return NULL;
    }
    if (T->dx != NULL && T->sx != NULL){
        parco min = parco_delete_min(T, T->dx);
        T -> key = min -> key;
        free(min);

        return T;
    }

    parco new_root;
    if (T->sx == NULL){
        new_root = T->dx;
    }
    else{
        new_root = T->sx;
    }

    free(T);

    return new_root;
}


stazione delete_min(stazione P, stazione T){
    if (P == NULL || T == NULL){
        return NULL;
    }

    if (T->sx != NULL){
        return delete_min(T, T->sx);
    }

    if (T == P->sx){
        P->sx = T->dx;
    }
    else{
        P->dx = T->dx;
    }
    return T;
}
parco parco_delete_min(parco P, parco T){
    if (P == NULL || T == NULL){
        return NULL;
    }

    if (T->sx != NULL){
        return parco_delete_min(T, T->sx);
    }

    if (T == P->sx){
        P->sx = T->dx;
    }
    else{
        P->dx = T->dx;
    }
    return T;
}



stazione demolisci_stazione(stazione radix, int distanza_demolizione){
    stazione da_demolire = ricerca(radix, distanza_demolizione);
    if (da_demolire != NULL){
        radix = BST_delete(radix, distanza_demolizione);
        printf("demolita\n");
        return radix;
    }
    else{
        printf("non demolita\n");
        return radix;
    }
}

void aggiungi_auto(stazione radix, int distanza, int auto_nuova){
    stazione da_aggiungere = ricerca(radix,distanza);
    if (da_aggiungere != NULL){
        if (auto_nuova > da_aggiungere -> max_autonomia){
            da_aggiungere -> max_autonomia = auto_nuova;
        }
        aggiungi_singola_auto(da_aggiungere->parco_macchine, auto_nuova);
        printf("aggiunta\n");
    }
    else{
        printf("non aggiunta\n");
    }
}

void rottama_auto(stazione radix, int distanza, int rottama){
    stazione da_rottamare = ricerca(radix,distanza);
    if (da_rottamare != NULL){
        parco autonomia_rottamata = parco_ricerca(da_rottamare->parco_macchine, rottama);
        if (autonomia_rottamata != NULL){
            if (rottama == da_rottamare -> max_autonomia){
                autonomia_rottamata = parco_BST_delete(autonomia_rottamata, rottama);
                da_rottamare -> max_autonomia = estraiMax(da_rottamare -> parco_macchine);
                /*
                autonomia_rottamata = parco_BST_delete(autonomia_rottamata, rottama);
                autonomia_rottamata -> max_autonomia = estraiMax(autonomia_rottamata);
                 */
            }
            if (da_rottamare -> parco_macchine -> ricorrenze > 1){
                da_rottamare -> parco_macchine -> ricorrenze--;
            }
            else{
                autonomia_rottamata = parco_BST_delete(autonomia_rottamata, rottama);
            }
            printf("rottamata\n");
            //parco_in_order(da_rottamare -> parco_macchine);
        }
        else{
            printf("non rottamata\n");
        }
    }
    else{
        printf("non rottamata\n");
    }
}

//Funzione per cercare il successore di un nodo nell'albero
stazione successore(stazione radix, stazione nodo){
    if (nodo->dx != NULL) {
        return minBarenghi(nodo->dx);
    }

    stazione succ = NULL;
    while (radix != NULL) {
        if (nodo->key < radix->key) {
            succ = radix;
            radix = radix->sx;
        } else if (nodo->key > radix->key) {
            radix = radix->dx;
        } else {
            break;
        }
    }
    return succ;
}

//Funzione per cercare il predecessore di un nodo nell'albero
stazione predecessore(stazione radix, stazione nodo){
    if (nodo->sx != NULL) {
        return maxBarenghi(nodo->sx);
    }

    stazione pred = NULL;
    while (radix != NULL) {
        if (nodo->key > radix->key) {
            pred = radix;
            radix = radix->dx;
        } else if (nodo->key < radix->key) {
            radix = radix->sx;
        } else {
            break;
        }
    }
    return pred;
}


int estraiMax(parco parco_macchine){
    if (parco_macchine == NULL) {
        printf("L'albero è vuoto.\n");
        return 0; // Valore segnaposto per indicare un errore
    }

    while (parco_macchine->dx != NULL) {
        parco_macchine = parco_macchine->dx;
    }

    return parco_macchine->key;
}

/****************************************** PIANIFICA-PERCORSO DA SINISTRA A DESTRA ****************************************************/
void pianifica_percorso_dx(stazione T, int partenza, int arrivo) {
    stazione nodo_partenza = ricerca(T, partenza);
    stazione nodo_arrivo = ricerca(T, arrivo);

    int num_stazioni_intermedie = 2; //parto da due perché così conto anche inizio e fine
    stazione succ = successore(T, nodo_partenza);
    while (succ->key < nodo_arrivo -> key){
        num_stazioni_intermedie++;
        succ = successore(T, succ);
    }

    int i=0;
    int max_vett[num_stazioni_intermedie];
    int vett_autonomie[num_stazioni_intermedie];

    max_vett[i] = ((nodo_partenza -> key) + (nodo_partenza -> max_autonomia));
    vett_autonomie[i] = ((nodo_partenza -> max_autonomia));
    succ = successore(T, nodo_partenza);
    for (i=1; i<num_stazioni_intermedie; i++){
        vett_autonomie[i] = (succ -> max_autonomia);
        max_vett[i] = (succ->key) + (succ -> max_autonomia);
        succ = successore(T, succ);
    }

    /*
    //STAMPA DI PROVA
    for (i=0; i<num_stazioni_intermedie; i++){
        printf("%d ", max_vett[i]);
    }
    printf("\n");
    for (i=0; i<num_stazioni_intermedie; i++){
        printf("%d ", vett_autonomie[i]);
    }
    */

    stazione nodo_attuale = ricerca(T, arrivo);
    int da_stampare[num_stazioni_intermedie];
    int num_stampati = 0;


    i=0;
    int j=0, k=0;
    while (i<(num_stazioni_intermedie-k)){
        if (max_vett[i]-vett_autonomie[i] == nodo_attuale -> key){
            printf("nessun percorso\n");
            return;
        }
        else if (max_vett[i] >= nodo_attuale -> key){
            da_stampare[j] = (max_vett[i] - vett_autonomie[i]);
            nodo_attuale = ricerca(T, da_stampare[j]); //nodo_attuale = ricerca(T, (max_vett[i] - vett_autonomie[i]));
            if ((nodo_attuale == NULL) || (nodo_attuale -> key == nodo_partenza -> key)){
                break;
            }
            k++;
            j++;
            num_stampati++;
            i=0;
        }
        else if (max_vett[i] < nodo_attuale -> key){
            i++;
        }
    }

    for (i=num_stampati; i>=0; i--){
        printf("%d ", da_stampare[i]);
    }
    printf("%d\n", arrivo);
}


/********************************************** FUNZIONI AUSILIARIE PER LE LISTE ***************************************/
lista agg_coda_best(lista testa, int num, int ragg){
    lista nuovoNodo = (lista)malloc(sizeof(struct Lista));
    nuovoNodo -> stazione = num;
    nuovoNodo -> ragg = ragg;
    nuovoNodo -> best_succ = NULL;
    nuovoNodo -> next = NULL;
    nuovoNodo -> prev = NULL;

    if (testa == NULL){
        // Se la lista è vuota, il nuovo nodo diventa la testa
        testa = nuovoNodo;
        return testa;
    }

    // Trova l'ultimo nodo nella lista
    lista attuale = testa;
    while (attuale->best_succ != NULL){
        attuale = attuale->best_succ;
    }

    // Collega il nuovo nodo all'ultimo nodo
    attuale->best_succ = nuovoNodo;
    return nuovoNodo;
}

lista agg_coda_next(lista testa, int num, int ragg){
    lista nuovoNodo = (lista)malloc(sizeof(struct Lista));
    nuovoNodo -> stazione = num;
    nuovoNodo -> ragg = ragg;
    nuovoNodo -> best_succ = NULL;
    nuovoNodo -> next = NULL;
    nuovoNodo -> prev = NULL;

    if (testa == NULL){
        // Se la lista è vuota, il nuovo nodo diventa la testa
        testa = nuovoNodo;
        return testa;
    }

    // Trova l'ultimo nodo nella lista
    lista attuale = testa;
    while (attuale->next != NULL){
        attuale = attuale->next;
    }

    // Collega il nuovo nodo all'ultimo nodo
    attuale->next = nuovoNodo;
    nuovoNodo->prev = attuale;
    return testa;
}

void libera_lista(lista testa){
    while (testa != NULL){
        lista da_liberare = testa;
        testa = testa -> next;
        free(da_liberare);
    }
}

void stampa_lista(lista testa){
    lista attuale = testa;
    while (attuale != NULL){
        printf("%d ", attuale -> stazione);
        if (attuale -> next != NULL){
            printf("(");
            lista attuale_n = attuale -> next;
            while(attuale_n != NULL){
                printf("%d ", attuale_n -> stazione);
                attuale_n = attuale_n -> next;
            }
            printf(")");
        }
        printf("( )");
        attuale = attuale->best_succ;
    }
}

/****************************************** PIANIFICA-PERCORSO DA DESTRA A SINISTRA (ver 5) ****************************************************/
void pianifica_percorso_sx(stazione T, int partenza, int arrivo) {
    stazione nodo_partenza = ricerca(T, partenza);
    stazione nodo_arrivo = ricerca(T, arrivo);
    stazione nodo_attuale = nodo_partenza;

    lista lis = (lista)malloc(sizeof(struct Lista));
    int sentinella = nodo_attuale -> key - (nodo_attuale -> max_autonomia) ;
    lis -> stazione = nodo_attuale -> key;
    lis -> ragg = sentinella;
    lis -> best_succ = NULL;
    lis -> next = NULL;
    lis -> prev = NULL;
    lista testa = NULL;
    stazione pred = predecessore(T, nodo_attuale);
    int min_ragg = 2147483647; //setto la raggiungiblità minima su MAX_INT all'inizio
    int min_staz = 0;
    nodo_attuale = pred;
    lista best_staz_corrente = NULL;
    while (1){
        if (nodo_attuale -> key == nodo_arrivo -> key){     //Gestire Fine
            if (min_ragg == 2147483647){
                printf("nessun percorso\n");
                return;
            }
            if (nodo_attuale -> key >= sentinella){
                best_staz_corrente = agg_coda_best(lis, nodo_attuale -> key, 0);
                stampa_percorso(lis, nodo_arrivo -> key);
                return;
                /*if (nodo_attuale -> key - (nodo_attuale -> max_autonomia)  <= min_ragg ){
                    min_ragg = nodo_attuale -> key - (nodo_attuale -> max_autonomia);
                    min_staz = nodo_attuale -> key;
                    libera_lista(testa);
                    testa = NULL;
                }*/
            }
            else{
                if (min_ragg == 2147483647){
                    printf("nessun percorso\n");
                    return;
                }
                best_staz_corrente = agg_coda_best(lis, min_staz, min_ragg);
                best_staz_corrente -> next = testa;
                sentinella = best_staz_corrente -> ragg;
                if(nodo_attuale -> key >= sentinella){
                    best_staz_corrente = agg_coda_best(lis, nodo_attuale -> key, 0);
                    stampa_percorso(lis, nodo_arrivo -> key);
                    return;
                }
                else{
                    printf("nessun percorso\n");
                    return;
                }
            }

            //stampa_lista(lis);
        }
        if (nodo_attuale -> key >= sentinella){     //Sto trovando il best_succ
            if (nodo_attuale -> key - (nodo_attuale -> max_autonomia)  <= min_ragg){
                min_ragg = nodo_attuale -> key - (nodo_attuale -> max_autonomia) ;
                min_staz = nodo_attuale -> key;
                libera_lista(testa);
                testa = NULL;
            }
            else{
                testa = agg_coda_next(testa, nodo_attuale->key, nodo_attuale->key - nodo_attuale->max_autonomia);
            }
            nodo_attuale = predecessore(T, nodo_attuale);
        }
        else{       //Ho trovato il best_succ, lo aggiungo in lista, collego i next se ci sono, e aggiorno la sentinella
            if (min_ragg == 2147483647){                                                      //in base al ragg di best_succ
                printf("nessun percorso\n");
                return;
            }
            best_staz_corrente = agg_coda_best(lis, min_staz, min_ragg);
            best_staz_corrente -> next = testa;
            sentinella = best_staz_corrente -> ragg;
            testa = NULL;
            min_ragg = 2147483647;
            min_staz = 0;
            //nodo_attuale = predecessore(T, nodo_attuale);
        }
    }
    return;
}

void stampa_percorso(lista ls, int arrivo){
    int suc = 0, bs_staz = 0;
    lista nx = NULL;
    if(ls == NULL){
        return;
    }
    if(ls->stazione == arrivo){
        printf("%d\n", arrivo);
        return;
    }
    if(ls->next == NULL){
        printf("%d ",ls->stazione);
        stampa_percorso(ls->best_succ, arrivo);
    }
    else{
        nx = ls->next;
        suc = ls->best_succ->stazione;
        bs_staz = ls->stazione;

        while(nx != NULL){
            if(nx->ragg<= suc){
                bs_staz = nx->stazione;
            }
            nx = nx->next;
        }
        printf("%d ", bs_staz);
        stampa_percorso(ls->best_succ, arrivo);
    }
}
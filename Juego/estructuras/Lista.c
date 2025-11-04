#include "Lista.h"
#include "Arbol.h"
void crearLista(tLista *lista)
{
    *lista = NULL;
}

int insertarPrimero(tLista *lista, const void *dato, unsigned cantBytes)
{
    tNodoL *nue;
    if((nue = (tNodoL*)malloc(sizeof(tNodoL))) == NULL || (nue->dato = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return -1;
    }
    memcpy(nue->dato, dato, cantBytes);
    nue->tam = cantBytes;
    nue->sig = *lista;
    *lista = nue;
    return 1;
}
int insertarFinal(tLista *lista, const void *dato, unsigned cantBytes)
{
    tNodoL *nue;
    if((nue = (tNodoL*)malloc(sizeof(tNodoL))) == NULL || (nue->dato = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return -1;
    }
    while(*lista)
        lista = &(*lista)->sig;
    memcpy(nue->dato, dato, cantBytes);
    nue->tam = cantBytes;
    nue->sig = NULL;
    *lista = nue;
    return 1;
}
void recorrer(tLista *lista, void accion(void*a))
{
    while(*lista)
    {
        accion((*lista)->dato);
        lista = &(*lista)->sig;
    }
}
//void mostrar(void *dato)
//{
//    int *pa =(int*)dato;
//    printf("%d\t",*pa);
//
//}


void vaciarLista(tLista *lista)
{
    while(*lista)
    {
        tNodoL *elim = *lista;
        *lista = elim->sig;
        free(elim->dato);
        free(elim);
    }
}
int listaVacia(const tLista *lista)
{
    return *lista == NULL;
}
int sacarPrimero(tLista *lista, void *dato, unsigned cantBytes)
{
    tNodoL *elim = *lista;
    if(*lista == NULL)
        return 0;
    *lista = elim->sig;
    memcpy(dato, elim->dato, MIN(cantBytes, elim->tam));
    free(elim->dato);
    free(elim);
    return 1;
}
int sacarUltimo(tLista *lista, void *dato, unsigned cantBytes)
{
    tNodoL *elim;
    if(*lista == NULL)
        return 0;
    while((*lista)->sig != NULL)
        lista = &(*lista)->sig;
    elim = *lista;
    *lista = NULL;
    memcpy(dato, elim->dato, MIN(cantBytes, elim->tam));
    free(elim->dato);
    free(elim);
    return 1;
}


int insertarOrdenado(tLista *lista, const void *dato, unsigned cantBytes, int (*cmp)(const void *a, const void*b))
{
    tNodoL *nue;
    if((nue = (tNodoL*) malloc(sizeof(tNodoL))) == NULL || (nue->dato = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return -1;
    }
    while(*lista && cmp((*lista)->dato, dato) < 0)
        lista = &(*lista)->sig;
    memcpy(nue->dato, dato, cantBytes);
    nue->tam = cantBytes;
    nue->sig = *lista;
    *lista = nue;
    return 1;
}
//int cmp_int (const void *a,const void *b)
//{
//    int *pa = (int*)a;
//    int *pb = (int*)b;
//
//return *pb-*pa;
//}

void ordenarLista(tLista *lista, int (*cmp)(const void *a, const void *b))
{
    tNodoL **menor, *aux;
    while(*lista != NULL)
    {
        menor = buscarMenor(lista, cmp);
        if(menor != lista)
        {
            aux = *menor;
            *menor = aux->sig;
            aux->sig = *lista;
            *lista = aux;
        }
        lista = &(*lista)->sig;
    }
}
tNodoL **buscarMenor(tLista *lista, int (*cmp)(const void *a, const void *b))
{
    tNodoL **menor;
    if(*lista == NULL)
        return NULL;
    menor = (tLista *)lista;
    while(*lista != NULL)
    {
        if(cmp((*lista)->dato, (*menor)->dato) > 0)
            menor = (tLista*)lista;
        lista = &(*lista)->sig;
    }
    return menor;
}

void recorrer_recursivo(tLista *lista, void accion(void*a))
{
    if(*lista == NULL)
        return;
    recorrer_recursivo(&(*lista)->sig, accion);
    accion((*lista)->dato);
}


int buscar_cla(tLista *lista, const void *clave, void *dato, unsigned cantBytes, int (*cmp)(const void *a, const void *b), int arg)
{
    tNodoL *elim;
    while(*lista != NULL && cmp((*lista)->dato, clave) != 0)
        lista = &(*lista)->sig;
    if(*lista == NULL)
    {
        return 0; // No encontre la clave
    }
    memcpy(dato, (*lista)->dato, MIN(cantBytes, (*lista)->tam));
    elim = *lista;
    if(arg == 1)
    {
        *lista = elim->sig;
        free(elim->dato);
        free(elim);
    }
    return 1;
}





int buscar_cla_recursivo(tLista *lista, const void *clave, void *dato, unsigned cantBytes, int (*cmp)(const void *a, const void *b), int arg)
{
    tNodoL *elim;
    if(*lista == NULL)
        return 0;
    if(cmp((*lista)->dato, clave) == 0)
    {
        memcpy(dato, (*lista)->dato, MIN(cantBytes, (*lista)->tam));
        if(arg == 1)
        {
            elim = *lista;
            *lista = elim->sig;
            free(elim->dato);
            free(elim);
        }
        return 1; // Encontrado
    }
    return buscar_cla_recursivo(&(*lista)->sig, clave, dato, cantBytes, cmp, arg);
}
int contar_cla(tLista *lista, const void *clave, int (*cmp)(const void *a, const void *b), int arg)
{
    int cant = 0;
    tNodoL *elim;
    while(*lista != NULL)
    {
        if(cmp((*lista)->dato, clave) == 0)
        {
            cant++;
            if(arg == 1)
            {
                elim = *lista;
                *lista = elim->sig;
                free(elim->dato);
                free(elim);
            }
            else lista = &(*lista)->sig;
        }
        else lista = &(*lista)->sig;
    }
    return cant;
}



int contar_cla_recursivo(tLista *lista, const void *clave, int (*cmp)(const void *a, const void *b), int arg)
{
    tNodoL *elim;
    int cant = 0;
    if(*lista == NULL)
        return 0;
    if(cmp(clave, (*lista)->dato) == 0)
    {
        cant = 1;
        if(arg == 1)
        {
            elim = *lista;
            *lista = elim->sig;
            free(elim->dato);
            free(elim);
            return cant + contar_cla_recursivo(lista, clave, cmp, arg);
        }
    }
    return cant + contar_cla_recursivo(&(*lista)->sig, clave, cmp, arg);
}

int buscar_cla_n(tLista *lista, const void *clave, int (*cmp)(const void *a, const void *b), int n, void *dato, unsigned cantBytes)
{
    while(*lista != NULL && n != 0)
    {
        if(cmp(clave, (*lista)->dato) == 0)
        {
            n--;
            if(n == 0)
            {
                memcpy(dato, (*lista)->dato, MIN((*lista)->tam, cantBytes));
                return 1;
            }
        }
        lista = &(*lista)->sig;
    }
    return 0;
}

int buscar_cla_n_recursivo(tLista *lista, const void *clave, int (*cmp)(const void *a, const void *b), int n, void *dato, unsigned cantBytes)
{
    if(*lista == NULL)
        return 0;
    if(cmp(clave, (*lista)->dato) == 0)
    {
        n--;
        if(n == 0)
        {
            memcpy(dato, (*lista)->dato, MIN(cantBytes, (*lista)->tam));
            return 1;
        }
    }
    return buscar_cla_n_recursivo(&(*lista)->sig, clave, cmp, n, dato, cantBytes);
}


int cmp_jugador_puntos_desc(const void *a, const void *b)
{
    const tJugadorDatos *j1 = (const tJugadorDatos *)a;
    const tJugadorDatos *j2 = (const tJugadorDatos *)b;
    // Orden descendente por puntos
    if(j1->total_puntos < j2->total_puntos)
        return 1;
    if(j1->total_puntos > j2->total_puntos)
        return -1;
    return 0;
}





















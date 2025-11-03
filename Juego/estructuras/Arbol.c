#include "Arbol.h"
void crearArbol(tArbol *arbol)
{
    *arbol = NULL;
}
int insertarnodoiterativo(tArbol *arbol, void *dato, unsigned cantBytes, int (*cmp)(const void*a, const void*b))
{
    tNodoA *nue;
    while(*arbol != NULL)
    {
        if(cmp((*arbol)->dato, dato) == 0)
        {
            return 0; //no quiero duplicados
        }
        else if(cmp((*arbol)->dato, dato) > 0)
            arbol = &(*arbol)->der;
        else if(cmp((*arbol)->dato, dato) < 0)
            arbol = &(*arbol)->izq;
    }
    if((nue = (tNodoA*)malloc(sizeof(tNodoA))) == NULL || (nue->dato = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->dato, dato, cantBytes);
    nue->tamInfo = cantBytes;
    nue->izq = NULL;
    nue->der = NULL;
    *arbol = nue;
    return 1;
}

int insertarnodorecursivo(tArbol *arbol, void *dato, unsigned cantBytes, int (*cmp)(const void*a, const void*b))
{
    tNodoA *nue;
    if(*arbol != NULL)
    {
        if(cmp((*arbol)->dato, dato) == 0)
            return 0;
        if(cmp((*arbol)->dato, dato) < 0)
        {
            arbol = &(*arbol)->izq;
            return insertarnodorecursivo(arbol, dato, cantBytes, cmp);
        }
        if(cmp((*arbol)->dato, dato) > 0)
        {
            arbol = &(*arbol)->der;
            return insertarnodorecursivo(arbol, dato, cantBytes, cmp);
        }
    }
    if((nue = (tNodoA*)malloc(sizeof(tNodoA))) == NULL || (nue->dato = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->dato, dato, cantBytes);
    nue->tamInfo = cantBytes;
    nue->izq = NULL;
    nue->der = NULL;
    *arbol = nue;
    return 1;
}


int cmp_int(const void *a, const void*b)
{
    int *pa = (int*)a;
    int *pb = (int*)b;
    return *pb - *pa;
}
void Preorden(tArbol *arbol, void accion(void*a))
{
    if(!*arbol)
        return;
    accion((*arbol)->dato);
    Preorden(&(*arbol)->izq, accion);
    Preorden(&(*arbol)->der, accion);
}

void Inorden(tArbol *arbol, void accion(void *))
{
    if(!*arbol)
        return;
    Inorden(&(*arbol)->izq, accion);
    accion((*arbol)->dato);
    Inorden(&(*arbol)->der, accion);
}
void Posorden(tArbol *arbol, void accion(void*))
{
    if(!*arbol)
        return;
    Posorden(&(*arbol)->izq, accion);
    Posorden(&(*arbol)->der, accion);
    accion((*arbol)->dato);
}

int contar_nodos(tArbol *arbol)
{
    int cont = 0 ;
    if(!*arbol)
        return 0;
    cont++;
    cont += contar_nodos(&(*arbol)->izq);
    cont += contar_nodos(&(*arbol)->der);
    return cont;
}


int contar_hojas(tArbol *arbol)
{
    if(!*arbol)
        return 0;
    if((*arbol)->izq == NULL && (*arbol)->der == NULL)
        return 1;
    return contar_hojas(&(*arbol)->izq) + contar_hojas(&(*arbol)->der);
}

void mostrar(void*a)
{
    int *pa = (int*)a;
    printf("%d\t", *pa);
}

int altura_arbol(tArbol *arbol)
{
    int izq, der;
    if(!*arbol)
        return 0;
    izq = altura_arbol(&(*arbol)->izq);
    der = altura_arbol(&(*arbol)->der);
    return ((izq > der ? izq : der) + 1);
}

tArbol* mayorvalor(tArbol *arbol)
{
    if((*arbol)->der == NULL)
        return arbol;
    if(!*arbol)
        return NULL;
    return mayorvalor(&(*arbol)->der);
}

void eliminarhojas(tArbol *arbol)
{
    if(!*arbol)
        return;
    if(((*arbol)->der) == NULL && (((*arbol)->izq) == NULL))
    {
        free((*arbol)->dato);
        free(*arbol);
        *arbol = NULL;
        return;
    }
    eliminarhojas(&(*arbol)->izq);
    eliminarhojas(&(*arbol)->der);
}

void eliminarArbol(tArbol *arbol)
{
    if(!*arbol)
        return;
    eliminarArbol(&(*arbol)->izq);
    eliminarArbol(&(*arbol)->der);
    if(!(*arbol)->izq && !(*arbol)->der)
    {
        free((*arbol)->dato);
        free(*arbol);
        *arbol = NULL;
    }
}

void insertarenarboldesdearch(tArbol *arbol, FILE *arch)
{
    int cant, ls, li;
    fseek(arch, 0, SEEK_END); // ir al final.
    cant = ftell(arch) / sizeof(tIndice);
    rewind(arch);
    li = 0;
    ls = cant - 1;
    _insertarEnArboldesdeArch(arbol, arch, li, ls);
}

int escompleto(tArbol *arbol)
{
    if(*arbol == NULL)
        return 0;
    int altura, nodos, i;
    int completo = 1;
    altura = altura_arbol(arbol);
    nodos = contar_nodos(arbol);
    for(i = 0; i < altura; i++)
        completo *= 2;
    completo -= 1;
    if(completo == nodos)
        return 1;
    return 0;
}

void _insertarEnArboldesdeArch(tArbol* arbol, FILE* arch, int li, int ls)
{
    int med;
    tIndice dato;
    if(li > ls)
        return;
    med = (ls + li) / 2;
    /// ahora recuperamos la clave del archivo .idx
    fseek(arch, sizeof(tIndice)*med, SEEK_SET);
    fread(&dato, sizeof(tIndice), 1, arch);  /// clave recuperada
    /// ahora vamos a insertar en el arbol
    insertarnodoiterativo(arbol, &dato, sizeof(dato), cmp_int);
    _insertarEnArboldesdeArch(&(*arbol)->izq, arch, li, med - 1);
    _insertarEnArboldesdeArch(&(*arbol)->der, arch, med + 1, ls);
}

tArbol *buscarnodo(tArbol *arbol, const void *dato, int(*cmp)(const void *a, const void *b))
{
    if(!*arbol)
        return NULL;
    if(cmp((*arbol)->dato, dato) == 0)
        return arbol;
    if(cmp((*arbol)->dato, dato) < 0)
        return  buscarnodo(&(*arbol)->izq, dato, cmp);

    else return  buscarnodo(&(*arbol)->der, dato, cmp);
}


tArbol *mayor_rama(tArbol *arbol)
{
    int hi, hd;
    if(!*arbol)
        return NULL;
    hi = altura_arbol(&(*arbol)->izq);
    hd = altura_arbol(&(*arbol)->der);
    return (hi > hd ? & (*arbol)->izq : & (*arbol)->der);
}
tArbol *menorvalor(tArbol *arbol)
{
    if(!*arbol)
        return NULL;
    if((*arbol)->izq == NULL)
        return arbol;
    return menorvalor(&(*arbol)->izq);
}

tArbol *nodoremplazo(tArbol *arbol)
{
    int hi, hd;
    //tArbol *remp;
    if(!*arbol)
        return NULL;
    hi = altura_arbol(&(*arbol)->izq);
    hd = altura_arbol(&(*arbol)->der);
    if(hi > hd)
        return mayorvalor(&(*arbol)->izq);
    return menorvalor(&(*arbol)->der);
}

int Eliminarelem(tArbol *arbol, void *dato, int (*cmp)(const void*a, const void*b))
{
    tArbol *elim;
    if(!*arbol)
        return 0;
    elim = buscarnodo(arbol, dato, cmp);
    return Eliminarraiz(elim, dato, cmp);
}

int Eliminarraiz(tArbol *arbol, void *dato, int (*cmp)(const void*a, const void*b))
{
    tArbol *remp;
    tNodoA *elim;
    if(!*arbol)
        return 0;
    memcpy(dato, (*arbol)->dato, (*arbol)->tamInfo);
    free((*arbol)->dato);
    if(!(*arbol)->izq && !(*arbol)->der)
    {
        free(*arbol);
        *arbol = NULL;
        return 1;
    }
    remp = nodoremplazo(arbol);
    elim = *remp;
    (*arbol)->dato = elim->dato;
    (*arbol)->tamInfo = elim->tamInfo;
    *remp = elim->izq ? elim->izq : elim->der;
    free(elim);
    return 1;
}

void guardarInorden(tArbol *arbol, FILE *pf)
{
    if(!*arbol)
        return;
    guardarInorden(&(*arbol)->izq, pf);
    fwrite((*arbol)->dato, sizeof(tIndice), 1, pf);
    guardarInorden(&(*arbol)->der, pf);
}


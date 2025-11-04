#include "Arbol.h"
#include "../header/jugador.h"


void crearArbol(tArbol *arbol)
{
    *arbol = NULL;
}
int insertarnodoiterativo(tArbol *arbol,void *dato, unsigned cantBytes, int (*cmp)(const void*a,const void*b))
{
    tNodoA *nue;

    while(*arbol != NULL)
    {
        if(cmp((*arbol)->dato,dato)==0)
        {
            return 0; //no quiero duplicados
        }
        else if(cmp((*arbol)->dato,dato)<0)
        {
            arbol = &(*arbol)->der;
        }
        else
        {
            arbol=&(*arbol)->izq;
        }
    }
    if( (nue=(tNodoA*)malloc(sizeof(tNodoA))) ==NULL || (nue->dato=malloc(cantBytes))==NULL )
    {
        free(nue);
        return 0;
    }
    memcpy(nue->dato,dato,cantBytes);
    nue->tamInfo = cantBytes;
    nue->izq = NULL;
    nue->der = NULL;
    *arbol = nue;
    return 1;
}

int insertarnodorecursivo(tArbol *arbol,void *dato,unsigned cantBytes,int (*cmp)(const void*a,const void*b))
{
    tNodoA *nue;
    if(*arbol != NULL)
    {

        if(cmp((*arbol)->dato,dato)==0)
        {
            return 0;
        }
        if(cmp((*arbol)->dato,dato)<0)
        {
            arbol = &(*arbol)->izq;
            return insertarnodorecursivo(arbol,dato,cantBytes,cmp);
        }
        if(cmp((*arbol)->dato,dato)>0)
        {
            arbol = &(*arbol)->der;
            return insertarnodorecursivo(arbol,dato,cantBytes,cmp);
        }

    }

    if( (nue=(tNodoA*)malloc(sizeof(tNodoA))) ==NULL || (nue->dato=malloc(cantBytes))==NULL )
    {
        free(nue);
        return 0;
    }
    memcpy(nue->dato,dato,cantBytes);
    nue->tamInfo = cantBytes;
    nue->izq = NULL;
    nue->der = NULL;
    *arbol = nue;
    return 1;
}

void eliminarArbol(tArbol *pa)
{
    if(*pa)
    {
        eliminarArbol(&(*pa)->izq);
        eliminarArbol(&(*pa)->der);
        free((*pa)->dato);
        free(*pa);
        *pa = NULL;
    }
}

int cmp_clave(const void *a, const void *b)
{
    tIndice *pa =(tIndice*) a;
    tIndice *pb = (tIndice*) b;
    return strcmp(pb->clave,pa->clave);
}

int compararRanking(const void *a, const void *b)
{
    const tRanking *ra = (const tRanking*)a;
    const tRanking *rb = (const tRanking*)b;

    //Queremos mayor puntaje primero
    if (ra->puntaje > rb->puntaje)
        return -1; // va a la izquierda (más grande)
    if (ra->puntaje < rb->puntaje)
        return 1;  // va a la derecha (más chico)

    //Desempatar por nombre alfabéticamente
    return strcmp(ra->nombre, rb->nombre);
}
int buscarEnArbol(const tArbol* pa, void *d,unsigned tam,int (*cmp)(const void *a, const void *b))
{
    if(!*pa)
        return 0;
    int comp = cmp(d, (*pa)->dato);
    if(comp == 0)
    {
        memcpy(d, (*pa)->dato, tam);
        return 1;
    }

    if(comp < 0)
        return buscarEnArbol(&(*pa)->izq, d, tam, cmp);
    else
        return buscarEnArbol(&(*pa)->der, d, tam, cmp);
}


int buscarEnArchivoConIndice(FILE *fp, const tArbol *pa, tJugadorDatos *datos)
{
    int claveEncontrada;
    tIndice idx;
    strcpy(idx.clave,datos->nombre);

    claveEncontrada = buscarEnArbol(pa,&idx,sizeof(tIndice),cmp_clave);

    if(claveEncontrada)
    {
        fseek(fp, idx.pos * sizeof(tJugadorDatos), SEEK_SET);
        fread(datos, sizeof(tJugadorDatos), 1, fp);
        return CLAVE_ENCONTRADA;
    }
    else
    {
        return CLAVE_NO_ENCONTRADA;
    }

}
int indexarArchivoPersonas(const char *nombreArchivoPersonas, tArbol *pa)
{
    FILE *fp;
    tJugadorDatos datos;
    tIndice idx;
    unsigned long numeroRegistro;

// Abrir el archivo
    fp = fopen(nombreArchivoPersonas, "rb");
    if(!fp)
    {
        printf("No se ha encontrado el archivo \"%s\" para indexarlo\n",
               nombreArchivoPersonas);
        return ARCHIVO_NO_INDEXADO;
    }

    fread(&datos, sizeof(tJugadorDatos), 1, fp);
    numeroRegistro = 0L;
    while(!feof(fp))
    {
        strcpy(idx.clave,datos.nombre);
        idx.pos = numeroRegistro;
        insertarnodoiterativo(pa, &idx, sizeof(tIndice), cmp_clave);
        fread(&datos, sizeof(tJugadorDatos), 1, fp);
        numeroRegistro++;
    }

    fclose(fp);
    return ARCHIVO_INDEXADO;
}
int guardarIndiceEnArchivo(const tArbol *p, const char *nombreArchivoIndice)
{
    FILE *fp = fopen(nombreArchivoIndice, "wb");

    if (!fp)
    {
        printf("Error: No se pudo abrir el archivo %s para escritura.\n",
               nombreArchivoIndice);
        return 0; // Fallo
    }

// Recorre el arbol en PreOrden y usa 'guardarNodoEnArchivo' como accion
    recorrerArbolPreOrder(p, 0, fp, guardarNodoEnArchivo);

    fclose(fp);
    return 1; // Éxito
}

void guardarNodoEnArchivo(void *info, unsigned tam, unsigned n, void *params)
{
    FILE *fp = (FILE *)params;
// La informacion del nodo es de tipo tIndice
    fwrite(info, tam, 1, fp);
}

void recorrerArbolPreOrder(const tArbol *pa,
                           unsigned n, void *params,void (*accion)(void *, unsigned, unsigned, void *))
{
    if(!*pa)
        return;

    accion((*pa)->dato, (*pa)->tamInfo, n, params);
    recorrerArbolPreOrder(&(*pa)->izq, n, params, accion);
    recorrerArbolPreOrder(&(*pa)->der, n, params, accion);
}



int cargarIndiceDesdeArchivo(tArbol *pa, const char *nombreArchivoIndice)
{
    FILE *fp;
    tIndice indice;
    int res;

// Abrir el archivo
    fp = fopen(nombreArchivoIndice, "rb");
    if (!fp)
    {
        printf("Error: No se ha encontrado el archivo \"%s\" para cargarlo\n",
               nombreArchivoIndice);
        return 0; // Fallo
    }

// Inicializar el arbol antes de cargar
//    crearArbol(pa); // Es fundamental inicializar el puntero a NULL

// Leer y cargar cada tIndice
    while (fread(&indice, sizeof(tIndice), 1, fp) == 1)
    {
// Insertar el elemento leido en el nuevo arbol
        res = insertarnodoiterativo(pa, &indice, sizeof(tIndice), cmp_clave);
        if (res == SIN_MEM)
        {
            printf("Error: Memoria insuficiente al cargar el indice.\n");
            eliminarArbol(pa); // Limpiar recursos
            fclose(fp);
            return 0; // Fallo
        }
// Se ignora CLA_DUP (clave duplicada) en este contexto ya que se asume
// que el archivo de índice es válido.
    }

    fclose(fp);
    return 1; // Éxito
}

void cargarArchivoOrdenadoEnIndiceBalanceado(tArbol *pa, FILE *fp,int inicio,int fin)
{
    if (inicio > fin)
        return;

    int medio = (inicio + fin) / 2;
    tIndice idx;
    tJugadorDatos datos;

    // Posicionarse en el registro medio
    fseek(fp, medio * sizeof(tJugadorDatos), SEEK_SET);
    fread(&datos, sizeof(tJugadorDatos), 1, fp);

    // Copiar clave y número de registro
    strcpy(idx.clave, datos.nombre);
    idx.pos = medio;

    // Insertar en el árbol
    insertarnodoiterativo(pa, &idx, sizeof(tIndice), cmp_clave);

    // Recursión izquierda y derecha
    cargarArchivoOrdenadoEnIndiceBalanceado(pa, fp, inicio, medio - 1);
    cargarArchivoOrdenadoEnIndiceBalanceado(pa, fp, medio + 1, fin);
}

int indexarArchivoPersonasOrdenado(const char *nombreArchivoPersonas,
                                   tArbol *p)
{
    FILE *fp;
    size_t cantidadRegistros;

// Abrir el archivo
    fp = fopen(nombreArchivoPersonas, "rb");
    if(!fp)
    {
        printf("No se ha encontrado el archivo \"%s\" para indexarlo\n",
               nombreArchivoPersonas);
        return ARCHIVO_NO_INDEXADO;
    }

// Determinar la cantidad de registros contenidos en el archivo.
    fseek(fp, 0, SEEK_END);
    cantidadRegistros = ftell(fp) / sizeof(tJugadorDatos);

// cargar indice en arbol
    cargarArchivoOrdenadoEnIndiceBalanceado(p, fp, 0, cantidadRegistros-1);

// Cerrar archivo
    fclose(fp);
    return ARCHIVO_INDEXADO;
}
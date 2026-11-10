## 1. Marco Teórico y Fundamentos

### 1.1 Definición y Propiedad de Ordenación Fundamental
Un **Árbol Binario de Búsqueda (ABB)** es una estructura no lineal y dinámica derivada de los árboles binarios, a la cual se le impone una **propiedad de ordenación estricta** sobre sus elementos para optimizar las operaciones de localización y recuperación de información.

Para todo nodo $N$ perteneciente a un ABB:
1. **Subárbol Izquierdo:** Todos los valores contenidos en las claves de las entidades del subárbol izquierdo de $N$ son **estrictamente menores** que la clave de $N$.
2. **Subárbol Derecho:** Todos los valores contenidos en las claves de las entidades del subárbol derecho de $N$ son **estrictamente mayores** que la clave de $N$.
3. **Subárboles Válidos:** Tanto el subárbol izquierdo como el derecho son, a su vez, Árboles Binarios de Búsqueda.
4. **Unicidad de Claves:** Por norma general de la cátedra **FaCENA - UNNE**, no se admiten claves duplicadas dentro del árbol. Ante un intento de inserción de un elemento con clave preexistente, la operación debe rechazar la inserción e informar el error.

```
                  (  25  )
                /          \
           (  12  )      (  42  )
          /        \     /      \
      (  5  )   ( 19 ) ( 30 )  ( 50 )
```

### 1.2 Eficiencia de Búsqueda e Inserción
Gracias a la propiedad de ordenación, para buscar o insertar una clave no se requiere recorrer la totalidad de la estructura como en una lista enlazada u orden secuencial. En cada paso recursivo se compara la clave buscada con el nodo actual y se descarta un subárbol completo:
* En un árbol **equilibrado**, la complejidad computacional en tiempo para las operaciones de búsqueda, inserción y eliminación es $\mathcal{O}(\log_2 n)$.
* En el **peor caso** (árbol totalmente degenerado en una lista enlazada por inserción ordenada consecutiva), la complejidad se degrada a $\mathcal{O}(n)$.

### 1.3 El Recorrido InOrden y el Orden Ascendente
Una característica matemática fundamental del ABB es que el recorrido en profundidad **InOrden** (visitar subárbol izquierdo $\rightarrow$ visitar raíz $\rightarrow$ visitar subárbol derecho) procesa e imprime todas las claves contenidas en el árbol en **orden estrictamente creciente (ascendente)**.

---

## 2. Declaración de Estructuras (Estilo Oficial FaCENA - UNNE)

Siguiendo las pautas formales de la cátedra FaCENA - UNNE, los nombres de tipos deben comenzar con `t`, los parámetros con `p`, el código debe ser modular y debe evitarse el uso de tildes o caracteres especiales dentro del código fuente C.

### 2.1 Declaración del TDA (`catalogo.h`)
```c
#ifndef CATALOGO_H
#define CATALOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TITULO 40

/* Declaracion de tipos de datos personalizados */
typedef char tString[MAX_TITULO];

typedef struct {
    int id;
    tString titulo;
} tCancion;

/* Estructura autorreferenciada del Nodo ABB */
typedef struct nodo {
    tCancion info;
    struct nodo* hijoIzq;
    struct nodo* hijoDer;
} tNodo;

/* Alias de puntero al nodo raiz */
typedef tNodo* tArbolCatalogo;

/* Prototipos de funciones publicas del TDA */
void crearArbol(tArbolCatalogo* pArbol);
bool arbolVacio(tArbolCatalogo pArbol);
void insertarCancion(tArbolCatalogo* pArbol, tCancion pCancion);
bool existeCancion(tArbolCatalogo pArbol, int pId);
void mostrarCatalogoInOrden(tArbolCatalogo pArbol, int pFlag);
tArbolCatalogo minimoElemento(tArbolCatalogo pArbol);
tArbolCatalogo maximoElemento(tArbolCatalogo pArbol);
void eliminarCancion(tArbolCatalogo* pArbol, int pId);
void destruirArbol(tArbolCatalogo* pArbol);

#endif
```

---

## 3. Implementación Paso a Paso de Operaciones Primitivas (`catalogo.c`)

### 3.1 Inicialización y Verificación de Estado Vacío
```c
#include "catalogo.h"

void crearArbol(tArbolCatalogo* pArbol) {
    *pArbol = NULL;
}

bool arbolVacio(tArbolCatalogo pArbol) {
    return pArbol == NULL;
}
```

### 3.2 Inserción Ordenada Recursiva (`insertarCancion`)
Recibe un doble puntero `pArbol` para poder modificar la dirección apuntada por los enlaces al reservar memoria en el *Heap*.

```c
void insertarCancion(tArbolCatalogo* pArbol, tCancion pCancion) {
    if (arbolVacio(*pArbol)) {
        /* Caso Base: Se encuentra una hoja libre donde crear el nodo */
        *pArbol = (tArbolCatalogo) malloc(sizeof(tNodo));
        if (*pArbol != NULL) {
            (*pArbol)->info = pCancion;
            (*pArbol)->hijoIzq = NULL;
            (*pArbol)->hijoDer = NULL;
        } else {
            printf("Error! No hay memoria disponible en el Heap.\n");
        }
    } else {
        /* Paso Recursivo: Desplazamiento segun la propiedad de orden */
        if (pCancion.id < (*pArbol)->info.id) {
            insertarCancion(&(*pArbol)->hijoIzq, pCancion);
        } else if (pCancion.id > (*pArbol)->info.id) {
            insertarCancion(&(*pArbol)->hijoDer, pCancion);
        } else {
            /* Rechazo de claves duplicadas */
            printf("Error! La cancion con ID %d ya existe en el catalogo.\n", pCancion.id);
        }
    }
}
```

### 3.3 Búsqueda Recursiva Óptima (`existeCancion`)
Aprovecha el orden del ABB para descartar subárboles enteros. No recorre nodos innecesarios.

```c
bool existeCancion(tArbolCatalogo pArbol, int pId) {
    if (arbolVacio(pArbol)) {
        return false;
    } else {
        if (pId < pArbol->info.id) {
            return existeCancion(pArbol->hijoIzq, pId);
        } else if (pId > pArbol->info.id) {
            return existeCancion(pArbol->hijoDer, pId);
        } else {
            return true; /* Elemento localizado */
        }
    }
}
```

### 3.4 Búsqueda de Mínimos y Máximos (`minimoElemento` y `maximoElemento`)
* El **mínimo** elemento se encuentra navegando sistemáticamente hacia la **extrema izquierda**.
* El **máximo** elemento se encuentra navegando sistemáticamente hacia la **extrema derecha**.

```c
tArbolCatalogo minimoElemento(tArbolCatalogo pArbol) {
    if (arbolVacio(pArbol)) {
        return NULL;
    } else if (pArbol->hijoIzq != NULL) {
        return minimoElemento(pArbol->hijoIzq);
    } else {
        return pArbol;
    }
}

tArbolCatalogo maximoElemento(tArbolCatalogo pArbol) {
    if (arbolVacio(pArbol)) {
        return NULL;
    } else if (pArbol->hijoDer != NULL) {
        return maximoElemento(pArbol->hijoDer);
    } else {
        return pArbol;
    }
}
```

### 3.5 Eliminación de Nodos (`eliminarCancion`) - Los 3 Casos

La eliminación en un ABB es la operación de mayor complejidad lógica, pues debe preservar la propiedad de orden tras remover el nodo deseado. Se contemplan tres escenarios:

1. **Caso 1: El nodo a eliminar es una Hoja (0 Hijos).**
   Se libera la memoria del nodo (`free`) y el puntero que lo referenciaba en el padre se asigna a `NULL`.
2. **Caso 2: El nodo a eliminar tiene 1 solo Hijo (izquierdo o derecho).**
   Se conecta el puntero del padre directamente con el único hijo del nodo a eliminar (el hijo reemplaza la posición del padre) y se libera la memoria.
3. **Caso 3: El nodo a eliminar tiene 2 Hijos.**
   No se puede eliminar directamente sin romper la ramificación. Se localiza el **mínimo elemento del subárbol derecho** (sucesor inmediato en InOrden) o el **máximo del subárbol izquierdo** (predecesor inmediato). Se copian los datos de dicho reemplazante al nodo actual y se elimina recursivamente el reemplazante del subárbol correspondiente.

```c
void eliminarCancion(tArbolCatalogo* pArbol, int pId) {
    tArbolCatalogo aux;

    if (arbolVacio(*pArbol)) {
        printf("\nNo existe una cancion con ID %d para eliminar.\n", pId);
    } else {
        if (pId < (*pArbol)->info.id) {
            eliminarCancion(&((*pArbol)->hijoIzq), pId);
        } else if (pId > (*pArbol)->info.id) {
            eliminarCancion(&((*pArbol)->hijoDer), pId);
        } else {
            /* Nodo localizado: Evaluacion de los 3 casos */
            
            if ((*pArbol)->hijoIzq == NULL) {
                /* Caso 1 (Hoja) y Caso 2 (Solo hijo derecho) */
                aux = *pArbol;
                *pArbol = (*pArbol)->hijoDer;
                free(aux);
            } else if ((*pArbol)->hijoDer == NULL) {
                /* Caso 2 (Solo hijo izquierdo) */
                aux = *pArbol;
                *pArbol = (*pArbol)->hijoIzq;
                free(aux);
            } else {
                /* Caso 3: El nodo posee 2 hijos */
                /* Se busca el minimo del subarbol derecho */
                aux = minimoElemento((*pArbol)->hijoDer);
                (*pArbol)->info = aux->info;
                /* Se elimina recursivamente el nodo duplicado del subarbol derecho */
                eliminarCancion(&((*pArbol)->hijoDer), aux->info.id);
            }
        }
    }
}
```

### 3.6 Recorrido InOrden para Muestreo Ordenado (`mostrarCatalogoInOrden`)
```c
void mostrarCatalogoInOrden(tArbolCatalogo pArbol, int pFlag) {
    if (pFlag == 0) {
        printf("\n*** CATALOGO DE CANCIONES (InOrden - Ascendente) ***\n");
    }
    if (!arbolVacio(pArbol)) {
        mostrarCatalogoInOrden(pArbol->hijoIzq, 1);
        printf("ID: %4d | Titulo: %s\n", pArbol->info.id, pArbol->info.titulo);
        mostrarCatalogoInOrden(pArbol->hijoDer, 1);
    }
}
```

### 3.7 Concatenación de Cadenas en InOrden (`concatenarInOrden`)
Presente en los Trabajos Prácticos de la cátedra para construir cadenas compuestas concatenando el valor de cada nodo al recorrer el ABB.

```c
void concatenarInOrden(tArbolCatalogo pArbol, char* pCadena, int pMaxLargo) {
    int largo;
    if (!arbolVacio(pArbol)) {
        concatenarInOrden(pArbol->hijoIzq, pCadena, pMaxLargo);
        
        largo = strlen(pCadena);
        if (largo < pMaxLargo - 1) {
            strcat(pCadena, pArbol->info.titulo);
            strcat(pCadena, " - ");
        }
        
        concatenarInOrden(pArbol->hijoDer, pCadena, pMaxLargo);
    }
}
```

### 3.8 Destrucción de la Estructura (`destruirArbol`)
```c
void destruirArbol(tArbolCatalogo* pArbol) {
    if (!arbolVacio(*pArbol)) {
        destruirArbol(&((*pArbol)->hijoIzq));
        destruirArbol(&((*pArbol)->hijoDer));
        free(*pArbol);
        *pArbol = NULL;
    }
}
```

---

## 4. Programa Principal de Prueba (`main.c`)

```c
#include <stdio.h>
#include "catalogo.h"

int main() {
    tArbolCatalogo catalogo;
    tCancion c1 = {105, "De Musica Ligera"};
    tCancion c2 = {42, "Crimen"};
    tCancion c3 = {200, "Puente"};
    tCancion c4 = {15, " Persiana Americana"};
    tCancion c5 = {80, "Signos"};

    crearArbol(&catalogo);

    /* Insercion de nodos */
    insertarCancion(&catalogo, c1);
    insertarCancion(&catalogo, c2);
    insertarCancion(&catalogo, c3);
    insertarCancion(&catalogo, c4);
    insertarCancion(&catalogo, c5);

    /* Muestreo ordenado */
    mostrarCatalogoInOrden(catalogo, 0);

    /* Prueba de busqueda */
    printf("\nExiste la cancion ID 80? %s\n", existeCancion(catalogo, 80) ? "Si" : "No");
    printf("Existe la cancion ID 999? %s\n", existeCancion(catalogo, 999) ? "Si" : "No");

    /* Prueba de eliminacion (Caso 3: Nodo con 2 hijos) */
    printf("\n--- Eliminando nodo ID 42 (nodo con 2 hijos) ---");
    eliminarCancion(&catalogo, 42);
    mostrarCatalogoInOrden(catalogo, 0);

    /* Liberacion de memoria */
    destruirArbol(&catalogo);
    printf("\nArbol destruido correctamente.\n");

    return 0;
}
```

---

## 5. Casos Borde y Gestión de Memoria Dinámica

### 5.1 Manejo de Retornos e Indirección en Eliminación (Caso 3)
En la eliminación de un nodo con 2 hijos (Caso 3):
* Se copia la **información** del reemplazante (`(*pArbol)->info = aux->info`).
* **¡CRÍTICO!** Se llama recursivamente a la eliminación pasándole la dirección del subárbol derecho: `eliminarCancion(&((*pArbol)->hijoDer), aux->info.id)`. Pasar `pArbol` completo provocaría la eliminación del nodo recién actualizado.

### 5.2 Control de Duplicados
Si se omite la validación de duplicados en `insertarCancion`, se violan las premisas de unicidad del TDA y las búsquedas binarias futuras responderán únicamente sobre la primera coincidencia localizada en el árbol.

---

## 6. Errores Típicos de Parcial y Diagnóstico de Bugs

### Bug 1: Usar búsqueda secuencial en un ABB
* **Error:** Recorrer ambos subárboles (izquierdo y derecho) incondicionalmente en la función `existeCancion`.
* **Consecuencia:** Anula la ventaja algorítmica del ABB, degradando la complejidad de $\mathcal{O}(\log n)$ a $\mathcal{O}(n)$.
* **Corrección:** Comparar `pId` con `pArbol->info.id` y ramificar hacia la izquierda si es menor, o hacia la derecha si es mayor.

### Bug 2: Perder subárboles en la eliminación de Caso 2
* **Error:** Hacer `free(*pArbol)` antes de reasignar `*pArbol = (*pArbol)->hijoDer`.
* **Consecuencia:** Se accede a memoria liberada (*Use-After-Free*) o se destruye la referencia a todo el subárbol descendiente.
* **Corrección:** Guardar la dirección en un puntero auxiliar `aux = *pArbol;`, actualizar la raíz del subárbol `*pArbol = (*pArbol)->hijoDer;` y finalmente ejecutar `free(aux);`.

---

## 7. Lista de Verificación de Código (Checklist de Examen)

1. [ ] **Verificación de ABB:** ¿La función de inserción compara la clave e inserta a la izquierda si es menor y a la derecha si es mayor?
2. [ ] **Control de Duplicados:** ¿Se muestra un mensaje de error si se intenta insertar una clave existente?
3. [ ] **Recorrido InOrden:** ¿Se utilizó el recorrido InOrden para mostrar los elementos en orden creciente/ascendente?
4. [ ] **Tres Casos de Eliminación:** ¿La función `eliminarCancion` contempla nodos hoja, nodos con 1 hijo y nodos con 2 hijos?
5. [ ] **Uso de Doble Puntero:** ¿Las funciones que modifican el árbol (`crearArbol`, `insertarCancion`, `eliminarCancion`, `destruirArbol`) reciben `tArbol*`?
6. [ ] **Manejo de Cero Tildes/Ñ:** ¿Se verificó la ausencia estricta de acentos y caracteres no ASCII en el código fuente C?

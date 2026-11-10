## 1. Marco Teórico y Fundamentos

### 1.1 Definición y Naturaleza Jerárquica
Un **Árbol Binario** es una estructura de datos no lineal y dinámica compuesta por un conjunto finito de elementos denominados **nodos**. A diferencia de las estructuras lineales (listas, pilas y colas) donde cada elemento posee un único sucesor y antecesor inmediato, en un árbol binario existe una relación jerárquica de tipo uno a muchos:
* Existe un nodo privilegiado denominado **Raíz**.
* Los demás nodos se dividen en dos subconjuntos disjuntos y jerárquicamente inferiores: el **Subárbol Izquierdo** y el **Subárbol Derecho**, los cuales son a su vez árboles binarios.

### 1.2 Terminología Formal de la Cátedra
* **Raíz:** Nodo principal de nivel cero a partir del cual se origina la estructura completa. No posee nodo padre.
* **Nodo Padre:** Nodo que contiene referencias explícitas a otros nodos de jerarquía inferior.
* **Nodo Hijo:** Nodo apuntado directamente por un nodo padre. Se clasifican en `hijoIzq` e `hijoDer`.
* **Hojas (Nodos Terminales):** Nodos que no poseen ningún descendiente (`hijoIzq == NULL && hijoDer == NULL`).
* **Nodos Internos:** Aquellos nodos que poseen al menos un hijo.
* **Grado de un Nodo:** Cantidad de hijos directos que posee un nodo (en árboles binarios varía entre 0, 1 o 2).
* **Nivel de un Nodo:** Distancia medida en aristas desde la raíz hasta dicho nodo. La raíz se ubica en el nivel 0.
* **Altura de un Árbol:** Longitud del camino más largo desde la raíz hasta la hoja más profunda. Por convención, un árbol vacío tiene altura -1 o 0 según la implementación (en esta cátedra, un árbol con solo la raíz tiene altura 1).

```
                ( Raiz )               <-- Nivel 0
               /        \
       (Hijo Izq)      (Hijo Der)      <-- Nivel 1
        /      \              \
     (Hoja)   (Hoja)         (Hoja)    <-- Nivel 2 (Hojas)
```

---

## 2. Declaración de Estructuras (Estilo Oficial FaCENA - UNNE)

De acuerdo con las **pautas de estilo de cátedra**, los tipos de datos deben utilizar el prefijo `t`, los nombres de campos deben ser descriptivos y no deben contener tildes ni la letra `ñ` en el código ni en los comentarios.

### 2.1 Definición en C
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CAD 30

/* Declaracion de tipos de datos personalizados */
typedef char tString[MAX_CAD];

typedef struct {
    int codigo;
    tString descripcion;
} tDatos;

/* Estructura autorreferenciada del Nodo */
typedef struct nodo {
    tDatos info;
    struct nodo* hijoIzq;
    struct nodo* hijoDer;
} tNodo;

/* Puntero principal para manipular el arbol */
typedef tNodo* tArbol;
```

### 2.2 Explicación del Manejo de Punteros
* `tArbol` es un alias para `tNodo*`. Representa un puntero a la raíz de un subárbol.
* **Simple puntero (`tArbol pArbol`):** Se utiliza cuando la función consulta o recorre la estructura sin modificar la dirección almacenada en el puntero raíz original.
* **Doble puntero (`tArbol* pArbol` o `tNodo** pArbol`):** Se utiliza cuando la función debe modificar la dirección de memoria apuntada por la raíz (por ejemplo, en la creación, inserción o destrucción del árbol).

---

## 3. Implementación Paso a Paso de Operaciones Primitivas

### 3.1 Creación e Inicialización
Asigna `NULL` a la variable puntero para establecer el estado inicial de árbol vacío.

```c
void crearArbol(tArbol* pArbol) {
    *pArbol = NULL;
}
```

### 3.2 Verificación de Estado Vacío
Retorna `true` si la raíz apunta a `NULL`.

```c
bool arbolVacio(tArbol pArbol) {
    return pArbol == NULL;
}
```

### 3.3 Creación de un Nuevo Nodo Dinámico (Auxiliar)
Reserva memoria en el *Heap* e inicializa sus punteros hijos en `NULL`.

```c
tArbol crearNodo(tDatos pInfo) {
    tArbol nuevoNodo = (tArbol) malloc(sizeof(tNodo));
    if (nuevoNodo != NULL) {
        nuevoNodo->info = pInfo;
        nuevoNodo->hijoIzq = NULL;
        nuevoNodo->hijoDer = NULL;
    } else {
        printf("Error! Memoria insuficiente en el Heap.\n");
    }
    return nuevoNodo;
}
```

---

### 3.4 Recorridos Recursivos en Profundidad (DFS)

Los recorridos procesan cada nodo según el orden en que se visita la **Raíz (N)**, el **Subárbol Izquierdo (I)** y el **Subárbol Derecho (D)**.

#### A. Preorden (N -> I -> D)
Utilizado para duplicar la estructura del árbol o generar representaciones jerárquicas prefijas.

```c
void visualizarPreOrden(tArbol pArbol, int pFlag) {
    if (pFlag == 0) {
        printf("\n*** RECORRIDO PRE-ORDEN (Nodo -> Izq -> Der) ***\n");
    }
    if (!arbolVacio(pArbol)) {
        /* 1. Visitar Raiz */
        printf("Cod: %d | Desc: %s\n", pArbol->info.codigo, pArbol->info.descripcion);
        /* 2. Recorrer Subarbol Izquierdo */
        visualizarPreOrden(pArbol->hijoIzq, 1);
        /* 3. Recorrer Subarbol Derecho */
        visualizarPreOrden(pArbol->hijoDer, 1);
    }
}
```

#### B. Inorden (I -> N -> D)
Procesa primero el subárbol izquierdo, luego el nodo actual y finalmente el subárbol derecho.

```c
void visualizarInOrden(tArbol pArbol, int pFlag) {
    if (pFlag == 0) {
        printf("\n*** RECORRIDO IN-ORDEN (Izq -> Nodo -> Der) ***\n");
    }
    if (!arbolVacio(pArbol)) {
        /* 1. Recorrer Subarbol Izquierdo */
        visualizarInOrden(pArbol->hijoIzq, 1);
        /* 2. Visitar Raiz */
        printf("Cod: %d | Desc: %s\n", pArbol->info.codigo, pArbol->info.descripcion);
        /* 3. Recorrer Subarbol Derecho */
        visualizarInOrden(pArbol->hijoDer, 1);
    }
}
```

#### C. Postorden (I -> D -> N)
Procesa ambos subárboles antes de procesar el nodo padre. Es el único recorrido válido para liberar memoria dinámicamente de forma ascendente.

```c
void visualizarPostOrden(tArbol pArbol, int pFlag) {
    if (pFlag == 0) {
        printf("\n*** RECORRIDO POST-ORDEN (Izq -> Der -> Nodo) ***\n");
    }
    if (!arbolVacio(pArbol)) {
        /* 1. Recorrer Subarbol Izquierdo */
        visualizarPostOrden(pArbol->hijoIzq, 1);
        /* 2. Recorrer Subarbol Derecho */
        visualizarPostOrden(pArbol->hijoDer, 1);
        /* 3. Visitar Raiz */
        printf("Cod: %d | Desc: %s\n", pArbol->info.codigo, pArbol->info.descripcion);
    }
}
```

---

### 3.5 Operaciones Métricas y de Consulta

#### A. Conteo Total de Nodos
```c
int contarNodos(tArbol pArbol) {
    if (arbolVacio(pArbol)) {
        return 0;
    } else {
        return 1 + contarNodos(pArbol->hijoIzq) + contarNodos(pArbol->hijoDer);
    }
}
```

#### B. Conteo de Nodos Hoja
Un nodo es hoja si sus dos punteros hijos son iguales a `NULL`.

```c
int contarHojas(tArbol pArbol) {
    if (arbolVacio(pArbol)) {
        return 0;
    } else {
        if (pArbol->hijoIzq == NULL && pArbol->hijoDer == NULL) {
            return 1;
        } else {
            return contarHojas(pArbol->hijoIzq) + contarHojas(pArbol->hijoDer);
        }
    }
}
```

#### C. Cálculo de la Altura del Árbol
La altura es 1 más el máximo entre las alturas de sus dos subárboles.

```c
int maximo(int pA, int pB) {
    return (pA > pB) ? pA : pB;
}

int calcularAltura(tArbol pArbol) {
    if (arbolVacio(pArbol)) {
        return 0;
    } else {
        return 1 + maximo(calcularAltura(pArbol->hijoIzq), calcularAltura(pArbol->hijoDer));
    }
}
```

---

### 3.6 Liberación Total de Memoria (Destrucción)

Para liberar la memoria sin perder las referencias a los subárboles descendientes, la eliminación debe realizarse estrictamente mediante un **recorrido en Postorden**.

```c
void destruirArbol(tArbol* pArbol) {
    if (!arbolVacio(*pArbol)) {
        /* 1. Destruir subarbol izquierdo */
        destruirArbol(&((*pArbol)->hijoIzq));
        /* 2. Destruir subarbol derecho */
        destruirArbol(&((*pArbol)->hijoDer));
        /* 3. Liberar el nodo actual */
        free(*pArbol);
        /* 4. Asignar NULL para evitar puntero colgante */
        *pArbol = NULL;
    }
}
```

---

## 4. Casos Borde y Gestión de Memoria Dinámica

### 4.1 Manejo de `NULL` en Llamadas Recursivas
En estructuras recursivas sobre árboles, el **caso base fundamental** es evaluar si `pArbol == NULL`. Intentar acceder a `pArbol->info` o `pArbol->hijoIzq` cuando `pArbol` es `NULL` produce inmediatamente un error de violación de segmento (*Segmentation Fault*).

### 4.2 Prevención de Punteros Colgantes (*Dangling Pointers*)
Al ejecutar `free(*pArbol)`, la memoria en el Heap queda liberada, pero la variable mantendrá la dirección numérica anterior si no se asigna explícitamente `*pArbol = NULL`. Por ello, toda función de destrucción debe recibir un doble puntero `tArbol*`.

---

## 5. Errores Típicos de Parcial y Diagnóstico de Bugs

### Bug 1: No inicializar los punteros hijos al reservar memoria
* **Error:** Usar `malloc` y asignar la información sin poner `hijoIzq = NULL` y `hijoDer = NULL`.
* **Consecuencia:** Los punteros contendrán direcciones de memoria basura, provocando lecturas erróneas o bucles infinitos en la recursión.
* **Corrección:**
  ```c
  /* CORRECTO */
  *pArbol = (tArbol) malloc(sizeof(tNodo));
  (*pArbol)->info = pInfo;
  (*pArbol)->hijoIzq = NULL;
  (*pArbol)->hijoDer = NULL;
  ```

### Bug 2: Intentar liberar el nodo antes que sus hijos (Uso de Preorden para destruir)
* **Error:** Ejecutar `free(*pArbol)` antes de llamar a `destruirArbol` en los subárboles izquierdo y derecho.
* **Consecuencia:** Al liberar el nodo padre, se pierden las direcciones `hijoIzq` e `hijoDer`, imposibilitando liberar los subárboles descendientes y generando fuga de memoria (*memory leak*).
* **Corrección:** Aplicar siempre el patrón Postorden (destruir hijos primero, liberar el padre al final).

### Bug 3: Indirección incorrecta en llamadas recursivas con doble puntero
* **Error:** Escribir `destruirArbol((*pArbol)->hijoIzq)` en lugar de pasar la dirección del puntero.
* **Consecuencia:** Error de compilación por incompatibilidad de tipos (`tArbol` en lugar de `tArbol*`).
* **Corrección:** Usar la sintaxis de dirección: `destruirArbol(&((*pArbol)->hijoIzq))`.

---

## 6. Lista de Verificación de Código (Checklist de Examen)

Antes de entregar el examen escrito o práctico, verificar punto por punto:

1. [ ] **Verificación de `NULL`:** ¿Todas las funciones recursivas incluyen la verificación `if (pArbol == NULL)` o `if (arbolVacio(pArbol))` como primer paso?
2. [ ] **Inicialización de Hijos:** Al crear cada nuevo nodo con `malloc`, ¿se asignó explícitamente `NULL` tanto a `hijoIzq` como a `hijoDer`?
3. [ ] **Nombres de Tipos y Parámetros:** ¿Los tipos comienzan con `t` (`tNodo`, `tArbol`, `tDatos`) y los parámetros con `p` (`pArbol`, `pInfo`, `pFlag`)?
4. [ ] **Sintaxis sin Caracteres Especiales:** ¿El código impreso o escrito está completamente libre de tildes y letras `ñ`?
5. [ ] **Firma en Funciones Modificadoras:** ¿Las funciones que alteran la estructura de la raíz reciben `tArbol*` (doble puntero)?
6. [ ] **Liberación de Memoria:** ¿La función de destrucción utiliza recorrido Postorden y asigna `NULL` al finalizar?

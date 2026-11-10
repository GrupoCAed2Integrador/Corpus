# Listas Simplemente Enlazadas Dinámicas

Una **lista simplemente enlazada dinámica** es una estructura de datos lineal compuesta por una secuencia de nodos alojados dinámicamente en el *heap*. A diferencia de un arreglo estático, cuyos elementos ocupan posiciones contiguas de memoria fija, en una lista enlazada cada nodo se compone de dos partes esenciales: la información o carga útil (de tipo simple o estructurada) y un puntero de enlace que contiene la dirección de memoria del siguiente nodo en la secuencia.

El control total de la estructura se mantiene a través de un único puntero de referencia conocido como **puntero a la cabeza** (`lista` o `cabeza`). Si la lista está vacía, dicho puntero contiene el valor `NULL`. El último nodo de la lista se distingue porque su campo `siguiente` apunta explícitamente a `NULL`, marcando el fin de la estructura.

---

## 1. Definición y Fundamentos Estructurales

En la cátedra Algoritmos y Estructuras de Datos II (FaCENA - UNNE), la definición de una lista enlazada dinámica utiliza la sentencia `typedef` para abstraer tanto el tipo de dato almacenado (`tElem` o `tDatos`) como la estructura del nodo dinámico (`tLista`).

### Declaración de Tipos Estándar de Cátedra

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 1. Definición del tipo de dato contenido en cada nodo
typedef int tElem; 

// 2. Definición de la estructura del nodo dinámico
typedef struct nodo {
    tElem elem;                 // Carga útil / información del nodo
    struct nodo *siguiente;     // Puntero autorreferenciado al sucesor
} tLista;

// 3. Declaración de la variable global o local de control
tLista *lista = NULL;           // Puntero de cabeza inicializado en NULL
```

Cuando el dato del nodo es una estructura compleja (como un producto, un videojuego o un pedido), se define una estructura `tDatos` previa:

```c
typedef char tString[50];

typedef struct {
    int idJuego;
    tString titulo;
    int anioLanzamiento;
    float precio;
} tVideojuego;

typedef struct nodoJuego {
    tVideojuego datos;
    struct nodoJuego *siguiente;
} tListaVideojuegos;
```

---

## 2. Operaciones Básicas y Algoritmos de Cátedra

Las operaciones fundamentales sobre listas enlazadas dinámicas deben garantizar la integridad de los enlaces y la correcta gestión de memoria en el *heap*.

### Operación 1: Inicializar Lista y Verificar Lista Vacía

Inicializar una lista consiste en asegurar que el puntero principal apunte a `NULL`. La verificación de lista vacía retorna `true` si el puntero de cabeza es igual a `NULL`.

```c
void inicializarLista(tLista **pLista) {
    *pLista = NULL;
}

bool listaVacia(tLista *pLista) {
    return (pLista == NULL);
}
```

---

### Operación 2: Inserción al Inicio (`insertarPrimero` / `insertarAdelante`)

La inserción al frente de la lista es una operación de complejidad constante $O(1)$. Permite agregar un nuevo nodo antes del actual primer elemento.

#### Algoritmo Paso a Paso
1. Reservar memoria en el *heap* mediante `malloc(sizeof(tLista))`.
2. Asignar el dato recibido al campo `elem` del nuevo nodo.
3. Conectar el campo `siguiente` del nuevo nodo hacia donde apunta actualmente la cabeza de la lista.
4. Actualizar el puntero de cabeza para que apunte al nuevo nodo.

```c
void insertarAdelante(tLista **pLista, tElem pElem) {
    tLista *nuevoNodo = (tLista *) malloc(sizeof(tLista));
    
    if (nuevoNodo != NULL) {
        nuevoNodo->elem = pElem;
        nuevoNodo->siguiente = *pLista;  // Apunta al que era el primer nodo
        *pLista = nuevoNodo;             // La cabeza ahora es el nuevo nodo
    } else {
        printf("Error: Memoria insuficiente en el Heap.\n");
    }
}
```

---

### Operación 3: Inserción al Final de la Lista

Para insertar un nodo al final de una lista simplemente enlazada, es necesario recorrer la estructura hasta ubicar el último nodo actual (aquel cuyo campo `siguiente` sea `NULL`).

#### Algoritmo Paso a Paso
1. Reservar memoria para el nuevo nodo, asignar su dato y establecer su campo `siguiente` en `NULL`.
2. Si la lista está vacía (`*pLista == NULL`), el nuevo nodo se convierte en el primer nodo (`*pLista = nuevoNodo`).
3. Si la lista ya contiene elementos, utilizar un puntero auxiliar `aux` para avanzar hasta el último nodo (`while (aux->siguiente != NULL)`).
4. Conectar `aux->siguiente = nuevoNodo`.

```c
void insertarFinal(tLista **pLista, tElem pElem) {
    tLista *nuevoNodo = (tLista *) malloc(sizeof(tLista));
    nuevoNodo->elem = pElem;
    nuevoNodo->siguiente = NULL;

    if (*pLista == NULL) {
        *pLista = nuevoNodo;
    } else {
        tLista *aux = *pLista;
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevoNodo;
    }
}
```

---

### Operación 4: Inserción en la k-ésima Posición (`insertarK`)

Permite insertar un nuevo elemento en una posición ordinal específica $k$ ($1 \le k \le n+1$).

#### Algoritmo Paso a Paso
1. Si $k = 1$, invocar directamente a `insertarAdelante`.
2. Para $k > 1$, posicionar un puntero auxiliar `aux` en el nodo previo ($k-1$) mediante un bucle `for` de $1$ hasta $k-2$.
3. Reservar memoria para el nuevo nodo y asignar su dato.
4. Conectar `nuevoNodo->siguiente = aux->siguiente`.
5. Enlazar `aux->siguiente = nuevoNodo`.

```c
void insertarK(tLista **pLista, int k, tElem nuevoDato) {
    if (k == 1) {
        insertarAdelante(pLista, nuevoDato);
        return;
    }

    tLista *aux = *pLista;
    for (int i = 1; i < k - 1 && aux != NULL; i++) {
        aux = aux->siguiente;
    }

    if (aux != NULL) {
        tLista *nuevoNodo = (tLista *) malloc(sizeof(tLista));
        nuevoNodo->elem = nuevoDato;
        nuevoNodo->siguiente = aux->siguiente; // 1. Conectar con el resto
        aux->siguiente = nuevoNodo;            // 2. Apuntar aux al nuevo nodo
    } else {
        printf("Posición k invalida fuera de rango.\n");
    }
}
```

---

### Operación 5: Eliminación del Primer Nodo (`eliminarPrimero`)

Elimina el nodo del frente de la lista y libera su memoria.

#### Algoritmo Paso a Paso
1. Verificar si la lista no está vacía.
2. Resguardar la dirección del primer nodo en un puntero auxiliar (`nodoSuprimir = *pLista`).
3. Avanzar la cabeza de la lista hacia el siguiente nodo (`*pLista = (*pLista)->siguiente`).
4. Liberar la memoria con `free(nodoSuprimir)`.
5. Asignar `nodoSuprimir = NULL` para evitar un puntero colgante.

```c
void eliminarPrimero(tLista **pLista) {
    if (listaVacia(*pLista)) {
        printf("La lista está vacía, no se puede eliminar.\n");
        return;
    }

    tLista *nodoSuprimir = *pLista;
    *pLista = (*pLista)->siguiente; // Avanzar la cabeza
    free(nodoSuprimir);
    nodoSuprimir = NULL;
}
```

---

### Operación 6: Eliminación en la k-ésima Posición (`eliminarK`)

Elimina el nodo ubicado en la posición $k$ ($1 \le k \le n$).

#### Algoritmo Paso a Paso
1. Si $k = 1$, llamar a `eliminarPrimero`.
2. Si $k > 1$, avanzar un puntero auxiliar `aux` hasta el nodo $k-1$.
3. Resguardar el nodo a suprimir: `nodoSuprimir = aux->siguiente`.
4. Saltear el nodo en la cadena de enlaces: `aux->siguiente = nodoSuprimir->siguiente`.
5. Liberar `free(nodoSuprimir)` y anular `nodoSuprimir = NULL`.

```c
void eliminarK(tLista **pLista, int k) {
    if (listaVacia(*pLista)) return;

    if (k == 1) {
        eliminarPrimero(pLista);
        return;
    }

    tLista *aux = *pLista;
    for (int i = 1; i < k - 1 && aux != NULL; i++) {
        aux = aux->siguiente;
    }

    if (aux != NULL && aux->siguiente != NULL) {
        tLista *nodoSuprimir = aux->siguiente;
        aux->siguiente = nodoSuprimir->siguiente;
        free(nodoSuprimir);
        nodoSuprimir = NULL;
    }
}
```

---

### Operación 7: Recorrido y Visualización No Destructiva

El recorrido transversal requiere emplear un puntero auxiliar para visitar cada nodo desde la cabeza hasta `NULL`, sin alterar el puntero de control principal.

```c
void visualizarElementos(tLista *pLista) {
    tLista *aux = pLista; // Puntero auxiliar de copia
    
    if (listaVacia(pLista)) {
        printf("\nLa lista esta vacia!!\n");
    } else {
        printf("\n*** Detalle de elementos en la lista ***\n");
        while (aux != NULL) {
            printf("%d -> ", aux->elem);
            aux = aux->siguiente; // Avanzar al siguiente nodo
        }
        printf("NULL\n");
    }
}
```

---

### Operación 8: Liberación Completa de la Lista (`liberarLista`)

Antes de finalizar la ejecución del programa, es obligatorio liberar todos los nodos acumulados en el *heap* para prevenir fugas de memoria (*memory leaks*).

```c
void liberarLista(tLista **pLista) {
    tLista *aux;
    while (*pLista != NULL) {
        aux = *pLista;
        *pLista = (*pLista)->siguiente; // Avanzar la cabeza antes de destruir
        free(aux);
        aux = NULL;
    }
}
```

---

## 3. Metodología de Resolución de Parciales y Casos Borde

En las evaluaciones parciales de FaCENA - UNNE, los ejercicios exigen la implementación de funciones con búsqueda, filtrado, acumulaciones y eliminación selectiva sobre listas dinámicas.

### Matriz de Casos Borde en Listas Enlazadas

| Escenario | Condición de Control | Acción / Precaución |
| :--- | :--- | :--- |
| **Lista Vacía** | `lista == NULL` | Validar siempre antes de intentar desreferenciar campos. Retornar mensaje o valor por defecto. |
| **Lista Unitaria** | `lista->siguiente == NULL` | Al eliminar el único elemento, el puntero de cabeza debe quedar en `NULL`. |
| **Inserción/Eliminación en $k=1$** | `k == 1` | Modifica directamente el puntero de cabeza. Requiere paso por referencia (`tLista **`). |
| **Búsqueda sin éxito** | `aux == NULL` al terminar bucle | Evitar acceder a `aux->elem` o `aux->siguiente` cuando `aux` vale `NULL`. |

---

## 4. Checklist de Evaluación para Exámenes

- [ ] ¿El tipo de nodo incluye el puntero autorreferenciado `struct nodo *siguiente`?
- [ ] ¿Las funciones que modifican la estructura reciben `tLista **pLista` (doble puntero) o modifican la variable global de cabeza correctamente?
- [ ] ¿Toda reserva con `malloc(sizeof(tLista))` verifica que la memoria no sea `NULL`?
- [ ] ¿En las eliminaciones se guarda el nodo en un puntero `nodoSuprimir` antes de mover los enlaces y llamar a `free()`?
- [ ] ¿Se realiza `nodoSuprimir = NULL` inmediatamente después de `free()`?
- [ ] ¿Los recorridos de visualización o cálculo usan un puntero auxiliar `aux` sin mover el puntero de cabeza original?
- [ ] ¿El programa liberó toda la memoria asignada antes de salir llamando a `liberarLista()`?

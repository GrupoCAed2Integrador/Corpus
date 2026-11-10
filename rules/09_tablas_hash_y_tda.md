## 1. Marco Teórico y Fundamentos

### 1.1 Concepto de Tipo de Dato Abstracto (TDA) y Encapsulamiento
Un **Tipo de Dato Abstracto (TDA)** es un modelo matemático acompañado por un conjunto de operaciones definidas sobre dicho modelo. El principio fundamental del TDA es el **encapsulamiento** y el **ocultamiento de la información** (*Information Hiding*):
* **Interfaz Pública (`.h`):** Define *qué* hace el TDA. Declara las estructuras de datos principales y los prototipos de las funciones accesibles por el usuario o programa principal.
* **Implementación Privada (`.c`):** Define *cómo* lo hace. Contiene el desarrollo algorítmico de las funciones y posibles rutinas auxiliares ocultas al cliente.

El objetivo principal es independizar al usuario de los detalles internos de implementación. Si la estructura interna cambia (por ejemplo, de un arreglo estático a una lista dinámicamente enlazada), el programa cliente que consume la interfaz `.h` no requiere modificaciones.

### 1.2 Dispersión y Tablas Hash (*Hash Tables*)
Una **Tabla Hash** es una estructura de datos asociativa que permite lograr búsquedas, inserciones y eliminaciones con una complejidad temporal promedio de **\\(O(1)\\)**.

A diferencia de las estructuras analizadas anteriormente (arreglos, listas, árboles binarios de búsqueda), donde el acceso depende de comparaciones entre claves (con complejidades de \\(O(N)\\) o \\(O(\log N)\\)), una Tabla Hash utiliza una **Función de Dispersión o Función Hash** \\(h(k)\\) para transformar una clave \\(k\\) en un índice numérico dentro de un arreglo primario denominado **Tabla o Vector de Cubetas (*Buckets*)**.

```
Clave (k) ---> [ Funcion Hash h(k) ] ---> Indice (0 a TAM-1) ---> Vector de Cubetas
```

### 1.3 Funciones de Dispersión Comunes
La función hash debe distribuir las claves de forma uniforme a lo largo del arreglo para minimizar colisiones y ejecutarse en tiempo \\(O(1)\\).

* **Método de la División (Módulo):** Es el método más utilizado en la cátedra.
  $$\text{Indice} = k \bmod \text{TAM}$$
  donde `TAM` es el tamaño de la tabla (idealmente un número primo cercano a la cantidad estimada de elementos para favorecer la distribución uniforme).

* **Tratamiento de Claves de Cadena de Caracteres:**
  Para claves alfanuméricas (como DNI en texto, códigos de productos o nombres), se convierte la cadena en un valor entero sumando los valores ASCII de sus caracteres (o combinándolos polinomialmente) antes de aplicar el módulo:
  $$k = \sum_{i=0}^{L-1} \text{cadena}[i] \times 31^i$$
  $$\text{Indice} = k \bmod \text{TAM}$$

### 1.4 Colisiones y Métodos de Resolución
Se produce una **colisión** cuando la función hash asigna dos claves distintas \\(k_1 \neq k_2\\) al mismo índice dentro de la tabla:
$$h(k_1) = h(k_2)$$

Debido a que el conjunto de claves posibles suele ser inmensamente mayor que el tamaño de la tabla (`TAM`), las colisiones son inevitables (Principio del Palomar). Para resolverlas existen dos enfoques clásicos:

1. **Direccionamiento Abierto (Resolución Interna):** Todos los elementos se almacenan dentro del mismo arreglo. Al ocurrir una colisión, se busca otra posición libre dentro del arreglo mediante prueba lineal (*linear probing*), prueba cuadrática o doble hashing.
2. **Encadenamiento Dinámico / Encadenamiento Separado (Resolución Externa - Estándar FaCENA):** Cada posición del vector de cubetas almacena un puntero a una **lista enlazada dinámica**. Cuando dos o más elementos colisionan en la misma posición `i`, se insertan como nodos en la lista apuntada por `tabla[i]`.

```
Vector de Cubetas (TAM = 5)
 +---+
 | 0 | --> NULL
 +---+
 | 1 | --> [Nodo: K=11] --> [Nodo: K=26] --> NULL   (Colision en indice 1: 11 % 5 = 1, 26 % 5 = 1)
 +---+
 | 2 | --> [Nodo: K=7]  --> NULL
 +---+
 | 3 | --> NULL
 +---+
 | 4 | --> [Nodo: K=19] --> NULL
 +---+
```

### 1.5 Factor de Carga ($\alpha$)
El **Factor de Carga** mide el nivel de ocupación de la tabla hash y se define como:
$$\alpha = \frac{N}{\text{TAM}}$$
donde $N$ es el número total de elementos almacenados y $\text{TAM}$ es la capacidad de la tabla.

* En encadenamiento dinámico, $\alpha$ representa la longitud promedio de las listas enlazadas en cada cubeta.
* Si la función hash es uniforme, la búsqueda promedio requiere evaluar $1 + \alpha / 2$ nodos.
* Para mantener el rendimiento cercano a $O(1)$, se recomienda mantener $\alpha \le 0.75$ o $1.0$.

---

## 2. Declaración de Estructuras (Estilo Oficial FaCENA - UNNE)

Siguiendo las pautas formales de programación de la cátedra:
* Tipos de datos con prefijo `t` (`tDatos`, `tNodoHash`, `tTablaHash`).
* Parámetros de funciones con prefijo `p` (`pTabla`, `pClave`, `pDatos`).
* Ausencia total de tildes y letra `ñ` dentro del código C y sus comentarios.

### 2.1 Archivo Cabecera `tablahash.h` (Interfaz del TDA)

```c
#ifndef TABLAHASH_H
#define TABLAHASH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TAM_TABLA 11
#define MAX_CAD 40

/* Declaracion de tipos de datos personalizados */
typedef char tString[MAX_CAD];

typedef struct {
    int clave;              /* Clave de busqueda unica (ej. DNI o Legajo) */
    tString nombre;         /* Descripcion o dato asociado */
    float promedio;
} tDatos;

/* Nodo de la lista enlazada para encadenamiento dinamico */
typedef struct nodoHash {
    tDatos info;
    struct nodoHash* siguiente;
} tNodoHash;

/* Puntero a nodo para manejar las listas enlazadas */
typedef tNodoHash* tListaHash;

/* Estructura del TDA Tabla Hash (Vector de Cubetas) */
typedef struct {
    tListaHash cubetas[TAM_TABLA];
    int cantidadElementos;
} tTablaHash;

/* Prototipos de las operaciones primitivas del TDA */
void inicializarTabla(tTablaHash* pTabla);
int funcionHash(int pClave);
bool insertarElemento(tTablaHash* pTabla, tDatos pDatos);
bool buscarElemento(tTablaHash pTabla, int pClave, tDatos* pResultado);
bool eliminarElemento(tTablaHash* pTabla, int pClave);
void mostrarTabla(tTablaHash pTabla);
void destruirTabla(tTablaHash* pTabla);

#endif
```

---

## 3. Implementación Paso a Paso de Operaciones Primitivas

### 3.1 Archivo de Implementación `tablahash.c`

#### A. Inicialización del TDA (`inicializarTabla`)
Establece cada puntero de cubeta en `NULL` y reinicia el contador de elementos almacenados.

```c
#include "tablahash.h"

void inicializarTabla(tTablaHash* pTabla) {
    int i;
    for (i = 0; i < TAM_TABLA; i++) {
        pTabla->cubetas[i] = NULL;
    }
    pTabla->cantidadElementos = 0;
}
```

#### B. Función de Dispersión (`funcionHash`)
Aplica la aritmética modular sobre la clave entera ingresada. Devuelve un valor positivo válido en el rango `[0, TAM_TABLA - 1]`.

```c
int funcionHash(int pClave) {
    int indice = pClave % TAM_TABLA;
    if (indice < 0) {
        indice += TAM_TABLA; /* Prevenir indices negativos si la clave fuera negativa */
    }
    return indice;
}
```

#### C. Inserción con Encadenamiento Dinámico (`insertarElemento`)
Calcula la cubeta correspondiente. Si el elemento ya existe (clave duplicada en la lista enlazada de esa cubeta), rechaza la inserción. En caso contrario, crea un nuevo nodo dinámico y lo inserta al principio de la lista de dicha cubeta ($O(1)$ en inserción).

```c
bool insertarElemento(tTablaHash* pTabla, tDatos pDatos) {
    int idx = funcionHash(pDatos.clave);
    tListaHash actual = pTabla->cubetas[idx];

    /* 1. Verificar si la clave ya existe en la lista de la cubeta */
    while (actual != NULL) {
        if (actual->info.clave == pDatos.clave) {
            printf("Error! La clave %d ya existe en la tabla hash.\n", pDatos.clave);
            return false;
        }
        actual = actual->siguiente;
    }

    /* 2. Crear nuevo nodo dinamico en el Heap */
    tListaHash nuevoNodo = (tListaHash) malloc(sizeof(tNodoHash));
    if (nuevoNodo == NULL) {
        printf("Error! Memoria insuficiente en el Heap.\n");
        return false;
    }

    nuevoNodo->info = pDatos;

    /* 3. Insercion al frente de la lista enlazada de la cubeta [idx] */
    nuevoNodo->siguiente = pTabla->cubetas[idx];
    pTabla->cubetas[idx] = nuevoNodo;
    pTabla->cantidadElementos++;

    return true;
}
```

#### D. Búsqueda Directa (`buscarElemento`)
Obtiene la posición en la tabla calculando `funcionHash(pClave)` y recorre únicamente la lista enlazada alojada en esa cubeta específica.

```c
bool buscarElemento(tTablaHash pTabla, int pClave, tDatos* pResultado) {
    int idx = funcionHash(pClave);
    tListaHash actual = pTabla.cubetas[idx];

    while (actual != NULL) {
        if (actual->info.clave == pClave) {
            if (pResultado != NULL) {
                *pResultado = actual->info; /* Copiar datos encontrados */
            }
            return true;
        }
        actual = actual->siguiente;
    }

    return false; /* No encontrado */
}
```

#### E. Eliminación de un Nodo Dinámico (`eliminarElemento`)
Busca el nodo dentro de la lista de la cubeta correspondiente guardando la referencia del nodo anterior para reestructurar los punteros y liberar la memoria con `free()`.

```c
bool eliminarElemento(tTablaHash* pTabla, int pClave) {
    int idx = funcionHash(pClave);
    tListaHash actual = pTabla->cubetas[idx];
    tListaHash anterior = NULL;

    while (actual != NULL && actual->info.clave != pClave) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        printf("No se encontro la clave %d para eliminar.\n", pClave);
        return false;
    }

    /* Desenlazar el nodo de la lista */
    if (anterior == NULL) {
        /* El elemento a eliminar es el primero de la cubeta */
        pTabla->cubetas[idx] = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }

    free(actual);
    pTabla->cantidadElementos--;
    printf("Elemento con clave %d eliminado exitosamente.\n", pClave);

    return true;
}
```

#### F. Visualización Completa de la Tabla (`mostrarTabla`)
Recorre el arreglo de cubetas e imprime la cadena de nodos enlazados en cada una.

```c
void mostrarTabla(tTablaHash pTabla) {
    int i;
    tListaHash actual;

    printf("\n*** ESTADO DE LA TABLA HASH (Factor de Carga: %.2f) ***\n",
           (float) pTabla.cantidadElementos / TAM_TABLA);

    for (i = 0; i < TAM_TABLA; i++) {
        printf("Cubeta [%02d]: ", i);
        actual = pTabla.cubetas[i];
        if (actual == NULL) {
            printf("NULL\n");
        } else {
            while (actual != NULL) {
                printf("[Clave: %d | Nom: %s] -> ", actual->info.clave, actual->info.nombre);
                actual = actual->siguiente;
            }
            printf("NULL\n");
        }
    }
}
```

#### G. Liberación Total de Memoria (`destruirTabla`)
Recorre cada cubeta del vector y libera de forma secuencial cada uno de los nodos dinámicos asignados en el *Heap*.

```c
void destruirTabla(tTablaHash* pTabla) {
    int i;
    tListaHash actual, aux;

    for (i = 0; i < TAM_TABLA; i++) {
        actual = pTabla->cubetas[i];
        while (actual != NULL) {
            aux = actual;
            actual = actual->siguiente;
            free(aux);
        }
        pTabla->cubetas[i] = NULL;
    }
    pTabla->cantidadElementos = 0;
    printf("\nMemoria de la Tabla Hash liberada correctamente.\n");
}
```

---

### 3.2 Programa Principal de Prueba (`main.c`)

```c
#include <stdio.h>
#include "tablahash.h"

int main() {
    tTablaHash miTabla;
    tDatos d1 = {1015, "Juan Perez", 8.50};
    tDatos d2 = {2026, "Maria Gomez", 9.25};
    tDatos d3 = {1004, "Carlos Lopez", 7.10};
    tDatos d4 = {3037, "Ana Martinez", 9.80}; /* Colision a proposito con 1015 si 1015%11 == 3037%11 */
    tDatos hallado;

    inicializarTabla(&miTabla);

    /* Insercion de datos */
    insertarElemento(&miTabla, d1);
    insertarElemento(&miTabla, d2);
    insertarElemento(&miTabla, d3);
    insertarElemento(&miTabla, d4);

    mostrarTabla(miTabla);

    /* Busqueda */
    if (buscarElemento(miTabla, 2026, &hallado)) {
        printf("\nPaciente encontrado: %s | Promedio: %.2f\n", hallado.nombre, hallado.promedio);
    } else {
        printf("\nClave no encontrada.\n");
    }

    /* Eliminacion */
    eliminarElemento(&miTabla, 1015);
    mostrarTabla(miTabla);

    /* Destruccion del TDA */
    destruirTabla(&miTabla);

    return 0;
}
```

---

## 4. Casos Borde y Gestión de Memoria Dinámica

### 4.1 Desbordamiento de Índice (*Index Out of Bounds*)
Un error crítico ocurre si la función hash retorna valores fuera de `[0, TAM_TABLA - 1]`. Ocurre principalmente al trabajar con claves negativas si no se aplica la corrección modular `(k % TAM + TAM) % TAM`.

### 4.2 Pérdida de Puntero de Cabeza de Cubeta
Al insertar nodos al frente de la lista (`nuevoNodo->siguiente = pTabla->cubetas[idx]`), es vital realizar la asignación en ese orden exacto. Si se asigna primero `pTabla->cubetas[idx] = nuevoNodo`, se pierde el puntero a los elementos existentes previamente en la lista, provocando fugas de memoria inalcanzables (*memory leaks*).

### 4.3 Control de Liberación de Memoria en Parciales
A diferencia de los arreglos estáticos donde la memoria se libera automáticamente al salir del scope, cada elemento insertado en encadenamiento dinámico ocupa un bloque en el *Heap* creado mediante `malloc`. El método `destruirTabla` es obligatorio para evitar descalificaciones en los exámenes de la cátedra.

---

## 5. Errores Típicos de Parcial y Diagnóstico de Bugs

### Bug 1: No inicializar el vector de punteros a `NULL`
* **Error:** Declarar la variable `tTablaHash tabla;` y comenzar a insertar elementos directamente sin ejecutar `inicializarTabla`.
* **Consecuencia:** Los punteros del arreglo `cubetas` contienen direcciones basura del Stack. La condición `while (actual != NULL)` fallará o leerá memoria inválida, provocando un *Segmentation Fault*.
* **Corrección:** Invocar siempre a `inicializarTabla(&tabla)` inmediatamente después de crear la variable.

### Bug 2: Confundir `TAM_TABLA` con `cantidadElementos`
* **Error:** Usar `pTabla.cantidadElementos` en el bucle `for` de `mostrarTabla` o `destruirTabla`.
* **Consecuencia:** Si la tabla tiene 2 elementos pero `TAM_TABLA` es 11, las cubetas 2 a 10 no se inicializarán ni limpiarán, dejando memoria colgada o datos sin revisar.
* **Corrección:** Los bucles que recorren el vector de cubetas deben iterar estrictamente desde `0` hasta `TAM_TABLA - 1`.

### Bug 3: Olvidar actualizar la estructura del nodo anterior en la eliminación
* **Error:** Hacer `free(actual)` sin desvincularlo primero de la lista enlazada de la cubeta.
* **Consecuencia:** El elemento anterior seguirá apuntando a la dirección liberada, generando un puntero colgante (*dangling pointer*).
* **Corrección:** Actualizar `pTabla->cubetas[idx] = actual->siguiente` si es el primer nodo, o `anterior->siguiente = actual->siguiente` en cualquier otro caso, antes de invocar a `free(actual)`.

---

## 6. Lista de Verificación de Código (Checklist de Examen)

Antes de entregar el ejercicio práctico o de desarrollo de TDA, verificar:

1. [ ] **Inicialización:** ¿Se implementó y ejecutó `inicializarTabla` asignando `NULL` a cada cubeta `[0..TAM_TABLA-1]`?
2. [ ] **Rango Hash:** ¿La función `funcionHash` garantiza que el valor retornado esté acotado entre `0` y `TAM_TABLA - 1`?
3. [ ] **Verificación de Duplicados:** ¿`insertarElemento` recorre la cubeta antes de insertar para evitar cargar dos veces la misma clave?
4. [ ] **Inserción Dinámica:** ¿Se encadena correctamente el nuevo nodo con `nuevoNodo->siguiente = pTabla->cubetas[idx]`?
5. [ ] **Nombres de Tipos y Parámetros:** ¿Los tipos comienzan con `t` (`tTablaHash`, `tNodoHash`, `tDatos`) y los parámetros con `p` (`pTabla`, `pClave`)?
6. [ ] **Destrucción Completa:** ¿La función `destruirTabla` recorre todas las cubetas y libera cada nodo con un puntero auxiliar antes de pasar al siguiente?
7. [ ] **Sin Caracteres Especiales:** ¿Se verificó la total ausencia de tildes y letras `ñ` en el código escrito en C?

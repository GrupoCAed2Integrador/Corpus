# Pilas Dinámicas en C: Estructura, Operaciones y Aplicaciones

## 1. Fundamentos Teóricos y Modelo LIFO (Last-In, First-Out)

Una **Pila** (*Stack*) es una estructura de datos lineal y dinámica en la que el acceso a los elementos está restringido a un único extremo denominado **cima**, **tope** o **cabeza** (*top*). El comportamiento de una pila se rige estrictamente por la regla **LIFO** (*Last-In, First-Out*), lo que significa que el último elemento que ingresa a la estructura es el primero en ser extraído.

En la implementación dinámica mediante punteros, la pila se construye como una lista simplemente enlazada orientada desde la cima hacia la base. Cada nodo contiene los datos de la entidad a almacenar y un puntero `siguiente` que apunta al nodo que fue insertado inmediatamente antes. El último nodo de la pila (la base) tiene su campo `siguiente` asignado a `NULL`, marcando el fin de la estructura.

```
       [ CIMA / TOPE ]
       ┌─────────────┐
pila ─>│ datos | seg ┼──> ┌─────────────┐
       └─────────────┘    │ datos | seg ┼──> ┌─────────────┐
                          └─────────────┘    │ datos | NULL│  [ BASE ]
                                             └─────────────┘
```

### Comparación de Implementaciones: Estática vs. Dinámica

| Característica | Implementación Estática (Arreglos) | Implementación Dinámica (Punteros) |
| :--- | :--- | :--- |
| **Uso de Memoria** | Fijo en tiempo de compilación. Reservado en Stack o Data segment. | Dinámico en tiempo de ejecución. Asignado en el *Heap*. |
| **Capacidad Máxima** | Acotada por una constante (`MAX`). Corre riesgo de desbordamiento. | Limitada únicamente por la memoria RAM disponible del sistema. |
| **Operaciones** | Requiere la función `pilaLlena()` para validar límites. | No requiere `pilaLlena()`. Solo se verifica `malloc() != NULL`. |
| **Costo de Modificación** | Constante $O(1)$, pero desperdicia espacio si no se llena. | Constante $O(1)$ por inserción y eliminación, sin desperdicio de memoria. |

### Casos de Uso en Sistemas y Aplicaciones

1. **Pila de Llamadas del Sistema (*Call Stack*)**: Gestión de las invocaciones a funciones y retornos en la ejecución de programas en C, almacenando variables locales, parámetros y direcciones de retorno.
2. **Historial de Navegación Web**: Implementación del botón "Atrás" en un navegador, donde cada sitio visitado se apila y al regresar se desapila la página actual.
3. **Mecanismos de Deshacer/Rehacer (*Undo/Redo*)**: Editores de texto y herramientas de diseño almacenan estados de acciones en una pila para permitir revertirlos.
4. **Evaluación de Expresiones y Balanceo de Paréntesis**: Análisis sintáctico en compiladores para verificar la correcta apertura y cierre de llaves, corchetes y paréntesis.

---

## 2. Declaración de Estructuras de Datos según Normativa FaCENA

Para garantizar la compatibilidad con las especificaciones de la cátedra de Algoritmos y Estructuras de Datos II (FaCENA - UNNE), el diseño modular de una pila dinámica requiere dividir la representación en tres componentes fundamentales: los datos del dominio (`tDatos`), el nodo dinámico autorreferenciado (`tNodo`) y el puntero de control de la cima (`tPila`).

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CAD 50

typedef char tString[MAX_CAD];

/* 1. Estructura de información o payload */
typedef struct {
    int id;
    tString descripcion;
    float valor;
} tDatos;

/* 2. Estructura del nodo dinámico autorreferenciado */
typedef struct nodo {
    tDatos data;
    struct nodo *siguiente;
} tPila;

/* 3. Declaración de la variable puntero de control (Cima) */
tPila *pila;
```

> **Nota de Cátedra**: En la convención de FaCENA, la estructura del nodo suele recibir el nombre `tPila` o `tNodoPila`, y la variable representativa de la cima se declara como un puntero simple de tipo `tPila *pila;`.

---

## 3. Implementación Estándar de Operaciones Básicas en C

A continuación se detallan las operaciones primarias para la manipulación de pilas dinámicas, respetando el paso de parámetros por referencia o por valor según corresponda.

### Prototipos Formales de Cátedra

```c
void inicializarPila(tPila **pPila);
bool pilaVacia(tPila *pPila);
void apilar(tPila **pPila, tDatos pDatos);
void desapilar(tPila **pPila);
tDatos cima(tPila *pPila);
void visualizarPila(tPila *pPila);
void liberarPila(tPila **pPila);
```

### 1. Inicialización de la Pila

La inicialización consiste en establecer el puntero a la cima en `NULL`, indicando que la estructura no contiene nodos almacenados.

```c
void inicializarPila(tPila **pPila) {
    *pPila = NULL;
}
```

### 2. Verificación de Pila Vacía

Devuelve `true` si el puntero que representa la cima es igual a `NULL`.

```c
bool pilaVacia(tPila *pPila) {
    return (pPila == NULL);
}
```

### 3. Operación Apilar (*Push*)

Inserta un nuevo elemento en la cima de la pila. El proceso requiere:
1. Solicitar memoria en el *heap* con `malloc(sizeof(tPila))`.
2. Asignar los datos recibidos al campo `data`.
3. Hacer que el campo `siguiente` del nuevo nodo apunte al nodo que actualmente ocupa la cima.
4. Actualizar el puntero de control de la cima para que apunte al nuevo nodo.

```c
void apilar(tPila **pPila, tDatos pDatos) {
    tPila *nuevoNodo = (tPila *) malloc(sizeof(tPila));
    if (nuevoNodo != NULL) {
        nuevoNodo->data = pDatos;
        nuevoNodo->siguiente = *pPila; /* Engancha con la cima actual */
        *pPila = nuevoNodo;            /* El nuevo nodo pasa a ser la cima */
    } else {
        printf("Error: Memoria insuficiente en el Heap para apilar.\n");
    }
}
```

### 4. Operación Desapilar (*Pop*)

Remueve el elemento ubicado en la cima de la pila. El algoritmo exige:
1. Verificar que la pila no esté vacía.
2. Guardar la dirección del nodo de la cima en un puntero auxiliar (`nodoSuprimir`).
3. Avanzar el puntero de la cima hacia el nodo siguiente (`*pPila = (*pPila)->siguiente`).
4. Liberar la memoria del nodo extraído con `free(nodoSuprimir)`.
5. Anular el puntero auxiliar para prevenir un puntero colgante.

```c
void desapilar(tPila **pPila) {
    if (!pilaVacia(*pPila)) {
        tPila *nodoSuprimir = *pPila;
        *pPila = (*pPila)->siguiente; /* Avanza la cima al siguiente */
        free(nodoSuprimir);
        nodoSuprimir = NULL;
    } else {
        printf("Advertencia: La pila esta vacia, no hay elementos para desapilar.\n");
    }
}
```

### 5. Consulta de la Cima (*Top*)

Retorna el contenido de la estructura de datos ubicada en la cima sin modificar ni desapilar la estructura.

```c
tDatos cima(tPila *pPila) {
    tDatos datoVacio = {0, "", 0.0f};
    if (!pilaVacia(pPila)) {
        return pPila->data;
    }
    printf("Advertencia: La pila esta vacia.\n");
    return datoVacio;
}
```

### 6. Visualización y Recorrido No Destructivo

Para recorrer la pila e imprimir sus elementos desde la cima hacia la base sin destruirla, se utiliza un puntero auxiliar que itera sobre la estructura.

```c
void visualizarPila(tPila *pPila) {
    if (pilaVacia(pPila)) {
        printf("La pila esta vacia.\n");
    } else {
        tPila *aux = pPila;
        printf("\n*** CONTENIDO DE LA PILA (CIMA -> BASE) ***\n");
        while (aux != NULL) {
            printf("ID: %d | Descripcion: %s | Valor: $%.2f\n", 
                   aux->data.id, aux->data.descripcion, aux->data.valor);
            aux = aux->siguiente;
        }
    }
}
```

### 7. Vaciado y Liberación de Memoria

Para evitar fugas de memoria al finalizar el programa, se deben desapilar sucesivamente todos los nodos presentes en el *heap*.

```c
void liberarPila(tPila **pPila) {
    tPila *aux;
    while (*pPila != NULL) {
        aux = *pPila;
        *pPila = (*pPila)->siguiente;
        free(aux);
    }
    *pPila = NULL;
}
```

---

## 4. Resolución de Problemas de Nivel Parcial y TP

A continuación se presentan las soluciones completas a dos problemas tomados de exámenes parciales y trabajos prácticos de la cátedra.

### Caso 1: Historial de Navegación Web (Parcial Tema 1 / Tema 2)

**Consigna**:
Un navegador web gestiona las páginas visitadas utilizando una pila dinámica. Cada página almacena: `titulo` (`string`), `url` (`string`), `tiempoPermanencia` (`int` en segundos) y `esFavorita` (`bool`). Se requiere:
1. Declarar las estructuras de datos en C.
2. Implementar `volverPaginaAnterior()` que muestre el título y la URL de la página en la cima, simule la carga y la elimine de la pila.
3. Implementar `mostrarResumenHistorial()` que recorra la pila mostrando el detalle de las páginas, el total de páginas visitadas y cuántas fueron marcadas como favoritas.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char tString[100];

typedef struct {
    tString titulo;
    tString url;
    int tiempoPermanencia;
    bool esFavorita;
} tPaginaWeb;

typedef struct nodoPila {
    tPaginaWeb datos;
    struct nodoPila *siguiente;
} tPilaHistorial;

/* Prototipos de operaciones basicas asumidas */
void inicializarPila(tPilaHistorial **pPila);
bool pilaVacia(tPilaHistorial *pPila);
void apilar(tPilaHistorial **pPila, tPaginaWeb pPagina);
void desapilar(tPilaHistorial **pPila);

/* Item 2: Volver a la pagina anterior */
void volverPaginaAnterior(tPilaHistorial **pPila) {
    if (pilaVacia(*pPila)) {
        printf("No hay paginas anteriores en el historial.\n");
    } else {
        tPaginaWeb actual = (*pPila)->datos;
        printf("Regresando a: %s (%s)...\n", actual.titulo, actual.url);
        desapilar(pPila);
    }
}

/* Item 3: Resumen del historial */
void mostrarResumenHistorial(tPilaHistorial *pPila) {
    int totalPaginas = 0;
    int paginasFavoritas = 0;
    tPilaHistorial *aux = pPila;

    printf("\n================ HISTORIAL DE NAVEGACION ================\n");
    printf("%-30s %-35s %-12s\n", "Titulo", "Direccion URL", "Permanencia");
    printf("---------------------------------------------------------\n");

    while (aux != NULL) {
        totalPaginas++;
        if (aux->datos.esFavorita) {
            paginasFavoritas++;
        }
        printf("%-30s %-35s %d s\n", 
               aux->datos.titulo, aux->datos.url, aux->datos.tiempoPermanencia);
        aux = aux->siguiente;
    }

    printf("---------------------------------------------------------\n");
    printf("Total de paginas en el historial: %d\n", totalPaginas);
    printf("Paginas marcadas como favoritas: %d\n", paginasFavoritas);
    printf("=========================================================\n");
}
```

---

### Caso 2: Historial de Llamadas Telefónicas (Trabajo Práctico 2)

**Consigna**:
Un teléfono móvil registra el historial de llamadas en una pila dinámica conteniendo: `numeroTelefono` (`string`), `duracion` (`int` en segundos) y `tipoLlamada` (`int`: 1-Entrante, 2-Saliente, 3-Perdida). Se solicita implementar:
1. `rellamar()`: Obtener el número de teléfono de la última llamada saliente registrada en la pila sin eliminarlo.
2. `borrarHistorial()`: Eliminar la totalidad de las llamadas registradas en la pila liberando la memoria.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char tTelefono[20];

typedef struct {
    tTelefono numero;
    int duracion;
    int tipoLlamada; /* 1-Entrante, 2-Saliente, 3-Perdida */
} tLlamada;

typedef struct nodoLlamada {
    tLlamada info;
    struct nodoLlamada *siguiente;
} tPilaLlamadas;

/* Funcion Rellamar: busca la ultima llamada saliente sin modificar la pila */
void rellamar(tPilaLlamadas *pPila) {
    tPilaLlamadas *aux = pPila;
    bool encontrada = false;

    while (aux != NULL && !encontrada) {
        if (aux->info.tipoLlamada == 2) { /* 2 = Saliente */
            printf("Rellamando al numero: %s (Duracion anterior: %d s)...\n", 
                   aux->info.numero, aux->info.duracion);
            encontrada = true;
        }
        aux = aux->siguiente;
    }

    if (!encontrada) {
        printf("No se encontraron llamadas salientes recientes en el historial.\n");
    }
}

/* Funcion Borrar Historial: desapila y libera todos los nodos */
void borrarHistorial(tPilaLlamadas **pPila) {
    tPilaLlamadas *aux;
    int eliminados = 0;

    while (*pPila != NULL) {
        aux = *pPila;
        *pPila = (*pPila)->siguiente;
        free(aux);
        eliminados++;
    }

    printf("Historial eliminado correctamente. Se liberaron %d registros.\n", eliminados);
}
```

---

## 5. Diagnóstico de Errores Críticos y Checklist para Exámenes

### Cuadro Diagnóstico de Errores Frecuentes

```
┌────────────────────────────────────────────────────────────────────────┐
│ ERROR 1: MODIFICAR EL PUNTERO DE CIMA DURANTE UN RECORRIDO              │
├────────────────────────────────────────────────────────────────────────┤
│ Causa: Usar pPila = pPila->siguiente en un bucle de impresion.         │
│ Consecuencia: Se desenganchan los nodos y se destruye la pila.         │
│ Solucion: Declarar un puntero auxiliar tPila *aux = pPila; para iterar. │
└────────────────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────────────────┐
│ ERROR 2: INVERSIÓN DE ASIGNACIÓN EN APILAR (PUSH)                      │
├────────────────────────────────────────────────────────────────────────┤
│ Causa: Escribir pila = nuevoNodo; antes de nuevoNodo->siguiente = pila;│
│ Consecuencia: nuevoNodo->siguiente se apunta a si mismo, perdiendo la  │
│              referencia a toda la pila anterior.                       │
│ Solucion: Enganchar primero nuevoNodo->siguiente = *pPila y luego      │
│           actualizar la cima *pPila = nuevoNodo.                       │
└────────────────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────────────────┐
│ ERROR 3: DESAPILAR EN PILA VACÍA SIN VALIDACIÓN                        │
├────────────────────────────────────────────────────────────────────────┤
│ Causa: Intentar acceder a (*pPila)->siguiente cuando *pPila es NULL.   │
│ Consecuencia: Fallo de segmentacion (Segmentation Fault).               │
│ Solucion: Validar siempre if (!pilaVacia(*pPila)) antes de desapilar.  │
└────────────────────────────────────────────────────────────────────────┘
```

### Lista de Comprobación (*Checklist*) para Evaluaciones

- [ ] ¿Incluyó las librerías necesarias (`<stdio.h>`, `<stdlib.h>`, `<stdbool.h>`, `<string.h>`)?
- [ ] ¿Definió el nodo autorreferenciado con el puntero al `siguiente` nodo?
- [ ] ¿Inicializó el puntero a la cima en `NULL` antes de realizar operaciones?
- [ ] En la función `apilar`, ¿verificó que el retorno de `malloc()` sea distinto de `NULL`?
- [ ] En la función `apilar`, ¿asignó `nuevoNodo->siguiente = *pPila` **antes** de actualizar `*pPila`?
- [ ] En la función `desapilar`, ¿validó que la pila no esté vacía antes de desvincular el nodo?
- [ ] En la función `desapilar`, ¿liberó la memoria con `free(nodoSuprimir)` y asignó `nodoSuprimir = NULL`?
- [ ] En las funciones de consulta y recorrido, ¿utilizó un puntero auxiliar para no alterar el puntero de control de la cima?
- [ ] Al finalizar el programa, ¿implementó una rutina para liberar todos los nodos acumulados en el *heap*?

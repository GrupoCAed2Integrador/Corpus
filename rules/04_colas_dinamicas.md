# 04. Colas Dinámicas en C (Estructura FIFO con Punteros)

Las **Colas Dinámicas** constituyen una de las estructuras de datos lineales fundamentales en la asignatura Algoritmos y Estructuras de Datos II (FaCENA - UNNE). A diferencia de las pilas (modelo LIFO), las colas imitan el comportamiento de las filas del mundo real bajo la disciplina **FIFO (*First-In, First-Out*)**, donde el primer elemento en ingresar es estrictamente el primero en ser procesado y removido.

Esta guía integra la metodología oficial de cátedra, las estructuras de control con doble puntero (`principio` y `final`), los algoritmos de manipulación en el *heap*, el análisis formal del desbordamiento y punteros colgantes (*dangling pointers*), y los ejercicios tipificados de parciales y trabajos prácticos.

---

## 1. Fundamentos y Modelo de Control FIFO

Una cola es una lista lineal restringida en la cual todas las inserciones se realizan por un extremo denominado **`final`** (o *trasero* / *back*), mientras que todas las eliminaciones y consultas se efectúan por el extremo opuesto denominado **`principio`** (o *frente* / *front*).

```
          Entrada de datos (Encolar / Push)
                         │
                         ▼
        ┌───────┬───────┬───────┬───────┐
        │ Nodo 1│ Nodo 2│ Nodo 3│ Nodo N│
        └───────┴───────┴───────┴───────┘
            │                       ▲
            ▼                       │
   Salida de datos             Puntero FINAL
(Desencolar / Pop)
  Puntero PRINCIPIO
```

### Comparativa Estructural y Mecánica de Almacenamiento

| Criterio | Pila Dinámica | Lista Enlazada | Cola Dinámica |
| :--- | :--- | :--- | :--- |
| **Disciplina de Acceso** | **LIFO** (*Last-In, First-Out*) | Acceso Posicional Arbitrario | **FIFO** (*First-In, First-Out*) |
| **Punteros de Control** | Un único puntero (`pila` / `tope` / `cima`) | Un único puntero (`cabeza` / `lista`) | **Doble puntero** (`principio` y `final`) |
| **Punto de Inserción** | Exclusivamente en la cima (`tope`) | Al inicio, al final o posición $K$ | Exclusivamente al final (`final`) |
| **Punto de Eliminación** | Exclusivamente en la cima (`tope`) | Al inicio, al final o posición $K$ | Exclusivamente al frente (`principio`) |
| **Uso de Memoria** | Crecimiento dinámico en *Heap* | Crecimiento dinámico en *Heap* | Crecimiento dinámico en *Heap* |

---

## 2. Declaración Formativa de Tipos y Estructuras de Control

Para gestionar una cola dinámica sin límite rígido de capacidad, la cátedra establece una arquitectura de tres niveles de abstracción:

1. **Datos del Dominio (`tDatos`)**: Estructura de contenido de negocio.
2. **Nodo Dinámico (`tNodo`)**: Contenedor autorreferenciado que aloja la información y un puntero hacia el nodo subsiguiente.
3. **Estructura de Control (`tCola`)**: Registro que agrupa los dos punteros de gestión de la fila (`principio` y `final`).

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CAD 30

typedef char tString[MAX_CAD];

// 1. Estructura con la información del elemento (Datos del Dominio)
typedef struct {
    int codigo;
    tString descripcion;
    float precio;
} tDatos;

// 2. Estructura del nodo dinámicamente enlazado
typedef struct nodo {
    tDatos datos;
    struct nodo *siguiente;
} tNodo;

// 3. Estructura de control principal de la cola (Doble Puntero)
typedef struct {
    tNodo *principio; // Puntero al primer nodo (frente)
    tNodo *final;     // Puntero al último nodo (atrás)
} tCola;
```

---

## 3. Operaciones Primitivas y Algoritmos Paso a Paso

### 3.1. Inicialización de la Cola (`inicializarCola`)

La creación de una cola vacía consiste en establecer ambos punteros de control en `NULL`. Al ser una función que modifica la estructura de control, debe recibir un puntero a `tCola`.

```c
void inicializarCola(tCola *pCola) {
    pCola->principio = NULL;
    pCola->final = NULL;
}
```

### 3.2. Verificación de Cola Vacía (`colaVacia`)

Una cola está vacía si y solo si **ambos** punteros (`principio` y `final`) son iguales a `NULL`. La función evalúa el estado y retorna `true` o `false`.

```c
bool colaVacia(tCola pCola) {
    return (pCola.principio == NULL && pCola.final == NULL);
}
```

### 3.3. Encolar / Insertar Elemento (`encolar` / `push`)

La inserción siempre se realiza al final de la cola. El algoritmo debe contemplar dos escenarios bien diferenciados:

1. **Reserva de Memoria**: Se solicita memoria en el *heap* con `malloc(sizeof(tNodo))`.
2. **Asignación de Datos**: Se asigna el registro recibido y se establece `nuevoNodo->siguiente = NULL` (dado que se convertirá en el último nodo).
3. **Evaluación de Escenarios**:
   - **Escenario A (Cola Vacía)**: Si la cola está vacía, el nuevo nodo es simultáneamente el primero y el último. Por ende, tanto `principio` como `final` deben apuntar a `nuevoNodo`.
   - **Escenario B (Cola con Elementos)**: Si la cola ya contiene nodos, el nodo que anteriormente era el último debe enlazar su campo `siguiente` al nuevo nodo (`pCola->final->siguiente = nuevoNodo`), y luego se actualiza el puntero `final` para que apunte al nuevo nodo (`pCola->final = nuevoNodo`).

```c
void encolar(tCola *pCola, tDatos pDatos) {
    // 1. Reserva de memoria para el nuevo nodo
    tNodo *nuevoNodo = (tNodo *) malloc(sizeof(tNodo));
    
    if (nuevoNodo == NULL) {
        printf("Error: Memoria insuficiente en el Heap.\n");
        return;
    }
    
    // 2. Carga de información y configuración de enlace terminal
    nuevoNodo->datos = pDatos;
    nuevoNodo->siguiente = NULL;
    
    // 3. Inserción según el estado previo de la cola
    if (colaVacia(*pCola)) {
        // Escenario A: Primera inserción en cola vacía
        pCola->principio = nuevoNodo;
        pCola->final = nuevoNodo;
    } else {
        // Escenario B: Inserción al final de cola con elementos
        pCola->final->siguiente = nuevoNodo;
        pCola->final = nuevoNodo;
    }
}
```

### 3.4. Desencolar / Quitar Elemento (`desencolar` / `pop`)

La remoción se realiza siempre en el frente (`principio`) de la cola. Procedimiento de cuatro pasos:

1. **Verificación de Seguridad**: Confirmar que la cola no esté vacía mediante `!colaVacia(*pCola)`.
2. **Resguardo del Nodo**: Guardar la dirección de memoria del primer nodo en un puntero auxiliar (`tNodo *nodoSuprimir = pCola->principio;`).
3. **Desvinculación y Actualización de Control**:
   - **Escenario A (Cola Unitaria - `principio == final`)**: Si solo había un nodo, la eliminación dejará la cola vacía. Se reinicializa llamando a `inicializarCola(pCola)` o asignando `NULL` a ambos punteros.
   - **Escenario B (Cola con Múltiples Nodos)**: El segundo nodo pasa a ser el frente (`pCola->principio = nodoSuprimir->siguiente;`).
4. **Liberación de Memoria**: Liberar el bloque resguardado con `free(nodoSuprimir)` y anular el puntero (`nodoSuprimir = NULL`).

```c
void desencolar(tCola *pCola) {
    // 1. Verificar si la cola contiene elementos
    if (colaVacia(*pCola)) {
        printf("Advertencia: La cola esta vacia. No hay elementos para desencolar.\n");
        return;
    }
    
    // 2. Resguardar el nodo del frente
    tNodo *nodoSuprimir = pCola->principio;
    
    // 3. Actualizar punteros de control según cantidad de nodos
    if (pCola->principio == pCola->final) {
        // Escenario A: Era el único elemento de la cola
        inicializarCola(pCola);
    } else {
        // Escenario B: Había más de un elemento
        pCola->principio = nodoSuprimir->siguiente;
    }
    
    // 4. Liberar memoria dinámica
    free(nodoSuprimir);
    nodoSuprimir = NULL;
}
```

### 3.5. Consulta del Primer Elemento (`primerElemento`)

Permite inspeccionar la información del frente sin alterar la cola. Puede implementarse retornando un puntero al primer nodo o retornando la estructura de datos:

```c
// Opción A: Retorna el puntero al primer nodo (o NULL si está vacía)
tNodo * primerNodo(tCola pCola) {
    return pCola.principio;
}

// Opción B: Retorna los datos del primer elemento
tDatos primerElemento(tCola pCola) {
    tDatos datoVacio = {0, "", 0.0f};
    if (!colaVacia(pCola)) {
        return pCola.principio->datos;
    }
    return datoVacio;
}
```

### 3.6. Recorrido Transversal No Destructivo (`visualizarCola`)

Para recorrer una cola sin vaciarla ni destruirla, se utiliza un **puntero auxiliar** inicializado en `pCola.principio` que itera mientras `aux != NULL`.

```c
void visualizarCola(tCola pCola) {
    if (colaVacia(pCola)) {
        printf("Cola vacia. Sin elementos para mostrar.\n");
        return;
    }
    
    tNodo *aux = pCola.principio;
    printf("\n*** ELEMENTOS EN LA COLA (Frente -> Final) ***\n");
    
    while (aux != NULL) {
        printf("Codigo: %d | Descripcion: %s | Precio: $%.2f\n",
               aux->datos.codigo, aux->datos.descripcion, aux->datos.precio);
        aux = aux->siguiente; // Avanzar al siguiente nodo
    }
    printf("--------------------------------------------------\n");
}
```

---

## 4. Casos de Frontera, Errores Críticos y Manejo de Punteros Colgantes

### El Peligro del Puntero Colgante en Cola Unitaria

Un fallo sumamente recurrente en parciales ocurre al desencolar una cola que contiene un único elemento (donde `principio == final`).

Si el código realiza lo siguiente:
```c
// ❌ CÓDIGO INCORRECTO EN COLA UNITARIA
pCola->principio = nodoSuprimir->siguiente; // principio pasa a ser NULL
free(nodoSuprimir);                         // Se libera la memoria
// ¡SE OLVIDÓ ACTUALIZAR pCola->final!
```

**Consecuencias Técnicas**:
1. `pCola->principio` vale `NULL`, pero `pCola->final` conserva la dirección de memoria que **ya fue liberada** (un *dangling pointer*).
2. La función `colaVacia(*pCola)` verifica `(principio == NULL && final == NULL)`. Como `final != NULL`, devuelve `false` (cree falsamente que la cola contiene elementos).
3. En la siguiente llamada a `encolar()`, el programa evalúa `if (colaVacia)` como falso y ejecuta:
   ```c
   pCola->final->siguiente = nuevoNodo; // 💥 SEGMENTATION FAULT
   ```
   Intenta desreferenciar `final` para escribir en su campo `siguiente`, accediendo a una región de memoria no autorizada y provocando el colapso del programa.

---

## 5. Ejercicios Resueltos Integradores de Parcial y TP

### Ejercicio 1: Sistema de Delivery (Parcial Tema 2)

Una aplicación de *delivery* gestiona una cola con los pedidos pendientes de asignación.
Cada pedido contiene:
- `numeroPedido` (`int`)
- `nombreComercio` (`tString`)
- `montoTotal` (`float`)
- `zonaEntrega` (`int`: 1-Centro, 2-Norte, 3-Sur, 4-Costanera)
- `pagadoAdelantado` (`bool`)

#### Código Fuente Completo de la Solución:

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CAD 50

typedef char tString[MAX_CAD];

// Declaración de estructuras de datos
typedef struct {
    int numeroPedido;
    tString nombreComercio;
    float montoTotal;
    int zonaEntrega;
    bool pagadoAdelantado;
} tPedido;

typedef struct nodoPedido {
    tPedido pedido;
    struct nodoPedido *siguiente;
} tNodoPedido;

typedef struct {
    tNodoPedido *principio;
    tNodoPedido *final;
} tColaPedidos;

// Prototipos de operaciones básicas asumidas
void inicializarCola(tColaPedidos *pCola);
bool colaVacia(tColaPedidos pCola);
tPedido primerPedido(tColaPedidos pCola);
void quitarPedido(tColaPedidos *pCola);

// Item B: Asignación del próximo pedido
void asignarProximoPedido(tColaPedidos *pCola) {
    if (colaVacia(*pCola)) {
        printf("No hay pedidos pendientes de asignacion.\n");
    } else {
        tPedido p = primerPedido(*pCola);
        printf("Asignando Pedido Nro: %d | Comercio: %s\n", p.numeroPedido, p.nombreComercio);
        printf("Simulando asignacion de pedido al repartidor...\n");
        quitarPedido(pCola);
    }
}

// Item C: Resumen consolidado de pedidos pendientes
void mostrarResumenPedidos(tColaPedidos pCola) {
    float montoTotalAcumulado = 0.0f;
    int totalPedidos = 0;
    
    int cantPagadosAdelantado = 0;
    float montoPagadosAdelantado = 0.0f;
    
    int cantAPagarEntrega = 0;
    float montoAPagarEntrega = 0.0f;
    
    tNodoPedido *aux = pCola.principio;
    
    while (aux != NULL) {
        totalPedidos++;
        montoTotalAcumulado += aux->pedido.montoTotal;
        
        if (aux->pedido.pagadoAdelantado) {
            cantPagadosAdelantado++;
            montoPagadosAdelantado += aux->pedido.montoTotal;
        } else {
            cantAPagarEntrega++;
            montoAPagarEntrega += aux->pedido.montoTotal;
        }
        
        aux = aux->siguiente;
    }
    
    float montoPromedio = (totalPedidos > 0) ? (montoTotalAcumulado / totalPedidos) : 0.0f;
    
    printf("\nResumen de pedidos pendientes\n");
    printf("--------------------------------------------------\n");
    printf("Monto total acumulado: $ %.2f (corresponde a %d pedidos pendientes)\n",
           montoTotalAcumulado, totalPedidos);
    printf("Monto promedio por pedido: $ %.2f\n", montoPromedio);
    printf("--------------------------------------------------\n");
    printf("Pagados por adelantado: %d - Total: $ %.2f\n",
           cantPagadosAdelantado, montoPagadosAdelantado);
    printf("A pagar en la entrega: %d - Total: $ %.2f\n",
           cantAPagarEntrega, montoAPagarEntrega);
}
```

---

### Ejercicio 2: Asignación de Memoria a Procesos (Modelo de Parcial)

Un sistema operativo administra la asignación de memoria para procesos en cola de espera.

```c
typedef struct {
    tString idProceso;
    float tamanioMB;
    int tipoProceso; // 1-sistema, 2-usuario
} tDatosProcesoEspera;

typedef struct nodoProceso {
    tDatosProcesoEspera proceso;
    struct nodoProceso *siguiente;
} tNodoProcesoEspera;

typedef struct {
    tNodoProcesoEspera *principio;
    tNodoProcesoEspera *final;
} tColaProcesosEspera;

// Función para asignar memoria al próximo proceso en espera
void asignarMemoriaAProceso(tColaProcesosEspera *pColaProcesos) {
    if (colaVaciaProcesos(*pColaProcesos)) {
        printf("\nNo hay procesos en cola de espera de asignacion de memoria!\n");
    } else {
        tDatosProcesoEspera proc = primerProceso(*pColaProcesos);
        printf("\nProceso por asignar memoria:\n\tID: %s\n\tTamanio: %.2f MB\n\tTipo: %s\n",
               proc.idProceso, proc.tamanioMB,
               (proc.tipoProceso == 1) ? "sistema" : "usuario");
        
        printf("\nAsignando memoria al proceso %s ...\n", proc.idProceso);
        desencolarProceso(pColaProcesos);
    }
}

// Función para visualizar el detalle tabular de procesos
void visualizarDetalleProcesos(tColaProcesosEspera pColaProcesos) {
    printf("\n*** Procesos en espera ***\n");
    printf("ID del Proceso\tTamaño de Memoria (MB)\tTipo de Proceso\n");
    printf("--------------------------------------------------\n");
    
    int totalProcesos = 0;
    tNodoProcesoEspera *aux = pColaProcesos.principio;
    
    while (aux != NULL) {
        totalProcesos++;
        printf("%s\t\t%.2f\t\t\t%s\n",
               aux->proceso.idProceso,
               aux->proceso.tamanioMB,
               (aux->proceso.tipoProceso == 1) ? "Sistema" : "Usuario");
        aux = aux->siguiente;
    }
    
    printf("\nTotal de procesos en espera: %d\n", totalProcesos);
}
```

---

## 6. Lista de Verificación de Código para Evaluaciones

| Aspecto Evaluado | Requisito de Cátedra FaCENA - UNNE | Estado |
| :--- | :--- | :---: |
| **Punteros de Control** | ¿La cola utiliza la estructura `tCola` con los punteros `principio` y `final`? |  |
| **Paso de Parámetros** | ¿`encolar`, `desencolar` e `inicializar` reciben `tCola *pCola` (referencia)? |  |
| **Consultas de Lectura** | ¿`colaVacia`, `visualizar` y reportes reciben `tCola pCola` (valor)? |  |
| **Cola Unitaria** | ¿`desencolar` verifica si `principio == final` para resetear ambos a `NULL`? |  |
| **Asignación `malloc`** | ¿Se verifica la reserva con `malloc(sizeof(tNodo))` antes de usar? |  |
| **Liberación `free`** | ¿Se realiza `free(nodoSuprimir)` resguardando previamente el frente? |  |
| **División por Cero** | ¿Los cálculos de promedio verifican que la cantidad total de elementos sea $> 0$? |  |

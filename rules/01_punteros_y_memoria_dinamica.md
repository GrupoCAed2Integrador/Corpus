# Manual Técnico de Punteros y Memoria Dinámica en C

Este documento establece los principios teóricos, reglas operativas y convenciones de codificación para el manejo de **Punteros y Memoria Dinámica** en lenguaje C, conforme a los contenidos de la asignatura **Algoritmos y Estructuras de Datos II** (FaCENA - UNNE). Su propósito es servir como guía de referencia estandarizada para la resolución de trabajos prácticos, exámenes parciales y el desarrollo del Proyecto Integrador.

---

## 1. Fundamentos Físicos y Semántica de Punteros

En el modelo de computación en C, cada variable declarada en un programa posee cuatro atributos fundamentales asociados en el sistema de memoria RAM:

1. **Nombre o Identificador**: Etiqueta utilizada en el código fuente para referirse al objeto.
2. **Tipo de Dato**: Determina la cantidad de bytes que ocupa en memoria y la interpretación de sus bits.
3. **Dirección de Memoria**: Ubicación física o lógica única en la memoria RAM donde reside el valor (representada habitualmente en formato hexadecimal mediante el especificador `%p`).
4. **Valor o Contenido**: El dato binario almacenado dentro de la celda de memoria.

```c
// Declaración estándar de una variable escalar
int edad = 25;
```

Mientras que una variable ordinaria almacena un valor de dato directo (un entero, un caracter o un flotante), una **variable puntero** (o apuntador) es una variable cuyo valor es explícitamente la **dirección de memoria de otra variable**.

```c
// Declaración e inicialización de un puntero estático
int *apuntaEdad = &edad;
```

Como establece la bibliografía de la cátedra, la variable puntero y la variable a la que apunta son dos entidades conceptual y físicamente independientes almacenadas en direcciones distintas. El puntero contiene la dirección donde reside la variable referenciada, permitiendo acceder y manipular su contenido de forma indirecta.

---

## 2. Operadores Fundamentales: `&`, `*` y `->`

El lenguaje C proporciona tres operadores unarios primarios para trabajar con punteros y estructuras:

### Operador de Dirección (`&`)
Devuelve la dirección de memoria de un objeto existente en el programa. Solo puede aplicarse a elementos con ubicación física en memoria (variables escalares, arreglos y campos de estructuras); no puede aplicarse a constantes literales ni a expresiones numéricas.

```c
int x = 10;
int *px = &x; // px almacena la dirección de memoria de la variable x
```

### Operador de Indirección o Desreferencia (`*`)
Aplicado a una variable puntero inicializada, da acceso directo a la celda de memoria señalada por dicho puntero, permitiendo leer o modificar el valor almacenado en ella.

```c
int x = 10;
int *px = &x;

*px = 20; // Modifica el valor de x a 20 a través del puntero
printf("Nuevo valor de x: %d\n", x); // Imprime 20
```

### Operador de Selección de Componente (`->`)
Cuando un puntero referencia a una estructura (`struct`), el acceso a sus campos mediante el operador de indirección requiere paréntesis debido a la precedencia del operador punto (`(*p).campo`). Para simplificar esta sintaxis, C introduce el operador de selección simple `->`, que desreferencia el puntero y accede directamente al miembro deseado.

```c
typedef struct {
    int codigo;
    float precio;
} tProducto;

tProducto prod = {101, 1500.50};
tProducto *pProd = &prod;

// Sintaxis equivalente:
(*pProd).codigo = 102;
pProd->precio = 1750.00; // Sintaxis preferida y obligatoria por la cátedra
```

---

## 3. Arquitectura de Memoria: Stack vs. Heap

La memoria asignada a un programa en ejecución se divide estructuralmente en dos regiones principales con comportamientos diferenciados:

| Característica | Pila de Llamadas (*Stack*) | Almacén Libre / Montículo (*Heap*) |
| :--- | :--- | :--- |
| **Tiempo de Asignación** | Compilación / Entrada a función | Tiempo de ejecución (*Runtime*) |
| **Duración del Ciclo de Vida** | Automático (se destruye al salir del ámbito) | Manual (permanece hasta ser liberado con `free()`) |
| **Gestión de Tamaño** | Estático y fijo | Dinámico (ampliable y contraíble) |
| **Velocidad de Acceso** | Muy rápida | Ligera latencia por gestión de punteros |
| **Mecanismo en C** | Declaración directa de variables | Funciones `malloc()` y `free()` |

En las estructuras de datos dinámicas (como listas, pilas y colas dinámicas), el espacio para los nuevos nodos se reserva en el *Heap*, manteniendo únicamente los punteros de control en la memoria local del *Stack*.

---

## 4. Asignación Dinámica de Memoria con `malloc()`, `sizeof()` y `NULL`

Para reservar memoria dinámicamente en el *Heap* se utiliza la función `malloc()` declarada en el archivo de cabecera `<stdlib.h>`.

### La Función `malloc()`
Recibe como argumento el número exacto de bytes que se desean reservar. Retorna un puntero genérico de tipo `void*` con la dirección inicial del bloque asignado, o `NULL` si el sistema no dispone de memoria suficiente.

### El Operador `sizeof()`
Debido a que el tamaño en bytes de los tipos de datos varía según la arquitectura de la computadora, se debe utilizar siempre el operador `sizeof` para calcular el tamaño requerido de manera portable.

### Sintaxis Estándar de Asignación
En la asignatura es obligatorio realizar la conversión explícita (*casting*) del tipo de puntero y verificar inmediatamente si la asignación fue exitosa:

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    char descripcion[30];
} tDatos;

typedef struct nodo {
    tDatos datos;
    struct nodo *siguiente;
} tNodo;

void ejemploAsignacion() {
    tNodo *nuevoNodo;

    // 1. Reserva dinámica de memoria con casting explícito
    nuevoNodo = (tNodo *) malloc(sizeof(tNodo));

    // 2. Control obligatorio de memoria disponible
    if (nuevoNodo == NULL) {
        printf("Error: No queda memoria disponible en el Heap.\n");
        return;
    }

    // 3. Inicialización del nodo reservado
    nuevoNodo->datos.id = 1;
    nuevoNodo->siguiente = NULL;
}
```

---

## 5. Liberación Formal de Memoria con `free()`

Toda memoria reservada en el *Heap* mediante `malloc()` permanece ocupada hasta que el programa finalice o sea liberada explícitamente. La función `free()` devuelve el bloque de memoria al almacenamiento libre para que pueda ser reutilizado por el sistema.

### Regla del Ciclo de Vida Completo
Por cada llamada a `malloc()` debe existir una llamada correspondiente a `free()`.

```c
void liberarNodo(tNodo *pNodo) {
    if (pNodo != NULL) {
        free(pNodo); // Libera el bloque de memoria apuntado
        pNodo = NULL; // Buena práctica: anula el puntero local para evitar referencias colgantes
    }
}
```

---

## 6. Diagnóstico de Errores Críticos de Memoria en C

El trabajo con punteros requiere disciplina para evitar fallos graves de ejecución. A continuación se detallan los cuatro errores más frecuentes evaluados en los exámenes:

### A. Uso de Puntero No Inicializado (*Wild Pointer*)
Ocurre al intentar desreferenciar un puntero que contiene basura de memoria por no haber sido asignado ni con `&` ni con `malloc()`.

```c
// ERROR CRÍTICO: p contiene una dirección basura aleatoria
int *p;
*p = 10; // Provoca fallo de segmentación (Segmentation Fault)
```

### B. Fuga de Memoria (*Memory Leak*)
Ocurre cuando se reasigna un puntero a una nueva dirección sin haber liberado previamente la memoria apuntada, perdiendo la referencia al bloque original.

```c
int *p = (int *) malloc(sizeof(int));
*p = 10;

// ERROR: Se pierde la dirección del primer bloque de memoria
p = (int *) malloc(sizeof(int));
*p = 20;
free(p); // Solo libera el segundo bloque; el primero quedó inaccesible en el Heap
```

### C. Puntero Colgante (*Dangling Pointer*)
Ocurre al acceder al contenido de un puntero cuya memoria ya fue liberada mediante `free()`.

```c
int *p = (int *) malloc(sizeof(int));
*p = 42;
free(p);

// ERROR: Acceso a memoria ya devuelta al sistema
printf("%d\n", *p); // Comportamiento indefinido / Corrupción de datos
```

### D. Doble Liberación (*Double Free*)
Ocurre al invocar `free()` dos veces sobre la misma dirección de memoria o sobre dos punteros distintos que apuntan al mismo bloque.

```c
int *a = (int *) malloc(sizeof(int));
int *b = a; // Ambos punteros comparten la misma dirección

free(a);
free(b); // ERROR CRÍTICO: Doble liberación (Double Free Corruption)
```

---

## 7. Lista de Verificación y Buenas Prácticas de Cátedra

Para garantizar la aprobación de los módulos de código y exámenes parciales, la solución debe cumplir con la siguiente lista de verificación:

- **Inicialización**: Todo puntero se inicializa al declararlo con `NULL` o con una dirección válida.
- **Casting Explícito**: Las llamadas a `malloc()` incluyen el *casting* correspondiente (ej. `(tNodo *) malloc(sizeof(tNodo))`).
- **Validación `NULL`**: Se valida la disponibilidad de memoria inmediatamente después de cada `malloc()`.
- **Operador `->`**: Se utiliza el operador flecha `->` para acceder a miembros de estructuras referenciadas por punteros.
- **Anulación Post-Free**: Tras invocar `free(p)`, se asigna `p = NULL` para evitar punteros colgantes.
- **Paridad `malloc` / `free`**: Se garantiza que todo nodo o bloque reservado dinámicamente sea liberado antes de destruir los punteros de control.

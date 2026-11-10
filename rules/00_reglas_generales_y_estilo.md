# Normas y Convenciones Generales de Programación en C
## Algoritmos y Estructuras de Datos II — FaCENA (UNNE)

Este documento constituye la guía normativa fundamental para el desarrollo de proyectos y evaluaciones en la cátedra de **Algoritmos y Estructuras de Datos II** de la Facultad de Ciencias Exactas y Naturales y Agrimensura (FaCENA - UNNE). Define las librerías estándar permitidas, los patrones de tipado mediante `typedef`, la estructura modular basada en la estrategia Top-Down, los mecanismos formales de paso de parámetros y la gestión segura de memoria dinámica en el *heap*.

---

## 1. Directivas del Preprocesador: Librerías y Constantes

Todo programa desarrollado bajo los lineamientos de la cátedra debe incluir explícitamente las cabeceras estándar correspondientes a las funciones y tipos de datos utilizados, evitando asumir inclusiones implícitas.

### Librerías Estándar Permitidas y sus Usos

* **`<stdio.h>`**: Cabecera fundamental para operaciones de entrada y salida estándar. Proporciona funciones de lectura y escritura por consola como `printf()` y `scanf()`.
* **`<stdlib.h>`**: Requerida para la gestión dinámica de memoria a través de `malloc()` y `free()`, además de la constante simbólica `NULL`.
* **`<stdbool.h>`**: Proporciona el tipo de dato booleano nativo `bool` y sus literales `true` y `false`, indispensables para validaciones de estado en estructuras de datos (`listaVacia`, `pilaVacia`, `colaVacia`).
* **`<string.h>`**: Necesaria para la manipulación y copia segura de cadenas de caracteres mediante `strcpy()`, `strcmp()` y `strlen()`.
* **`<math.h>`**: Empleada en ejercicios específicos que requieren operaciones matemáticas avanzadas como `pow()` o `fmod()`.

### Definición de Constantes con `#define`

Las dimensiones máximas de arreglos y la longitud fija de cadenas de caracteres deben parametrizarse mediante la directiva `#define` al inicio del archivo, utilizando nombres en mayúsculas sostenidas:

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CAD 25
#define Maximo 5
```

---

## 2. Tipado de Datos y Nomenclatura de Cátedra (`typedef`)

Para garantizar la abstracción de datos y facilitar la modificación del tipo de elementos sin reescribir la lógica del código, se exige el uso sistemático de `typedef`.

### Convención del Prefijo `t`

Todos los tipos de datos personalizados definidos por el programador deben iniciar obligatoriamente con la letra minúscula `t` (por ejemplo: `tString`, `tDatos`, `tNodo`, `tLista`, `tPila`, `tCola`, `tArbol`).

### Definición de Cadenas y Registros de Dominio

1. **Cadenas de caracteres**: Se abstraen definiendo un tipo `tString` basado en un arreglo unidimensional de tipo `char` parametrizado con la constante de longitud:
   ```c
   typedef char tString[MAX_CAD];
   ```
2. **Registros de información de dominio**: Los datos que transporta cada nodo o posición de una estructura se agrupan en una estructura `struct` anonimizada con `typedef`. Dependiendo del contexto del problema (pedidos de delivery, vehículos de taller, clientes), se estructuran bajo identificadores como `tDatos`, `tPedido`, `tVehiculo` o `tPaquete`:
   ```c
   typedef struct {
       int codigo;
       tString descripcion;
   } tDatos;
   ```

### Estructuras Contenedoras y Nodos Dinámicos

La declaración de nodos dinámicos requiere referenciarse a sí misma mediante punteros al siguiente (o hijos izquierdo/derecho), por lo que se utiliza la etiqueta de estructura `struct nodo`:

```c
// Nodo para Lista Enlazada o Pila Dinámica
typedef struct nodo {
    tDatos datos;
    struct nodo *siguiente;
} tNodo;

// Estructura de Control para Cola Dinámica (Doble Puntero)
typedef struct {
    tNodo *principio;
    tNodo *final;
} tCola;
```

---

## 3. Programación Modular y Estrategia Top-Down

El diseño de software en la asignatura sigue estrictamente el paradigma de la **Programación Estructurada y Modular**.

### Estrategia Top-Down (De lo General a lo Particular)

Un problema complejo se descompone en subproblemas mediante módulos o subprogramas funcionalmente independientes.
* **El módulo `main()`**: Debe limitarse exclusivamente a coordinar el flujo general de ejecución, controlar menús o invocar a los subprogramas. Se prohíbe concentrar la lógica de procesamiento en `main()`.
* **Principio de Autonomía**: Cada subprograma debe comunicarse con el exterior **únicamente** a través de sus parámetros formales y su valor de retorno, evitando interferencias del entorno o modificación indeseada de datos globales.
* **Principio de Máxima Localidad**: Todo objeto o variable requerida por un subprograma debe declararse localmente dentro de su propio ámbito. Se debe **evitar el uso de variables globales**, previniendo efectos laterales negativos.

### Prototipos de Funciones

Todos los subprogramas deben declarar su **prototipo** antes de la función `main()`. El prototipo informa al compilador sobre la firma de la función (tipo de retorno, nombre y lista de parámetros con sus tipos de datos):

```c
// Declaración de prototipos al inicio del archivo
void inicializarCola(tCola *pCola);
bool colaVacia(tCola pCola);
void push(tCola *pCola, tDatos pDatos);
void pop(tCola *pCola);

int main() {
    // Invocación modular
    return 0;
}

// Desarrollo / Definición de las funciones debajo de main()
void inicializarCola(tCola *pCola) {
    pCola->principio = NULL;
    pCola->final = NULL;
}
```

---

## 4. Mecanismos de Paso de Parámetros

El intercambio de información entre el programa principal y los subprogramas se efectúa mediante parámetros formales (en la cabecera del subprograma) y argumentos reales (en la llamada).

| Criterio | Paso por Valor | Paso por Referencia |
| :--- | :--- | :--- |
| **Mecanismo** | Se copia el valor del argumento real en la variable formal local. | Se pasa la dirección de memoria mediante el operador puntero (`*`) en la declaración y el operador de dirección (`&`) en la llamada. |
| **Efecto en el Origen** | Las modificaciones dentro del subprograma **no afectan** la variable original. | Las modificaciones dentro del subprograma **persisten** en la variable original tras finalizar la ejecución. |
| **Uso en Cátedra** | Subprogramas de consulta, visualización, cálculo de resúmenes o comprobación de estado (`colaVacia`, `visualizarElementos`, `sumar`). | Subprogramas que deben modificar el estado de la estructura o variable (`inicializarCola`, `push`, `pop`, `eliminarNodo`). |

### Regla para Estructuras Complejas
Cuando una función de consulta recibe una estructura completa por valor (por ejemplo `tCola pCola`), C copia el valor de sus campos (punteros `principio` y `final`). Esto permite iterar y recorrer la estructura con un puntero auxiliar sin alterar los punteros originales en el ámbito llamador.

```c
// Ejemplo de consulta por valor
void visualizarCola(tCola pCola) {
    tNodo *aux = pCola.principio; // Recorrido seguro mediante aux
    while (aux != NULL) {
        printf("%d | %s\n", aux->datos.codigo, aux->datos.descripcion);
        aux = aux->siguiente;
    }
}
```

---

## 5. Gestión Segura de Memoria Dinámica y Punteros

A diferencia de las estructuras estáticas reservadas en el *stack*, las estructuras dinámicas asignan y liberan memoria en el *heap* durante la ejecución del programa.

### Reglas de Oro en el Uso de Punteros

1. **Inicialización Explícita**: Todo puntero debe inicializarse al declararse con una dirección válida o con la constante `NULL`. Un puntero no inicializado contiene basura y provoca fallos de segmentación.
2. **Reserva Dinámica con Casting**: La asignación de memoria se realiza con la función `malloc()`, aplicando el casting explícito al tipo de puntero correspondiente y calculando el tamaño en bytes con `sizeof()`:
   ```c
   tNodo *nuevoNodo = (tNodo *) malloc(sizeof(tNodo));
   ```
3. **Verificación de Memoria Disponible**: Tras invocar `malloc()`, se debe verificar que el puntero devuelto no sea `NULL` antes de intentar acceder a sus campos:
   ```c
   if (nuevoNodo != NULL) {
       nuevoNodo->datos = pDatos;
       nuevoNodo->siguiente = NULL;
   } else {
       printf("Error: No queda memoria disponible en el Heap.\n");
   }
   ```
4. **Liberación Estricta de Memoria (`free`)**: Cada bloque de memoria asignado con `malloc()` debe ser liberado explícitamente mediante `free()` en cuanto deje de ser necesario:
   ```c
   free(nodoSuprimir);
   ```
5. **Anulación del Puntero Colgante (*Dangling Pointer*)**: Inmediatamente después de ejecutar `free(p)`, se debe asignar `p = NULL;`. Esto evita que la variable conserve una dirección hacia memoria ya liberada, previniendo errores graves de corrupción de memoria o doble liberación (*double free*).
6. **Ecuación Balanceada**: Por cada llamada a `malloc()`, debe existir exactamente una llamada a `free()` correspondiente durante el ciclo de vida del programa.

```c
// Patrón canónico de eliminación de un nodo dinámico
tNodo *nodoSuprimir = pCola->principio;
pCola->principio = nodoSuprimir->siguiente;

free(nodoSuprimir);     // Libera el bloque en el Heap
nodoSuprimir = NULL;    // Anula el puntero descolgado
```

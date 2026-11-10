## 1. Marco Teórico y Fundamentos

### 1.1 Definición Matemática y Conceptual
Un **Grafo** es una estructura de datos no lineal y no jerárquica definida como un par ordenado $G = (V, A)$, donde:
* **$V$ (Vértices o Nodos):** Conjunto finito y no vacío de elementos que representan entidades abstractas o reales (ej. ciudades, computadoras, usuarios, conceptos).
* **$A$ (Arcos o Aristas):** Conjunto de pares de vértices $(u, v)$ que representan las relaciones, conexiones o enlaces existentes entre dichas entidades.

A diferencia de los árboles binarios, en los grafos se eliminan las restricciones de jerarquía: un nodo puede estar conectado con cualquier número de nodos, pueden existir ciclos y no existe el concepto de nodo raíz.

```
      [0] ------ (5) ------ [1]
       |                     |
      (2)                   (8)
       |                     |
      [3] ------------------ [2]
```

### 1.2 Clasificación Formal de Grafos
1. **Grafo Dirigido (Digrafo):** Los pares de vértices en $A$ son ordenados. El arco $(u, v)$ indica un sentido único desde el vértice $u$ (origen) hacia el vértice $v$ (destino).
2. **Grafo No Dirigido:** Las conexiones son simétricas y bilaterales. La arista $\{u, v\}$ une ambos vértices sin sentido prioritario, pudiendo recorrerse de $u$ a $v$ o de $v$ a $u$.
3. **Grafo Ponderado (o con Peso):**
   * **Peso Simple:** Cada arco/arista tiene asociado un valor numérico (costo, distancia, tiempo, capacidad).
   * **Peso Compuesto:** El costo asociado es un registro (`struct`) con múltiples atributos (ej. distancia en km y costo de peajes).
4. **Grafo Completo:** Aquel donde existe un arco/arista entre todo par de vértices. Si el orden del grafo es $n = |V|$:
   * En grafos no dirigidos contiene $\frac{n(n-1)}{2}$ aristas.
   * En grafos dirigidos contiene $n(n-1)$ arcos.
5. **Grado de un Vértice:**
   * En grafos no dirigidos: Cantidad de aristas incidentes en el vértice.
   * En grafos dirigidos: Se divide en **Grado de Entrada** (arcos que llegan al vértice) y **Grado de Salida** (arcos que nacen del vértice).

---

## 2. Declaración de Estructuras (Estilo Oficial FaCENA - UNNE)

La cátedra propone dos representaciones fundamentales en lenguaje C: la **representación estática mediante arreglos (matrices)** y la **representación dinámica mediante listas de adyacencia**.

### 2.1 Representación con Arreglos (Matriz de Adyacencia / Pesos)
Es la representación estándar para los parciales escritos y trabajos prácticos de la asignatura. Utiliza un vector de booleanos para el estado de los vértices y una matriz de $N \times N$ para las conexiones.

#### A. Grafo No Ponderado (Matriz Booleana de Adyacencia)
```c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 10

typedef int tVertice;

typedef struct {
    tVertice origen;
    tVertice destino;
} tArco;

typedef bool conjuntoVertices[N];
typedef bool conjuntoArcos[N][N];

typedef struct {
    conjuntoVertices vertices;
    conjuntoArcos arcos;
} tGrafoNoPonderado;
```

#### B. Grafo Ponderado Compuesto (Ejemplo Red Vial / Rutas)
```c
#define MAX_VERTICES 60

typedef int tVertice;

typedef struct {
    int estaciones;
    int peajes;
} tPesoRuta;

typedef struct {
    tVertice origen;
    tVertice destino;
    tPesoRuta peso;
} tAristaCompuesta;

typedef bool conjuntoVertices[MAX_VERTICES];
typedef tPesoRuta conjuntoArcos[MAX_VERTICES][MAX_VERTICES];

typedef struct {
    conjuntoVertices vertices;
    conjuntoArcos arcos;
} tGrafoRedVial;
```

### 2.2 Representación con Listas de Adyacencia (Dinámica)
Se utiliza para grafos dispersos, optimizando el consumo de memoria de $O(V^2)$ a $O(V + A)$.

```c
typedef struct nodoAdy {
    tVertice destino;
    float peso;
    struct nodoAdy* siguiente;
} tNodoAdy;

typedef tNodoAdy* tListaAdyacencia;

typedef struct {
    bool activo;
    tListaAdyacencia lista;
} tElementoVertice;

typedef tElementoVertice tGrafoDinamico[N];
```

---

## 3. Implementación Paso a Paso de Operaciones Primitivas (Representación con Arrays)

### 3.1 Inicialización del Grafo
Desactiva todos los vértices y limpia las celdas de la matriz de arcos.

```c
void inicializarGrafo(tGrafoNoPonderado* pGrafo) {
    int x, y;
    for (x = 0; x < N; x++) {
        pGrafo->vertices[x] = false;
        for (y = 0; y < N; y++) {
            pGrafo->arcos[x][y] = false;
        }
    }
    printf("Grafo inicializado correctamente.\n");
}
```

### 3.2 Alta de Vértices y Arcos / Aristas

#### A. Alta de Vértice
```c
void agregarVertice(tGrafoNoPonderado* pGrafo, tVertice pVertice) {
    if (pVertice >= 0 && pVertice < N) {
        pGrafo->vertices[pVertice] = true;
        printf("Vertice %d activado.\n", pVertice);
    } else {
        printf("Error! Vertice %d fuera de rango (0 a %d).\n", pVertice, N - 1);
    }
}
```

#### B. Alta de Arco en Grafo Dirigido
```c
void agregarArco(tGrafoNoPonderado* pGrafo, tArco pArco) {
    if (pArco.origen >= 0 && pArco.origen < N &&
        pArco.destino >= 0 && pArco.destino < N) {
        if (pGrafo->vertices[pArco.origen] && pGrafo->vertices[pArco.destino]) {
            pGrafo->arcos[pArco.origen][pArco.destino] = true;
            printf("Arco (%d -> %d) activado.\n", pArco.origen, pArco.destino);
        } else {
            printf("Error! Alguno de los vertices no esta activo.\n");
        }
    } else {
        printf("Error! Vertices fuera de rango.\n");
    }
}
```

#### C. Alta de Arista en Grafo NO Dirigido (Simetría)
```c
void agregarAristaNoDirigida(tGrafoRedVial* pGrafo, tAristaCompuesta pArista) {
    if (pArista.origen >= 0 && pArista.origen < MAX_VERTICES &&
        pArista.destino >= 0 && pArista.destino < MAX_VERTICES) {
        
        /* Auto-activacion de vertices segun practica de catedra */
        pGrafo->vertices[pArista.origen] = true;
        pGrafo->vertices[pArista.destino] = true;

        /* Asignacion simetrica indispensable en grafos no dirigidos */
        pGrafo->arcos[pArista.origen][pArista.destino] = pArista.peso;
        pGrafo->arcos[pArista.destino][pArista.origen] = pArista.peso;
        
        printf("Arista simetrica {%d, %d} registrada.\n", pArista.origen, pArista.destino);
    } else {
        printf("Error! Indices fuera de limites permitidos.\n");
    }
}
```

---

### 3.3 Borrado de Vértices y Arcos

#### A. Borrar Vértice
Desactiva el vértice en el vector de vértices.
```c
void borrarVertice(tGrafoNoPonderado* pGrafo, tVertice pVertice) {
    if (pVertice >= 0 && pVertice < N && pGrafo->vertices[pVertice]) {
        pGrafo->vertices[pVertice] = false;
        printf("Vertice %d desactivado.\n", pVertice);
    } else {
        printf("El vertice %d no existe o ya esta inactivo.\n", pVertice);
    }
}
```

#### B. Borrar Arco
```c
void borrarArco(tGrafoNoPonderado* pGrafo, tArco pArco) {
    if (pGrafo->arcos[pArco.origen][pArco.destino]) {
        pGrafo->arcos[pArco.origen][pArco.destino] = false;
        printf("Arco (%d -> %d) desactivado.\n", pArco.origen, pArco.destino);
    } else {
        printf("El arco especificado no existe.\n");
    }
}
```

---

### 3.4 Funciones Auxiliares de Consulta

#### A. Búsqueda del Vértice Activo Máximo
Retorna el índice del vértice activo más alto para acotar la impresión de la matriz.

```c
int maximoVertice(tGrafoNoPonderado pGrafo) {
    int x;
    int max = 0;
    for (x = 0; x < N; x++) {
        if (pGrafo.vertices[x]) {
            max = x;
        }
    }
    return max;
}

bool existeVertice(tGrafoNoPonderado pGrafo, tVertice pVertice) {
    if (pVertice >= 0 && pVertice < N) {
        return pGrafo.vertices[pVertice];
    }
    return false;
}
```

#### B. Visualización de Matriz de Adyacencia
```c
void visualizarMatrizAdyacencia(tGrafoNoPonderado pGrafo) {
    int x, y;
    int maxV = maximoVertice(pGrafo);
    bool existeArista;

    printf("\n*** MATRIZ DE ADYACENCIA ***\n");
    for (x = 0; x <= maxV; x++) {
        for (y = 0; y <= maxV; y++) {
            existeArista = pGrafo.arcos[x][y] && 
                           existeVertice(pGrafo, x) && 
                           existeVertice(pGrafo, y);
            if (existeArista) {
                printf("1 ");
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
}
```

#### C. Visualización de Matriz de Pesos Compuestos
```c
void mostrarMatrizPesosCompuesta(tGrafoRedVial pGrafo) {
    int x, y;
    int maxV = 0;
    for (x = 0; x < MAX_VERTICES; x++) {
        if (pGrafo.vertices[x]) maxV = x;
    }

    printf("\nReferencias: S = estaciones de servicio, P = peajes\n");
    printf("*** MATRIZ DE PESOS COMPUESTOS ***\n\n");

    for (x = 0; x <= maxV; x++) {
        for (y = 0; y <= maxV; y++) {
            bool existe = pGrafo.vertices[x] && pGrafo.vertices[y] &&
                          (pGrafo.arcos[x][y].estaciones > 0 || pGrafo.arcos[x][y].peajes > 0);
            if (existe) {
                printf("(S:%d, P:%d) | ", pGrafo.arcos[x][y].estaciones, pGrafo.arcos[x][y].peajes);
            } else {
                printf("  (-, -)    | ");
            }
        }
        printf("\n");
    }
}
```

---

### 3.5 Cálculo de Grado de un Vértice

#### A. En Grafos No Dirigidos
```c
int calcularGradoNoDirigido(tGrafoNoPonderado pGrafo, tVertice pVertice) {
    int y;
    int grado = 0;
    if (existeVertice(pGrafo, pVertice)) {
        for (y = 0; y < N; y++) {
            if (pGrafo.arcos[pVertice][y] && existeVertice(pGrafo, y)) {
                grado++;
            }
        }
    }
    return grado;
}
```

#### B. En Grafos Dirigidos (Grado de Entrada y Salida)
```c
int calcularGradoSalida(tGrafoNoPonderado pGrafo, tVertice pVertice) {
    int y, grado = 0;
    if (existeVertice(pGrafo, pVertice)) {
        for (y = 0; y < N; y++) {
            if (pGrafo.arcos[pVertice][y] && existeVertice(pGrafo, y)) grado++;
        }
    }
    return grado;
}

int calcularGradoEntrada(tGrafoNoPonderado pGrafo, tVertice pVertice) {
    int x, grado = 0;
    if (existeVertice(pGrafo, pVertice)) {
        for (x = 0; x < N; x++) {
            if (pGrafo.arcos[x][pVertice] && existeVertice(pGrafo, x)) grado++;
        }
    }
    return grado;
}
```

---

### 3.6 Recorridos Sistemáticos de Grafos (DFS y BFS)

#### A. Recorrido en Profundidad (DFS - Depth First Search)
Avanza sistemáticamente explorando lo más profundo posible a lo largo de cada rama antes de retroceder.

```c
void dfsRecursivo(tGrafoNoPonderado pGrafo, tVertice pOrigen, bool pVisitados[]) {
    int v;
    pVisitados[pOrigen] = true;
    printf("%d ", pOrigen);

    for (v = 0; v < N; v++) {
        if (pGrafo.arcos[pOrigen][v] && 
            existeVertice(pGrafo, v) && 
            !pVisitados[v]) {
            dfsRecursivo(pGrafo, v, pVisitados);
        }
    }
}

void recorridoDFS(tGrafoNoPonderado pGrafo, tVertice pInicio) {
    bool visitados[N] = {false};
    printf("\nRecorrido DFS desde el vertice %d: ", pInicio);
    if (existeVertice(pGrafo, pInicio)) {
        dfsRecursivo(pGrafo, pInicio, visitados);
        printf("\n");
    } else {
        printf("Vertice inicial no valido.\n");
    }
}
```

---

## 4. Casos Borde y Gestión de Memoria Dinámica

### 4.1 Paso por Valor vs. Paso por Referencia
* **`tGrafo* pGrafo` (Puntero):** Se requiere obligatoriamente en `inicializarGrafo`, `agregarVertice`, `agregarArco`, `borrarVertice` y `borrarArco`. Si no se pasa puntero, las modificaciones sobre el arreglo ocurrirán en una copia local y se perderán al salir de la función.
* **`tGrafo pGrafo` (Por Valor):** Se utiliza en funciones de consulta e impresión como `visualizarMatrizAdyacencia` o `maximoVertice`.

### 4.2 Desactivación de Vértices y Arcos Huérfanos
Al ejecutar `borrarVertice(&grafo, v)`, el vértice se marca como `false` en el vector de vértices. Sin embargo, los valores en `arcos[v][y]` o `arcos[x][v]` permanecen almacenados en la matriz. Las funciones de impresión y cómputo deben filtrar siempre mediante la doble condición:
```c
pGrafo.arcos[x][y] && pGrafo.vertices[x] && pGrafo.vertices[y]
```

---

## 5. Errores Típicos de Parcial y Diagnóstico de Bugs

### Bug 1: No registrar ambos sentidos en Grafos No Dirigidos
* **Error:** Escribir solo `pGrafo->arcos[origen][destino] = peso;`.
* **Consecuencia:** El grafo se comporta como dirigido, provocando errores en consultas de caminos y matrices no simétricas.
* **Corrección:** Asignar ambas coordenadas:
  ```c
  pGrafo->arcos[pArista.origen][pArista.destino] = pArista.peso;
  pGrafo->arcos[pArista.destino][pArista.origen] = pArista.peso;
  ```

### Bug 2: Desbordamiento del límite del arreglo (`<= N` en lugar de `< N`)
* **Error:** Usar `for(x = 0; x <= N; x++)` en los bucles de iteración.
* **Consecuencia:** Intento de acceso al índice `N` (fuera del arreglo declarado de tamaño `N`), provocando *Segmentation Fault* o corrupción de variables contiguas en el Stack.
* **Corrección:** Utilizar strictly `x < N`.

### Bug 3: Operador de asignación invertido (`=!`) en borrado de vértices
* **Error:** Escribir `if (pGrafo->vertices[pVertice] =! 0)`.
* **Consecuencia:** C interpreta `=!` como la asignación del valor `!0` (`true`), activando el vértice en lugar de evaluar desigualdad.
* **Corrección:** Escribir correctamente `!=`:
  ```c
  if (pGrafo->vertices[pVertice] != false)
  ```

---

## 6. Lista de Verificación de Código (Checklist de Examen)

1. [ ] **Simetría en No Dirigidos:** En la función de agregar aristas no dirigidas, ¿se actualizaron ambas posiciones `[origen][destino]` y `[destino][origen]`?
2. [ ] **Validación de Límites:** ¿Se validó que `pVertice`, `origen` y `destino` estén dentro del rango válido `[0, N - 1]`?
3. [ ] **Doble Verificación de Estado:** En recorridos e impresiones, ¿se verificó que existan activos TANTO el vértice origen COMO el destino?
4. [ ] **Firma de Funciones:** ¿Las funciones que modifican la estructura reciben `tGrafo*` (puntero)?
5. [ ] **Normas de Estilo Cátedra:** ¿Los tipos inician con `t` (`tVertice`, `tArco`, `tGrafoNoPonderado`) y los parámetros con `p` (`pGrafo`, `pArco`)?
6. [ ] **Sin Caracteres Especiales:** ¿El código está exento de tildes y letras `ñ`?

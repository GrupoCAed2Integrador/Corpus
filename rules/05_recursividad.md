# 05. Recursividad y Pila de Llamadas

La recursividad es un paradigma de programación fundado en el principio de autosimilitud o definición auto-alusiva, mediante el cual un problema complejo se resuelve descomponiéndolo en subproblemas del mismo tipo pero de menor dimensión o complejidad. En la asignatura Algoritmos y Estructuras de Datos II (FaCENA - UNNE), la recursividad no solo constituye una alternativa a la iteración mediante bucles, sino que representa la base conceptual indispensable para el tratamiento de estructuras no lineales dinámicas, tales como los árboles binarios de búsqueda.

Comprender la recursividad exige dominar tanto la lógica algorítmica de la función como el comportamiento interno del sistema operativo y del compilador de C al administrar la memoria durante la ejecución de llamadas sucesivas.

---

## 1. Fundamentos Teóricos de la Recursividad

Un subprograma es recursivo cuando se invoca a sí mismo, ya sea de forma directa (la función `A` se llama a sí misma) o indirecta (la función `A` llama a `B`, y `B` llama a `A`). Para que un proceso recursivo sea correcto y finalice en tiempo finito, la función debe estructurarse obligatoriamente en dos componentes fundamentales:

1. **Caso Base (o Condición de Corte)**: Es el punto de parada del algoritmo. Representa la instancia del problema cuya solución es trivial o inmediata y no requiere de una nueva llamada recursiva. Cuando el programa alcanza el caso base, interrumpe la cadena de auto-invocaciones y comienza la fase de retorno.
2. **Caso Recursivo (o Paso Recursivo)**: Es la sentencia donde la función se invoca a sí misma. La regla de oro del caso recursivo exige que los argumentos pasados en la llamada se aproximen estrictamente al caso base en cada etapa. De lo contrario, el algoritmo entra en una secuencia infinita de llamadas.

### Comparativa: Algoritmo Iterativo vs. Algoritmo Recursivo

| Criterio | Enfoque Iterativo | Enfoque Recursivo |
| :--- | :--- | :--- |
| **Mecanismo de Control** | Estructuras repetitivas (`while`, `for`, `do-while`). | Auto-invocación de la función dentro de sí misma. |
| **Condición de Finalización** | Expresión booleana que evalúa el bucle en cada iteración. | Alcance explícito del **Caso Base**. |
| **Uso de Memoria** | Constante $O(1)$. Modifica variables en el mismo espacio. | Proporcional a la profundidad $O(n)$ en el *Stack*. |
| **Riesgo de Fallo** | Bucles infinitos por condición booleana mal construida. | Desbordamiento de pila (*Stack Overflow*) por falta de caso base. |
| **Elegancia Algorítmica** | Adecuado para estructuras secuenciales planas (arreglos). | Natural para estructuras recursivas (árboles, listas, fractales). |

### Ejemplo Matemático Canónico: Factorial de un Número

El factorial de un entero no negativo $n$ ($n!$) se define matemáticamente como:

$$n! = \begin{cases} 1 & \text{si } n = 0 \quad (\text{Caso Base}) \\ n \times (n-1)! & \text{si } n > 0 \quad (\text{Caso Recursivo}) \end{cases}$$

Implementación estándar en C respetando los tipos de datos de cátedra:

```c
#include <stdio.h>

// Prototipo de la función
long long int factorial(int pNro);

int main() {
    int numero = 5;
    printf("El factorial de %d es: %lld\n", numero, factorial(numero));
    return 0;
}

// Implementación de la función recursiva
long long int factorial(int pNro) {
    if (pNro == 0) {
        return 1; // Caso Base: 0! = 1
    } else {
        return pNro * factorial(pNro - 1); // Caso Recursivo
    }
}
```

---

## 2. La Pila de Llamadas (Call Stack) y Desbordamiento

Para ejecutar cualquier programa en lenguaje C, la arquitectura del sistema operativo reserva una zona de memoria dinámica en el segmento de memoria estático/dinámico denominada **Pila de Llamadas** (*Call Stack*).

### Mecanismo de Funcionamiento Interno

Cada vez que una función es invocada, el sistema crea un bloque de memoria temporal en la cima de la pila conocido como **Marco de Pila** (*Stack Frame* o registro de activación). Este marco almacena:
* Los **parámetros de entrada** recibidos por la función.
* Las **variables locales** declaradas dentro del cuerpo del subprograma.
* La **dirección de retorno**, que indica la instrucción exacta del código a la que debe volver el procesador una vez finalizada la ejecución.

```
       ESTADO DE LA PILA DE LLAMADAS PARA factorial(3)

   CIMA DE LA PILA   ┌────────────────────────────────┐
  (Último apilado)   │ Frame: factorial(0)            │ -> Retorna 1
                     │   pNro = 0                     │
                     ├────────────────────────────────┤
                     │ Frame: factorial(1)            │ -> Aguarda resultado (1 * 1)
                     │   pNro = 1                     │
                     ├────────────────────────────────┤
                     │ Frame: factorial(2)            │ -> Aguarda resultado (2 * 1)
                     │   pNro = 2                     │
                     ├────────────────────────────────┤
                     │ Frame: factorial(3)            │ -> Aguarda resultado (3 * 2)
                     │   pNro = 3                     │
                     ├────────────────────────────────┤
   BASE DE LA PILA   │ Frame: main()                  │ -> Invocador original
   (Primer apilado)  └────────────────────────────────┘
```

#### Traza de Ejecución y Desapilado

1. **Fase de Invocación (Apilado / LIFO)**:
   * `main()` invoca a `factorial(3)`. `main()` queda pausado en estado parcialmente completado.
   * `factorial(3)` evalúa `3 == 0` (falso) e invoca `factorial(2)`. Se apila el marco de `factorial(2)`.
   * `factorial(2)` evalúa `2 == 0` (falso) e invoca `factorial(1)`. Se apila el marco de `factorial(1)`.
   * `factorial(1)` evalúa `1 == 0` (falso) e invoca `factorial(0)`. Se apila el marco de `factorial(0)`.
   * `factorial(0)` evalúa `0 == 0` (verdadero) y devuelve `1`.

2. **Fase de Retorno (Desapilado)**:
   * El marco de `factorial(0)` se destruye y el control retorna a `factorial(1)`, recibiendo el valor `1`. Calcula `1 * 1 = 1` y retorna.
   * El marco de `factorial(1)` se destruye y el control retorna a `factorial(2)`, recibiendo `1`. Calcula `2 * 1 = 2` y retorna.
   * El marco de `factorial(2)` se destruye y el control retorna a `factorial(3)`, recibiendo `2`. Calcula `3 * 2 = 6` y retorna.
   * El marco de `factorial(3)` se destruye y el control retorna a `main()`, obteniendo el resultado final `6`.

### Desbordamiento de Pila (*Stack Overflow*)

El tamaño de la Pila de Llamadas es acotado por el sistema operativo. Si una función recursiva carece de caso base o si sus argumentos no convergen hacia la condición de corte, las auto-invocaciones continúan indefinidamente apilando marcos de ejecución hasta agotar el espacio asignado. Este error crítico provoca la interrupción inmediata del programa por **desbordamiento de pila** (*Stack Overflow* o *Segmentation Fault*).

---

## 3. Algoritmos Matemáticos y Conversiones Numéricas (TP3)

El Trabajo Práctico N° 3 de la cátedra propone un conjunto de ejercicios orientados a afianzar el razonamiento recursivo sobre transformaciones numéricas y operaciones sobre tipos de datos primarios.

### 3.1. Cuenta Regresiva con Retardo de Tiempo (TP3 - Ejercicios 1 y 2)

El algoritmo debe realizar un conteo decreciente e informar al usuario el agotamiento del tiempo al alcanzar el valor cero. Para incorporar la suspensión de la ejecución por segundos, se utiliza la función `sleep()` definida en `<unistd.h>` (Linux/POSIX) o en `<windows.h>` (Windows).

```c
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #define SLEEP_SEGUNDO(s) Sleep((s) * 1000)
#else
    #include <unistd.h>
    #define SLEEP_SEGUNDO(s) sleep(s)
#endif

void cuentaRegresiva(int pNumero);

int main() {
    int valorInicial;
    printf("Ingrese el valor inicial para la cuenta regresiva: ");
    scanf("%d", &valorInicial);

    printf("\n--- Iniciando Conteo ---\n");
    cuentaRegresiva(valorInicial);
    return 0;
}

void cuentaRegresiva(int pNumero) {
    if (pNumero <= 0) {
        // Caso Base: Alcanza cero o valor negativo inicial
        printf("0 - ¡El tiempo se ha agotado!\n");
    } else {
        // Caso Recursivo
        printf("%d...\n", pNumero);
        SLEEP_SEGUNDO(1); // Pausa de 1 segundo
        cuentaRegresiva(pNumero - 1);
    }
}
```

### 3.2. Conversión de Decimal a Binario (TP3 - Ejercicio 3)

La conversión de un número decimal a su representación binaria mediante divisiones sucesivas por 2 produce los residuos binarios de derecha a izquierda (desde el bit menos significativo al más significativo). Al emplear recursividad, los residuos calculados en la fase descendente se imprimen durante la fase de retorno (desapilado), lo que invierte automáticamente el orden y genera el número binario correcto.

```c
#include <stdio.h>

void verEnBinario(int pNumero);

int main() {
    int numero = 13;
    printf("El numero %d en binario es: ", numero);
    if (numero == 0) {
        printf("0");
    } else {
        verEnBinario(numero);
    }
    printf("\n");
    return 0;
}

void verEnBinario(int pNumero) {
    if (pNumero > 0) {
        // Caso Recursivo: Primero procesa la división por 2
        verEnBinario(pNumero / 2);
        // La impresión se realiza al desapilar (orden ascendente)
        printf("%d", pNumero % 2);
    }
    // Caso Base implícito: pNumero == 0 frena las llamadas sin imprimir
}
```

### 3.3. Inversión de Números Enteros (TP3 - Ejercicio 4)

Para invertir la secuencia de dígitos de un número entero positivo, se aplica el principio de extracción de dígitos por módulo 10 (`n % 10`) y reducción por división entera (`n / 10`).

```c
#include <stdio.h>

void mostrarInvertido(int pNumero);

int main() {
    int numero = 1234;
    printf("Numero original: %d\nNumero invertido: ", numero);
    mostrarInvertido(numero);
    printf("\n");
    return 0;
}

void mostrarInvertido(int pNumero) {
    if (pNumero < 10) {
        // Caso Base: Un único dígito
        printf("%d", pNumero);
    } else {
        // Caso Recursivo: Imprime el último dígito y procesa el resto
        printf("%d", pNumero % 10);
        mostrarInvertido(pNumero / 10);
    }
}
```

### 3.4. Verificación de Cadenas Palíndromas (TP3 - Ejercicio 5)

Una palabra es palíndroma si se lee igual de izquierda a derecha que de derecha a izquierda. La función recursiva compara los caracteres en las posiciones extremas (`inicio` y `fin`) y reduce los índices hacia el centro.

```c
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool esPalindromoRecursivo(char pCadena[], int pInicio, int pFin);

int main() {
    char palabra[30] = "RECONOCER";
    int longitud = strlen(palabra);

    if (esPalindromoRecursivo(palabra, 0, longitud - 1)) {
        printf("La palabra '%s' ES palindroma.\n", palabra);
    } else {
        printf("La palabra '%s' NO es palindroma.\n", palabra);
    }
    return 0;
}

bool esPalindromoRecursivo(char pCadena[], int pInicio, int pFin) {
    if (pInicio >= pFin) {
        // Caso Base 1: Los índices se cruzaron o igualaron (es palíndromo)
        return true;
    }
    if (pCadena[pInicio] != pCadena[pFin]) {
        // Caso Base 2: Descoincidencia de caracteres extremos
        return false;
    }
    // Caso Recursivo: Avanza el extremo izquierdo y retrocede el derecho
    return esPalindromoRecursivo(pCadena, pInicio + 1, pFin - 1);
}
```

### 3.5. Sucesión de Fibonacci (TP3 - Ejercicio 10)

La sucesión de Fibonacci se define formalmente como $F_0 = 0$, $F_1 = 1$, y $F_n = F_{n-1} + F_{n-2}$ para $n > 1$.

```c
#include <stdio.h>

int fibonacci(int n);

int main() {
    int terminos = 7;
    printf("Secuencia de Fibonacci hasta el termino %d:\n", terminos);
    for (int i = 0; i < terminos; i++) {
        printf("%d ", fibonacci(i));
    }
    printf("\n");
    return 0;
}

int fibonacci(int n) {
    if (n == 0) {
        return 0; // Caso Base 1
    }
    if (n == 1) {
        return 1; // Caso Base 2
    }
    return fibonacci(n - 1) + fibonacci(n - 2); // Caso Recursivo
}
```

---

## 4. Recursividad sobre Arreglos de Estructuras (TP3 y Parciales)

En los parciales y trabajos prácticos de la cátedra FaCENA, un ejercicio típico consiste en procesar arreglos estáticos de estructuras de forma estrictamente recursiva. Para ello, la dimensión del arreglo (`pDimension` o `pCantidad`) actúa como el acumulador decreciente que guía el alcance del caso base.

### 4.1. Búsqueda del Elemento Máximo en un Arreglo (TP3 - Ejercicio 6)

```c
#include <stdio.h>

int buscarMaximo(int pArreglo[], int pDimension);

int main() {
    int datos[5] = {1, -2, 7, 4, 3};
    int maximo = buscarMaximo(datos, 5);
    printf("El elemento maximo del arreglo es: %d\n", maximo);
    return 0;
}

int buscarMaximo(int pArreglo[], int pDimension) {
    if (pDimension == 1) {
        // Caso Base: Arreglo de un solo elemento, el máximo es ese elemento
        return pArreglo[0];
    } else {
        // Caso Recursivo: Compara el último elemento con el máximo del subarreglo restante
        int maxSubArreglo = buscarMaximo(pArreglo, pDimension - 1);
        if (pArreglo[pDimension - 1] > maxSubArreglo) {
            return pArreglo[pDimension - 1];
        } else {
            return maxSubArreglo;
        }
    }
}
```

### 4.2. Acumulación Total sobre Arreglos de Estructuras (Modelo de Parcial)

Ejercicio tomado en el Modelo de Primer Parcial: Calcular la cantidad total de horas extras trabajadas por los empleados de una empresa a partir de un arreglo `tEmpleado empleados[MAX]`.

```c
#include <stdio.h>

#define MAX 15

typedef struct {
    int legajo;
    int horasExtras;
    float sueldoBase;
} tEmpleado;

// Prototipo de la función recursiva
int sumarHorasExtras(tEmpleado pEmpleados[], int pDimension);

int main() {
    tEmpleado empleados[MAX];
    int totalHorasExtras;

    // Carga de datos de prueba
    empleados[0].legajo = 1001; empleados[0].horasExtras = 10;
    empleados[1].legajo = 1002; empleados[1].horasExtras = 5;
    empleados[2].legajo = 1003; empleados[2].horasExtras = 8;
    empleados[3].legajo = 1004; empleados[3].horasExtras = 12;
    empleados[4].legajo = 1005; empleados[4].horasExtras = 6;

    // Invocación a la función recursiva desde el main especificando la cantidad actual
    totalHorasExtras = sumarHorasExtras(empleados, 5);

    printf("Total acumulado de horas extras: %d\n", totalHorasExtras);
    return 0;
}

// Implementación de la función recursiva
int sumarHorasExtras(tEmpleado pEmpleados[], int pDimension) {
    if (pDimension == 0) {
        // Caso Base: Arreglo vacío o sin elementos pendientes por procesar
        return 0;
    } else {
        // Caso Recursivo: Suma las horas del elemento actual (pDimension - 1) + llamado recursivo
        return pEmpleados[pDimension - 1].horasExtras + sumarHorasExtras(pEmpleados, pDimension - 1);
    }
}
```

### 4.3. Suma de Stock de Artículos (TP3 - Ejercicio 7)

```c
#include <stdio.h>

#define TOTAL 5

typedef struct {
    int codigo;
    int cantidad;
    float precio;
} tArticulos;

int totalArticulosComprar(tArticulos pArticulos[], int pDimension);

int main() {
    tArticulos inventario[TOTAL] = {
        {101, 15, 250.0},
        {102, 30, 120.0},
        {103, 5, 500.0},
        {104, 12, 310.0},
        {105, 20, 150.0}
    };

    int cantidadTotal = totalArticulosComprar(inventario, TOTAL);
    printf("Total de articulos a adquirir para reponer stock: %d unidades\n", cantidadTotal);
    return 0;
}

int totalArticulosComprar(tArticulos pArticulos[], int pDimension) {
    if (pDimension == 0) {
        return 0; // Caso Base
    } else {
        return pArticulos[pDimension - 1].cantidad + totalArticulosComprar(pArticulos, pDimension - 1);
    }
}
```

---

## 5. Recursividad sobre Listas Enlazadas

El Trabajo Práctico N° 3 contempla la conversión de algoritmos iterativos sobre listas simplemente enlazadas a versiones puramente recursivas.

### Conversión de Recorrido Iterativo a Recursivo (TP3 - Ejercicio 8)

Dado el tipo de dato y la función iterativa `mostrarColores`:

```c
#define MAXCAR 25

typedef char tString[MAXCAR];

typedef struct {
    int codigo;
    tString nombre;
} tColor;

typedef struct nodoColor {
    tColor color;
    struct nodoColor *siguiente;
} tListaColores;
```

**Versión Iterativa Original**:
```c
void mostrarColoresIterativo(tListaColores *pListaColores) {
    if (pListaColores == NULL) {
        printf("No hay colores para mostrar en la lista!\n");
    } else {
        tListaColores *listaAux = pListaColores;
        printf("\n *** Lista de colores ***\n");
        while (listaAux != NULL) {
            printf("%d-%s | ", listaAux->color.codigo, listaAux->color.nombre);
            listaAux = listaAux->siguiente;
        }
    }
}
```

**Versión Recursiva Equivalente**:
Para mantener la estructura del encabezado sin repetirlo en cada llamada, se implementa una función auxiliar recursiva o se utiliza un parámetro de control:

```c
#include <stdio.h>
#include <stdbool.h>

void mostrarColoresRecursivoAux(tListaColores *pNodo) {
    if (pNodo != NULL) {
        // Imprime el nodo actual
        printf("%d-%s | ", pNodo->color.codigo, pNodo->color.nombre);
        // Llamada recursiva con el puntero al siguiente nodo
        mostrarColoresRecursivoAux(pNodo->siguiente);
    }
    // Caso Base implícito: pNodo == NULL frena la recursión
}

void mostrarColoresRecursivo(tListaColores *pListaColores) {
    if (pListaColores == NULL) {
        printf("No hay colores para mostrar en la lista!\n");
    } else {
        printf("\n *** Lista de colores ***\n");
        mostrarColoresRecursivoAux(pListaColores);
        printf("\n");
    }
}
```

---

## 6. Diagnóstico y Corrección de Errores Típicos de Parciales

En las evaluaciones escritas de la cátedra FaCENA, un ítem de 2,50 puntos consiste en analizar funciones recursivas con errores deliberados de lógica o de control de flujo, explicando el comportamiento exacto del programa y reescribiendo la solución corregida.

### Caso de Examen 1: Ejecución del `printf` Fuera del Caso Base (Parcial Tema 1 - Ejercicio 2)

**Código con error de la consigna**:
```c
void mostrarNiveles(int pNivel) {
    printf("Nivel %d\n", pNivel);
    if (pNivel > 0) {
        mostrarNiveles(pNivel - 1);
    }
}
```

#### Análisis del Error
1. **Comportamiento al invocar `mostrarNiveles(4)`**: La función muestra por pantalla: `Nivel 4`, `Nivel 3`, `Nivel 2`, `Nivel 1` y **`Nivel 0`**.
2. **Causa Técnica**: El `printf` se encuentra posicionado **antes** de la evaluación condicional del `if (pNivel > 0)`. Cuando `pNivel` alcanza el valor `0`, la función primero ejecuta `printf("Nivel 0\n")` y recién después evalúa la condición `0 > 0`, la cual resulta falsa y frena la llamada recursiva. Como consecuencia, imprime un nivel adicional no deseado.

#### Código Corregido
```c
void mostrarNivelesCorregida(int pNivel) {
    if (pNivel > 0) {
        printf("Nivel %d\n", pNivel);
        mostrarNivelesCorregida(pNivel - 1);
    }
}
```

---

### Caso de Examen 2: Condición Evaluada en la Variable Incorrecta (Parcial Tema 1 - Ejercicio 3)

**Código con error de la consigna**:
```c
void mostrarTabla(int pNumero, int pMultiplicador) {
    if (pNumero <= 10) {
        printf("%d x %d = %d\n", pNumero, pMultiplicador, pNumero * pMultiplicador);
        mostrarTabla(pNumero, pMultiplicador + 1);
    }
}
```

#### Análisis del Error
1. **Comportamiento**:
   * Al invocar con `mostrarTabla(7, 1)`: el valor `pNumero = 7` cumple `7 <= 10` (verdadero). Dado que `pNumero` nunca se modifica en la llamada recursiva, la condición `pNumero <= 10` se mantiene infinitamente verdadera, incrementando `pMultiplicador` hasta agotar la Pila de Llamadas (*Stack Overflow*).
   * Al invocar con `mostrarTabla(12, 1)`: el valor `pNumero = 12` evalúa `12 <= 10` (falso) desde el inicio, por lo que la función finaliza inmediatamente sin imprimir nada por pantalla.
2. **Causa Técnica**: La condición del `if` evalúa la variable `pNumero` (que permanece constante) en lugar de evaluar la variable de avance `pMultiplicador`.

#### Código Corregido
```c
void mostrarTablaCorregida(int pNumero, int pMultiplicador) {
    if (pMultiplicador <= 10) {
        printf("%d x %d = %d\n", pNumero, pMultiplicador, pNumero * pMultiplicador);
        mostrarTablaCorregida(pNumero, pMultiplicador + 1);
    }
}
```

---

### Caso de Examen 3: Impresión Excesiva de Elementos (Parcial Tema 2 - Ejercicio 2)

**Código con error de la consigna**:
```c
void mostrarAsteriscos(int pCantidad) {
    printf("* ");
    if (pCantidad > 0) {
        mostrarAsteriscos(pCantidad - 1);
    }
}
```

#### Análisis del Error
1. **Comportamiento al invocar `mostrarAsteriscos(3)`**: Imprime **4 asteriscos** (`* * * * `) en lugar de los 3 solicitados.
2. **Causa Técnica**: Similar al Caso 1, la instrucción de impresión `printf("* ")` se ejecuta indefectiblemente antes de verificar la condición `if (pCantidad > 0)`. Al llegar a `pCantidad = 0`, imprime el cuarto asterisco antes de cortar.

#### Código Corregido
```c
void mostrarAsteriscosCorregida(int pCantidad) {
    if (pCantidad > 0) {
        printf("* ");
        mostrarAsteriscosCorregida(pCantidad - 1);
    }
}
```

---

### Caso de Examen 4: Caso Base Rígido e Inestabilidad de Paridad (Parcial Tema 2 - Ejercicio 3)

**Código con error de la consigna**:
```c
void mostrarTiempoRestante(int pSegundos) {
    if (pSegundos != 0) {
        printf("%d ", pSegundos);
        mostrarTiempoRestante(pSegundos - 2);
    }
}
```

#### Análisis del Error
1. **Comportamiento**:
   * Al invocar con `mostrarTiempoRestante(8)` (número par): Imprime `8 6 4 2 `. Al restar 2, la variable `pSegundos` toma el valor `0`, la condición `0 != 0` resulta falsa y el programa finaliza correctamente.
   * Al invocar con `mostrarTiempoRestante(7)` (número impar): La secuencia de valores generada es $7 \rightarrow 5 \rightarrow 3 \rightarrow 1 \rightarrow -1 \rightarrow -3 \ldots$ Como los decrementos son de 2 en 2, la variable saltea exactamente el valor `0`, volviéndose negativa. La condición `-1 != 0` continúa siendo verdadera de forma indefinida, provocando un desbordamiento de pila (*Stack Overflow*).
2. **Causa Técnica**: El uso del operador de desigualdad estricta `!= 0` crea un caso base rígido que depende de la paridad del argumento de entrada.

#### Código Corregido
```c
void mostrarTiempoRestanteCorregida(int pSegundos) {
    if (pSegundos > 0) { // Condición de corte amplia y segura
        printf("%d ", pSegundos);
        mostrarTiempoRestanteCorregida(pSegundos - 2);
    }
}
```

---

## 7. Lista de Verificación para Exámenes

Al resolver o revisar un ejercicio recursivo en parciales o recuperatorios, verifique el cumplimiento de las siguientes reglas de control:

- [ ] **Identificación Clara del Caso Base**: ¿Existe un bloque condicional que detenga la recursión sin realizar nuevas llamadas auto-alusivas?
- [ ] **Convergencia del Caso Recursivo**: ¿Los argumentos pasados en la llamada recursiva se aproximan de forma garantizada hacia el Caso Base en cada etapa?
- [ ] **Uso de Desigualdades Amplias**: ¿Se utilizó `> 0` o `>= 0` en lugar de comparaciones rígidas como `!= 0` o `== 0` para evitar bucles infinitos con valores impares o fuera de rango?
- [ ] **Posición de las Impresiones**: ¿El `printf` se encuentra ubicado **dentro** del bloque condicional cuando se busca evitar elementos o niveles adicionales?
- [ ] **Evaluación del Parámetro Correcto**: En funciones con múltiples parámetros, ¿la condición de corte evalúa la variable que se modifica en la invocación recursiva?
- [ ] **Invocación desde `main()`**: ¿Se incluyó la llamada a la función desde el programa principal pasando la dimensión o valor inicial adecuado?

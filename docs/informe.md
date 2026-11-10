# Corpus

## Sistema de Gestion de Pacientes de un Hospital

**Trabajo Practico Integrador**

Algoritmos y Estructuras de Datos II

FaCENA - Universidad Nacional del Nordeste

Noviembre de 2026

---

### Integrantes y roles

| Integrante | Rol principal |
| :--- | :--- |
| Alan | Analisis, requisitos y diccionario de datos. |
| Benja | Diseno de tipos y archivos `.h`, inicializacion e insercion con control de duplicados. |
| Mariano | Busqueda recursiva y modificacion de datos. |
| Santino | Eliminacion recursiva con los tres casos y eliminacion de la raiz. |
| Brian | Recorrido InOrden, conteo y validaciones. |
| Gaston | `main.c`, menu, integracion, pruebas generales, README e informe. |

---

## Indice

1. [Introduccion](#1-introduccion)
2. [Objetivos](#2-objetivos)
3. [Marco teorico](#3-marco-teorico)
4. [Diseno de la solucion](#4-diseno-de-la-solucion)
5. [Implementacion](#5-implementacion)
6. [Pruebas y resultados](#6-pruebas-y-resultados)
7. [Conclusiones](#7-conclusiones)
8. [Bibliografia](#8-bibliografia)
9. [Anexos](#9-anexos)

---

## 1. Introduccion

La administracion de pacientes en un hospital requiere localizar informacion
por un identificador estable y unico. En este trabajo se implementa una
aplicacion de consola en lenguaje C que almacena los datos administrativos de
los pacientes en un **arbol binario de busqueda (ABB)** cuya clave es el
**numero de historia clinica**.

Cada operacion del sistema (alta, busqueda, modificacion, baja, listado y
conteo) trabaja sobre esa estructura. La aplicacion incorpora validacion de
entradas, control de memoria dinamica y persistencia en un archivo binario,
de modo que la informacion sobreviva al cierre del programa.

El sistema emplea unicamente datos ficticios y administrativos. No almacena
diagnosticos, tratamientos ni datos clinicos.

## 2. Objetivos

### Objetivo general

Desarrollar un sistema modular en C que gestione pacientes mediante un ABB
ordenado por historia clinica, aplicando punteros, memoria dinamica y
recursividad, con validacion de datos y liberacion completa de la memoria.

### Objetivos especificos

- Definir el tipo de dato abstracto paciente y el nodo del arbol.
- Implementar el alta con rechazo de claves duplicadas.
- Implementar la busqueda recursiva por historia clinica.
- Implementar la modificacion de los datos no clave.
- Implementar la baja contemplando los tres casos posibles y la raiz.
- Recorrer el arbol en InOrden para listar los pacientes en forma ascendente.
- Contar los pacientes con una funcion recursiva.
- Persistir los datos en un archivo binario y recuperarlos al iniciar.
- Validar las entradas y verificar el resultado de `malloc`.
- Liberar toda la memoria antes de finalizar el programa.

## 3. Marco teorico

### 3.1. Arbol binario de busqueda

Un arbol binario de busqueda es una estructura no lineal y dinamica en la que
cada nodo tiene como maximo dos hijos y se cumple, para todo nodo:

- todas las claves del subarbol izquierdo son menores que la clave del nodo;
- todas las claves del subarbol derecho son mayores que la clave del nodo;
- no existen claves repetidas.

Estas propiedades permiten descartar una rama completa en cada comparacion,
por lo que la busqueda, el alta y la baja tienen complejidad proporcional a la
altura del arbol.

Ejemplo de ABB con las historias clinicas utilizadas en las pruebas:

```text
           150
          /   \
        90     210
       /  \   /   \
     60  120 180  240
```

El recorrido InOrden de este arbol produce las claves en orden ascendente:
`60, 90, 120, 150, 180, 210, 240`.

### 3.2. Recursividad

Las operaciones del arbol se implementan mediante recursion porque la
definicion misma del ABB es recursiva: cada subarbol es, a su vez, un ABB.
La recursion permite expresar el alta, la busqueda, la baja, el recorrido y
la liberacion como una accion sobre el nodo actual mas la misma accion sobre
sus subarboles.

### 3.3. Memoria dinamica

Cada paciente se almacena en un nodo reservado con `malloc`. El nodo se libera
con `free` cuando se elimina un paciente o cuando se destruye el arbol
completo. La liberacion total se realiza en postorden para no perder la
referencia a los hijos antes de liberar el nodo padre.

## 4. Diseno de la solucion

### 4.1. Tipo de dato paciente

```c
#define MAX_NOMBRE 60
#define MAX_DNI 16
#define MAX_TELEFONO 24
#define MAX_AREA 40
#define MAX_HISTORIA 999999999
#define EDAD_MINIMA 0
#define EDAD_MAXIMA 120

typedef char tNombre[MAX_NOMBRE];
typedef char tDni[MAX_DNI];
typedef char tTelefono[MAX_TELEFONO];
typedef char tArea[MAX_AREA];

typedef struct {
    int historiaClinica;
    tNombre nombre;
    tDni dni;
    int edad;
    tTelefono telefono;
    tArea area;
} tPaciente;
```

El DNI y el telefono se almacenan como texto porque el DNI puede tener ceros
iniciales y el telefono admite separadores. El telefono es opcional.

### 4.2. Nodo del arbol

```c
typedef struct nodoPaciente {
    tPaciente dato;
    struct nodoPaciente *izquierdo;
    struct nodoPaciente *derecho;
} tNodoPaciente;

typedef tNodoPaciente *tArbolPacientes;
```

`tArbolPacientes` es un alias del puntero a la raiz. Las funciones que
modifican la estructura reciben `tArbolPacientes *` para poder cambiar la raiz
o un enlace del arbol; las funciones de consulta reciben el puntero por valor.

### 4.3. Organizacion modular

| Archivo | Responsabilidad |
| :--- | :--- |
| `main.c` | Menu, coordinacion de las operaciones y carga/guardado automatico. |
| `pacientes.h` / `.c` | Tipo `tPaciente`, carga, modificacion y muestra de datos. |
| `arbol_pacientes.h` / `.c` | Tipo del nodo y operaciones del ABB. |
| `archivo_pacientes.h` / `.c` | Carga y guardado del archivo binario. |
| `validaciones.h` / `.c` | Lectura robusta y validacion de cada campo. |
| `pruebas/casos_prueba.txt` | Casos de prueba y resultados esperados. |

Esta division mantiene el `main` como simple coordinador y evita concentrar
la logica en un solo archivo. No se utilizan variables globales.

## 5. Implementacion

### 5.1. Operaciones del ABB

| Operacion | Estrategia | Complejidad promedio |
| :--- | :--- | :--- |
| Inicializacion | Asignar `NULL` a la raiz. | O(1) |
| Alta | Insercion recursiva segun la clave; rechazo de duplicados. | O(log n) |
| Busqueda | Descenso recursivo por una sola rama. | O(log n) |
| Modificacion | Busqueda del nodo y actualizacion de los campos no clave. | O(log n) |
| Baja | Tres casos; con dos hijos se usa el minimo del subarbol derecho. | O(log n) |
| Listado | Recorrido InOrden. | O(n) |
| Conteo | `1 + contar(izquierdo) + contar(derecho)`. | O(n) |
| Liberacion | Recorrido en postorden con `free`. | O(n) |
| Carga del archivo | Lectura de registros e insercion de cada uno. | O(n log n) |
| Guardado del archivo | Recorrido en preorden con `fwrite`. | O(n) |

En el peor caso, cuando el arbol queda degenerado (por ejemplo, al insertar
claves ya ordenadas), la altura es O(n) y las operaciones de alta, busqueda y
baja se degradan a O(n).

### 5.2. Alta con control de duplicados

La insercion compara la clave con el nodo actual y desciende por la rama
correspondiente. Si encuentra la misma clave, informa el error y no modifica
el arbol:

```c
if (pPaciente.historiaClinica < (*pRaiz)->dato.historiaClinica) {
    return insertarPaciente(&((*pRaiz)->izquierdo), pPaciente);
} else if (pPaciente.historiaClinica > (*pRaiz)->dato.historiaClinica) {
    return insertarPaciente(&((*pRaiz)->derecho), pPaciente);
} else {
    printf("Error: la historia clinica %d ya existe.\n", pPaciente.historiaClinica);
    return false;
}
```

### 5.3. Busqueda

La busqueda desciende por una sola rama y devuelve la direccion del dato
encontrado, o `NULL` si la clave no existe. Devolver un puntero permite que la
modificacion actualice el paciente sin reconstruir el arbol.

### 5.4. Modificacion

La modificacion busca el nodo y actualiza nombre, DNI, edad, telefono y area.
La historia clinica no se solicita, de modo que la clave del ABB permanece
inalterada y la propiedad de orden se conserva. Para cada campo, si el usuario
presiona Enter se conserva el valor anterior; en el telefono, un guion (`-`)
permite borrarlo.

### 5.5. Baja con los tres casos

La eliminacion se implementa con doble puntero para tratar de igual manera un
nodo interno y la raiz:

```c
if ((*pRaiz)->izquierdo == NULL) {          /* hoja o solo hijo derecho */
    nodoAEliminar = *pRaiz;
    *pRaiz = (*pRaiz)->derecho;
    free(nodoAEliminar);
    return true;
}

if ((*pRaiz)->derecho == NULL) {            /* solo hijo izquierdo */
    nodoAEliminar = *pRaiz;
    *pRaiz = (*pRaiz)->izquierdo;
    free(nodoAEliminar);
    return true;
}

nodoAEliminar = buscarMenor((*pRaiz)->derecho);   /* dos hijos */
claveSucesor = nodoAEliminar->dato.historiaClinica;
(*pRaiz)->dato = nodoAEliminar->dato;
return eliminarPaciente(&((*pRaiz)->derecho), claveSucesor);
```

- **Caso 1 - nodo hoja:** el enlace del padre queda en `NULL` y se libera el
  nodo.
- **Caso 2 - nodo con un hijo:** el hijo reemplaza al nodo eliminado.
- **Caso 3 - nodo con dos hijos:** se copia el sucesor InOrden (el menor del
  subarbol derecho) en el nodo y se elimina ese sucesor, que tiene a lo sumo un
  hijo.

La raiz se elimina con la misma logica porque el doble puntero permite
actualizar la variable raiz del programa.

### 5.6. Recorrido, conteo y liberacion

El listado usa InOrden, por lo que los pacientes aparecen en orden ascendente
por historia clinica. El conteo es recursivo y el conteo de un arbol vacio es
cero. La liberacion recorre en postorden: primero libera los subarboles y
luego el nodo, y deja el puntero en `NULL`.

### 5.7. Validaciones

| Dato | Regla |
| :--- | :--- |
| Historia clinica | Entero entre 1 y 999999999. |
| Nombre | No vacio; letras, espacios, apostrofo y guion; al menos una letra. |
| DNI | Solo digitos, longitud 7 u 8. |
| Edad | Entero entre 0 y 120. |
| Telefono | Opcional; digitos, espacios, `+`, `-`, `(` y `)`; al menos un digito. |
| Area | No vacia. |

La lectura de cadenas usa `fgets` con el tamanio del campo, elimina el salto de
linea y descarta el resto de una linea demasiado larga. Los enteros se leen
como texto y se convierten con `strtol`, verificando que no queden caracteres
sobrantes y que el valor este dentro del rango. Todo `malloc` se verifica
antes de usar el nodo.

### 5.8. Persistencia binaria

Al iniciar, el sistema intenta abrir `pacientes.dat` en modo lectura binaria.
Si el archivo no existe, comienza con un arbol vacio. Si existe, lee un
registro `tPaciente` por vez, valida cada registro y lo inserta en el arbol.
Los registros invalidos o duplicados se omiten y se informa la cantidad.

Al salir (opcion `0` o fin de la entrada), el arbol se recorre en preorden y
cada paciente se escribe con `fwrite`. Guardar en preorden permite que, al
reinsertar los registros, se reconstruya la misma forma del arbol.

No se guardan los punteros `izquierdo` y `derecho`, porque son direcciones de
memoria que no tienen sentido en otra ejecucion. El archivo contiene solo los
datos de los pacientes. En la configuracion utilizada, cada registro ocupa
148 bytes.

## 6. Pruebas y resultados

El plan completo se documenta en `pruebas/casos_prueba.txt`. Se ejecutaron
las operaciones con entradas automatizadas sobre el ejecutable compilado con
TDM-GCC 9.2.0, el mismo compilador de Dev-C++.

| Escenario | Resultado obtenido |
| :--- | :--- |
| Arbol vacio: listado y conteo | Sin pacientes, conteo 0. |
| Altas 150, 90, 210, 60, 120, 180, 240 | Todas aceptadas. |
| Historia duplicada 150 | Rechazada con mensaje de error. |
| Busqueda de 120 | Paciente encontrado. |
| Busqueda de 999 | Informa que no existe. |
| InOrden | 60, 90, 120, 150, 180, 210, 240. |
| Modificacion de nombre y edad | Cambios aplicados; resto conservado. |
| Baja de hoja 60 | Arbol valido y conteo actualizado. |
| Baja de nodo con un hijo 90 | El hijo 120 ocupa su lugar. |
| Baja de raiz con dos hijos 150 | El sucesor 180 pasa a ser raiz. |
| Baja de raiz con un hijo 180 | El hijo 240 pasa a ser raiz. |
| Cancelacion de baja | El paciente permanece. |
| Entradas invalidas | Se informa el error y se vuelve a pedir el dato. |
| Persistencia | Los datos se recuperan al reabrir el programa. |
| Archivo inexistente | Inicia vacio, sin error. |
| Archivo danado | Omite los registros invalidos y continua. |
| Cierre | Libera la memoria e informa el guardado. |

La compilacion se realizo con:

```bash
gcc -std=c99 -Wall -Wextra -Wpedantic \
    main.c pacientes.c arbol_pacientes.c archivo_pacientes.c validaciones.c \
    -o sistema_pacientes
```

No se obtuvieron errores ni advertencias.

## 7. Conclusiones

El sistema cumple con los requisitos funcionales planteados: alta con clave
unica, busqueda, modificacion sin alterar la clave, baja con los tres casos y
la raiz, listado InOrden, conteo y liberacion total de memoria. La persistencia
binaria agrega continuidad entre ejecuciones sin alterar el diseno del ABB.

La implementacion permitio aplicar de forma integrada los contenidos de la
materia: punteros y doble puntero, reserva y liberacion de memoria, recursion,
tipos de datos abstractos y analisis de complejidad. La modularizacion facilito
las pruebas y la correccion de cada operacion por separado.

Como limitaciones, el arbol no se balancea: si las claves se insertan
ordenadas, la altura crece y las operaciones se degradan a O(n). Ademas, el
formato binario depende del compilador que lo genero, por lo que no esta
pensado para intercambiar archivos entre plataformas distintas.

Como trabajo futuro se podria incorporar un arbol balanceado (AVL), estadisticas
por area, exportacion del listado a texto y una interface con pausas entre
pantallas.

## 8. Bibliografia

- Material de catedra de Algoritmos y Estructuras de Datos II, FaCENA-UNNE:
  reglas generales y de estilo, punteros y memoria dinamica, recursividad,
  arboles binarios y arboles binarios de busqueda.
- ISO/IEC 9899:1999. *Programming languages - C*.
- Kernighan, B. W. y Ritchie, D. M. (1988). *The C Programming Language*.
  Prentice Hall.

## 9. Anexos

### Anexo A. Estructura del proyecto

```text
Corpus/
├── main.c
├── pacientes.h
├── pacientes.c
├── arbol_pacientes.h
├── arbol_pacientes.c
├── archivo_pacientes.h
├── archivo_pacientes.c
├── validaciones.h
├── validaciones.c
├── pacientes.dat
├── pruebas/
│   └── casos_prueba.txt
├── docs/
│   ├── Corpus.jpg
│   └── informe.md
├── README.md
└── rules/
```

### Anexo B. Menu del sistema

```text
1. Registrar paciente
2. Buscar paciente
3. Modificar paciente
4. Eliminar paciente
5. Listar pacientes en orden ascendente
6. Mostrar cantidad de pacientes
0. Salir
```

### Anexo C. Compilacion en Dev-C++

1. Abrir el proyecto `Corpus.dev`.
2. Verificar que esten incluidos los cinco archivos `.c`.
3. Ejecutar **Execute - Rebuild All**.
4. Ejecutar el programa y operar con el menu.

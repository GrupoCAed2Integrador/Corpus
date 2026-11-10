<p align="center">
  <img src="docs/Corpus.jpg" alt="Logo del sistema Corpus" width="180">
</p>

<h1 align="center">Corpus - Sistema de Gestión de Pacientes de un Hospital</h1>

<p align="center">
  Trabajo práctico integrador de <strong>Algoritmos y Estructuras de Datos II</strong><br>
  FaCENA — Universidad Nacional del Nordeste (UNNE)
</p>

## Integrantes y roles

| Integrante | Rol principal |
| :--- | :--- |
| Alan | Análisis, requisitos y diccionario de datos. |
| Benja | Diseño de tipos y archivos `.h`, inicialización e inserción con control de duplicados. |
| Mariano | Búsqueda recursiva y modificación de datos. |
| Santino | Eliminación recursiva con los tres casos y eliminación de la raíz. |
| Brian | Recorrido InOrden, conteo y validaciones. |
| Gastón | `main.c`, menú, integración, pruebas generales, README e informe. |

## Descripción

Aplicación de consola desarrollada en lenguaje C para administrar los pacientes de un hospital. La información se organiza en un **árbol binario de búsqueda (ABB)** ordenado por **número de historia clínica**, que actúa como clave única.

El sistema utiliza únicamente datos ficticios y administrativos. No se almacenan diagnósticos, tratamientos ni datos clínicos.

## Objetivos

- Aplicar los conceptos de punteros, memoria dinámica y recursividad.
- Implementar un tipo de dato abstracto basado en un árbol binario de búsqueda.
- Mantener el código modularizado, comentado y legible.
- Garantizar la validación de entradas y la liberación completa de la memoria.

## Requisitos funcionales

1. Registrar un paciente mediante inserción recursiva en el ABB.
2. Rechazar historias clínicas duplicadas.
3. Buscar un paciente por número de historia clínica.
4. Modificar los datos de un paciente sin alterar su clave.
5. Eliminar un paciente contemplando los tres casos:
   - nodo hoja;
   - nodo con un solo hijo;
   - nodo con dos hijos, usando el sucesor InOrden o el predecesor.
   La eliminación de la raíz debe funcionar correctamente.
6. Listar los pacientes en orden ascendente mediante un recorrido InOrden.
7. Informar la cantidad total de pacientes con un conteo recursivo.
8. Repetir el menú hasta que el usuario elija salir y liberar toda la memoria antes de finalizar.

## Requisitos no funcionales

- Código escrito en C estándar.
- Organización modular y estrategia Top-Down.
- El `main` solo coordina el menú y las llamadas a los módulos.
- Sin variables globales.
- Validación de historias clínicas no positivas, edades fuera de rango y cadenas vacías.
- Verificación del resultado de `malloc`.
- Prevención de accesos a memoria inválida, punteros colgantes y fugas de memoria.
- Mensajes claros y consistentes para el usuario.
- Código y comentarios sin tildes ni letras `ñ`, según la convención de la cátedra.

## Estructura de datos

### Paciente

```c
#define MAX_NOMBRE 60
#define MAX_DNI 16
#define MAX_TELEFONO 24
#define MAX_AREA 40
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

El DNI se almacena como texto para conservar los ceros iniciales. El teléfono es opcional.

### Nodo del árbol

```c
typedef struct nodoPaciente {
    tPaciente dato;
    struct nodoPaciente *izquierdo;
    struct nodoPaciente *derecho;
} tNodoPaciente;

typedef tNodoPaciente *tArbolPacientes;
```

`tArbolPacientes` es un alias del puntero a la raíz. De esta manera, las funciones que modifican la estructura reciben `tArbolPacientes *`, mientras que las funciones de consulta reciben `tArbolPacientes` por valor.

### Propiedad de orden del ABB

Para cada nodo del árbol:

- todas las claves del subárbol izquierdo son menores que la clave del nodo;
- todas las claves del subárbol derecho son mayores que la clave del nodo;
- no se permiten claves repetidas.

Ejemplo:

```text
          150
         /   \
       90    210
      /  \   /  \
    60  120 180 240
```

El recorrido InOrden de este árbol produce: `60, 90, 120, 150, 180, 210, 240`.

## Menú principal

```text
1. Registrar paciente
2. Buscar paciente
3. Modificar paciente
4. Eliminar paciente
5. Listar pacientes en orden ascendente
6. Mostrar cantidad de pacientes
0. Salir
```

## Organización del proyecto

Organización de archivos:

```text
Corpus/
├── main.c                  # Menu y coordinacion general
├── pacientes.h             # Tipos y prototipos del paciente
├── pacientes.c             # Carga, muestra y modificacion de datos
├── arbol_pacientes.h       # Tipos y prototipos del ABB
├── arbol_pacientes.c       # Operaciones del ABB
├── archivo_pacientes.h     # Prototipos de persistencia
├── archivo_pacientes.c     # Carga y guardado del archivo binario
├── validaciones.h          # Prototipos de validacion
├── validaciones.c          # Implementacion de validaciones
├── pacientes.dat           # Datos del sistema (generado al salir)
├── pruebas/
│   └── casos_prueba.txt    # Casos de prueba del sistema
├── docs/
│   └── Corpus.jpg          # Logo del sistema
├── README.md               # Documentacion del repositorio
└── rules/                  # Normas y material de catedra
```

La entrega final puede incluir una copia de este documento como `README.txt`, según lo solicitado por la cátedra.

## Operaciones del ABB

| Operación | Estrategia | Complejidad promedio |
| :--- | :--- | :--- |
| Inicialización | Asignar `NULL` a la raíz. | O(1) |
| Alta | Inserción recursiva según la clave; rechazo de duplicados. | O(log n) |
| Búsqueda | Descenso recursivo por una sola rama. | O(log n) |
| Modificación | Búsqueda del nodo y actualización de los campos no clave. | O(log n) |
| Baja | Tres casos; con dos hijos se usa el mínimo del subárbol derecho. | O(log n) |
| Listado | Recorrido InOrden. | O(n) |
| Conteo | `1 + contar(izquierdo) + contar(derecho)`. | O(n) |
| Liberación | Recorrido en postorden con `free`. | O(n) |

En el peor caso, cuando el árbol queda degenerado, las operaciones de búsqueda, alta y baja pueden degradarse a O(n).

## Persistencia

Los pacientes se guardan automáticamente en `pacientes.dat` al elegir la opción `0` (o al finalizar la entrada por teclado) y se cargan al iniciar el programa.

- El archivo es binario: se escribe un `tPaciente` completo por registro con `fwrite` y se lee con `fread`.
- Se guarda en preorden, de modo que al reinsertar los registros se reconstruye la misma forma del árbol.
- No se guardan los punteros `izquierdo` y `derecho`: al cargar, el árbol se reconstruye insertando cada paciente.
- Si el archivo no existe (primera ejecución), el sistema inicia con un árbol vacío.
- Al leer, se validan los datos y se omiten los registros inválidos o con historia duplicada, informando la cantidad omitida.
- El archivo se crea en el directorio de trabajo desde el que se ejecuta el programa, normalmente la carpeta del proyecto.
- El tamaño de `tPaciente` depende del compilador, por lo que el archivo debe abrirse con el mismo compilador que lo generó.

## Compilación y ejecución

Con GCC:

```bash
gcc -std=c11 -Wall -Wextra main.c pacientes.c arbol_pacientes.c archivo_pacientes.c validaciones.c -o sistema_pacientes
```

En Windows:

```bash
gcc -std=c11 -Wall -Wextra main.c pacientes.c arbol_pacientes.c archivo_pacientes.c validaciones.c -o sistema_pacientes.exe
```

Ejecución:

```bash
./sistema_pacientes
```

o, en Windows:

```powershell
.\sistema_pacientes.exe
```

## Plan de pruebas

- [ ] Operar sobre un árbol vacío.
- [ ] Dar de alta el paciente inicial con historia clínica 150.
- [ ] Dar de alta un paciente menor, con historia 90.
- [ ] Dar de alta un paciente mayor, con historia 210.
- [ ] Intentar registrar una historia clínica duplicada.
- [ ] Buscar una historia existente.
- [ ] Buscar una historia inexistente.
- [ ] Verificar que el listado InOrden sea ascendente.
- [ ] Eliminar un nodo hoja.
- [ ] Eliminar un nodo con un solo hijo.
- [ ] Eliminar un nodo con dos hijos.
- [ ] Eliminar la raíz del árbol.
- [ ] Probar historias no positivas, edades inválidas y cadenas vacías.
- [ ] Verificar que toda la memoria se libere al salir.
- [ ] Verificar que al salir se genere `pacientes.dat` y al reabrir se recuperen los datos.
- [ ] Verificar que la primera ejecución, sin archivo, inicie con el árbol vacío.
- [ ] Verificar que se omitan los registros inválidos al cargar un archivo dañado.

## Convenciones de código

- Tipos personalizados con prefijo `t`: `tPaciente`, `tNodoPaciente`, `tArbolPacientes`.
- Parámetros formales con prefijo `p`: `pRaiz`, `pPaciente`, `pHistoria`.
- Constantes con `#define` y nombres en mayúsculas.
- Prototipos declarados en los archivos `.h`.
- Implementación en los archivos `.c`.
- Uso de `malloc` con casting explícito y verificación de `NULL`.
- Uso de `free` y asignación posterior de `NULL`.
- Liberación del árbol mediante recorrido en postorden.
- Comentarios claros y sin caracteres especiales.

## Estado del proyecto

- [x] Lectura de las normas de cátedra y definición inicial de la estructura.
- [x] Implementación de `pacientes.h` y `pacientes.c`.
- [x] Implementación de `validaciones.h` y `validaciones.c`.
- [x] Primera parte de `arbol_pacientes.h` y `arbol_pacientes.c`: inicialización, alta con control de duplicados, búsqueda, recorrido InOrden, conteo y liberación.
- [x] Integración del menú en `main.c` para alta, búsqueda, listado, cantidad y salida.
- [x] Implementación de la modificación de datos sin alterar la clave.
- [x] Implementación de la eliminación con los tres casos y de la raíz.
- [x] Integración de la modificación y la eliminación en el menú.
- [x] Persistencia automática en `pacientes.dat`: carga al iniciar y guardado al salir.
- [ ] Ejecución completa del plan de pruebas en Dev-C++.
- [ ] Redacción del informe final.

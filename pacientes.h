#ifndef PACIENTES_H
#define PACIENTES_H

#include <stdbool.h>

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

bool cargarDatosPaciente(tPaciente *pPaciente, int pHistoriaClinica);
void mostrarPaciente(tPaciente pPaciente);
void mostrarEncabezadoPacientes(void);

#endif

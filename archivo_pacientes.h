#ifndef ARCHIVO_PACIENTES_H
#define ARCHIVO_PACIENTES_H

#include <stdbool.h>

#include "arbol_pacientes.h"

#define ARCHIVO_PACIENTES "pacientes.dat"

bool cargarPacientesDesdeArchivo(tArbolPacientes *pRaiz, const char *pNombreArchivo);
bool guardarPacientesEnArchivo(tArbolPacientes pRaiz, const char *pNombreArchivo);

#endif

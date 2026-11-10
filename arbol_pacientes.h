#ifndef ARBOL_PACIENTES_H
#define ARBOL_PACIENTES_H

#include <stdbool.h>
#include "pacientes.h"

typedef struct nodoPaciente {
    tPaciente dato;
    struct nodoPaciente *izquierdo;
    struct nodoPaciente *derecho;
} tNodoPaciente;

typedef tNodoPaciente *tArbolPacientes;

void iniciarArbol(tArbolPacientes *pRaiz);
bool arbolVacio(tArbolPacientes pRaiz);
bool insertarPaciente(tArbolPacientes *pRaiz, tPaciente pPaciente);
tPaciente *buscarPaciente(tArbolPacientes pRaiz, int pHistoriaClinica);
void mostrarInOrden(tArbolPacientes pRaiz);
int contarPacientes(tArbolPacientes pRaiz);
void liberarArbol(tArbolPacientes *pRaiz);

#endif

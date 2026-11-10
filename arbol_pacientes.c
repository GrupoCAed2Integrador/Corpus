#include "arbol_pacientes.h"
#include "validaciones.h"

#include <stdio.h>
#include <stdlib.h>

static tArbolPacientes crearNodoPaciente(tPaciente pPaciente) {
    tArbolPacientes nuevoNodo;

    nuevoNodo = (tArbolPacientes) malloc(sizeof(tNodoPaciente));
    if (nuevoNodo != NULL) {
        nuevoNodo->dato = pPaciente;
        nuevoNodo->izquierdo = NULL;
        nuevoNodo->derecho = NULL;
    }
    return nuevoNodo;
}

void iniciarArbol(tArbolPacientes *pRaiz) {
    if (pRaiz != NULL) {
        *pRaiz = NULL;
    }
}

bool arbolVacio(tArbolPacientes pRaiz) {
    return pRaiz == NULL;
}

bool insertarPaciente(tArbolPacientes *pRaiz, tPaciente pPaciente) {
    if (pRaiz == NULL) {
        return false;
    }

    if (!validarHistoriaClinica(pPaciente.historiaClinica)) {
        printf("Error: la historia clinica debe ser un entero positivo.\n");
        return false;
    }

    if (arbolVacio(*pRaiz)) {
        *pRaiz = crearNodoPaciente(pPaciente);
        if (*pRaiz == NULL) {
            printf("Error: no hay memoria disponible para registrar al paciente.\n");
            return false;
        }
        return true;
    }

    if (pPaciente.historiaClinica < (*pRaiz)->dato.historiaClinica) {
        return insertarPaciente(&((*pRaiz)->izquierdo), pPaciente);
    } else if (pPaciente.historiaClinica > (*pRaiz)->dato.historiaClinica) {
        return insertarPaciente(&((*pRaiz)->derecho), pPaciente);
    } else {
        printf("Error: la historia clinica %d ya existe.\n", pPaciente.historiaClinica);
        return false;
    }
}

tPaciente *buscarPaciente(tArbolPacientes pRaiz, int pHistoriaClinica) {
    if (arbolVacio(pRaiz)) {
        return NULL;
    }

    if (pHistoriaClinica < pRaiz->dato.historiaClinica) {
        return buscarPaciente(pRaiz->izquierdo, pHistoriaClinica);
    } else if (pHistoriaClinica > pRaiz->dato.historiaClinica) {
        return buscarPaciente(pRaiz->derecho, pHistoriaClinica);
    } else {
        return &(pRaiz->dato);
    }
}

static void mostrarInOrdenAux(tArbolPacientes pRaiz) {
    if (!arbolVacio(pRaiz)) {
        mostrarInOrdenAux(pRaiz->izquierdo);
        mostrarPaciente(pRaiz->dato);
        mostrarInOrdenAux(pRaiz->derecho);
    }
}

void mostrarInOrden(tArbolPacientes pRaiz) {
    if (arbolVacio(pRaiz)) {
        printf("\nNo hay pacientes registrados.\n");
        return;
    }

    printf("\n--- Pacientes en orden ascendente por historia clinica ---\n");
    mostrarEncabezadoPacientes();
    mostrarInOrdenAux(pRaiz);
}

int contarPacientes(tArbolPacientes pRaiz) {
    if (arbolVacio(pRaiz)) {
        return 0;
    }
    return 1 + contarPacientes(pRaiz->izquierdo) + contarPacientes(pRaiz->derecho);
}

void liberarArbol(tArbolPacientes *pRaiz) {
    if (pRaiz == NULL) {
        return;
    }

    if (!arbolVacio(*pRaiz)) {
        liberarArbol(&((*pRaiz)->izquierdo));
        liberarArbol(&((*pRaiz)->derecho));
        free(*pRaiz);
        *pRaiz = NULL;
    }
}

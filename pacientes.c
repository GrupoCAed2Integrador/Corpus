#include "pacientes.h"
#include "validaciones.h"

#include <stdio.h>

bool cargarDatosPaciente(tPaciente *pPaciente, int pHistoriaClinica) {
    if (pPaciente == NULL) {
        return false;
    }

    if (!validarHistoriaClinica(pHistoriaClinica)) {
        printf("Error: la historia clinica debe ser un entero positivo.\n");
        return false;
    }

    pPaciente->historiaClinica = pHistoriaClinica;

    do {
        if (!leerCadena("Nombre: ", pPaciente->nombre, MAX_NOMBRE)) {
            return false;
        }
        if (!validarNombre(pPaciente->nombre)) {
            printf("Error: el nombre no es valido. Use solo letras y espacios.\n");
        }
    } while (!validarNombre(pPaciente->nombre));

    do {
        if (!leerCadena("DNI: ", pPaciente->dni, MAX_DNI)) {
            return false;
        }
        if (!validarDni(pPaciente->dni)) {
            printf("Error: el DNI debe contener 7 u 8 digitos.\n");
        }
    } while (!validarDni(pPaciente->dni));

    if (!leerEnteroEnRango("Edad: ", &pPaciente->edad, EDAD_MINIMA, EDAD_MAXIMA)) {
        return false;
    }

    do {
        if (!leerCadena("Telefono (opcional): ", pPaciente->telefono, MAX_TELEFONO)) {
            return false;
        }
        if (!validarTelefono(pPaciente->telefono)) {
            printf("Error: telefono invalido. Use digitos, espacios, +, - o parentesis.\n");
        }
    } while (!validarTelefono(pPaciente->telefono));

    do {
        if (!leerCadena("Area: ", pPaciente->area, MAX_AREA)) {
            return false;
        }
        if (!validarArea(pPaciente->area)) {
            printf("Error: el area no puede estar vacia.\n");
        }
    } while (!validarArea(pPaciente->area));

    return true;
}

void mostrarEncabezadoPacientes(void) {
    printf("%-10s %-22s %-10s %-3s %-14s %-16s\n",
           "Historia", "Nombre", "DNI", "Edad", "Telefono", "Area");
    printf("%-10s %-22s %-10s %-3s %-14s %-16s\n",
           "----------", "----------------------", "----------", "---",
           "--------------", "----------------");
}

void mostrarPaciente(tPaciente pPaciente) {
    const char *telefono;

    telefono = (pPaciente.telefono[0] != '\0') ? pPaciente.telefono : "-";

    printf("%-10d %-22.22s %-10.10s %-3d %-14.14s %-16.16s\n",
           pPaciente.historiaClinica,
           pPaciente.nombre,
           pPaciente.dni,
           pPaciente.edad,
           telefono,
           pPaciente.area);
}

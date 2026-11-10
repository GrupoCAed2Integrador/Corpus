#include "pacientes.h"
#include "validaciones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Copia una cadena controlando el tamanio del destino. */
static void copiarCadena(char *pDestino, int pTamanio, const char *pOrigen) {
    size_t longitud;

    longitud = strlen(pOrigen);
    if (longitud >= (size_t) pTamanio) {
        longitud = (size_t) pTamanio - 1;
    }
    memcpy(pDestino, pOrigen, longitud);
    pDestino[longitud] = '\0';
}

/* Lee un campo de texto. Si el usuario presiona Enter se conserva el valor
   actual. Si pPermiteLimpiar es verdadero, el caracter '-' borra el campo. */
static bool leerCampoTexto(const char *pMensaje, char *pDestino, int pTamanio,
                           bool (*pValidador)(const char *),
                           const char *pMensajeError, bool pPermiteLimpiar) {
    char entrada[MAX_NOMBRE];

    do {
        if (!leerCadena(pMensaje, entrada, (int) sizeof(entrada))) {
            return false;
        }

        if (entrada[0] == '\0') {
            return true;
        }

        if (pPermiteLimpiar && strcmp(entrada, "-") == 0) {
            pDestino[0] = '\0';
            return true;
        }

        if (pValidador(entrada)) {
            copiarCadena(pDestino, pTamanio, entrada);
            return true;
        }

        printf("%s", pMensajeError);
    } while (true);
}

/* Lee la edad mostrando el valor actual. Enter conserva el valor previo. */
static bool leerEdadModificada(tPaciente *pPaciente) {
    char entrada[16];
    char *fin;
    long valor;

    while (true) {
        printf("Edad [%d]: ", pPaciente->edad);
        if (!leerCadena("", entrada, (int) sizeof(entrada))) {
            return false;
        }

        if (entrada[0] == '\0') {
            return true;
        }

        valor = strtol(entrada, &fin, 10);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }

        if (fin != entrada && *fin == '\0' &&
            valor >= EDAD_MINIMA && valor <= EDAD_MAXIMA) {
            pPaciente->edad = (int) valor;
            return true;
        }

        printf("Error: la edad debe ser un numero entre %d y %d.\n",
               EDAD_MINIMA, EDAD_MAXIMA);
    }
}

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

bool modificarDatosPaciente(tPaciente *pPaciente) {
    char mensaje[100];

    if (pPaciente == NULL) {
        return false;
    }

    printf("\nDeje vacio un campo para conservar el valor actual.\n");
    printf("La historia clinica no se modifica.\n");

    snprintf(mensaje, sizeof(mensaje), "Nombre [%s]: ", pPaciente->nombre);
    if (!leerCampoTexto(mensaje, pPaciente->nombre, MAX_NOMBRE, validarNombre,
                        "Error: el nombre no es valido. Use solo letras y espacios.\n",
                        false)) {
        return false;
    }

    snprintf(mensaje, sizeof(mensaje), "DNI [%s]: ", pPaciente->dni);
    if (!leerCampoTexto(mensaje, pPaciente->dni, MAX_DNI, validarDni,
                        "Error: el DNI debe contener 7 u 8 digitos.\n", false)) {
        return false;
    }

    if (!leerEdadModificada(pPaciente)) {
        return false;
    }

    snprintf(mensaje, sizeof(mensaje), "Telefono [%s]: ",
             (pPaciente->telefono[0] != '\0') ? pPaciente->telefono : "-");
    if (!leerCampoTexto(mensaje, pPaciente->telefono, MAX_TELEFONO, validarTelefono,
                        "Error: telefono invalido. Use digitos, espacios, +, - o parentesis.\n",
                        true)) {
        return false;
    }

    snprintf(mensaje, sizeof(mensaje), "Area [%s]: ", pPaciente->area);
    if (!leerCampoTexto(mensaje, pPaciente->area, MAX_AREA, validarArea,
                        "Error: el area no puede estar vacia.\n", false)) {
        return false;
    }

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

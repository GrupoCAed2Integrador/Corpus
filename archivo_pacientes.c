#include "archivo_pacientes.h"
#include "validaciones.h"

#include <stdio.h>

/* Verifica que los datos leidos del archivo sean coherentes antes de
   insertarlos en el arbol. */
static bool pacienteValido(const tPaciente *pPaciente) {
    if (pPaciente == NULL) {
        return false;
    }

    return validarHistoriaClinica(pPaciente->historiaClinica) &&
           validarEdad(pPaciente->edad) &&
           validarNombre(pPaciente->nombre) &&
           validarDni(pPaciente->dni) &&
           validarTelefono(pPaciente->telefono) &&
           validarArea(pPaciente->area);
}

/* Recorre el arbol en preorden y escribe un registro por paciente.
   Al cargarse en ese mismo orden, el arbol recupera su forma original. */
static bool escribirPreOrden(tArbolPacientes pRaiz, FILE *pArchivo) {
    if (arbolVacio(pRaiz)) {
        return true;
    }

    if (fwrite(&(pRaiz->dato), sizeof(tPaciente), 1, pArchivo) != 1) {
        return false;
    }

    if (!escribirPreOrden(pRaiz->izquierdo, pArchivo)) {
        return false;
    }

    return escribirPreOrden(pRaiz->derecho, pArchivo);
}

bool cargarPacientesDesdeArchivo(tArbolPacientes *pRaiz, const char *pNombreArchivo) {
    FILE *archivo;
    tPaciente paciente;
    size_t leidos;
    int omitidos;

    if (pRaiz == NULL || pNombreArchivo == NULL) {
        return false;
    }

    archivo = fopen(pNombreArchivo, "rb");
    if (archivo == NULL) {
        /* El archivo todavia no existe: es la primera ejecucion. */
        return true;
    }

    omitidos = 0;

    while (true) {
        leidos = fread(&paciente, sizeof(tPaciente), 1, archivo);

        if (leidos != 1) {
            if (ferror(archivo)) {
                printf("Error: no se pudo leer el archivo %s.\n", pNombreArchivo);
                fclose(archivo);
                return false;
            }
            break;
        }

        /* Proteccion ante archivos danados: asegura el fin de cadena. */
        paciente.nombre[MAX_NOMBRE - 1] = '\0';
        paciente.dni[MAX_DNI - 1] = '\0';
        paciente.telefono[MAX_TELEFONO - 1] = '\0';
        paciente.area[MAX_AREA - 1] = '\0';

        if (!pacienteValido(&paciente) ||
            buscarPaciente(*pRaiz, paciente.historiaClinica) != NULL) {
            omitidos++;
            continue;
        }

        if (!insertarPaciente(pRaiz, paciente)) {
            fclose(archivo);
            return false;
        }
    }

    fclose(archivo);

    if (omitidos > 0) {
        printf("Aviso: se omitieron %d registro(s) invalidos o duplicados de %s.\n",
               omitidos, pNombreArchivo);
    }

    return true;
}

bool guardarPacientesEnArchivo(tArbolPacientes pRaiz, const char *pNombreArchivo) {
    FILE *archivo;
    bool guardadoCorrectamente;

    if (pNombreArchivo == NULL) {
        return false;
    }

    archivo = fopen(pNombreArchivo, "wb");
    if (archivo == NULL) {
        printf("Error: no se pudo abrir %s para guardar.\n", pNombreArchivo);
        return false;
    }

    guardadoCorrectamente = escribirPreOrden(pRaiz, archivo);

    if (fclose(archivo) != 0) {
        guardadoCorrectamente = false;
    }

    if (!guardadoCorrectamente) {
        printf("Error: no se pudieron guardar todos los datos en %s.\n",
               pNombreArchivo);
    }

    return guardadoCorrectamente;
}

#include <stdio.h>

#include "arbol_pacientes.h"
#include "pacientes.h"
#include "validaciones.h"

void mostrarMenu(void);
void registrarPaciente(tArbolPacientes *pRaiz);
void buscarPacienteMenu(tArbolPacientes pRaiz);
void modificarPacienteMenu(tArbolPacientes pRaiz);
void eliminarPacienteMenu(tArbolPacientes *pRaiz);
void listarPacientes(tArbolPacientes pRaiz);
void mostrarCantidadPacientes(tArbolPacientes pRaiz);

int main(void) {
    tArbolPacientes arbol;
    int opcion;

    iniciarArbol(&arbol);
    opcion = -1;

    do {
        mostrarMenu();

        if (!leerEnteroEnRango("Opcion: ", &opcion, 0, 6)) {
            printf("\nEntrada finalizada. Saliendo del sistema...\n");
            break;
        }

        switch (opcion) {
            case 1:
                registrarPaciente(&arbol);
                break;
            case 2:
                buscarPacienteMenu(arbol);
                break;
            case 3:
                modificarPacienteMenu(arbol);
                break;
            case 4:
                eliminarPacienteMenu(&arbol);
                break;
            case 5:
                listarPacientes(arbol);
                break;
            case 6:
                mostrarCantidadPacientes(arbol);
                break;
            case 0:
                printf("\nSaliendo del sistema...\n");
                break;
            default:
                printf("\nOpcion invalida.\n");
                break;
        }
    } while (opcion != 0);

    liberarArbol(&arbol);
    printf("Memoria liberada correctamente.\n");

    return 0;
}

void mostrarMenu(void) {
    printf("\n==============================================\n");
    printf("   SISTEMA DE GESTION DE PACIENTES\n");
    printf("==============================================\n");
    printf("1. Registrar paciente\n");
    printf("2. Buscar paciente\n");
    printf("3. Modificar paciente\n");
    printf("4. Eliminar paciente\n");
    printf("5. Listar pacientes en orden ascendente\n");
    printf("6. Mostrar cantidad de pacientes\n");
    printf("0. Salir\n");
    printf("----------------------------------------------\n");
}

void registrarPaciente(tArbolPacientes *pRaiz) {
    tPaciente paciente;
    int historiaClinica;

    printf("\n--- Registro de paciente ---\n");

    if (!leerEnteroEnRango("Historia clinica: ", &historiaClinica, 1, MAX_HISTORIA)) {
        return;
    }

    if (buscarPaciente(*pRaiz, historiaClinica) != NULL) {
        printf("Error: ya existe un paciente con la historia clinica %d.\n",
               historiaClinica);
        return;
    }

    if (!cargarDatosPaciente(&paciente, historiaClinica)) {
        return;
    }

    if (insertarPaciente(pRaiz, paciente)) {
        printf("Paciente registrado correctamente.\n");
    }
}

void buscarPacienteMenu(tArbolPacientes pRaiz) {
    tPaciente *paciente;
    int historiaClinica;

    printf("\n--- Busqueda de paciente ---\n");

    if (!leerEnteroEnRango("Historia clinica: ", &historiaClinica, 1, MAX_HISTORIA)) {
        return;
    }

    paciente = buscarPaciente(pRaiz, historiaClinica);
    if (paciente == NULL) {
        printf("No se encontro un paciente con la historia clinica %d.\n",
               historiaClinica);
    } else {
        printf("\nPaciente encontrado:\n");
        mostrarEncabezadoPacientes();
        mostrarPaciente(*paciente);
    }
}

void modificarPacienteMenu(tArbolPacientes pRaiz) {
    tPaciente *paciente;
    int historiaClinica;

    printf("\n--- Modificacion de paciente ---\n");

    if (!leerEnteroEnRango("Historia clinica: ", &historiaClinica, 1, MAX_HISTORIA)) {
        return;
    }

    paciente = buscarPaciente(pRaiz, historiaClinica);
    if (paciente == NULL) {
        printf("No se encontro un paciente con la historia clinica %d.\n",
               historiaClinica);
        return;
    }

    printf("\nDatos actuales:\n");
    mostrarEncabezadoPacientes();
    mostrarPaciente(*paciente);

    if (!confirmar("\nDesea modificar los datos de este paciente? (s/n): ")) {
        printf("Modificacion cancelada.\n");
        return;
    }

    if (!modificarDatosPaciente(paciente)) {
        printf("No se pudieron modificar los datos del paciente.\n");
        return;
    }

    printf("\nDatos actualizados:\n");
    mostrarEncabezadoPacientes();
    mostrarPaciente(*paciente);
}

void eliminarPacienteMenu(tArbolPacientes *pRaiz) {
    tPaciente *paciente;
    int historiaClinica;

    printf("\n--- Eliminacion de paciente ---\n");

    if (!leerEnteroEnRango("Historia clinica: ", &historiaClinica, 1, MAX_HISTORIA)) {
        return;
    }

    paciente = buscarPaciente(*pRaiz, historiaClinica);
    if (paciente == NULL) {
        printf("No se encontro un paciente con la historia clinica %d.\n",
               historiaClinica);
        return;
    }

    printf("\nPaciente a eliminar:\n");
    mostrarEncabezadoPacientes();
    mostrarPaciente(*paciente);

    if (!confirmar("\nConfirma la eliminacion? (s/n): ")) {
        printf("Eliminacion cancelada.\n");
        return;
    }

    if (eliminarPaciente(pRaiz, historiaClinica)) {
        printf("Paciente eliminado correctamente.\n");
    } else {
        printf("No se pudo eliminar el paciente.\n");
    }
}

void listarPacientes(tArbolPacientes pRaiz) {
    mostrarInOrden(pRaiz);
}

void mostrarCantidadPacientes(tArbolPacientes pRaiz) {
    printf("\nCantidad total de pacientes: %d\n", contarPacientes(pRaiz));
}

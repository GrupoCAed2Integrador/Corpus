#include "validaciones.h"
#include "pacientes.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Limpieza del buffer de entrada de forma clásica y sencilla */
void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Lectura de cadenas clásica usando fgets sin complicaciones */
bool leerCadena(const char *pMensaje, char *pCadena, int pTamanio) {
    int i = 0;
    
    if (pCadena == NULL || pTamanio <= 0) {
        return false;
    }

    printf("%s", pMensaje);
    if (fgets(pCadena, pTamanio, stdin) == NULL) {
        pCadena[0] = '\0';
        return false;
    }

    /* Quitar el salto de línea '\n' si quedó grabado */
    while (pCadena[i] != '\0') {
        if (pCadena[i] == '\n') {
            pCadena[i] = '\0';
            break;
        }
        i++;
    }

    /* Si la cadena se llenó y no encontró el '\n', limpiamos el buffer sobrante */
    if (i == pTamanio - 1 && pCadena[i - 1] != '\0') {
        limpiarBuffer();
    }

    return true;
}

/* Validación de entero básico con scanf tradicional */
bool leerEntero(const char *pMensaje, int *pValor) {
    if (pValor == NULL) {
        return false;
    }

    printf("%s", pMensaje);
    if (scanf("%d", pValor) != 1) {
        limpiarBuffer();
        printf("Error: ingrese un numero entero valido.\n");
        return false;
    }
    limpiarBuffer();
    return true;
}

/* Lectura de entero validando un rango mínimo y máximo */
bool leerEnteroEnRango(const char *pMensaje, int *pValor, int pMinimo, int pMaximo) {
    int valor;

    if (pValor == NULL) {
        return false;
    }

    while (true) {
        if (!leerEntero(pMensaje, &valor)) {
            continue;
        }
        if (valor < pMinimo || valor > pMaximo) {
            printf("Error: el valor debe estar entre %d y %d.\n", pMinimo, pMaximo);
            continue;
        }
        *pValor = valor;
        return true;
    }
}

/* Validaciones específicas del dominio */
bool validarHistoriaClinica(int pHistoria) {
    return (pHistoria >= 1 && pHistoria <= MAX_HISTORIA);
}

bool validarEdad(int pEdad) {
    return (pEdad >= EDAD_MINIMA && pEdad <= EDAD_MAXIMA);
}

bool cadenaVacia(const char *pCadena) {
    if (pCadena == NULL || pCadena[0] == '\0') {
        return true;
    }
    return false;
}

bool validarNombre(const char *pNombre) {
    int i;
    if (cadenaVacia(pNombre)) {
        return false;
    }

    for (i = 0; pNombre[i] != '\0'; i++) {
        char c = pNombre[i];
        /* Acepta letras mayúsculas, minúsculas y espacios */
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')) {
            return false;
        }
    }
    return true;
}

bool validarDni(const char *pDni) {
    int i = 0;
    int longitud = 0;

    if (pDni == NULL) {
        return false;
    }

    while (pDni[longitud] != '\0') {
        longitud++;
    }

    if (longitud < 7 || longitud > 8) {
        return false;
    }

    for (i = 0; i < longitud; i++) {
        if (pDni[i] < '0' || pDni[i] > '9') {
            return false;
        }
    }
    return true;
}

bool validarTelefono(const char *pTelefono) {
    int i;
    if (pTelefono == NULL || cadenaVacia(pTelefono)) {
        return true; // Es opcional
    }

    for (i = 0; pTelefono[i] != '\0'; i++) {
        char c = pTelefono[i];
        if (!((c >= '0' && c <= '9') || c == ' ' || c == '+' || c == '-' || c == '(' || c == ')')) {
            return false;
        }
    }
    return true;
}

bool validarArea(const char *pArea) {
    return !cadenaVacia(pArea);
}

bool confirmar(const char *pMensaje) {
    char respuesta[10];

    while (true) {
        if (!leerCadena(pMensaje, respuesta, sizeof(respuesta))) {
            return false;
        }
        if (respuesta[0] == 's' || respuesta[0] == 'S') {
            return true;
        }
        if (respuesta[0] == 'n' || respuesta[0] == 'N') {
            return false;
        }
        printf("Error: responda 's' o 'n'.\n");
    }
}
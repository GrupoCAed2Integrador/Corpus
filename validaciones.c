#include "validaciones.h"
#include "pacientes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void descartarLineaRestante(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

bool leerEntero(const char *pMensaje, int *pValor) {
    char linea[64];
    char *fin;
    long valor;
    size_t longitud;
    int i;
    int digitos;

    if (pValor == NULL) {
        return false;
    }

    printf("%s", pMensaje);
    if (fgets(linea, sizeof(linea), stdin) == NULL) {
        return false;
    }

    longitud = strlen(linea);
    if (longitud > 0 && linea[longitud - 1] == '\n') {
        linea[longitud - 1] = '\0';
    } else {
        descartarLineaRestante();
        printf("Error: la entrada es demasiado larga.\n");
        return false;
    }

    longitud = strlen(linea);
    if (longitud > 0 && linea[longitud - 1] == '\r') {
        linea[longitud - 1] = '\0';
    }

    if (linea[0] == '\0') {
        printf("Error: ingrese un numero entero.\n");
        return false;
    }

    i = 0;
    digitos = 0;
    while (linea[i] == ' ' || linea[i] == '\t') {
        i++;
    }
    if (linea[i] == '+' || linea[i] == '-') {
        i++;
    }
    while (linea[i] >= '0' && linea[i] <= '9') {
        digitos++;
        i++;
    }
    if (digitos == 0 || digitos > 9) {
        printf("Error: ingrese un numero entero valido.\n");
        return false;
    }

    valor = strtol(linea, &fin, 10);
    while (*fin == ' ' || *fin == '\t') {
        fin++;
    }
    if (fin == linea || *fin != '\0') {
        printf("Error: ingrese un numero entero valido.\n");
        return false;
    }

    if (valor < -2147483647L || valor > 2147483647L) {
        printf("Error: el numero esta fuera de rango.\n");
        return false;
    }

    *pValor = (int) valor;
    return true;
}

bool leerEnteroEnRango(const char *pMensaje, int *pValor, int pMinimo, int pMaximo) {
    int valor;

    if (pValor == NULL) {
        return false;
    }

    while (true) {
        if (!leerEntero(pMensaje, &valor)) {
            if (feof(stdin) || ferror(stdin)) {
                return false;
            }
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

bool leerCadena(const char *pMensaje, char *pCadena, int pTamanio) {
    size_t longitud;
    int c;

    if (pCadena == NULL || pTamanio <= 0) {
        return false;
    }

    printf("%s", pMensaje);
    if (fgets(pCadena, pTamanio, stdin) == NULL) {
        pCadena[0] = '\0';
        return false;
    }

    longitud = strlen(pCadena);
    if (longitud > 0 && pCadena[longitud - 1] == '\n') {
        pCadena[longitud - 1] = '\0';
    } else {
        c = getchar();
        if (c != '\n' && c != EOF) {
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            pCadena[0] = '\0';
        }
    }

    longitud = strlen(pCadena);
    if (longitud > 0 && pCadena[longitud - 1] == '\r') {
        pCadena[longitud - 1] = '\0';
    }

    return true;
}

bool validarHistoriaClinica(int pHistoria) {
    return pHistoria >= 1 && pHistoria <= MAX_HISTORIA;
}

bool validarEdad(int pEdad) {
    return pEdad >= EDAD_MINIMA && pEdad <= EDAD_MAXIMA;
}

bool cadenaVacia(const char *pCadena) {
    int i;

    if (pCadena == NULL) {
        return true;
    }

    i = 0;
    while (pCadena[i] != '\0') {
        if (pCadena[i] != ' ' && pCadena[i] != '\t' &&
            pCadena[i] != '\r' && pCadena[i] != '\n') {
            return false;
        }
        i++;
    }
    return true;
}

bool validarNombre(const char *pNombre) {
    int i;
    bool tieneLetra;

    if (cadenaVacia(pNombre)) {
        return false;
    }

    tieneLetra = false;
    for (i = 0; pNombre[i] != '\0'; i++) {
        unsigned char c;

        c = (unsigned char) pNombre[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c >= 128) {
            tieneLetra = true;
        } else if (c != ' ' && c != '\'' && c != '-') {
            return false;
        }
    }
    return tieneLetra;
}

bool validarDni(const char *pDni) {
    int i;
    int longitud;

    if (pDni == NULL) {
        return false;
    }

    longitud = (int) strlen(pDni);
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
    bool tieneDigito;

    if (pTelefono == NULL || cadenaVacia(pTelefono)) {
        return true;
    }

    tieneDigito = false;
    for (i = 0; pTelefono[i] != '\0'; i++) {
        char c;

        c = pTelefono[i];
        if (c >= '0' && c <= '9') {
            tieneDigito = true;
        } else if (c != ' ' && c != '+' && c != '-' && c != '(' && c != ')') {
            return false;
        }
    }
    return tieneDigito;
}

bool validarArea(const char *pArea) {
    return !cadenaVacia(pArea);
}

bool confirmar(const char *pMensaje) {
    char respuesta[8];

    while (true) {
        if (!leerCadena(pMensaje, respuesta, (int) sizeof(respuesta))) {
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

#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <stdbool.h>

bool leerEntero(const char *pMensaje, int *pValor);
bool leerEnteroEnRango(const char *pMensaje, int *pValor, int pMinimo, int pMaximo);
bool leerCadena(const char *pMensaje, char *pCadena, int pTamanio);

bool validarHistoriaClinica(int pHistoria);
bool validarEdad(int pEdad);
bool cadenaVacia(const char *pCadena);
bool validarNombre(const char *pNombre);
bool validarDni(const char *pDni);
bool validarTelefono(const char *pTelefono);
bool validarArea(const char *pArea);
bool confirmar(const char *pMensaje);

#endif

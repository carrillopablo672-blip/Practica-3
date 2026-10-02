#ifndef HEADERS_H
#define HEADERS_H

#include <string>

//Ejercicio 1

std::string comprimirRLE(const std::string& texto);

std::string descomprimirRLE(const std::string& comprimido);

bool verificarRLE(const std::string& original, const std::string& recuperado);

//------------------------------------------------------------------------------------

//Ejercicio 2
struct ParLZ78 {
    int indice;
    char caracter;
};

char* leerLinea(int& longitud);

ParLZ78* comprimirLZ78(const char* texto, int longitud, int& cantidadPares);


char* descomprimirLZ78(const ParLZ78* pares, int cantidadPares, int& longitud);

bool verificarLZ78(const char* original, int longitudOriginal, const char* recuperado, int longitudRecuperado);

void imprimirParesLZ78(const ParLZ78* pares, int cantidadPares);

void imprimirDiccionarioLZ78(const ParLZ78* pares, int cantidadPares);

//------------------------------------------------------------------------------------

//Ejercicio 3

unsigned char rotarIzquierda(unsigned char byte, int n);

unsigned char rotarDerecha(unsigned char byte, int n);


unsigned char* encriptar(const unsigned char* datos, int longitud, int n, unsigned char clave);

// Proceso inverso: XOR con la misma clave y luego rotacion a la derecha de n bits.
unsigned char* desencriptar(const unsigned char* datos, int longitud, int n, unsigned char clave);

bool verificarBytes(const unsigned char* original, const unsigned char* recuperado, int longitud);

// Imprime los 8 bits de un byte (ej. 01000001).
void imprimirBinario(unsigned char byte);

//-------------------------------------------------------------------------------------

//Ejercicio 4


char* leerArchivo(const char* nombre, int& longitud);

void escribirArchivo(const char* nombre, const char* datos, int longitud);

void leerParametrosEncriptacion(int& n, unsigned char& clave);

//-------------------------------------------------------------------------------------

#endif // HEADERS_H

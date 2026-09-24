#ifndef HEADERS_H
#define HEADERS_H

#include <string>

std::string comprimirRLE(const std::string& texto);

std::string descomprimirRLE(const std::string& comprimido);

bool verificarRLE(const std::string& original, const std::string& recuperado);

//------------------------------------------------------------------------------------

struct ParLZ78 {
    int indice;
    char caracter;
};

char* leerLinea(int& longitud);

ParLZ78* comprimirLZ78(const char* texto, int longitud, int& cantidadPares);


char* descomprimirLZ78(const ParLZ78* pares, int cantidadPares, int& longitud);

bool verificarLZ78(const char* original, int longitudOriginal,
                   const char* recuperado, int longitudRecuperado);

void imprimirParesLZ78(const ParLZ78* pares, int cantidadPares);

void imprimirDiccionarioLZ78(const ParLZ78* pares, int cantidadPares);

#endif // HEADERS_H

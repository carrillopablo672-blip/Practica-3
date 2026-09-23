#ifndef HEADERS_H
#define HEADERS_H

#include <string>

std::string comprimirRLE(const std::string& texto);

std::string descomprimirRLE(const std::string& comprimido);

bool verificarRLE(const std::string& original, const std::string& recuperado);

#endif // HEADERS_H

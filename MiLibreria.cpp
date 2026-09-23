#include "headers.h"
#include <stdexcept>

using namespace std;

namespace {

const string::size_type MAX_REPETICIONES = 9;

bool esDigito(char c)
{
    return c >= '0' && c <= '9';
}

} // namespace

string comprimirRLE(const string& texto)
{
    string comprimido;
    const string::size_type longitud = texto.length();
    string::size_type i = 0;

    while (i < longitud) {
        const char simbolo = texto[i];
        string::size_type conteo = 1;

        // Contar repeticiones consecutivas del mismo simbolo (maximo 9 por par)
        while (i + conteo < longitud &&
               texto[i + conteo] == simbolo &&
               conteo < MAX_REPETICIONES) {
            ++conteo;
        }

        comprimido += static_cast<char>('0' + conteo);  // digito del conteo
        comprimido += simbolo;                            // simbolo repetido
        i += conteo;
    }

    return comprimido;
}

string descomprimirRLE(const string& comprimido)
{
    const string::size_type longitud = comprimido.length();

    // Cada par ocupa exactamente 2 caracteres
    if (longitud % 2 != 0) {
        throw invalid_argument(
            "Texto comprimido invalido: la longitud (" + to_string(longitud) +
            ") no es par.");
    }

    string texto;
    for (string::size_type i = 0; i < longitud; i += 2) {
        const char digito = comprimido[i];

        if (!esDigito(digito) || digito == '0') {
            throw invalid_argument(
                "Texto comprimido invalido: se esperaba un conteo entre 1 y 9 en la posicion " +
                to_string(i) + ".");
        }

        const string::size_type conteo = static_cast<string::size_type>(digito - '0');
        texto.append(conteo, comprimido[i + 1]);
    }

    return texto;
}

bool verificarRLE(const string& original, const string& recuperado)
{
    return original == recuperado;
}
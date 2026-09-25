#include "headers.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>

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
        while (i + conteo < longitud && texto[i + conteo] == simbolo && conteo < MAX_REPETICIONES) {

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

//--------------------------------------------------------

namespace {

// Busca en el diccionario la entrada (prefijo, caracter).
// Retorna su indice o -1 si no existe. La entrada 0 es la frase vacia.
int buscarEntrada(const ParLZ78* diccionario, int tamano, int prefijo, char caracter)
{
    for (int j = 1; j < tamano; j++) {
        if (diccionario[j].indice == prefijo && diccionario[j].caracter == caracter) {
            return j;
        }
    }
    return -1;
}

// Duplica la capacidad del diccionario: reserva un arreglo nuevo,
// copia las entradas y libera el anterior.
void ampliarDiccionario(ParLZ78*& diccionario, int& capacidad, int tamano)
{
    const int nuevaCapacidad = capacidad * 2;
    ParLZ78* nuevo = new ParLZ78[nuevaCapacidad];

    for (int j = 0; j < tamano; j++) {
        nuevo[j] = diccionario[j];
    }

    delete[] diccionario;
    diccionario = nuevo;
    capacidad = nuevaCapacidad;
}

// Imprime la frase de la entrada 'indice' siguiendo la cadena de prefijos.
void imprimirFrase(const ParLZ78* pares, int indice)
{
    if (indice == 0) {
        return;                                     // frase vacia
    }
    imprimirFrase(pares, pares[indice - 1].indice); // primero el prefijo
    cout << pares[indice - 1].caracter;             // luego el caracter
}

} // namespace

char* leerLinea(int& longitud)
{
    int capacidad = 16;
    char* texto = new char[capacidad];
    longitud = 0;
    char c;

    while (cin.get(c) && c != '\n') {
        if (longitud + 1 >= capacidad) {            // +1 para el '\0' final
            char* nuevo = nullptr;
            try {
                nuevo = new char[capacidad * 2];
            } catch (...) {
                delete[] texto;
                throw;
            }
            for (int j = 0; j < longitud; j++) {
                nuevo[j] = texto[j];
            }
            delete[] texto;
            texto = nuevo;
            capacidad *= 2;
        }
        texto[longitud++] = c;
    }

    if (longitud > 0 && texto[longitud - 1] == '\r') { // fin de linea de Windows
        longitud--;
    }
    texto[longitud] = '\0';
    return texto;
}

ParLZ78* comprimirLZ78(const char* texto, int longitud, int& cantidadPares)
{
    if (texto == nullptr || longitud < 0) {

        throw invalid_argument("Texto invalido para comprimir con LZ78.");
    }

    // El diccionario crece durante la compresion (no se sabe cuantas frases habra).
    int capacidad = 16;

    int tamano = 1;

    ParLZ78* diccionario = new ParLZ78[capacidad];

    diccionario[0].indice = 0;    // entrada 0: frase vacia

    diccionario[0].caracter = '\0';

    // Nunca hay mas pares que caracteres, asi que ese es el tamano maximo de la salida.
    ParLZ78* pares = nullptr;

    try {
        pares = new ParLZ78[longitud > 0 ? longitud : 1];

    } catch (...) {

        delete[] diccionario;

        throw;
    }

    cantidadPares = 0;
    int i = 0;

    try {
        while (i < longitud) {

            // Buscar la frase mas larga del diccionario que empieza en la posicion i
            int actual = 0;

            int siguiente = 0;

            while (i < longitud &&

                   (siguiente = buscarEntrada(diccionario, tamano, actual, texto[i])) != -1) { actual = siguiente; i++;
            }

            if (i < longitud) {

                // Emitir (actual, texto[i]) y agregar la nueva frase al diccionario
                if (tamano == capacidad) {

                    ampliarDiccionario(diccionario, capacidad, tamano);
                }
                diccionario[tamano].indice = actual;

                diccionario[tamano].caracter = texto[i];

                tamano++;

                pares[cantidadPares].indice = actual;

                pares[cantidadPares].caracter = texto[i];

                cantidadPares++;
                i++;
            } else {
                // El texto termino en una frase que ya existe: solo se emite su indice
                pares[cantidadPares].indice = actual;

                pares[cantidadPares].caracter = '\0';

                cantidadPares++;
            }
        }
    } catch (...) {

        delete[] diccionario;

        delete[] pares;

        throw;
    }

    delete[] diccionario;

    return pares;
}

char* descomprimirLZ78(const ParLZ78* pares, int cantidadPares, int& longitud)
{
    if (cantidadPares < 0 || (pares == nullptr && cantidadPares > 0)) {

        throw invalid_argument("Pares invalidos para descomprimir con LZ78.");
    }

    // Validar antes de reservar memoria: el par k solo puede referirse
    // a entradas que ya existen (0..k), y solo el ultimo puede ir sin caracter.
    for (int k = 0; k < cantidadPares; k++) {

        if (pares[k].indice < 0 || pares[k].indice > k) {

            throw invalid_argument("Par " + to_string(k + 1) + " invalido: el indice " +

                                   to_string(pares[k].indice) + " no existe en el diccionario.");
        }
        if (pares[k].caracter == '\0' && k != cantidadPares - 1) {
            throw invalid_argument("Par " + to_string(k + 1) +
                                   " invalido: solo el ultimo par puede ir sin caracter.");
        }
    }

    ParLZ78* diccionario = nullptr;

    int* largo = nullptr;       // largo[j] = longitud de la frase j

    char* texto = nullptr;

    try {
        // Reconstruir el diccionario: la entrada k+1 es el par k
        diccionario = new ParLZ78[cantidadPares + 1];

        largo = new int[cantidadPares + 1];

        diccionario[0].indice = 0;

        diccionario[0].caracter = '\0';

        largo[0] = 0;

        longitud = 0;

        for (int k = 0; k < cantidadPares; k++) {

            diccionario[k + 1] = pares[k];

            const bool sinCaracter = (pares[k].caracter == '\0');

            largo[k + 1] = largo[pares[k].indice] + (sinCaracter ? 0 : 1);

            longitud += largo[k + 1];
        }

        texto = new char[longitud + 1];

    } catch (...) {

        delete[] diccionario;

        delete[] largo;

        throw;
    }

    // Escribir cada frase: se recorre la cadena de prefijos desde el final
    // de la frase hacia el inicio, llenando el texto de derecha a izquierda.
    int posicion = 0;

    for (int k = 0; k < cantidadPares; k++) {

        int entrada = (pares[k].caracter == '\0') ? pares[k].indice : k + 1;

        int p = posicion + largo[k + 1] - 1;

        while (entrada != 0) {

            texto[p] = diccionario[entrada].caracter;
            p--;
            entrada = diccionario[entrada].indice;
        }
        posicion += largo[k + 1];
    }
    texto[longitud] = '\0';

    delete[] diccionario;
    delete[] largo;
    return texto;
}

bool verificarLZ78(const char* original, int longitudOriginal, const char* recuperado, int longitudRecuperado)

{
    if (longitudOriginal != longitudRecuperado) {
        return false;
    }
    for (int i = 0; i < longitudOriginal; i++) {

        if (original[i] != recuperado[i]) {

            return false;
        }
    }
    return true;
}

void imprimirParesLZ78(const ParLZ78* pares, int cantidadPares)
{
    for (int k = 0; k < cantidadPares; k++) {

        if (pares[k].caracter == '\0') {

            cout << "(" << pares[k].indice << ")";

        } else {
            cout << "(" << pares[k].indice << ", " << pares[k].caracter << ")";
        }
        if (k < cantidadPares - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

void imprimirDiccionarioLZ78(const ParLZ78* pares, int cantidadPares)
{
    for (int k = 0; k < cantidadPares; k++) {

        if (pares[k].caracter == '\0') {

            continue;                               // el par final no crea entrada nueva
        }
        cout << k + 1 << " -> ";

        imprimirFrase(pares, k + 1);

        cout << endl;
    }
}

//-------------------------------------------------------------

namespace {

void validarParametros(const unsigned char* datos, int longitud, int n)
{
    if (n <= 0 || n >= 8) {

        throw invalid_argument("El valor de n debe cumplir 0 < n < 8 , se recibio " + to_string(n) + ".");
    }
    if (longitud < 0 || (datos == nullptr && longitud > 0)) {

        throw invalid_argument("Datos invalidos para encriptar o desencriptar.");
    }
}

} // namespace

unsigned char rotarIzquierda(unsigned char byte, int n)
{
    // Los n bits de la izquierda que se pierden con << reaparecen a la derecha con >>
    return static_cast<unsigned char>((byte << n) | (byte >> (8 - n)));
}

unsigned char rotarDerecha(unsigned char byte, int n)
{
    return static_cast<unsigned char>((byte >> n) | (byte << (8 - n)));
}

unsigned char* encriptar(const unsigned char* datos, int longitud, int n, unsigned char clave)
{
    validarParametros(datos, longitud, n);

    unsigned char* resultado = new unsigned char[longitud > 0 ? longitud : 1];

    for (int i = 0; i < longitud; i++) {

        resultado[i] = rotarIzquierda(datos[i], n) ^ clave;     // 1. rotar, 2. XOR
    }
    return resultado;
}

unsigned char* desencriptar(const unsigned char* datos, int longitud, int n, unsigned char clave)
{
    validarParametros(datos, longitud, n);

    unsigned char* resultado = new unsigned char[longitud > 0 ? longitud : 1];

    for (int i = 0; i < longitud; i++) {

        // Orden inverso: primero se deshace el XOR, luego la rotacion
        resultado[i] = rotarDerecha(static_cast<unsigned char>(datos[i] ^ clave), n);
    }
    return resultado;
}

bool verificarBytes(const unsigned char* original, const unsigned char* recuperado, int longitud)
{
    for (int i = 0; i < longitud; i++) {

        if (original[i] != recuperado[i]) {

            return false;
        }
    }
    return true;
}

void imprimirHex(const unsigned char* datos, int longitud)
{
    for (int i = 0; i < longitud; i++) {

        cout << uppercase << hex << setw(2) << setfill('0')

        << static_cast<int>(datos[i]) << " ";

    }
    cout << dec << setfill(' ') << endl;    // se restaura el formato decimal
}

void imprimirBinario(unsigned char byte)
{
    for (int bit = 7; bit >= 0; bit--) {
        cout << ((byte >> bit) & 1);
    }
}
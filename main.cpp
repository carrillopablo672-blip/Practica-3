#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include "headers.h"
#include <iomanip>

using namespace std;

using namespace std;

int main(){

    int ejercicio;

    cout << "Ingrese el numero del ejercicio: ";

    cin >> ejercicio;

    while (ejercicio > 0) {

        switch (ejercicio) {

        case 1:{

            string original;

            cout << "Ingrese una cadena de caracteres: ";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');   // quita el Enter que deja cin >>

            getline(cin, original);                                 // getline conserva los espacios

            try {

                string comprimido = comprimirRLE(original);

                string recuperado = descomprimirRLE(comprimido);

                cout << "Original: " << original << endl;

                cout << "Comprimido: " << comprimido << endl;

                cout << "Recuperado: " << recuperado << endl;

                if (verificarRLE(original, recuperado)){

                    cout << "La descompresion SI coincide con el texto original." << endl;

                }
                else{

                    cout << "La descompresion NO coincide con el texto original." << endl;

                }

            }
            catch (const invalid_argument &error){

                cout << "Error: " << error.what() << endl;

            }

            break;
        }

        case 2:{

            char *original = nullptr;
            ParLZ78 *pares = nullptr;
            char *recuperado = nullptr;

            int longitudOriginal = 0;
            int cantidadPares = 0;
            int longitudRecuperado = 0;

            cout << "Ingrese una cadena de caracteres: ";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');   // quita el Enter que deja cin >>

            try {

                original = leerLinea(longitudOriginal);

                pares = comprimirLZ78(original, longitudOriginal, cantidadPares);

                recuperado = descomprimirLZ78(pares, cantidadPares, longitudRecuperado);

                cout << "Original: " << original << endl;

                cout << "Pares generados (" << cantidadPares << "): ";

                imprimirParesLZ78(pares, cantidadPares);

                cout << "Diccionario:" << endl;

                imprimirDiccionarioLZ78(pares, cantidadPares);

                cout << "Recuperado: " << recuperado << endl;

                if (verificarLZ78(original, longitudOriginal, recuperado, longitudRecuperado)){

                    cout << "La descompresion si coincide con el texto original." << endl;

                }
                else{

                    cout << "La descompresion no coincide con el texto original." << endl;

                }

            }
            catch (const invalid_argument &error){

                cout << "Error: " << error.what() << endl;

            }
            catch (const bad_alloc &){

                cout << "Error: no hay memoria suficiente." << endl;

            }

            delete[] original;      // se libera siempre, haya o no excepcion
            delete[] pares;
            delete[] recuperado;

            break;
        }

        case 3:{

            char *texto = nullptr;
            unsigned char *encriptado = nullptr;
            unsigned char *desencriptado = nullptr;

            int longitud = 0;
            int n = 0;
            int clave = 0;

            cout << "Ingrese una cadena de caracteres: ";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');   // quita el Enter que deja cin >>

            try {

                texto = leerLinea(longitud);

                cout << "Ingrese el numero de bits a rotar n (0 < n < 8): ";

                if (!(cin >> n)){

                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("n debe ser un numero entero.");

                }

                if (n <= 0 || n >= 8){

                    throw invalid_argument("El valor de n debe cumplir 0 < n < 8.");

                }

                cout << "Ingrese la clave K (0 a 255): ";

                if (!(cin >> clave)){

                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("K debe ser un numero entero.");

                }

                if (clave < 0 || clave > 255){

                    throw invalid_argument("La clave K debe ser un byte (0 a 255).");

                }

                const unsigned char K = static_cast<unsigned char>(clave);

                // Los datos se tratan como bytes (unsigned char) para que las
                // operaciones de bits no se vean afectadas por el signo.
                const unsigned char *datos = reinterpret_cast<const unsigned char*>(texto);

                encriptado = encriptar(datos, longitud, n, K);

                desencriptado = desencriptar(encriptado, longitud, n, K);


                if (longitud > 0){

                    // Paso a paso del primer byte, en binario
                    cout << "Primer byte paso a paso:" << endl;

                    cout << "  Original:           ";
                    imprimirBinario(datos[0]);

                    cout << endl << "  Rotado " << n << " bits:      ";
                    imprimirBinario(rotarIzquierda(datos[0], n));

                    cout << endl << "  XOR con K:          ";
                    imprimirBinario(K);

                    cout << endl << "  Encriptado:         ";
                    imprimirBinario(encriptado[0]);

                    cout << endl;

                }

                cout << "Texto recuperado: ";

                for (int i = 0; i < longitud; i++){

                    cout << static_cast<char>(desencriptado[i]);

                }

                cout << endl;

                if (verificarBytes(datos, desencriptado, longitud)){

                    cout << "La desencriptacion si coincide con el texto original." << endl;

                }
                else{

                    cout << "La desencriptacion no coincide con el texto original." << endl;

                }

            }
            catch (const invalid_argument &error){

                cout << "Error: " << error.what() << endl;

            }
            catch (const bad_alloc &){

                cout << "Error: no hay memoria suficiente." << endl;

            }

            delete[] texto;         // se libera siempre, haya o no excepcion
            delete[] encriptado;
            delete[] desencriptado;

            break;
        }

        case 4:{

            char *nombreArchivo = nullptr;
            char *original = nullptr;
            unsigned char *encriptado = nullptr;
            unsigned char *desencriptado = nullptr;
            char *textoFinal = nullptr;

            int longitudNombre = 0;
            int longitudOriginal = 0;
            int longitudComprimido = 0;
            int longitudFinal = 0;
            int n = 0;
            unsigned char K = 0;

            int origen = 0;

            try {


                cout << "De donde quiere tomar el texto?" << endl;

                cout << "  1. Escribirlo por teclado" << endl;

                cout << "  2. Leerlo desde un archivo .txt" << endl;

                cout << "Opcion: ";

                if (!(cin >> origen)){

                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw invalid_argument("La opcion debe ser 1 o 2.");

                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');   // quita el Enter que deja cin >>

                switch (origen){

                case 1:{

                    cout << "Ingrese el texto: ";

                    original = leerLinea(longitudOriginal);

                    break;
                }

                case 2:{

                    cout << "Ingrese el nombre del archivo (es : entrada.txt jeje): ";

                    nombreArchivo = leerLinea(longitudNombre);

                    original = leerArchivo(nombreArchivo, longitudOriginal);

                    cout << "Se leyeron " << longitudOriginal << " caracteres de " << nombreArchivo << "." << endl;

                    break;
                }

                default:

                    throw invalid_argument("La opcion debe ser 1 o 2.");
                }

                leerParametrosEncriptacion(n, K);

                // 2. Comprimir con RLE
                string comprimido = comprimirRLE(string(original, longitudOriginal));

                longitudComprimido = static_cast<int>(comprimido.length());

                // 3. Encriptar el resultado comprimido y guardarlo en un archivo
                encriptado = encriptar(reinterpret_cast<const unsigned char*>(comprimido.data()), longitudComprimido, n, K);

                escribirArchivo("encriptado.bin", reinterpret_cast<const char*>(encriptado), longitudComprimido);

                // 4. Desencriptar
                desencriptado = desencriptar(encriptado, longitudComprimido, n, K);

                // 5. Descomprimir
                string recuperado = descomprimirRLE(string(reinterpret_cast<const char*>(desencriptado), longitudComprimido));

                // 6. Imprimir el texto final en un archivo
                escribirArchivo("salida.txt", recuperado.data(), static_cast<int>(recuperado.length()));

                // 7. Leer de nuevo el archivo de salida para verificar lo que realmente quedo escrito
                textoFinal = leerArchivo("salida.txt", longitudFinal);

                cout << endl << "Tamano original:     " << longitudOriginal << " bytes" << endl;

                cout << "El texto original es: " << original;

                cout << "El cmprimido es: " <<comprimido;

                cout << "Tamano comprimido:   " << longitudComprimido << " bytes" << endl;




                if (longitudOriginal == longitudFinal &&
                    verificarBytes(reinterpret_cast<const unsigned char*>(original), reinterpret_cast<const unsigned char*>(textoFinal), longitudOriginal)){

                    cout << "El texto de salida.txt si coincide con el original." << endl;

                }
                else{

                    cout << "El texto de salida.txt no coincide con el original." << endl;

                }

            }
            catch (const invalid_argument &error){

                cout << "Error: " << error.what() << endl;

            }
            catch (const runtime_error &error){

                cout << "Error: " << error.what() << endl;

            }
            catch (const bad_alloc &){

                cout << "Error: no hay memoria suficiente." << endl;

            }

            delete[] nombreArchivo;     // se libera siempre, haya o no excepcion
            delete[] original;
            delete[] encriptado;
            delete[] desencriptado;
            delete[] textoFinal;

            break;
        }

        default:

            cout << "Ese ejercicio no existe." << endl;
        }
        cout << endl;
        cout << "Ingrese el numero del ejercicio: ";
        cin >> ejercicio;

    }

    cout << "Programa terminado." << endl;
    return 0;
}
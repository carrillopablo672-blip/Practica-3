#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include "headers.h"

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

                cout << "Original (hex):      ";
                imprimirHex(datos, longitud);

                cout << "Encriptado (hex):    ";
                imprimirHex(encriptado, longitud);

                cout << "Desencriptado (hex): ";
                imprimirHex(desencriptado, longitud);

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

                    cout << "La desencriptacion SI coincide con el texto original." << endl;

                }
                else{

                    cout << "La desencriptacion NO coincide con el texto original." << endl;

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
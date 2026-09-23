#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include "headers.h"

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

            cout << "LZ78: en construccion." << endl;

            break;
        }

        case 3:{

            cout << "Encriptacion: en construccion." << endl;

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
#include "red.h"


using namespace std;

int main() {


char opcion;
    char eleccionAlgoritmo = 'a';


    //string ruta="calls1.txt";


    string ruta="";
    cout << "Ingrese nombre archivo (Ej: calls1.txt)" << endl;
    cin >> ruta;
    while (ruta == "") {
        cout << "Ingrese nuevamente" << endl;
        cin >> ruta;
    };



    char* pRuta;
    pRuta=&ruta[0];




    cout << "Elija el tipo de recorrido:" << endl;
    cout << "a. Camino minimo por km" << endl;
    cout << "b. Camino minimo por centavos" << endl;
    cin >> eleccionAlgoritmo;
    while (eleccionAlgoritmo != 'a' && eleccionAlgoritmo != 'b') {
        cout << "Ingrese nuevamente:  " << endl;
        cin >> eleccionAlgoritmo;
    };



    Red Red1(pRuta, eleccionAlgoritmo);



    cout << endl << "CENTRAL TELEFONICA" << endl << endl;
    while ( (opcion!='S') ){
        cout << endl << endl << "Ingrese la opcion deseada:" << endl << endl;
        cout << "A: Llamadas entre el interno X de la central A y el interno Y de la central B" << endl;
        cout << "B: Interno que mas hablo de todos y de cada central" << endl; // Suma recibidas y emitidas
        cout << "C: Interno que mas llamo de todos y de cada central" << endl;
        cout << "D: Interno que mas le dio ocupado sus intentos y de cada central" << endl; // Emitidas
        cout << "E: Interno al que mas llamaron de todos de cada central" << endl;
        cout << "F: Interno que mas gasto de todos y de cada central" << endl;
        cout << "G: Interno al que mas le hablaron de todos y de cada central" << endl;
        cout << "H: Interno que mas dio ocupado de todos y de cada central" << endl; // Recibidas
        cout << "I: Detalle de llamadas emitidas por el interno X de la central A" << endl;
        cout << "J: Detalle de llamadas recibidas por el interno X de la central A" << endl;
        cout << "M: Listado de llamadas anuladas por falta de enlaces por central" << endl;
        cout << "N: Listado de enlaces (origen, destino, canales)" << endl;
        cout << "O: Listado de internos (central, numeros)" << endl;
        cout << "P: Listado de centrales" << endl;
        cout << "Q: Imprimir red" << endl;
        cout << "S: Salir" << endl << endl;
        cout << "Ingrese letra:  "; cin >> opcion; cout << endl << endl;

        opcion=toupper(opcion);
        switch(opcion){
            case 'A':
                Red1.listadoInternos();
                Red1.llamadasTotalesXAYB();
                break;
            case 'B':
                Red1.internoMasHablo();
                break;
            case 'C':
                Red1.internoMasLLamo();
                break;
            case 'D':
                Red1.internoMasLeDioOcupado();
                break;
            case 'E':
                Red1.internoMasLLamaron();
                break;
            case 'F':
                Red1.internoMasGasto();
                break;
            case 'G':
                Red1.internoMasLeHablaron();
                break;
            case 'H':
                Red1.internoMasDioOcupado();
                break;
            case 'I':
                Red1.listadoInternos();
                Red1.llamadasEmitidasXA();
                break;
            case 'J':
                Red1.listadoInternos();
                Red1.llamadasRecibidasXA();
                break;
            case 'M':
                Red1.listadoAnuladas();
                break;
            case 'N':
                Red1.listadoEnlaces();
                break;
            case 'O':
                Red1.listadoInternos();
                break;
            case 'P':
                Red1.listadoCentrales();
                break;
            case 'Q':
                Red1.imprimirRed();
                break;
            case 'S':
                //delete Red1;
                break;
            default:
                cout << "OPCION INCORRECTA" << endl;
                break;
        }
    }



    return 0;
}

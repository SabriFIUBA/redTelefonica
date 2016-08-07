#ifndef DATOSLLAMADAS_H_
#define DATOSLLAMADAS_H_

#include <iostream>

using namespace std;

/* Cada interno tiene una lista con punteros a estos objetos,
es el detalle de llamadas con los internos con los que interactuó.
El id corresponde al id del interno con el que interactuó.
*/
class DatosLlamadas {
private:
    unsigned int id; // formato  00010001
    unsigned int centralId;
    unsigned int emitidas;
    unsigned int recibidas;
    unsigned int minEmitidos;
    unsigned int minRecibidos;
    unsigned int ocupadoEmitidos;
    unsigned int ocupadoRecibido;
    float gasto;
public:
    /*  Constructor

        Crea un detalle de llamada con el id de interno y central provistos.
        Se inicializan los demás atributos en 0.

    */
    DatosLlamadas(unsigned int id, unsigned int centralId);
    /*
        Métodos para obtener y modificar los atributos del detalle.

    */
    unsigned int obtenerIdLlamada();
    unsigned int obtenerCentralId();
    unsigned int obtenerEmitidas();
    unsigned int obtenerRecibidas();
    unsigned int obtenerMinEmitidos();
    unsigned int obtenerMinRecibidos();
    unsigned int obtenerOcupadosEmitidos();
    unsigned int obtenerOcupadoRecibido();
    float obtenerGasto();
    void modificarEmitidas(unsigned int emitidas);
    void modificarRecibidas(unsigned int recibidas);
    void modificarMinEmitidos(unsigned int minemitidos);
    void modificarMinRecibidos(unsigned int minrecibidos);
    void modificarOcupadoEmitidos(unsigned int ocupadoemitido);
    void modificarOcupadoRecibido(unsigned int ocupadorecibido);
    void modificarGasto(float gastos);
};

#endif // DATOSLLAMADAS_H_

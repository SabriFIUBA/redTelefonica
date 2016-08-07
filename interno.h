#ifndef INTERNO_H_
#define INTERNO_H_

#include "lista.h"
#include "datosllamadas.h"



class Interno {
private:
    unsigned int idInterno;
    unsigned int idCentral;
    /*
        Minuto de la última llamada.
        Indica el minuto si el interno se encuentra actualmente en una llamada.
        Indica 9999 si el interno está disponible.
    */
    unsigned int minuto;
    Lista<unsigned int>* recorridoUltimaLlamada;
    /*
        Lista con los internos con los que interactuó.
    */
    Lista<DatosLlamadas*>* detallesLlamadas; // Lista con los internos con los que interactuó con los detalles de llamadas

public:

    /*
        Constructor

        Pre: recibe la central donde radica el interno y un nuevo id del interno a crear.

        Post: crea un interno con el id recibido, crea una lista de detalle de llamdas vacía.
        Coloca minuto en 9999 para indicar que el interno está disponible.

    */
    Interno(unsigned int idInterno, unsigned int idCentral);

    /*  Destructor
        Post: libera los recursos asociados al interno.

    */
    ~Interno();

    /*
        Post: devuelve un puntero a la lista de los detalle de las llamadas.

    */
    Lista<DatosLlamadas*>* obtenerDetallesLlamadas();

    /*
        Post: devuelve el puntero al recorrido de la última comunicación.

    */
    Lista<unsigned int>* obtenerUltimoRecorrido();

    /*
        Pre: recibe un nuevo recorrido válido.

        Post: intercambia el recorrido actual por el nuevo recorrido.

    */
    void modificarUltimoRecorrido(Lista<unsigned int>* nuevoRecorrido);

    /*

        Pre: recibe un id de interno y otro de su central.

        Post: crea un dato de llamada en la lista de detalles de llamadas.
        Devuelve un puntero al dato de llamada recién creado.

    */
    DatosLlamadas* agregarDatosLlamadas(unsigned int idInterno, unsigned int idCentral);

    /*
        Post: devuelve el id.

    */
    unsigned int obtenerId();

    /*
        Post: devuelve el minuto de la última llamada o 9999 si el interno está disponible.

    */
    unsigned int obtenerMinutos();

    /*
        Pre: recibe el dato del minuto a modificar.

        Post: reemplaza los minutos actuales por el nuevo dato.

    */
    void modificarMinutos(unsigned int minutos);

    unsigned int sumarMinutosEmitidos();
    unsigned int sumarLlamadasEmitidas();
    unsigned int sumarOcupadasEmitidas();
    unsigned int sumarMinutosRecibidos();
    unsigned int sumarLlamadasRecibidas();
    unsigned int sumarOcupadasRecibidas();
    float sumarGasto();



    DatosLlamadas* buscarDatosLlamadas(unsigned int internoSolicitado, unsigned int centralSolicitada);
};


#endif // INTERNO_H_

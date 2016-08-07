#ifndef INTERNO_H_
#define INTERNO_H_

#include "lista.h"
#include "datosllamadas.h"



class Interno {
private:
    unsigned int idInterno;
    unsigned int idCentral;
    /*
        Minuto de la �ltima llamada.
        Indica el minuto si el interno se encuentra actualmente en una llamada.
        Indica 9999 si el interno est� disponible.
    */
    unsigned int minuto;
    Lista<unsigned int>* recorridoUltimaLlamada;
    /*
        Lista con los internos con los que interactu�.
    */
    Lista<DatosLlamadas*>* detallesLlamadas; // Lista con los internos con los que interactu� con los detalles de llamadas

public:

    /*
        Constructor

        Pre: recibe la central donde radica el interno y un nuevo id del interno a crear.

        Post: crea un interno con el id recibido, crea una lista de detalle de llamdas vac�a.
        Coloca minuto en 9999 para indicar que el interno est� disponible.

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
        Post: devuelve el puntero al recorrido de la �ltima comunicaci�n.

    */
    Lista<unsigned int>* obtenerUltimoRecorrido();

    /*
        Pre: recibe un nuevo recorrido v�lido.

        Post: intercambia el recorrido actual por el nuevo recorrido.

    */
    void modificarUltimoRecorrido(Lista<unsigned int>* nuevoRecorrido);

    /*

        Pre: recibe un id de interno y otro de su central.

        Post: crea un dato de llamada en la lista de detalles de llamadas.
        Devuelve un puntero al dato de llamada reci�n creado.

    */
    DatosLlamadas* agregarDatosLlamadas(unsigned int idInterno, unsigned int idCentral);

    /*
        Post: devuelve el id.

    */
    unsigned int obtenerId();

    /*
        Post: devuelve el minuto de la �ltima llamada o 9999 si el interno est� disponible.

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

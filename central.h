#ifndef CENTRAL_H_
#define CENTRAL_H_

#include "lista.h"
#include "interno.h"
#include "enlaceacentral.h"




class Central {
private:

    unsigned int id;
    Lista<Interno*>* listaInternos;
    /*
        Lista de centrales con las que está conectada
        mediante enlace directo (las separa 1 solo enlace de por medio)
    */
    Lista<EnlaceACentral*>* enlacesACentrales;
    /*
        Cantidad de llamadas anuladas por falta de enlaces disponibles en la red.
    */
    unsigned int anuladas;

    /* Máximos */

    Interno* maxMinutosEmitidos;
    Interno* maxLlamadasEmitidas;
    Interno* maxOcupadoEmitidas;
    Interno* maxLlamadasRecibidas;
    Interno* maxGasto;
    Interno* maxMinutoRecibidos;
    Interno* maxOcupadoRecibidos;

    /* Etiqueta */
    int peso;
    Central* vieneDe;
    int nivel;
    int estado;

public:

    unsigned int obtenerPeso();
    Central* obtenerVieneDe();
    unsigned int obtenerNivel();
    unsigned int obtenerEstado();

    void cambiarPeso(unsigned int nuevoPeso);
    void cambiarVieneDe(Central* nuevoVieneDe);
    void cambiarNivel(unsigned int nuevoNivel);
    void cambiarEstado(unsigned int nuevoEstado);

    /*  Constructor

        Pre: recibe un id válido.

        Post: crea una central con el id solicitado, su lista de internos vacía,
        y una lista con centrales con las cuales está enlazada.

    */
    Central(unsigned int nuevoId);

    /*  Destructor
        Post: libera los recursos asociados a la central.

    */
    ~Central();

    /*  Buscar interno por id.

        Pre: recibe id del interno solicitado.

        Post: devuelve un puntero al interno solicitado, null si no existe.

    */
    Interno* buscarInterno(unsigned int idSolicitado);

    /*  Agregar un interno a la central

        Pre: recibe el id de un interno que no existe en la central.

        Post:  Crea un interno con el id recibido.

    */
    Interno* agregarInterno(unsigned int internoIdA);

    /*
        Post: devuelve el id de la central.

    */
    unsigned int obtenerId();

    /*
        Post: devuelve la lista de enlaces a otras centrales.

    */
    Lista<EnlaceACentral*>* obtenerEnlacesAOtrasCentrales();

    /*  Buscar enlaces a otras centrales.

        Pre: recibe un id válido.

        Post: devuelve un puntero a la central solicitada, null si no existiere.

    */
    unsigned int buscarEnlaceAOtraCentral (unsigned int idSolicitado);

    /*  Agregar una central a la lista de enlaces a otras centrales.

        Pre: recibe un id y el puntero de la central a agregar.

        Post: agregar la central a la lista de enlaces a otra centrales si no existiere.

    */
    void agregarEnlaceAOtraCentral(unsigned int idAgregar,unsigned int nuevoPeso);

    /*
        Post: incrementa en 1 las llamadas anuladas por falta de enlace.

    */
    void agregarAnuladaFaltaEnlace();

    /*
        Post: devuelve la cantidad de llamadas que se quisieron realizar desde la central
        y que no se pudieron efectivizar por falta de disponibilidas en los enlaces hacia
        la central destino.

    */
    unsigned int obtenerAnuladaFaltaEnlace();

    /*
        Post: devuelve un puntero a la lista de internos.
    */
    Lista<Interno*>* obtenerListaInternos();

    void calcularMaximosCentral();

    Interno* obtenerMaxMinutosEmitidos();
    Interno* obtenerMaxLlamadasEmitidas();
    Interno* obtenerMaxOcupadasEmitidas();
    Interno* obtenerMaxMinutosRecibidos();
    Interno* obtenerMaxLlamadasRecibidas();
    Interno* obtenerMaxOcupadasRecibidas();
    Interno* obtenerMaxGasto();


};


#endif // CENTRAL_H_

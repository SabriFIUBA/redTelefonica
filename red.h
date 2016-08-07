#ifndef RED_H_
#define RED_H_
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
//#include <iomanip>
//#include <vector>

using std::exit;
//using std::vector;
using namespace std;

#include "central.h"
#include "enlaces.h"
#include "lista.h"
#include "datosllamadas.h"
#include "enlaceacentral.h"
#include "lista4.h"



/*  Una red es una red telefónica con sus centrales, enlaces, internos,
    que se crean y se distribuyen según estén dispuestos en un archivo de origen.

    Define operaciones tales como:

    *   Buscar centrales, enlaces, internos, detalles de llamadas.
    *   Iniciar y finalizar llamadas.
    *   Establecer recorridos entre centrales, verificar la disponibilidad de estos.
    *   Calcular costos de una llamada.

*/

class Red {
private:
    Lista<Enlace*>* listaEnlaces;
    Lista<Central*>* listaCentrales;
    Lista<unsigned int>* caminoMinimo;


    bool maxCalculados;
    Central* maxMinutosEmitidos;
    Central* maxLlamadasEmitidas;
    Central* maxOcupadoEmitidas;
    Central* maxLlamadasRecibidas;
    Central* maxGasto;
    Central* maxMinutoRecibidos;
    Central* maxOcupadoRecibidos;

    unsigned int coordVertice[2];

    void imprimirNiveles(Lista<Lista4<Central*>*>* listasNiveles);

    void imprimirCamino();

public:

        void imprimirRed();

    /*  Constructor de la red

        Pre:  recibe una ruta válida de un archivo de texto con datos
        sobre la red (teléfonica) a construir y la elección de un algoritmo.

        Post:  construye una red con sus respectivas centrales, los enlaces entre estas,
        sus internos. Computa los datos de las llamadas realizadas.
        La red queda lista para realizar consultas sobre la red y las llamadas.

    */
    Red(char* ruta, char eleccionAlgoritmo);

    /*  Destructor
        Post: libera los recursos asociados a la red.

    */
    ~Red();


    /*  Métodos */


    /*  Buscar central por id.

        Pre: recibe un número válido de id.

        Post: devuelve un puntero a la central con el id solicitado o null
        si el id no existe en la lista de centrales de la red.

    */
    Central* buscarCentral (unsigned int idSolicitado);

    /*  Buscar central por puntero.

        Pre: recibe un puntero de una central.

        Post: devuelve el id de la central solicitada.

    */
    unsigned int buscarCentral (Central* centralSolicitada);

    /*  Buscar una central entre una lista de centrales.

        Pre: recibe una central y una lista de centrales.

        Post: Si la central se encuentra en la lista recibida devuelve 1.
        Si no devuelve 0.

    */
    unsigned int buscarCentral (unsigned int centralSolicitada, Lista<EnlaceACentral*>* enlacesACentrales);

    /*  Iniciar una llamada en la red.

        Pre: recibe dos centrales de la red (iguales o distintas),
        dos internos distintos, y el minuto en que se inicia la llamada.

        Post: Estable un recorrido entre las centrales, si el recorrido está disponible
        y el interno destino también, se inicializa la llamada. Agrega una llamada
        emitida al interno origen y agrega una recibida al interno destino.
        Indica, colocando el minuto de inicio en cada interno, que tal interno
        se encuentra ocupado. Resta a cada enlace del recorrido 1 canal disponible.

        Si el recorrido no está disponible por falta de canales se incrementan
        las llamadas anuladas por falta de enlace en la central origen.

        Si el interno destino se encuentra en una llamada en curso, da ocupado. Se agrega
        una ocupada emitida al interno origen y una ocupada recibia al interno destino.

    */
    void iniciarLlamada(unsigned int centralIdA, unsigned int internoIdA,
                   unsigned int centralIdB, unsigned int internoIdB,
                   unsigned int minuto);

    /*  Finalizar una llamada

        Pre: existe la llamada a finalizar. Minuto debe ser mayor al minuto del inicio
        de la llamada.

        Post: finaliza la llamada en curso. Calcula la duración de la llamada
        y agrega los minutos emitidos al interno origen y los minutos recibidos
        al interno destino. Calcula el gasto y lo guarda en el interno origen.
        Vuelve a poner a los internos disponibles y libera los enlaces utilizados.

    */
    void finalizarLlamada(unsigned int centralIdA, unsigned int internoIdA,
                          unsigned int centralIdB, unsigned int internoIdB,
                          unsigned int minutoFin);

    /*  Calcular costo de una llamada y liberar los canales utilizados por la misma.

        Pre: recibe un recorrido y una duración de una llamada previamente inicializada.

        Post: devuelve el costo de dicha llamada.

    */
    float calcularCostoYLiberarCanales(Lista<unsigned int>* camino, unsigned int duracion);

    /*  Verificar disponibilidad de un recorrido.

        Pre: recibe un recorrido de una llamada.

        Post: Devuelve true si todos los enlaces del recorrido tienen al menos un canal
        para comenzar la comunicación. Devuelve false si existe al menos un enlace
        que no dispone de canales disponibles al momento de establecer la llamada.

    */
    bool verificarDisponibilidad();

    /*  Buscar un enlace entre dos centrales por id de central

        Pre: Recibe dos centrales.

        Post: Busca un enlace en la lista de enlaces de la red que contenga
        a las dos centrales recibidas. Devuelve un puntero al enlace,
        si no lo encuentra devuelve un puntero a null.

    */
    Enlace* buscarEnlace(unsigned int idCentralA, unsigned int idCentralB);


    /*  Obtener la lista de centrales de la red.

        Post: devuelve un puntero a la lista de centrales de la red.

    */
    Lista<Central*>* obtenerListaDeCentrales();

    /* Obtener la lista de enlaces de la red.

    Post: devuelve un puntero a la lista de enlaces de la red.

    */
    Lista<Enlace*>*  obtenerListaDeEnlaces();

    void calcularMaximos();

    Central* obtenerMaxMinutosEmitidos();
    Central* obtenerMaxLlamadasEmitidas();
    Central* obtenerMaxOcupadasEmitidas();
    Central* obtenerMaxMinutosRecibidos();
    Central* obtenerMaxLlamadasRecibidas();
    Central* obtenerMaxOcupadasRecibidas();
    Central* obtenerMaxGasto();






    void ocuparCanales();

    bool maximosCalculados ();


    // Métodos camino

    Lista<unsigned int>* crearCamino(Central* centralA, Central* centralB);

    Lista<unsigned int>* obtenerCamino(Lista<Lista4<Central*>*>* niveles, Central* cDestino);

    Lista4<Central*>* buscarVertices(Central* cDestino, Lista<Lista4<Central*>*>* niveles);

    void restaurarVertices(Lista<Lista4<Central*>*>* niveles, Nodo4<Central*>* vDuplicado, Nodo4<Central*>* vNuevo);

    Nodo4<Central*>* ponderarVertice(Lista4<Central*>* nivel);

    void cargarVertices (Nodo4<Central*>* vPonderado,  Lista<Lista4<Central*>*>* niveles);

    void etiquetarVertices(Lista4<Central*>* nivel, Lista<Lista4<Central*>*>* listasNiveles);

    Nodo4<Central*>* buscarCentralVertice(Central* central, Lista<Lista4<Central*>*>* listasNiveles);


        // Métodos menú
    /*PRE: Solo se debera ingresar numeros de centrales e internos que se procesaron del txt*/
    /*POST: Muestra los totales de las llamadas de un interno x de la central A a un interno y de la central B*/
    void llamadasTotalesXAYB();
    /*POST: Muestra el interno que mas hablo de cada central y el que mas hablo de toda la red*/
    void internoMasHablo();
    /*POST: Muestra el interno que mas llamo de cada central y el que mas llamo de toda la red*/
    void internoMasLLamo();
    /*POST: Muestra el interno que mas le dio ocupado de cada central y el que mas le dio ocupado de toda la red*/
    void internoMasLeDioOcupado();
    /*POST: Muestra el interno al que mas llamaron de cada central y al que mas llamoron de toda la red*/
    void internoMasLLamaron();
    /*POST: Muestra el interno que mas gasto de cada central y el que mas gasto de toda la red*/
    void internoMasGasto();
    /*POST: Muestra el interno al que mas le hablaron de cada central y al que mas le hablaron de toda la red*/
    void internoMasLeHablaron();
    /*POST: Muestra el interno que mas dio ocupado de cada central y el que mas dio ocupado de toda la red*/
    void internoMasDioOcupado();
    /*PRE: Solo se debera ingresar numeros de centrales e internos que se procesaron del txt*/
    /*POST: Muestra las llamadas emitidas por un interno x de la central A*/
    void llamadasEmitidasXA();
    /*PRE: Solo se debera ingresar numeros de centrales e internos que se procesaron del txt*/
    /*POST: Muestra las llamadas recibidas por un interno x de la central A*/
    void llamadasRecibidasXA();

    /*POST: Muestra la cantidad de llamadas anuladas por falta de enlace por central*/
    void listadoAnuladas();
    /*POST: Muestra la lista de central-internos*/
    void listadoInternos();
    /*POST: Muestra la lista de enlaces*/
    void listadoEnlaces();
    /*POST: Muestra el listado de centrales */
    void listadoCentrales();

};

#endif // RED_H_

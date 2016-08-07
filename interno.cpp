#include "interno.h"

// Constructor

Interno::Interno(unsigned int nuevoIdInterno, unsigned int idCentral){
    this->minuto=9999;
    this->idInterno=nuevoIdInterno;
    this->detallesLlamadas = new Lista<DatosLlamadas*>;
    this->idCentral = idCentral;

}

// Destructor

Interno::~Interno(){
    /*

        Liberación de memoria:

        Se borran todos los DatosLLamadas y luego la lista de punteros a estos.


    */
    DatosLlamadas* datoABorrar;
    detallesLlamadas->iniciarCursor();
    while (detallesLlamadas->avanzarCursor()){
        datoABorrar = detallesLlamadas->obtenerCursor();
        delete datoABorrar;
    }

    delete detallesLlamadas;
}

// Métodos

Lista<DatosLlamadas*>* Interno::obtenerDetallesLlamadas(){
    return this->detallesLlamadas;
}

Lista<unsigned int>* Interno::obtenerUltimoRecorrido(){
    return this->recorridoUltimaLlamada;
}

void Interno::modificarUltimoRecorrido(Lista<unsigned int>* nuevoRecorrido){
    this->recorridoUltimaLlamada=nuevoRecorrido;
}

DatosLlamadas* Interno::agregarDatosLlamadas(unsigned int idInterno, unsigned int idCentral){
    DatosLlamadas* nuevoDatoLlamada = NULL;
    nuevoDatoLlamada = new DatosLlamadas(idInterno,idCentral);
    this->detallesLlamadas->agregar(nuevoDatoLlamada);
    return nuevoDatoLlamada;
}

unsigned int Interno::obtenerId(){
    return this->idInterno;  }

unsigned int Interno::obtenerMinutos() {
    return this->minuto;  }

void Interno::modificarMinutos(unsigned int minutos)  {
    this->minuto = minutos;  }

unsigned int Interno::sumarMinutosEmitidos(){
    unsigned int minutosEmitidos = 0;
    detallesLlamadas->iniciarCursor();
    DatosLlamadas* datoActual = NULL;
    while (detallesLlamadas->avanzarCursor()){
        datoActual = detallesLlamadas->obtenerCursor();
        minutosEmitidos += datoActual->obtenerMinEmitidos();
    }
    return minutosEmitidos;
}

unsigned int Interno::sumarLlamadasEmitidas(){
    unsigned int llamadasEmitidas = 0;
    detallesLlamadas->iniciarCursor();
    DatosLlamadas* datoActual = NULL;
    while (detallesLlamadas->avanzarCursor()){
        datoActual = detallesLlamadas->obtenerCursor();
        llamadasEmitidas += datoActual->obtenerEmitidas();
    }
    return llamadasEmitidas;
}

unsigned int Interno::sumarOcupadasEmitidas(){
    unsigned int ocupadasEmitidas = 0;
    detallesLlamadas->iniciarCursor();
    DatosLlamadas* datoActual = NULL;
    while (detallesLlamadas->avanzarCursor()){
        datoActual = detallesLlamadas->obtenerCursor();
        ocupadasEmitidas += datoActual->obtenerOcupadosEmitidos();
    }
    return ocupadasEmitidas;
}
unsigned int Interno::sumarMinutosRecibidos(){
    unsigned int minutosRecibidos = 0;
    detallesLlamadas->iniciarCursor();
    DatosLlamadas* datoActual = NULL;
    while (detallesLlamadas->avanzarCursor()){
        datoActual = detallesLlamadas->obtenerCursor();
        minutosRecibidos += datoActual->obtenerMinRecibidos();
    }
    return minutosRecibidos;
}
unsigned int Interno::sumarLlamadasRecibidas(){
    unsigned int llamadasRecibidas = 0;
    detallesLlamadas->iniciarCursor();
    DatosLlamadas* datoActual = NULL;
    while (detallesLlamadas->avanzarCursor()){
        datoActual = detallesLlamadas->obtenerCursor();
        llamadasRecibidas += datoActual->obtenerRecibidas();
    }
    return llamadasRecibidas;
}
unsigned int Interno::sumarOcupadasRecibidas(){
    unsigned int ocupadasRecibidas = 0;
    detallesLlamadas->iniciarCursor();
    DatosLlamadas* datoActual = NULL;
    while (detallesLlamadas->avanzarCursor()){
        datoActual = detallesLlamadas->obtenerCursor();
        ocupadasRecibidas += datoActual->obtenerOcupadoRecibido();
    }
    return ocupadasRecibidas;
}
float Interno::sumarGasto(){
    float gasto = 0;
    detallesLlamadas->iniciarCursor();
    DatosLlamadas* datoActual = NULL;
    while (detallesLlamadas->avanzarCursor()){
        datoActual = detallesLlamadas->obtenerCursor();
        gasto += datoActual->obtenerGasto();
    }
    return gasto;
}

DatosLlamadas* Interno::buscarDatosLlamadas(unsigned int internoSolicitado, unsigned int centralSolicitada){


    DatosLlamadas* datoSolicitado = NULL;

    detallesLlamadas->iniciarCursor();

    DatosLlamadas* datoActual = NULL;

    while ( detallesLlamadas->avanzarCursor() && datoSolicitado==NULL ) {

        datoActual = detallesLlamadas->obtenerCursor();

        if ( datoActual->obtenerIdLlamada() == internoSolicitado && datoActual->obtenerCentralId() == centralSolicitada ){
            datoSolicitado = datoActual;
        }
    }
    return datoSolicitado;
}

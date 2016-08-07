#include "datosllamadas.h"


// Constructor
DatosLlamadas::DatosLlamadas(unsigned int id, unsigned int centralId)
{

    this->id = id;
    this->centralId = centralId;
    this->emitidas = 0;
    this->recibidas = 0;
    this->minEmitidos = 0;
    this->minRecibidos = 0;
    this->ocupadoEmitidos = 0;
    this->ocupadoRecibido = 0;
    this->gasto = 0;

}

    /*
        En esta clase no se hace new por lo que no agrego destructor.
        Se hace delete de DatosLlamada en Interno donde tengo un puntero
        a una lista de punteros a DatosLlamada.

    */




//Metodos

unsigned int DatosLlamadas::obtenerIdLlamada(){
    return this->id;
}

unsigned int DatosLlamadas::obtenerCentralId()
{
    return this->centralId;
}

unsigned int DatosLlamadas::obtenerEmitidas()
{
    return this->emitidas;
}

unsigned int DatosLlamadas::obtenerRecibidas()
{
    return this->recibidas;
}

unsigned int DatosLlamadas::obtenerMinEmitidos()
{
    return this->minEmitidos;
}

unsigned int DatosLlamadas::obtenerMinRecibidos()
{
    return this->minRecibidos;
}

unsigned int DatosLlamadas::obtenerOcupadosEmitidos()
{
    return this->ocupadoEmitidos;
}

unsigned int DatosLlamadas::obtenerOcupadoRecibido()
{
    return this->ocupadoRecibido;
}

float DatosLlamadas::obtenerGasto()
{
    return this->gasto;
}

void DatosLlamadas::modificarEmitidas(unsigned int emitidas)
{
    this->emitidas = emitidas;
}

void DatosLlamadas::modificarRecibidas(unsigned int recibidas)
{
    this->recibidas = recibidas;
}

void DatosLlamadas::modificarMinEmitidos(unsigned int minemitidos)
{
    this->minEmitidos = minemitidos;
}

void DatosLlamadas::modificarMinRecibidos(unsigned int minrecibidos)
{
    this->minRecibidos = minrecibidos;
}

void DatosLlamadas::modificarOcupadoEmitidos(unsigned int ocupadoemitido)
{
    this->ocupadoEmitidos = ocupadoemitido;
}

void DatosLlamadas::modificarOcupadoRecibido(unsigned int ocupadorecibido)
{
    this->ocupadoRecibido = ocupadorecibido;
}

void DatosLlamadas::modificarGasto(float gastos)
{
    this->gasto = gastos;
}

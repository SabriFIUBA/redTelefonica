#include "enlaces.h"

// Constructor
Enlace::Enlace(unsigned int nuevoCentralIdA,
               Central* nuevoCentralPtrA,
               unsigned int nuevoCentralIdB,
               Central* nuevoCentralPtrB,
               unsigned int nuevoKM,
               unsigned int nuevoCentavos,
               unsigned int nuevoCanales,
               unsigned int nuevoCanalesDisponibles ){
    this->centralIdA = nuevoCentralIdA;
    this->centralPtrA = nuevoCentralPtrA;
    this->centralIdB = nuevoCentralIdB;
    this->centralPtrB = nuevoCentralPtrB;
    this->km = nuevoKM;
    this->centavos = nuevoCentavos;
    this->canales = nuevoCanales;
    this->canalesDisponibles = nuevoCanales;

}

    /*

        Liberación de memoria:

        Borro los Enlaces en ~Red y la lista que los contiene.


    */




// Métodos
unsigned int Enlace::obtenerCanalesDisponibles(){
    return this->canalesDisponibles;
}

void Enlace::ocuparCanal(){
    this->canalesDisponibles--;
}

void Enlace::liberarCanal(){
    this->canalesDisponibles++;
}

unsigned int Enlace::obtenerCosto(){
    return this->centavos;
}

unsigned int Enlace::obtenerCentralIdA(){
    return this->centralIdA;
}

unsigned int Enlace::obtenerCentralIdB(){
    return this->centralIdB;
}

unsigned int Enlace::obtenerCanales(){
    return this->canales;
}

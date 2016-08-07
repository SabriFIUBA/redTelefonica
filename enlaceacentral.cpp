#include "enlaceacentral.h"
#include <ostream>
using namespace std;

// Constructor
EnlaceACentral::EnlaceACentral(unsigned int nuevoIdOtraCentral, unsigned int nuevoPesoEnlace){
    this->pesoEnlace = nuevoPesoEnlace;
    this->idOtraCentral = nuevoIdOtraCentral;
}

    /*
        En esta clase no se hace new por lo que no agrego destructor.
        Se hace delete de enlacesACentrales en Central donde tengo un puntero
        a una lista de punteros a EnlaceACentral.

    */


// Metodos

unsigned int EnlaceACentral::obtenerIdOtraCentral(){
    return this->idOtraCentral;
}

unsigned int EnlaceACentral::obtenerPesoEnlace(){
    return this->pesoEnlace;
}

void EnlaceACentral::cambiarIdOtraCentral(unsigned int nuevoIdOtraCentral){
    this->idOtraCentral = nuevoIdOtraCentral;
}

void EnlaceACentral::cambiarPesoEnlace(unsigned int nuevoPesoEnlace){
    this->pesoEnlace = nuevoPesoEnlace;
}


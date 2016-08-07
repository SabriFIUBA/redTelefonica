#ifndef ENLACEACENTRAL_H_
#define ENLACEACENTRAL_H_

#include "lista.h"

class EnlaceACentral {
private:
    unsigned int idOtraCentral;
    unsigned int pesoEnlace;
public:
    // Constructor
    EnlaceACentral(unsigned int nuevoIdOtraCentral, unsigned int nuevoPesoEnlace);
    // Métodos
    unsigned int obtenerIdOtraCentral();
    unsigned int obtenerPesoEnlace();

    void cambiarIdOtraCentral(unsigned int nuevoIdOtraCentral);
    void cambiarPesoEnlace(unsigned int nuevoPesoEnlace);



};

#endif // ENLACEACENTRAL_H_

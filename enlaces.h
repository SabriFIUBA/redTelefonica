#ifndef ENLACES_H_
#define ENLACES_H_

#include "central.h"
#include "lista.h"


class Enlace {
private:
    unsigned int id;
    unsigned int centralIdA;
    Central* centralPtrA;
    unsigned int centralIdB;
    Central* centralPtrB;
    unsigned int km;
    unsigned int centavos;
    unsigned int canales;
    unsigned int canalesDisponibles;
public:
    //Constructor
    Enlace(unsigned int nuevoCentralIdA,
           Central* nuevoCentralPtrA,
           unsigned int nuevoCentralIdB,
           Central* nuevoCentralPtrB,
           unsigned int nuevoKM,
           unsigned int nuevoCentavos,
           unsigned int nuevoCanales,
           unsigned int nuevoCanalesDisponibles);
    // Métodos
    unsigned int obtenerCentralIdA();
    unsigned int obtenerCentralIdB();
    unsigned int obtenerCanales();
    unsigned int obtenerCanalesDisponibles();
    void ocuparCanal();
    void liberarCanal();
    unsigned int obtenerCosto();

};

#endif // ENLACES_H_

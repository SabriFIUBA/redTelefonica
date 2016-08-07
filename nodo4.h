#ifndef NODO4_H_
#define NODO4_H_

#ifndef NULL
#define NULL 0
#endif
#include "central.h"

template<class T> class Nodo4 {

    private:

        unsigned int peso;

        unsigned int estado;

        unsigned int nivel;

        T central;

        Nodo4<T>* adyacente;

        unsigned int pesoAdyacente;

        Nodo4<T>* siguiente;

    public:

        /*
         * post: el Nodo resulta inicializado con el dato dado
         *       y sin un Nodo siguiente.
         */
        Nodo4(unsigned int nuevoPeso, unsigned int nuevoEstado,
              T nuevaCentral, Nodo4<T>* nuevoAdyacente,
              unsigned int nuevoPesoAdyacente, unsigned int nuevoNivel) {

            this->peso = nuevoPeso;
            this->siguiente = NULL;
            this->central = nuevaCentral;
            this->adyacente = nuevoAdyacente;
            this->pesoAdyacente = nuevoPesoAdyacente;
            this->estado = nuevoEstado;
            this->nivel = nuevoNivel;
        }

        /*
         * post: devuelve el valor del dato.
         */
        unsigned int obtenerPeso() {

            return this->peso;
        }

        unsigned int obtenerEstado() {
            return this->estado;
        }
        T obtenerCentral() {
            return this->central;
        }
        Nodo4<T>* obtenerAdyacente(){
            return this->adyacente;
        }

        Nodo4<T>* obtenerSiguiente(){
            return this->siguiente;
        }

        unsigned int obtenerNivel(){
            return this->nivel;
        }
        unsigned int obtenerPesoAdyacente(){
            return this->pesoAdyacente;
        }

        /*
         * post: cambia el valor del dato.
         */
        void cambiarPeso(unsigned int nuevoPeso) {
            this->peso = nuevoPeso;
        }
        /*
         * post: cambia el siguiente Nodo por nuevoSiguiente.
         */
        void cambiarSiguiente(Nodo4<T>* nuevoSiguiente) {

            this->siguiente = nuevoSiguiente;
        }
        void cambiarEstado(unsigned int nuevoEstado){
            this->estado = nuevoEstado;
        }
        void cambiarAdyacente(Nodo4<T>* nuevoAdyacente){
            this->adyacente = nuevoAdyacente;
        }
        void cambiarPesoAdyacente(unsigned int nuevoPeso){
            this->pesoAdyacente = nuevoPeso;
        }
        void cambiarNivel (unsigned int nuevoNivel){
            this->nivel = nuevoNivel;
        }
};


#endif /* NODO_H4_ */

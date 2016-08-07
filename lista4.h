#ifndef LISTA4_H_
#define LISTA4_H_
#include "nodo4.h"

/*
 * Una Lista es una colecci�n din�mica de elementos dispuestos en una secuencia.
 *
 * Define operaciones para agregar, remover, acceder y cambiar elementos
 * en cualquier posici�n.
 *
 * Tiene un cursor que permite recorrer todos los elementos secuencialmente.
 *
 */
template<class T> class Lista4 {

    private:

        Nodo4<T>* primero;

        unsigned int tamanio;

        Nodo4<T>* cursor;

    public:

        /*
         * post: Lista vac�a.
         */
        Lista4();

        /*
         * post: indica si la Lista tiene alg�n elemento.
         */
        bool estaVacia();

        /*
         * post: devuelve la cantidad de elementos que tiene la Lista.
         */
        unsigned int contarElementos();

        /*
         * post: agrega el elemento al final de la Lista, en la posici�n:
         *       contarElementos() + 1.
         */
        Nodo4<T>* agregar(unsigned int nuevoPeso, unsigned int nuevoEstado,
                     T nuevaCentral, Nodo4<T>* nuevoAdyacente,
                     unsigned int nuevoPesoAdyacente, unsigned int nuevoNivel);

        /*
         * pre : posici�n pertenece al intervalo: [1, contarElementos() + 1]
         * post: agrega el elemento en la posici�n indicada.
         *
         */
        //void agregar(unsigned int nuevoPeso, unsigned int nuevoEstado, Central* adyacente, unsigned int posicion);

        /*
         * pre : posici�n pertenece al intervalo: [1, contarElementos()]
         * post: devuelve el elemento en la posici�n indicada.
         */
        Nodo4<T>* obtener(unsigned int posicion);

        /*
         * pre : posicio� pertenece al intervalo: [1, contarElementos()]
         * post: cambia el elemento en la posici�n indicada por el
         *       elemento dado.
         */
        //void asignar(T elemento, unsigned int posicion);

        /*
         * pre : posici�n pertenece al intervalo: [1, contarElementos()]
         * post: remueve de la Lista el elemento en la posici�n indicada.
         */
        void remover(unsigned int posicion);

        /*
         * post: deja el cursor de la Lista preparado para hacer un nuevo
         *       recorrido sobre sus elementos.
         */
        void iniciarCursor();

        /*
         * pre : se ha iniciado un recorrido (invocando el m�todo
         *       iniciarCursor()) y desde entonces no se han agregado o
         *       removido elementos de la Lista.
         * post: mueve el cursor y lo posiciona en el siguiente elemento
         *       del recorrido.
         *       El valor de retorno indica si el cursor qued� posicionado
         *       sobre un elemento o no (en caso de que la Lista est� vac�a o
         *       no existan m�s elementos por recorrer.)
         */
        bool avanzarCursor();

        /*
         * pre : el cursor est� posicionado sobre un elemento de la Lista,
         *       (fue invocado el m�todo avanzarCursor() y devolvi� true)
         * post: devuelve el elemento en la posici�n del cursor.
         *
         */
        Nodo4<T>* obtenerCursor();

        /*
         * post: libera los recursos asociados a la Lista.
         */
        ~Lista4();



    //private:

        /*
         * pre : posici�n pertenece al intervalo: [1, contarElementos()]
         * post: devuelve el nodo en la posici�n indicada.
         */
        //Nodo* obtenerNodo(unsigned int posicion); // NOTA: primitiva PRIVADA
};
template<class T>
Lista4<T>::Lista4() {

    this->primero = NULL;
    this->tamanio = 0;
    this->cursor = NULL;
}
template<class T>
bool Lista4<T>::estaVacia() {

    return (this->tamanio == 0);
}
template<class T>
unsigned int Lista4<T>::contarElementos() {

    return this->tamanio;
}
/*void Lista4::agregar(T elemento) {

    this->agregar(elemento, this->tamanio + 1);
}*/

template<class T>
Nodo4<T>* Lista4<T>::agregar(unsigned int nuevoPeso, unsigned int nuevoEstado,
                     T nuevaCentral, Nodo4<T>* nuevoAdyacente,
                     unsigned int nuevoPesoAdyacente, unsigned int nuevoNivel) {


        Nodo4<T>* nuevo = new Nodo4<T>(nuevoPeso, nuevoEstado,nuevaCentral, nuevoAdyacente,nuevoPesoAdyacente, nuevoNivel);

        if (tamanio == 0) {

            nuevo->cambiarSiguiente(this->primero);
            this->primero = nuevo;

        } else {

            Nodo4<T>* anterior = this->obtener(tamanio);
            //nuevo->cambiarSiguiente(anterior->obtenerSiguiente());
            anterior->cambiarSiguiente(nuevo);
        }

        this->tamanio++;

        // cualquier recorrido actual queda invalidado
        this->iniciarCursor();

    return nuevo;

}
template<class T>
Nodo4<T>* Lista4<T>::obtener(unsigned int posicion) {

    Nodo4<T>* elemento = NULL;

    if ((posicion > 0) && (posicion <= this->tamanio)) {
        elemento = this->primero;
        for (unsigned int i = 1; i < posicion; i++) {

        elemento = elemento->obtenerSiguiente();

        }
    }

    return elemento;
}

/*void Lista4::asignar(T elemento, unsigned int posicion) {

    if ((posicion > 0) && (posicion <= this->tamanio)) {

        this->obtenerNodo(posicion)->cambiarDato(elemento);
    }
}*/
template<class T>
void Lista4<T>::remover(unsigned int posicion) {

    if ((posicion > 0) && (posicion <= this->tamanio)) {

        Nodo4<T>* removido = obtener(posicion);

        if (posicion == 1) {

            //removido = this->primero;
            this->primero = primero->obtenerSiguiente();

        } else {

            Nodo4<T>* anterior = this->obtener(posicion-1);
            //removido = anterior->obtenerSiguiente();
            anterior->cambiarSiguiente(removido->obtenerSiguiente());
        }

        delete removido;
        this->tamanio--;

        /* cualquier recorrido actual queda invalidado */
        this->iniciarCursor();
    }
}
template<class T>
void Lista4<T>::iniciarCursor() {

    this->cursor = NULL;
}
template<class T>
bool Lista4<T>::avanzarCursor() {

    if (this->cursor == NULL) {

        this->cursor = this->primero;

    } else {

        this->cursor = this->cursor->obtenerSiguiente();
    }

    /* pudo avanzar si el cursor ahora apunta a un nodo */
    return (this->cursor != NULL);
}
template<class T>
Nodo4<T>* Lista4<T>::obtenerCursor() {

    Nodo4<T>* elemento;

    if (this->cursor != NULL) {

        //elemento = this->cursor->obtenerSiguiente();
        elemento = this->cursor;

    }

    return elemento;
}
template<class T>
Lista4<T>::~Lista4() {

    while (this->primero != NULL) {

        Nodo4<T>* aBorrar = this->obtener(1);
        this->primero = this->primero->obtenerSiguiente();

        delete aBorrar;
    }
}



#endif // LISTA4_H_

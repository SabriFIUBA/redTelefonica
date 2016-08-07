#include "red.h"




// Constructor
Red::Red(char* pRuta, char eleccionAlgoritmo){

    this->listaEnlaces = new Lista<Enlace*>;
    this->listaCentrales = new Lista<Central*>;

    ifstream archivo(pRuta);

    if (archivo.fail()){
        cout << "No se pudo cargar el archivo." << endl;
        exit(1);
    }

    string linea="";
    while(getline(archivo,linea)){
        char accionChr=' ';
        accionChr=linea[0];
        accionChr=toupper(accionChr);

        if (accionChr=='E') {
            string str = linea.substr(7,10);
            unsigned int centralIdA = atoi(str.c_str());
            str = linea.substr(12,15);
            unsigned int centralIdB = atoi(str.c_str());
            str = linea.substr(17,21);
            unsigned int km = atoi(str.c_str());
            str = linea.substr(23,27);
            unsigned int centavos = atoi(str.c_str());
            str = linea.substr(29,32);
            unsigned int canales = atoi(str.c_str());

            /* Creo centrales y enlaces */
            Central* centralPtrA = NULL;
            Central* centralPtrB = NULL;

            unsigned int peso = 0;

            if (eleccionAlgoritmo=='a'){
                peso = km;
            } else {
                if (eleccionAlgoritmo=='b'){
                    peso = centavos;
                }
            }

            listaCentrales->iniciarCursor();
            bool centralAEncontrada = false;
            bool centralBEncontrada = false;
            while ((listaCentrales->avanzarCursor()) && (!centralAEncontrada || !centralBEncontrada)) {
                Central* centralActual = listaCentrales->obtenerCursor();
                unsigned int centralActualId = centralActual->obtenerId();

                if (centralActualId==centralIdA){
                    centralAEncontrada = true;
                    centralPtrA = centralActual;
                }

                if (centralActualId==centralIdB){
                    centralBEncontrada = true;
                    centralPtrB = centralActual;
                }
            }

            if (!centralAEncontrada) {
                centralPtrA=new Central(centralIdA);
                listaCentrales->agregar(centralPtrA);
            }

            if (!centralBEncontrada) {
                centralPtrB=new Central(centralIdB);
                listaCentrales->agregar(centralPtrB);
            }
            centralPtrA->agregarEnlaceAOtraCentral(centralIdB,peso);
            centralPtrB->agregarEnlaceAOtraCentral(centralIdA,peso);
            Enlace* nuevoEnlace = new Enlace(centralIdA, centralPtrA, centralIdB, centralPtrB, km, centavos, canales, canales);
            listaEnlaces->agregar(nuevoEnlace);

        }
        if (accionChr=='I'){
            string str = linea.substr(7,10);
            unsigned int centralIdA = atoi(str.c_str());
            str = linea.substr(12,15);
            unsigned int internoIdA = atoi(str.c_str());
            str = linea.substr(17,20);
            unsigned int centralIdB = atoi(str.c_str());
            str = linea.substr(22,25);
            unsigned int internoIdB = atoi(str.c_str());
            str = linea.substr(27,30);
            unsigned int minuto = atoi(str.c_str());

            iniciarLlamada(centralIdA, internoIdA, centralIdB, internoIdB, minuto);
        }

        if (accionChr=='F'){
            string str = linea.substr(7,10);
            unsigned int centralIdA = atoi(str.c_str());
            str = linea.substr(12,15);
            unsigned int internoIdA = atoi(str.c_str());
            str = linea.substr(17,20);
            unsigned int centralIdB = atoi(str.c_str());
            str = linea.substr(22,25);
            unsigned int internoIdB = atoi(str.c_str());
            str = linea.substr(27,30);
            unsigned int minuto = atoi(str.c_str());

            finalizarLlamada(centralIdA, internoIdA, centralIdB, internoIdB, minuto);
        }
    }

    this->coordVertice[0]=0;
    this->coordVertice[1]=0;

    //this->imprimirRed();

    this->maxCalculados = false;
    this->maxMinutosEmitidos = NULL;
    this->maxLlamadasEmitidas = NULL;
    this->maxOcupadoEmitidas = NULL;
    this->maxLlamadasRecibidas = NULL;
    this->maxGasto = NULL;
    this->maxMinutoRecibidos = NULL;
    this->maxOcupadoRecibidos = NULL;

}

// Destructor

Red::~Red(){
    /*

        Liberación de memoria:

        Se borran los enlaces y luego la lista de punteros a enlaces.
        Ídem con centrales. Se borra la lista de enteros del recorrido de la última llamada.

     */


    Enlace* enlaceABorrar = NULL;
    listaEnlaces->iniciarCursor();
    while (listaEnlaces->avanzarCursor()){
        enlaceABorrar = listaEnlaces->obtenerCursor();
        delete enlaceABorrar;
    }

    delete listaEnlaces;
    Central* centralABorrar = NULL;
    listaCentrales->iniciarCursor();
    while (listaCentrales->avanzarCursor()){
        centralABorrar = listaCentrales->obtenerCursor();
        delete centralABorrar;
    }

    delete listaCentrales;

    delete caminoMinimo;
}


// Métodos

/*  Busca la central origen(A) y la destino(B), crea un recorrido posible,
    crea los internos si no existieren,
    inicializa la llamada en el detalle de llamada de ambos internos. */
void Red::iniciarLlamada(   unsigned int centralIdA, unsigned int internoIdA,
                            unsigned int centralIdB, unsigned int internoIdB,
                            unsigned int minuto){

    Central* centralA = buscarCentral(centralIdA);
    Central* centralB = buscarCentral(centralIdB);

    bool recorridoDisponible = true;


    caminoMinimo = crearCamino(centralA,centralB);
    recorridoDisponible = verificarDisponibilidad();
    imprimirCamino();

    Interno* internoA = centralA->buscarInterno(internoIdA);
    Interno* internoB = centralB->buscarInterno(internoIdB);

    if (!recorridoDisponible) {
        centralA->agregarAnuladaFaltaEnlace();
    } else {

        if (internoA == NULL){
            internoA = centralA->agregarInterno(internoIdA);
        }

        if (internoB == NULL){
            internoB = centralB->agregarInterno(internoIdB);
        }

        /*  Busco datos de llamada del interno B en la lista de detalles de llamadas de A
            Si no existiere, la creo*/
        Lista<DatosLlamadas*>* detallesLlamadasA = internoA->obtenerDetallesLlamadas();

        DatosLlamadas* datosLlamadasAB = NULL;
        DatosLlamadas* datosLlamadasActual = NULL;

        if (!detallesLlamadasA->estaVacia()){
            detallesLlamadasA->iniciarCursor();
            while (detallesLlamadasA->avanzarCursor() && datosLlamadasAB==NULL){
                datosLlamadasActual = detallesLlamadasA->obtenerCursor();
                if (datosLlamadasActual->obtenerIdLlamada()==internoIdB &&
                datosLlamadasActual->obtenerCentralId()==centralIdB){
                    datosLlamadasAB = datosLlamadasActual;
                }
            }
        }

        if (datosLlamadasAB==NULL){
            datosLlamadasAB=internoA->agregarDatosLlamadas(internoIdB, centralIdB);
        }

        /*  Busco datos de llamada del interno A en la lista de detalles de llamadas de B
            Si no existiere, la creo*/
        Lista<DatosLlamadas*>* detallesLlamadasB = internoB->obtenerDetallesLlamadas();

        DatosLlamadas* datosLlamadasBA = NULL;
        datosLlamadasActual = NULL;

        if (!detallesLlamadasB->estaVacia()){
            detallesLlamadasB->iniciarCursor();
            while (detallesLlamadasB->avanzarCursor() && datosLlamadasBA==NULL){
                datosLlamadasActual = detallesLlamadasB->obtenerCursor();
                if (datosLlamadasActual->obtenerIdLlamada()==internoIdA &&
                datosLlamadasActual->obtenerCentralId()==centralIdA){
                    datosLlamadasBA = datosLlamadasActual;
                }
            }
        }

        if (datosLlamadasBA==NULL){
            datosLlamadasBA=internoB->agregarDatosLlamadas(internoIdA, centralIdA);
        }

        /*Ya creados los datos de llamada, computo el inicio de la llamada,
            si interno B está ocupado, agrego ocupado en los dos internos,
            si no agrego la llamada y cambio los minutos del interno*/

        bool internoBDisponible  = (internoB->obtenerMinutos()>=9990); // 9999 indica que el interno está disponible

        if (internoBDisponible) {

            /*  Guardo camino en interno origen para recuperarlo al finalizar
                la llamada y liberar canales
            */

            internoA->modificarUltimoRecorrido(caminoMinimo);
            ocuparCanales();

            datosLlamadasAB->modificarEmitidas(datosLlamadasAB->obtenerEmitidas()+1);
            datosLlamadasBA->modificarRecibidas(datosLlamadasBA->obtenerRecibidas()+1);
            internoA->modificarMinutos(minuto);
            internoB->modificarMinutos(minuto);
        } else {
            datosLlamadasAB->modificarOcupadoEmitidos(datosLlamadasAB->obtenerOcupadosEmitidos()+1);
            datosLlamadasBA->modificarOcupadoRecibido(datosLlamadasBA->obtenerOcupadoRecibido()+1);

        }
    }
}

void Red::finalizarLlamada(unsigned int centralIdA, unsigned int internoIdA,
                          unsigned int centralIdB, unsigned int internoIdB,
                          unsigned int minutoFin){
    Central* centralA = buscarCentral(centralIdA);
    Central* centralB = buscarCentral(centralIdB);
    Interno* internoA = centralA->buscarInterno(internoIdA);
    Interno* internoB = centralB->buscarInterno(internoIdB);

    unsigned int minutoInicio = internoA->obtenerMinutos();

    unsigned int duracion = minutoFin - minutoInicio;

    Lista<unsigned int>* recorridoUltimaLlamada = internoA->obtenerUltimoRecorrido();



    unsigned int costo = calcularCostoYLiberarCanales(recorridoUltimaLlamada, duracion);

    //cout<<endl<<endl<<endl<<"Costo:   "<<costo<<endl;
    //cout<<"duracion:   "<<duracion<<endl;
    //cout<<"minutoInicio:   "<<minutoInicio<<endl;

    /*Busco en el detalle de llamadas de A el DatoLlamada del interno B*/
    Lista<DatosLlamadas*>* detallesLlamadasA = internoA->obtenerDetallesLlamadas();

    DatosLlamadas* datosLlamadasAB = NULL;
    DatosLlamadas* datosLlamadasActual = NULL;

    detallesLlamadasA->iniciarCursor();
    while (detallesLlamadasA->avanzarCursor() && datosLlamadasAB==NULL){
        datosLlamadasActual = detallesLlamadasA->obtenerCursor();
        if (datosLlamadasActual->obtenerIdLlamada()==internoIdB &&
        datosLlamadasActual->obtenerCentralId()==centralIdB){
            datosLlamadasAB = datosLlamadasActual;
        }
    }
    /*Busco en el detalle de llamadas de B el DatoLlamada del interno A*/
    Lista<DatosLlamadas*>* detallesLlamadasB = internoB->obtenerDetallesLlamadas();

    DatosLlamadas* datosLlamadasBA = NULL;
    datosLlamadasActual = NULL;

    detallesLlamadasB->iniciarCursor();
    while (detallesLlamadasB->avanzarCursor() && datosLlamadasBA==NULL){
        datosLlamadasActual = detallesLlamadasB->obtenerCursor();
        if (datosLlamadasActual->obtenerIdLlamada()==internoIdA &&
        datosLlamadasActual->obtenerCentralId()==centralIdA){
            datosLlamadasBA = datosLlamadasActual;
        }
    }

    /*  Agrego minutos a cada interno, gasto a interno A,
        dejo los interno en minuto 9999 para indicar que ahora están disponibles*/
    datosLlamadasAB->modificarMinEmitidos(datosLlamadasAB->obtenerMinEmitidos()+duracion);
    datosLlamadasAB->modificarGasto(datosLlamadasAB->obtenerGasto() + costo);
    datosLlamadasBA->modificarMinRecibidos(datosLlamadasBA->obtenerMinRecibidos()+duracion);

    internoA->modificarMinutos(9999);
    internoB->modificarMinutos(9999);

}

void Red::ocuparCanales(){

    Enlace* enlaceActual = NULL;
    unsigned int idAnterior = 0;
    unsigned int idActual = 0;

    bool disponible = true;
    unsigned int i = 1;
    unsigned int pasos = caminoMinimo->contarElementos();

    while (i<pasos && disponible){

        idAnterior = caminoMinimo->obtener(i);

        idActual = caminoMinimo->obtener(i+1);

        enlaceActual = buscarEnlace(idAnterior, idActual);

        enlaceActual->ocuparCanal();

        i++;
    }

}

Lista<unsigned int>* Red::crearCamino(Central* c0, Central* cDestino){


    Lista<Lista4<Central*>*>* listasNiveles = new Lista<Lista4<Central*>*>;

    unsigned int nivelActual = 1;

    Lista4<Central*>* nivel = new Lista4<Central*>;

    listasNiveles->agregar(nivel, nivelActual);

    nivel->agregar(0, 1, c0, NULL,0, nivelActual);

    bool caminoFinalizado = false;

    //nivelActual++;

    while (!caminoFinalizado){

        Nodo4<Central*>* vPonderado = ponderarVertice(listasNiveles->obtener(nivelActual));


        if (vPonderado!=NULL) {
            vPonderado->cambiarEstado(2);
            //cout<<"vPonderado:  "<< vPonderado->obtenerCentral()->obtenerId()<<endl;

            //cout<<"Listas niveles:  "<<listasNiveles->contarElementos()<<endl;
            if (listasNiveles->contarElementos()<= nivelActual) {
                Lista4<Central*>* nuevoNivel = new Lista4<Central*>;
                //cout<<endl<<"Nueva lista nivel:  "<<nivelActual+1<<endl;
                listasNiveles->agregar(nuevoNivel);
                //nivelActual++;

            }

            cargarVertices(vPonderado, listasNiveles);

        } else {
            nivelActual++;
            //cout<<endl<<"Nivel ++"<<endl;
            if (listasNiveles->contarElementos() < nivelActual){

                caminoFinalizado = true;
            }
        }
    }

    caminoMinimo = obtenerCamino(listasNiveles, cDestino);


    /*
        Liberación de memoria:

        Borro todos los niveles y la lista que los contiene.
        Las centrales las borro en el destructor de la red.

    */
    listasNiveles->iniciarCursor();
    Lista4<Central*>* nivelABorrar = NULL;
    while (listasNiveles->avanzarCursor()){
        nivelABorrar = listasNiveles->obtenerCursor();
        delete nivelABorrar;
    }
    delete listasNiveles;

    return this->caminoMinimo;
}

Lista<unsigned int>* Red::obtenerCamino(Lista<Lista4<Central*>*>* niveles, Central* cDestino){
    Nodo4<Central*>* vertice;
    Nodo4<Central*>* verticeActual;
    Lista<unsigned int>* camino = new Lista<unsigned int>;

    Lista4<Central*>* vertices = buscarVertices(cDestino, niveles);

    vertices->iniciarCursor();

    //cout << "cDestino:  " << cDestino->obtenerId() << endl;

    while (vertices->avanzarCursor()){

        verticeActual = vertices->obtenerCursor();
        //cout << "verticeActual:  " << verticeActual->obtenerCentral()->obtenerId() << endl;
        if ( cDestino == verticeActual->obtenerCentral()){
            vertice = verticeActual;
        }

    }

    camino->agregar(cDestino->obtenerId());

    Central* cAdyacente;


    while (vertice->obtenerAdyacente() != NULL){
        cAdyacente = vertice->obtenerAdyacente()->obtenerCentral();
        vertice = vertice->obtenerAdyacente();
        camino->agregar(cAdyacente->obtenerId(),1);
        //cout << "cAdyacente:  " << cAdyacente->obtenerId() << endl;
    }

    delete vertices;
    return camino;
}

Lista4<Central*>* Red::buscarVertices(Central* cDestino, Lista<Lista4<Central*>*>* niveles){
    Lista4<Central*>* vertices = new Lista4<Central*>;
    Lista4<Central*>* nivel;
    Nodo4<Central*>* vertice = NULL;
    Nodo4<Central*>* verticeAnterior = NULL;


    niveles->iniciarCursor();

    while (niveles->avanzarCursor()){
        nivel = niveles->obtenerCursor();

        nivel->iniciarCursor();



        while (nivel->avanzarCursor()){
            if (vertices->estaVacia()){
                verticeAnterior = nivel->obtenerCursor();
                if (verticeAnterior->obtenerCentral()== cDestino){
                    vertices->agregar(verticeAnterior->obtenerPeso(),0, verticeAnterior->obtenerCentral(), verticeAnterior->obtenerAdyacente(),0,0);
                }
            } else {
                vertice = nivel->obtenerCursor();
                if (vertice->obtenerCentral() == cDestino &&
                vertice->obtenerPeso() <= verticeAnterior->obtenerPeso()){

                vertices->agregar(vertice->obtenerPeso(),0, vertice->obtenerCentral(), vertice->obtenerAdyacente(),0,0);
                verticeAnterior = vertice;
                }

            }

        }
    }

    return vertices;

}

void Red::cargarVertices (Nodo4<Central*>* vPonderado, Lista<Lista4<Central*>*>* niveles){

    Central* cPonderada = vPonderado->obtenerCentral();
    Lista<EnlaceACentral*>* enlacesPonderada = cPonderada->obtenerEnlacesAOtrasCentrales();
    EnlaceACentral* enlaceActual = NULL;

    unsigned int nNivelActual = vPonderado->obtenerNivel() + 1;

    Lista4<Central*>* nivel = niveles->obtener(nNivelActual);

    enlacesPonderada->iniciarCursor();

    while (enlacesPonderada->avanzarCursor()){
        enlaceActual = enlacesPonderada->obtenerCursor();
        //cout<<"enlaceActual->obtenerIdOtraCentral():  "<<enlaceActual->obtenerIdOtraCentral()<<endl;

        Central* cNueva = buscarCentral(enlaceActual->obtenerIdOtraCentral());
        Nodo4<Central*>* vDuplicado = buscarCentralVertice(cNueva, niveles);

        Lista4<Central*>* nivelDuplicado = niveles->obtener(this->coordVertice[0]);


        if (vDuplicado!=NULL) {

            //Nodo4<Central*>* vNuevo = NULL;
            /*if (vDuplicado->obtenerEstado()==3){
                nivel->remover(coordVertice[1]);
                vDuplicado = buscarCentralVertice(cNueva, niveles);
            }*/

            /*while (vDuplicado->obtenerEstado()==3){
                nivel->remover(coordVertice[1]);
                cout<<"  Removido duplicado 3:  "<<vDuplicado->obtenerCentral()->obtenerId()<<"  Peso:  "<<vDuplicado->obtenerPeso()<<endl;
                vDuplicado = buscarCentralVertice(cNueva, niveles);
            }*/

            while ((vDuplicado->obtenerEstado()==1 || vDuplicado->obtenerEstado()==3 ) &&
                vDuplicado->obtenerPeso()> enlaceActual->obtenerPesoEnlace()+vPonderado->obtenerPeso()){

                //Lista4<Central*>* nivelDuplicado = niveles->obtener(this->coordVertice[0]);
                //cout<<"  Removido duplicado 1:  "<<vDuplicado->obtenerCentral()->obtenerId()<<"  Peso:  "<<vDuplicado->obtenerPeso()<<endl;
                nivelDuplicado->remover(this->coordVertice[1]);
                //delete vDuplicado;




                Nodo4<Central*>* vNuevo = nivel->agregar(enlaceActual->obtenerPesoEnlace()+vPonderado->obtenerPeso(),1,cNueva,vPonderado,enlaceActual->obtenerPesoEnlace(), nNivelActual);
                //cout<<"Agregada:  "<<cNueva->obtenerId()<<"   Peso:  "<<enlaceActual->obtenerPesoEnlace()+vPonderado->obtenerPeso()<<endl;
            }

            if (vDuplicado->obtenerEstado()==2 &&
                vDuplicado->obtenerPeso()> enlaceActual->obtenerPesoEnlace()+vPonderado->obtenerPeso()){

                Nodo4<Central*>* vNuevo = nivel->agregar(enlaceActual->obtenerPesoEnlace()+vPonderado->obtenerPeso(),
                                                         2, vDuplicado->obtenerCentral(),
                                                         vPonderado, enlaceActual->obtenerPesoEnlace(),
                                                         vPonderado->obtenerNivel()+1);

                //cout<<"  Removido duplicado 2:  "<<vDuplicado->obtenerCentral()->obtenerId()<<"  Peso:  "<<vDuplicado->obtenerPeso()<<endl;

                //vDuplicado->cambiarAdyacente(vNuevo);
                restaurarVertices(niveles, vDuplicado, vNuevo);
                //imprimirNiveles(niveles);
                vDuplicado->cambiarEstado(3);


            }


        } else {
            Lista4<Central*>* nivel = niveles->obtener(nNivelActual);
            Nodo4<Central*>* vNuevo = nivel->agregar(enlaceActual->obtenerPesoEnlace()+vPonderado->obtenerPeso(),1,cNueva,vPonderado,enlaceActual->obtenerPesoEnlace(), nNivelActual);
        }

    }



}

Nodo4<Central*>* Red::ponderarVertice(Lista4<Central*>* nivel) {
    Nodo4<Central*>* vPonderado = NULL;
    Nodo4<Central*>* vActual = NULL;

    nivel->iniciarCursor();

    while (nivel->avanzarCursor()){

        vActual = nivel->obtenerCursor();

        if (vActual->obtenerEstado()==1){
            if (vPonderado==NULL){
                vPonderado = vActual;
            } else {
                if (vPonderado->obtenerPeso() > vActual->obtenerPeso()){
                    vPonderado = vActual;
                }
            }
        }
    }


    return vPonderado;
}

Nodo4<Central*>* Red::buscarCentralVertice(Central* central, Lista<Lista4<Central*>*>* listasNiveles){
    Nodo4<Central*>* verticeSolicitado = NULL;
    Nodo4<Central*>* verticeActual = NULL;
    Lista4<Central*>* nivelActual = NULL;

    unsigned int i=1;
    unsigned int j;



    listasNiveles->iniciarCursor();

    while (listasNiveles->avanzarCursor() && verticeSolicitado==NULL){

        nivelActual = listasNiveles->obtenerCursor();

        nivelActual->iniciarCursor();

        j=1;
        while (nivelActual->avanzarCursor() && verticeSolicitado==NULL){
            verticeActual = nivelActual->obtenerCursor();


            if (verticeActual->obtenerCentral()==central) {
                verticeSolicitado = verticeActual;
            }
            j++;

        }
        i++;


    }
    this->coordVertice[0]=i-1;
    this->coordVertice[1]=j-1;

    return verticeSolicitado;
}

void Red::restaurarVertices(Lista<Lista4<Central*>*>* niveles, Nodo4<Central*>* vDuplicado,
                            Nodo4<Central*>* vNuevo){
    unsigned int nNivel = 2;
    //cout<<endl<<"Restaurar nivel"<<endl;
    Lista4<Central*>* nivelActual = niveles->obtener(nNivel);

    while (nNivel<=niveles->contarElementos()) {

        Lista4<Central*>* nivelActual = niveles->obtener(nNivel);

        nivelActual->iniciarCursor();

        unsigned int pos = 1;

        while (nivelActual->avanzarCursor()){

            Nodo4<Central*>* vActual = nivelActual->obtenerCursor();

            if (vActual->obtenerAdyacente() == vDuplicado){
                vActual->cambiarAdyacente(vNuevo);
            }

            Nodo4<Central*>* vAdyacente = vActual->obtenerAdyacente();
            vActual->cambiarPeso(vAdyacente->obtenerPeso()+vActual->obtenerPesoAdyacente());



            if (vActual->obtenerNivel() != vAdyacente->obtenerNivel()+1){

                while (niveles->contarElementos() < vAdyacente->obtenerNivel()+1){
                    Lista4<Central*>* nuevoNivel = new Lista4<Central*>;
                    //cout<<endl<<"Nueva lista nivel(restaurar):  "<<niveles->contarElementos()+1<<endl;
                    niveles->agregar(nuevoNivel);
                }

                Lista4<Central*>* agregarANivel = niveles->obtener(vAdyacente->obtenerNivel() + 1);

                Nodo4<Central*>* nuevoVertice = agregarANivel->agregar(vActual->obtenerPeso(), 2,
                                                        vActual->obtenerCentral(), vAdyacente,
                                                        vAdyacente->obtenerPesoAdyacente(), vAdyacente->obtenerNivel()+1);

                vActual->cambiarEstado(3);
            }
            vActual->cambiarNivel(vAdyacente->obtenerNivel()+1);
            pos++;
        }
        nNivel++;
    }
}



/*  Devuelve true si los enlaces entre los pasos del recorrido
    tienen algún canal libre para establecer la comunicación.
    Si algún enlace no dispone de canales, devuelve false
    y en la central destino agrega una llamada anulada por falta de enlace.*/
bool Red::verificarDisponibilidad(){

    Enlace* enlaceActual = NULL;
    unsigned int idAnterior = 0;
    unsigned int idActual = 0;
    bool disponible = true;
    unsigned int i = 1;
    unsigned int pasos = caminoMinimo->contarElementos();

    while (i<pasos && disponible){

        idAnterior = caminoMinimo->obtener(i);

        idActual = caminoMinimo->obtener(i+1);

        enlaceActual = buscarEnlace(idAnterior, idActual);

        unsigned canalesDisponibles = enlaceActual->obtenerCanalesDisponibles();
        //cout<<"Canales disp:   "<<canalesDisponibles<<endl;
        disponible = (canalesDisponibles > 0);

        i++;
    }

    return disponible;
}

Enlace* Red::buscarEnlace(unsigned int idCentralA, unsigned int idCentralB){
    Enlace* enlaceActual = NULL;
    listaEnlaces->iniciarCursor();
    bool encontrado = false;
    while (!encontrado && listaEnlaces->avanzarCursor()){
        enlaceActual = listaEnlaces->obtenerCursor();
        unsigned int idEnlaceA = enlaceActual->obtenerCentralIdA();
        unsigned int idEnlaceB = enlaceActual->obtenerCentralIdB();
        if (
            (idEnlaceA==idCentralA && idEnlaceB==idCentralB)||
            (idEnlaceA==idCentralB && idEnlaceB==idCentralA)
            )
        {
            encontrado = true;
        }
    }
    return enlaceActual;
}

float Red::calcularCostoYLiberarCanales(Lista<unsigned int>* camino, unsigned int duracion){

    unsigned int idA = 0;
    unsigned int idB = 0;

    Enlace* enlaceActual = NULL;

    float costoTotal = 0;
    float costoEnlace = 0;



    unsigned int pasos = camino->contarElementos();
    for (unsigned int i=1; i<pasos; i++)
    {
        idA = camino->obtener(i);
        idB = camino->obtener(i+1);

        enlaceActual = buscarEnlace(idA, idB);
        costoEnlace = enlaceActual->obtenerCosto();
        enlaceActual->liberarCanal();

        costoTotal += costoEnlace*duracion;
    }
    return costoTotal;
}

Central* Red::buscarCentral (unsigned int idSolicitado){
    Central* centralActual = NULL;

    if (!listaCentrales->estaVacia()){
        this->listaCentrales->iniciarCursor();
        bool centralEncontrada = false;
        while (!centralEncontrada && this->listaCentrales->avanzarCursor()) {
            centralActual = this->listaCentrales->obtenerCursor();
            unsigned int idActual = centralActual->obtenerId();
            centralEncontrada = (idSolicitado==idActual);
        }
        if (!centralEncontrada){
            centralActual=NULL;
        }
    }
    return centralActual;
}

unsigned int Red::buscarCentral(Central* centralSolicitada){

    unsigned int idCentralSolicitado = 0;
    listaCentrales->iniciarCursor();
    Central* centralActual = NULL;

    while (listaCentrales->avanzarCursor()){
        centralActual = listaCentrales->obtenerCursor();
        if (centralActual==centralSolicitada){
            idCentralSolicitado = centralActual->obtenerId();
        }
    }
    return idCentralSolicitado;
}

unsigned int Red::buscarCentral (unsigned int centralSolicitada, Lista<EnlaceACentral*>* enlacesACentrales){
    EnlaceACentral* enlaceACentralActual = NULL;
    unsigned int centralActual = 0;
    enlacesACentrales->iniciarCursor();
    while (centralActual==0 && enlacesACentrales->avanzarCursor()){
        enlaceACentralActual = enlacesACentrales->obtenerCursor();
        centralActual = enlaceACentralActual->obtenerIdOtraCentral();
        if (!centralActual==centralSolicitada){
            centralActual = 0;
        }
    }
    return centralActual;
}

Lista<Central*>* Red::obtenerListaDeCentrales()
{
    return this->listaCentrales;
}

Lista<Enlace*>* Red::obtenerListaDeEnlaces()
{
    return this->listaEnlaces;
}

void Red::calcularMaximos(){

if (!this->maxCalculados){

    unsigned int parcialMinutosEmitidos = 0;
    unsigned int parcialLlamadasEmitidas = 0;
    unsigned int parcialOcupadasEmitidas = 0;
    unsigned int parcialMinutosRecibidos = 0;
    unsigned int parcialLlamadasRecibidas = 0;
    unsigned int parcialOcupadasRecibidas = 0;
    float parcialGasto = 0;

    listaCentrales->iniciarCursor();
    Central* centralActual = NULL;
    Interno* internoActual = NULL;

    while (listaCentrales->avanzarCursor()){

        unsigned int actualMinutosEmitidos = 0;
        unsigned int actualLlamadasEmitidas = 0;
        unsigned int actualOcupadasEmitidas = 0;
        unsigned int actualMinutosRecibidos = 0;
        unsigned int actualLlamadasRecibidas = 0;
        unsigned int actualOcupadasRecibidas = 0;
        float actualGasto = 0;

        centralActual = listaCentrales->obtenerCursor();
        centralActual->calcularMaximosCentral();

        internoActual = centralActual->obtenerMaxMinutosEmitidos();
        if (internoActual!=NULL){
            actualMinutosEmitidos = internoActual->sumarMinutosEmitidos();
            if (actualMinutosEmitidos > parcialMinutosEmitidos){
                this->maxMinutosEmitidos = centralActual;
                parcialMinutosEmitidos = actualMinutosEmitidos;
            }
        }

        internoActual = centralActual->obtenerMaxLlamadasEmitidas();
        if (internoActual!=NULL){
            actualLlamadasEmitidas = internoActual->sumarLlamadasEmitidas();
            if (actualLlamadasEmitidas > parcialLlamadasEmitidas){
                this->maxLlamadasEmitidas = centralActual;
                parcialLlamadasEmitidas = actualLlamadasEmitidas;
            }
        }

        internoActual = centralActual->obtenerMaxOcupadasEmitidas();
        if (internoActual!=NULL){
            actualOcupadasEmitidas = internoActual->sumarOcupadasEmitidas();
            if (actualOcupadasEmitidas > parcialOcupadasEmitidas){
                this->maxOcupadoEmitidas = centralActual;
                parcialOcupadasEmitidas = actualOcupadasEmitidas;
            }
        }

        internoActual = centralActual->obtenerMaxMinutosRecibidos();
        if (internoActual!=NULL){
            actualMinutosRecibidos = internoActual->sumarMinutosRecibidos();
            if (actualMinutosRecibidos > parcialMinutosRecibidos){
                this->maxMinutoRecibidos = centralActual;
                parcialMinutosRecibidos = actualMinutosRecibidos;
            }
        }

        internoActual = centralActual->obtenerMaxLlamadasRecibidas();
        if (internoActual!=NULL){
            actualLlamadasRecibidas = internoActual->sumarLlamadasRecibidas();
            if (actualLlamadasRecibidas > parcialLlamadasRecibidas){
                this->maxLlamadasRecibidas = centralActual;
                parcialLlamadasRecibidas = actualLlamadasRecibidas;
            }
        }

        internoActual = centralActual->obtenerMaxOcupadasRecibidas();
        if (internoActual!=NULL){
            actualOcupadasRecibidas = internoActual->sumarOcupadasRecibidas();
            if (actualOcupadasRecibidas > parcialOcupadasRecibidas){
                this->maxOcupadoRecibidos = centralActual;
                parcialOcupadasRecibidas = actualOcupadasRecibidas;
            }
        }

        internoActual = centralActual->obtenerMaxGasto();
        if (internoActual!=NULL){
            actualGasto = internoActual->sumarGasto();
            if (actualGasto > parcialGasto){
                this->maxGasto = centralActual;
                parcialGasto = actualGasto;
            }
        }


    }

    this->maxCalculados = true;

}

}


bool Red::maximosCalculados (){
    return this->maxCalculados;
}

Central* Red::obtenerMaxMinutosEmitidos(){
    return this->maxMinutosEmitidos;
}
Central* Red::obtenerMaxLlamadasEmitidas(){
    return this->maxLlamadasEmitidas;
}
Central* Red::obtenerMaxOcupadasEmitidas(){
    return this->maxOcupadoEmitidas;
}
Central* Red::obtenerMaxMinutosRecibidos(){
    return this->maxMinutoRecibidos;
}
Central* Red::obtenerMaxLlamadasRecibidas(){
    return this->maxLlamadasRecibidas;
}
Central* Red::obtenerMaxOcupadasRecibidas(){
    return this->maxOcupadoRecibidos;
}
Central* Red::obtenerMaxGasto(){
    return this->maxGasto;
}

void Red::imprimirRed(){

    listaCentrales->iniciarCursor();
    while (listaCentrales->avanzarCursor()) {
        Central* centralAlctual = listaCentrales->obtenerCursor();

        cout <<endl<<endl<< "********************     CENTRAL ID:  " << centralAlctual->obtenerId() << "     ********************"<<endl<<endl;
        cout << "Llamadas anuladas por falta de enlace:   " << centralAlctual->obtenerAnuladaFaltaEnlace() << endl<<endl;
        Lista<EnlaceACentral*>* enlacesAOtrasCentrales = centralAlctual->obtenerEnlacesAOtrasCentrales();
        EnlaceACentral* enlaceACentral = NULL;
        enlacesAOtrasCentrales->iniciarCursor();
        while (enlacesAOtrasCentrales->avanzarCursor()){
            enlaceACentral = enlacesAOtrasCentrales->obtenerCursor();
            cout << "Enlazada con central:   " << enlaceACentral->obtenerIdOtraCentral()<<endl;
        }

        Lista<Interno*>* listaInternosActual = centralAlctual->obtenerListaInternos();

        if (!listaInternosActual->estaVacia()){
            listaInternosActual->iniciarCursor();
            while (listaInternosActual->avanzarCursor()){
                Interno* internoActual = listaInternosActual->obtenerCursor();
                cout <<endl<<endl<< "********     INTERNO ID:  " << internoActual->obtenerId() <<"     ********"<<endl;

                Lista<DatosLlamadas*>* listaLlamadas = internoActual->obtenerDetallesLlamadas();

                if (!listaLlamadas->estaVacia()){
                    cout <<endl<< "Llamadas con:" << endl;
                    while (listaLlamadas->avanzarCursor()){
                        DatosLlamadas* llamadaActual = listaLlamadas->obtenerCursor();
                        cout <<endl<< "***     ID:  "<< llamadaActual->obtenerIdLlamada();
                        cout << "   Central:  "<< llamadaActual->obtenerCentralId()<<"     ***"<<endl<<endl;
                        cout << "   Emitidas:  " << llamadaActual->obtenerEmitidas();
                        cout << "             Recibidas:  " << llamadaActual->obtenerRecibidas();
                        cout <<endl<< "   Ocupadas emitidas:  " << llamadaActual->obtenerOcupadosEmitidos();
                        cout << "    Ocupadas recibidas:  " << llamadaActual->obtenerOcupadoRecibido();
                        cout <<endl<< "   Min emitidos: " << llamadaActual->obtenerMinEmitidos();
                        cout << "          Min recibidos:  " << llamadaActual->obtenerMinRecibidos();
                        cout << endl << "   Gasto:  " << llamadaActual->obtenerGasto()<<endl;
                    }
                }
            }

        }
    }
}

void Red::imprimirNiveles(Lista<Lista4<Central*>*>* listasNiveles){

    listasNiveles->iniciarCursor();

    while (listasNiveles->avanzarCursor()){

        Lista4<Central*>* nivelAux = listasNiveles->obtenerCursor();

        nivelAux->iniciarCursor();

        while (nivelAux->avanzarCursor()){

            Nodo4<Central*>* verticeActual= NULL;
            Central* centralAux = NULL;
            Nodo4<Central*>* verticeAdyacente = NULL;
            Central* centralAdy = NULL;

            verticeActual = nivelAux->obtenerCursor();
            centralAux = verticeActual->obtenerCentral();
            verticeAdyacente = verticeActual->obtenerAdyacente();
            cout << "Central:  "<< centralAux->obtenerId();
            if (verticeAdyacente!=NULL){
                centralAdy = verticeAdyacente->obtenerCentral();
                cout<<"  Ady:  "<<centralAdy->obtenerId();
            }
            cout<<"  PesoAdy:  "<<verticeActual->obtenerPesoAdyacente();
            cout<<"  Peso:  "<< verticeActual->obtenerPeso();
            cout<<"  Estado:  "<< verticeActual->obtenerEstado();
            cout<<"  Nivel:  "<< verticeActual->obtenerNivel() << endl;
        }

    }

}

void Red::imprimirCamino(){

    unsigned int idActual=0;
    caminoMinimo->iniciarCursor();
    cout<<endl<<endl<<"Camino minimo: "<<idActual<<endl;
    while (caminoMinimo->avanzarCursor()){
        idActual = caminoMinimo->obtenerCursor();
        cout<<"Id central: "<<idActual<<endl;
    }
}












// Métodos menú

void Red::llamadasTotalesXAYB()
{
    unsigned int internoX = 0;
    unsigned int centralA = 0;
    unsigned int internoY = 0;
    unsigned int centralB = 0;


    cout<<endl<< "Ingrese una central A: ";
    cin >> centralA;
    Central* CentralA = buscarCentral(centralA);


    while ( CentralA == NULL || CentralA->obtenerListaInternos()->estaVacia() ) {

        cout<<endl<< "La central: " << centralA << "  no existe o no posee internos. Ingrese una nueva central A: ";
        cin >> centralA;
        CentralA = buscarCentral(centralA);

    }

    cout<<endl<< "Ingrese un interno X: ";
    cin >> internoX;
    Interno* InternoX = CentralA->buscarInterno(internoX);

    while ( InternoX == NULL) {

        cout<<endl<< "El Interno: " << internoX << "  no existe. Ingrese un nuevo interno X: ";
        cin >> internoX;
        InternoX = CentralA->buscarInterno(internoX);

    }






    cout<<endl<< "Ingrese una central B: ";
    cin >> centralB;
    Central* CentralB = buscarCentral(centralB);


    while ( CentralB == NULL || CentralB->obtenerListaInternos()->estaVacia() ) {

        cout<<endl<< "La central: " << centralB << "  no existe o no posee internos. Ingrese una nueva central B: ";
        cin >> centralB;
        CentralB = buscarCentral(centralB);

    }

    cout<<endl<< "Ingrese un interno Y: ";
    cin >> internoY;
    Interno* InternoY = CentralB->buscarInterno(internoY);

    while ( InternoY == NULL) {

        cout<<endl<< "El Interno: " << internoY << "  no existe. Ingrese un nuevo interno Y: ";
        cin >> internoY;
        InternoY = CentralB->buscarInterno(internoY);

    }


    DatosLlamadas* llamadasXY = InternoX->buscarDatosLlamadas(internoY,centralB);

    if ( llamadasXY != NULL) {

        cout<<endl<<endl<< "Detalle de llamadas del interno " << internoX << " de la central " << centralA << ", al interno " << internoY << " de la central " << centralB << endl<<endl;

        cout << "Llamadas emitidas: " << llamadasXY->obtenerEmitidas() << endl;

        cout << "Minutos emitidos: " << llamadasXY->obtenerMinEmitidos() << endl;

        cout << "Ocupadas: " << llamadasXY->obtenerOcupadosEmitidos();




        cout<<endl<<endl<< "Detalle de llamadas del interno " << internoY << " de la central " << centralB << ", al interno " << internoX << " de la central " << centralA << endl<<endl;

        cout << "Llamadas emitidas: " << llamadasXY->obtenerRecibidas() <<endl;

        cout << "Minutos emitidos: " << llamadasXY->obtenerMinRecibidos() <<endl;

        cout << "Ocupadas: " << llamadasXY->obtenerOcupadoRecibido() <<endl<<endl;
    } else {

        cout<<endl<<endl<< "No hubo comunicaciones entre los internos pedidos." << endl;
    }

}

void Red::internoMasHablo(){

    this->calcularMaximos();

    cout << endl << endl << "Interno que mas minutos hablo (minutos emitidos) de toda la red y de cada central" << endl << endl;

    Central* maximoCentral = this->obtenerMaxMinutosEmitidos();
    if (maximoCentral!=NULL){
        Interno* maximoInterno = maximoCentral->obtenerMaxMinutosEmitidos();
        cout << "MAXIMO RED. Central: " << maximoCentral->obtenerId() << "  Interno:  " << maximoInterno->obtenerId()<<endl  << endl;
    } else {
        cout << "No hubo llamados en la red" << endl << endl;
    }

    Lista<Central*>* centrales = this->obtenerListaDeCentrales();
    Central* centralActual = NULL;
    Interno* internoMax = NULL;
    centrales->iniciarCursor();
    while (centrales->avanzarCursor()){
        centralActual = centrales->obtenerCursor();
        internoMax = centralActual->obtenerMaxMinutosEmitidos();
        if (internoMax!=NULL){
            cout << "Maximo central:  " << centralActual->obtenerId() << "  Interno:  " << internoMax->obtenerId() << "  Minutos:  " << internoMax->sumarMinutosEmitidos() << endl << endl;
        }
    }

}

void Red::internoMasLLamo(){
    this->calcularMaximos();

    cout << endl << endl << "Interno que mas llamo de toda la red y de cada central" << endl << endl;

    Central* maximoCentral = this->obtenerMaxLlamadasEmitidas();
    if (maximoCentral!=NULL){
        Interno* maximoInterno = maximoCentral->obtenerMaxLlamadasEmitidas();
        cout << "MAXIMO RED. Central: " << maximoCentral->obtenerId() << "  Interno:  " << maximoInterno->obtenerId()<<endl  << endl;
    } else {
        cout << "No hubo llamados en la red" << endl << endl;
    }

    Lista<Central*>* centrales = this->obtenerListaDeCentrales();
    Central* centralActual = NULL;
    Interno* internoMax = NULL;
    centrales->iniciarCursor();
    while (centrales->avanzarCursor()){
        centralActual = centrales->obtenerCursor();
        internoMax = centralActual->obtenerMaxLlamadasEmitidas();
        if (internoMax!=NULL){
            cout << "Maximo central:  " << centralActual->obtenerId() << "  Interno:  " << internoMax->obtenerId() << "  Cantidad:  " << internoMax->sumarLlamadasEmitidas() << endl << endl;
        }
    }
}

void Red::internoMasLeDioOcupado(){

    this->calcularMaximos();

    cout << endl << endl << "Interno que mas le dio ocupado sus intentos de toda la red y de cada central" << endl << endl;

    Central* maximoCentral = this->obtenerMaxOcupadasEmitidas();
    if (maximoCentral!=NULL){
        Interno* maximoInterno = maximoCentral->obtenerMaxOcupadasEmitidas();
        cout << "MAXIMO RED. Central: " << maximoCentral->obtenerId() << "  Interno:  " << maximoInterno->obtenerId()<<endl  << endl;
    } else {
        cout << "No hubo intentos ocupados en la red" << endl << endl;
    }

    Lista<Central*>* centrales = this->obtenerListaDeCentrales();
    Central* centralActual = NULL;
    Interno* internoMax = NULL;
    centrales->iniciarCursor();
    while (centrales->avanzarCursor()){
        centralActual = centrales->obtenerCursor();
        internoMax = centralActual->obtenerMaxOcupadasEmitidas();
        if (internoMax!=NULL){
            cout << "Maximo central:  " << centralActual->obtenerId() << "  Interno:  " << internoMax->obtenerId() << "  Cantidad:  " << internoMax->sumarOcupadasEmitidas() << endl << endl;
        }
    }

}

void Red::internoMasLLamaron(){

    this->calcularMaximos();

    cout << endl << endl << "Interno al que mas llamaron de toda la red y de cada central" << endl << endl;

    Central* maximoCentral = this->obtenerMaxLlamadasRecibidas();
    if (maximoCentral!=NULL){
        Interno* maximoInterno = maximoCentral->obtenerMaxLlamadasRecibidas();
        cout << "MAXIMO RED. Central: " << maximoCentral->obtenerId() << "  Interno:  " << maximoInterno->obtenerId()<<endl  << endl;
    } else {
        cout << "No hubo llamados en la red" << endl << endl;
    }

    Lista<Central*>* centrales = this->obtenerListaDeCentrales();
    Central* centralActual = NULL;
    Interno* internoMax = NULL;
    centrales->iniciarCursor();
    while (centrales->avanzarCursor()){
        centralActual = centrales->obtenerCursor();
        internoMax = centralActual->obtenerMaxLlamadasRecibidas();
        if (internoMax!=NULL){
            cout << "Maximo central:  " << centralActual->obtenerId() << "  Interno:  " << internoMax->obtenerId() << "  Cantidad:  " << internoMax->sumarLlamadasRecibidas() << endl << endl;
        }
    }


}

void Red::internoMasGasto(){

    this->calcularMaximos();

    cout << endl << endl << "Interno que mas gasto de toda la red y de cada central" << endl << endl;

    Central* maximoCentral = this->obtenerMaxGasto();
    if (maximoCentral!=NULL){
        Interno* maximoInterno = maximoCentral->obtenerMaxGasto();
        cout << "MAXIMO RED. Central: " << maximoCentral->obtenerId() << "  Interno:  " << maximoInterno->obtenerId()<<endl  << endl;
    } else {
        cout << "No hubo llamados en la red" << endl << endl;
    }

    Lista<Central*>* centrales = this->obtenerListaDeCentrales();
    Central* centralActual = NULL;
    Interno* internoMax = NULL;
    centrales->iniciarCursor();
    while (centrales->avanzarCursor()){
        centralActual = centrales->obtenerCursor();
        internoMax = centralActual->obtenerMaxGasto();
        if (internoMax!=NULL){
            cout << "Maximo central:  " << centralActual->obtenerId() << "  Interno:  " << internoMax->obtenerId() << "  Centavos:  " << internoMax->sumarGasto() << endl << endl;
        }
    }


}

void Red::internoMasLeHablaron(){

    this->calcularMaximos();

    cout << endl << endl << "Interno que mas minutos le hablaron de toda la red y de cada central" << endl << endl;

    Central* maximoCentral = this->obtenerMaxMinutosRecibidos();

    if (maximoCentral!=NULL){
        Interno* maximoInterno = maximoCentral->obtenerMaxMinutosRecibidos();
        cout << "MAXIMO RED. Central: " << maximoCentral->obtenerId() << "  Interno:  " << maximoInterno->obtenerId()<<endl  << endl;
    } else {
        cout << "No hubo llamados en la red" << endl << endl;
    }

    Lista<Central*>* centrales = this->obtenerListaDeCentrales();
    Central* centralActual = NULL;
    Interno* internoMax = NULL;
    centrales->iniciarCursor();
    while (centrales->avanzarCursor()){
        centralActual = centrales->obtenerCursor();
        internoMax = centralActual->obtenerMaxMinutosRecibidos();
        if (internoMax!=NULL){
            cout << "Maximo central:  " << centralActual->obtenerId() << "  Interno:  " << internoMax->obtenerId() << "  Minutos:  " << internoMax->sumarMinutosRecibidos() << endl << endl;
        }
    }

}

void Red::internoMasDioOcupado(){

    this->calcularMaximos();

    cout << endl << endl << "Interno que mas dio ocupado de toda la red y de cada central" << endl << endl;

    Central* maximoCentral = this->obtenerMaxOcupadasRecibidas();
    if (maximoCentral!=NULL){
        Interno* maximoInterno = maximoCentral->obtenerMaxOcupadasRecibidas();
        cout << "MAXIMO RED. Central: " << maximoCentral->obtenerId() << "  Interno:  " << maximoInterno->obtenerId()<<endl  << endl;
    } else {
        cout << "No hubo intentos ocupados en la red" << endl << endl;
    }

    Lista<Central*>* centrales = this->obtenerListaDeCentrales();
    Central* centralActual = NULL;
    Interno* internoMax = NULL;
    centrales->iniciarCursor();
    while (centrales->avanzarCursor()){
        centralActual = centrales->obtenerCursor();
        internoMax = centralActual->obtenerMaxOcupadasRecibidas();
        if (internoMax!=NULL){
            cout << "Maximo central:  " << centralActual->obtenerId() << "  Interno:  " << internoMax->obtenerId() << "  Cantidad:  " << internoMax->sumarOcupadasRecibidas() << endl << endl;
        }
    }

}

void Red::llamadasEmitidasXA(){
    unsigned int internoX = 0;
    unsigned int centralA = 0;


    cout<<endl<< "Ingrese una central A: ";
    cin >> centralA;
    Central* CentralA = buscarCentral(centralA);


    while ( CentralA == NULL || CentralA->obtenerListaInternos()->estaVacia() ) {

        cout<<endl<< "La central: " << centralA << "  no existe o no posee internos. Ingrese una nueva central A: ";
        cin >> centralA;
        CentralA = buscarCentral(centralA);

    }

    cout<<endl<< "Ingrese un interno X: ";
    cin >> internoX;
    Interno* InternoX = CentralA->buscarInterno(internoX);

    while ( InternoX == NULL) {

        cout<<endl<< "El Interno: " << internoX << "  no existe. Ingrese un nuevo interno X: ";
        cin >> internoX;
        InternoX = CentralA->buscarInterno(internoX);

    }

    Lista<DatosLlamadas*>* llamadasXA = InternoX->obtenerDetallesLlamadas();

    llamadasXA->iniciarCursor();

    cout<<endl<<endl<<"LLamadas emitidas por el interno "<<internoX<<" de la central "<<centralA<<endl;

    while ( llamadasXA->avanzarCursor() ){

        DatosLlamadas* datoActual = llamadasXA->obtenerCursor();

        cout<<endl<<endl<<"***   Central: "<<datoActual->obtenerCentralId()<<"    Interno: "<<datoActual->obtenerIdLlamada()<<"   ***"<<endl;
        cout<<"Llamadas: "<<datoActual->obtenerEmitidas()<<endl;
        cout<<"Minutos: "<<datoActual->obtenerMinEmitidos()<<endl;
        cout<<"Ocupadas: "<<datoActual->obtenerOcupadosEmitidos()<<endl;

    }






}

void Red::llamadasRecibidasXA(){

    unsigned int internoX = 0;
    unsigned int centralA = 0;


    cout<<endl<< "Ingrese una central A: ";
    cin >> centralA;
    Central* CentralA = buscarCentral(centralA);


    while ( CentralA == NULL || CentralA->obtenerListaInternos()->estaVacia() ) {

        cout<<endl<< "La central: " << centralA << "  no existe o no posee internos. Ingrese una nueva central A: ";
        cin >> centralA;
        CentralA = buscarCentral(centralA);

    }

    cout<<endl<< "Ingrese un interno X: ";
    cin >> internoX;
    Interno* InternoX = CentralA->buscarInterno(internoX);

    while ( InternoX == NULL) {

        cout<<endl<< "El Interno: " << internoX << "  no existe. Ingrese un nuevo interno X: ";
        cin >> internoX;
        InternoX = CentralA->buscarInterno(internoX);

    }

    Lista<DatosLlamadas*>* llamadasXA = InternoX->obtenerDetallesLlamadas();

    llamadasXA->iniciarCursor();

    cout<<endl<<endl<<"LLamadas recibidas por el interno "<<internoX<<" de la central "<<centralA<<endl;

    while ( llamadasXA->avanzarCursor() ){

        DatosLlamadas* datoActual = llamadasXA->obtenerCursor();

        cout<<endl<<endl<<"***   Central: "<<datoActual->obtenerCentralId()<<"    Interno: "<<datoActual->obtenerIdLlamada()<<"   ***"<<endl;
        cout<<"Llamadas: "<<datoActual->obtenerRecibidas()<<endl;
        cout<<"Minutos: "<<datoActual->obtenerMinRecibidos()<<endl;
        cout<<"Ocupadas: "<<datoActual->obtenerOcupadoRecibido()<<endl;

    }

}



void Red::listadoAnuladas()
{
    this->obtenerListaDeCentrales()->iniciarCursor();
    while (this->obtenerListaDeCentrales()->avanzarCursor())
    {
        Central* central = this->obtenerListaDeCentrales()->obtenerCursor();
        if (central->obtenerAnuladaFaltaEnlace() != 0)
        {
            cout << "Central: " << central->obtenerId() << " Cantidad de llamadas anuladas: " << central->obtenerAnuladaFaltaEnlace() << endl;
        }
    }

}
void Red::listadoInternos()
{
    cout << "Central " << "Interno" << endl;
    this->obtenerListaDeCentrales()->iniciarCursor();
    while (this->obtenerListaDeCentrales()->avanzarCursor())
    {
        Central* central = this->obtenerListaDeCentrales()->obtenerCursor();
        central->obtenerListaInternos()->iniciarCursor();
        while (central->obtenerListaInternos()->avanzarCursor())
        {
            Interno* interno = central->obtenerListaInternos()->obtenerCursor();
            cout << central->obtenerId() << " - " << interno->obtenerId() << endl;
        }
    }

}



void Red::listadoEnlaces()
{
    cout << "Lista de Enlaces" << endl << endl;
    this->obtenerListaDeEnlaces()->iniciarCursor();
    while (this->obtenerListaDeEnlaces()->avanzarCursor())
    {
        Enlace* Enlaces = this->obtenerListaDeEnlaces()->obtenerCursor();
        cout << "Origen: " << Enlaces->obtenerCentralIdA() << "   Destino: " << Enlaces->obtenerCentralIdB() << "   Costo: "<<Enlaces->obtenerCosto()<< "   Canales " << Enlaces->obtenerCanales() << endl<<endl;
    }

}
void Red::listadoCentrales()
{
    this->obtenerListaDeCentrales()->iniciarCursor();
    unsigned int i = 1;
    while (this->obtenerListaDeCentrales()->avanzarCursor())
    {

        Central* Central = this->obtenerListaDeCentrales()->obtenerCursor();

        cout << "     Central: " << Central->obtenerId() << "     ";
        if (i%3 == 0){ cout<<endl<<endl; };
        i++;
    }

}

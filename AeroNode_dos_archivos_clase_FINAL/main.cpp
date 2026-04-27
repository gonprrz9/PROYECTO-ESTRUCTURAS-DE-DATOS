/*
    AeroNode - Gestor de Aerolineas
    Estructura de Datos | Grupo 4 | UIE

    En este archivo estan:
    1) La implementacion de las clases declaradas en AeroNode.h.
    2) El programa principal y los menus de usuario.
*/

#include "AeroNode.h"


// ============================================================
// METODOS CORTOS DECLARADOS EN AeroNode.h
// ============================================================

Pasajero::~Pasajero() {}
std::string Pasajero::getNombre() const { return nombre; }
std::string Pasajero::getDni() const { return dni; }
std::string Pasajero::getLocalizador() const { return localizador; }
int Pasajero::getPrioridad() const { return prioridad; }
void Pasajero::setNombre(const std::string& n) { nombre = n; }
void Pasajero::setLocalizador(const std::string& l) { localizador = l; }

std::string PasajeroRegular::getTipo() const { return "REGULAR"; }
std::string PasajeroVIP::getTipo() const { return "VIP"; }
std::string PasajeroVIP::getBeneficios() const { return beneficios; }
std::string PasajeroConexion::getTipo() const { return "CONEXION"; }
std::string PasajeroConexion::getVueloConexion() const { return vueloConexion; }
int PasajeroConexion::getTiempoMin() const { return tiempoMinConex; }

Equipaje::~Equipaje() {}
std::string Equipaje::getId() const { return id; }
std::string Equipaje::getPropietarioDni() const { return propietarioDni; }
double Equipaje::getPeso() const { return peso; }
double Equipaje::getVolumen() const { return volumen; }

std::string EquipajeMano::getTipo() const { return "MANO"; }
std::string EquipajeFacturado::getTipo() const { return "FACTURADO"; }
bool EquipajeFacturado::esFragil() const { return fragil; }
std::string EquipajeEspecial::getTipo() const { return "ESPECIAL"; }
std::string EquipajeEspecial::getDescripcion() const { return descripcion; }

NodoCola::NodoCola(Pasajero* p) : pasajero(p), siguiente(nullptr) {}
bool ColaEspera::estaVacia() const { return tamano == 0; }
int ColaEspera::getTamano() const { return tamano; }

NodoPila::NodoPila(Equipaje* e) : equipaje(e), siguiente(nullptr) {}
bool PilaBodega::estaVacia() const { return tamano == 0; }
bool PilaBodega::estaLlena() const { return tamano >= capacidadMax; }
int PilaBodega::getTamano() const { return tamano; }
int PilaBodega::getCapacidadMaxima() const { return capacidadMax; }

NodoReserva::NodoReserva(Pasajero* p, int a)
    : pasajero(p), asiento(a), siguiente(nullptr), anterior(nullptr) {}
bool ListaReservas::estaLlena() const { return tamano >= capacidadMaxima; }
bool ListaReservas::estaVacia() const { return tamano == 0; }
int ListaReservas::getTamano() const { return tamano; }
int ListaReservas::getCapacidad() const { return capacidadMaxima; }
int ListaReservas::asientosLibres() const { return capacidadMaxima - tamano; }
NodoReserva* ListaReservas::getCabeza() const { return cabeza; }

NodoAVL::NodoAVL(const std::string& k, Pasajero* p)
    : clave(k), pasajero(p), altura(1), izq(nullptr), der(nullptr) {}
bool ArbolAVL::estaVacio() const { return raiz == nullptr; }

std::string Vuelo::getId() const { return id; }
std::string Vuelo::getOrigen() const { return origen; }
std::string Vuelo::getDestino() const { return destino; }
std::string Vuelo::getFechaHora() const { return fechaHora; }
int Vuelo::getCapacidad() const { return capacidad; }
int Vuelo::getCapacidadBodega() const { return bodega.getCapacidadMaxima(); }
bool Vuelo::isActivo() const { return activo; }
int Vuelo::getOcupacion() const { return reservas.getTamano(); }
bool Vuelo::bodegaEstaVacia() const { return bodega.estaVacia(); }
int Vuelo::getEsperaSize() const { return listaEspera.getTamano(); }
const std::vector<Incidencia>& Vuelo::getIncidencias() const { return incidencias; }
const ListaReservas& Vuelo::getReservas() const { return reservas; }
const ColaEspera& Vuelo::getEspera() const { return listaEspera; }
void Vuelo::registrarPasajero(Pasajero* p) { pasajerosAllocated.push_back(p); }

// ============================================================
// IMPLEMENTACION
// ============================================================


// ---------- Pasajero ----------

// Pasajero base
Pasajero::Pasajero(const std::string& n, const std::string& d,
                   const std::string& l, int p)
    : nombre(n), dni(d), localizador(l), prioridad(p) {}

void Pasajero::mostrar() const {
    std::cout << "[" << getTipo() << "] " << nombre
              << " | DNI: " << dni
              << " | Loc: " << localizador
              << " | Prio: " << prioridad << "\n";
}

std::string Pasajero::serializar() const {
    return getTipo() + ";" + nombre + ";" + dni + ";" + localizador;
}

// PasajeroRegular
PasajeroRegular::PasajeroRegular(const std::string& n, const std::string& d,
                                 const std::string& l)
    : Pasajero(n, d, l, 0) {}

void PasajeroRegular::mostrar() const {
    std::cout << "[REGULAR] " << nombre
              << " | DNI: " << dni
              << " | Loc: " << localizador << "\n";
}

std::string PasajeroRegular::serializar() const {
    return "REGULAR;" + nombre + ";" + dni + ";" + localizador;
}

// PasajeroVIP
PasajeroVIP::PasajeroVIP(const std::string& n, const std::string& d,
                         const std::string& l, const std::string& b)
    : Pasajero(n, d, l, 10), beneficios(b) {}

void PasajeroVIP::mostrar() const {
    std::cout << "[VIP] " << nombre
              << " | DNI: " << dni
              << " | Loc: " << localizador
              << " | Beneficios: " << beneficios << "\n";
}

std::string PasajeroVIP::serializar() const {
    return "VIP;" + nombre + ";" + dni + ";" + localizador + ";" + beneficios;
}

// PasajeroConexion
PasajeroConexion::PasajeroConexion(const std::string& n, const std::string& d,
                                   const std::string& l,
                                   const std::string& vc, int t)
    : Pasajero(n, d, l, 5), vueloConexion(vc), tiempoMinConex(t) {}

void PasajeroConexion::mostrar() const {
    std::cout << "[CONEXION] " << nombre
              << " | DNI: " << dni
              << " | Loc: " << localizador
              << " | Vuelo conexion: " << vueloConexion
              << " | Min: " << tiempoMinConex << "\n";
}

std::string PasajeroConexion::serializar() const {
    return "CONEXION;" + nombre + ";" + dni + ";" + localizador + ";"
           + vueloConexion + ";" + std::to_string(tiempoMinConex);
}



// ---------- Equipaje ----------

// Equipaje base
Equipaje::Equipaje(const std::string& id, const std::string& pd,
                   double p, double v)
    : id(id), propietarioDni(pd), peso(p), volumen(v) {}

void Equipaje::mostrar() const {
    std::cout << "[" << getTipo() << "] ID:" << id
              << " | Prop:" << propietarioDni
              << " | " << peso << "kg " << volumen << "L\n";
}

std::string Equipaje::serializar() const {
    return getTipo() + ";" + id + ";" + propietarioDni + ";"
           + std::to_string(peso) + ";" + std::to_string(volumen);
}

// EquipajeMano
EquipajeMano::EquipajeMano(const std::string& id, const std::string& pd,
                           double p, double v)
    : Equipaje(id, pd, p, v) {}

void EquipajeMano::mostrar() const {
    std::cout << "[MANO] ID:" << id << " | Prop:" << propietarioDni
              << " | " << peso << "kg\n";
}

std::string EquipajeMano::serializar() const {
    return "MANO;" + id + ";" + propietarioDni + ";"
           + std::to_string(peso) + ";" + std::to_string(volumen);
}

// EquipajeFacturado
EquipajeFacturado::EquipajeFacturado(const std::string& id,
                                     const std::string& pd,
                                     double p, double v, bool f)
    : Equipaje(id, pd, p, v), fragil(f) {}

void EquipajeFacturado::mostrar() const {
    std::cout << "[FACTURADO] ID:" << id << "| Prop:" << propietarioDni
              << "| " << peso << "kg"
              << (fragil ? "[FRAGIL]" : "") << "\n";
}

std::string EquipajeFacturado::serializar() const {
    return "FACTURADO;" + id + ";" + propietarioDni + ";"
           + std::to_string(peso) + ";" + std::to_string(volumen) + ";"
           + (fragil ? "1" : "0");
}

// EquipajeEspecial
EquipajeEspecial::EquipajeEspecial(const std::string& id,
                                   const std::string& pd,
                                   double p, double v,
                                   const std::string& desc)
    : Equipaje(id, pd, p, v), descripcion(desc) {}

void EquipajeEspecial::mostrar() const {
    std::cout << "[ESPECIAL] ID:" << id << "| Prop:" << propietarioDni
              << "| " << peso << "kg | " << descripcion << "\n";
}

std::string EquipajeEspecial::serializar() const {
    return "ESPECIAL;" + id + ";" + propietarioDni + ";"
           + std::to_string(peso) + ";" + std::to_string(volumen) + ";"
           + descripcion;
}



// ---------- Estructuras ----------

// ColaEspera (FIFO)
ColaEspera::ColaEspera() : frente(nullptr), final_(nullptr), tamano(0) {}

ColaEspera::~ColaEspera() {
    while (!estaVacia()) desencolar();
}

void ColaEspera::encolar(Pasajero* p) {
    NodoCola* nuevo = new NodoCola(p);
    if (!final_) {
        frente = final_ = nuevo;
    } else {
        final_->siguiente = nuevo;
        final_ = nuevo;
    }
    ++tamano;
}

Pasajero* ColaEspera::desencolar() {
    if (estaVacia()) return nullptr;
    NodoCola* tmp = frente;
    Pasajero* p = tmp->pasajero;
    frente = frente->siguiente;
    if (!frente) final_ = nullptr;
    delete tmp;
    --tamano;
    return p;
}

Pasajero* ColaEspera::verFrente() const {
    return frente ? frente->pasajero : nullptr;
}

bool ColaEspera::contieneDni(const std::string& dni) const {
    NodoCola* actual = frente;
    while (actual) {
        if (actual->pasajero->getDni() == dni) return true;
        actual = actual->siguiente;
    }
    return false;
}

bool ColaEspera::contieneLocalizador(const std::string& loc) const {
    NodoCola* actual = frente;
    while (actual) {
        if (actual->pasajero->getLocalizador() == loc) return true;
        actual = actual->siguiente;
    }
    return false;
}

void ColaEspera::mostrar() const {
    if (estaVacia()) {
        std::cout << "  (cola vacia)\n";
        return;
    }
    NodoCola* actual = frente;
    int pos = 1;
    while (actual) {
        std::cout << "  Pos " << pos++ << ": ";
        actual->pasajero->mostrar();
        actual = actual->siguiente;
    }
}

// PilaBodega (LIFO)
PilaBodega::PilaBodega(int cap) : tope(nullptr), tamano(0), capacidadMax(cap > 0 ? cap : 0) {}

PilaBodega::~PilaBodega() {
    while (!estaVacia()) {
        Equipaje* e = desapilar();
        delete e;
    }
}

bool PilaBodega::apilar(Equipaje* e) {
    if (estaLlena()) return false;
    NodoPila* nuevo = new NodoPila(e);
    nuevo->siguiente = tope;
    tope = nuevo;
    ++tamano;
    return true;
}

Equipaje* PilaBodega::desapilar() {
    if (estaVacia()) return nullptr;
    NodoPila* tmp = tope;
    Equipaje* e = tmp->equipaje;
    tope = tope->siguiente;
    delete tmp;
    --tamano;
    return e;
}

Equipaje* PilaBodega::verTope() const {
    return tope ? tope->equipaje : nullptr;
}

void PilaBodega::mostrar() const {
    if (estaVacia()) {
        std::cout << "  (bodega vacia)\n";
        return;
    }
    NodoPila* actual = tope;
    int nivel = tamano;
    std::cout << "  [TOPE - ultimo cargado / primero en descargar]\n";
    while (actual) {
        std::cout << "  Nivel " << nivel-- << ": ";
        actual->equipaje->mostrar();
        actual = actual->siguiente;
    }
}



// ---------- ListaReservas ----------

ListaReservas::ListaReservas(int capacidad)
    : cabeza(nullptr), cola(nullptr), tamano(0), capacidadMaxima(capacidad) {}

ListaReservas::~ListaReservas() {
    NodoReserva* actual = cabeza;
    while (actual) {
        NodoReserva* sig = actual->siguiente;
        // El Pasajero es propiedad del sistema, NO se elimina aqui
        delete actual;
        actual = sig;
    }
}

bool ListaReservas::insertar(Pasajero* p, int asiento) {
    if (estaLlena()) return false;
    // Verificar asiento duplicado
    NodoReserva* tmp = cabeza;
    while (tmp) {
        if (tmp->asiento == asiento) return false;
        tmp = tmp->siguiente;
    }
    NodoReserva* nuevo = new NodoReserva(p, asiento);
    if (!cabeza) {
        cabeza = cola = nuevo;
    } else {
        nuevo->anterior = cola;
        cola->siguiente = nuevo;
        cola = nuevo;
    }
    ++tamano;
    return true;
}

bool ListaReservas::eliminar(const std::string& localizador,
                             Pasajero*& pasajeroEliminado) {
    NodoReserva* nodo = buscarPorLocalizador(localizador);
    if (!nodo) return false;

    pasajeroEliminado = nodo->pasajero;

    if (nodo->anterior) nodo->anterior->siguiente = nodo->siguiente;
    else                cabeza = nodo->siguiente;

    if (nodo->siguiente) nodo->siguiente->anterior = nodo->anterior;
    else                 cola = nodo->anterior;

    delete nodo;
    --tamano;
    return true;
}

NodoReserva* ListaReservas::buscarPorLocalizador(const std::string& loc) const {
    NodoReserva* actual = cabeza;
    while (actual) {
        if (actual->pasajero->getLocalizador() == loc) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

NodoReserva* ListaReservas::buscarPorDni(const std::string& dni) const {
    NodoReserva* actual = cabeza;
    while (actual) {
        if (actual->pasajero->getDni() == dni) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

int ListaReservas::primerAsientoLibre() const {
    // Busca el menor numero de asiento libre entre 1 y capacidadMaxima
    for (int i = 1; i <= capacidadMaxima; ++i) {
        bool ocupado = false;
        NodoReserva* actual = cabeza;
        while (actual) {
            if (actual->asiento == i) { ocupado = true; break; }
            actual = actual->siguiente;
        }
        if (!ocupado) return i;
    }
    return -1; // lleno
}

void ListaReservas::mostrar() const {
    if (estaVacia()) {
        std::cout << "(sin reservas)\n";
        return;
    }
    NodoReserva* actual = cabeza;
    while (actual) {
        std::cout << "Asiento " << actual->asiento << " -> ";
        actual->pasajero->mostrar();
        actual = actual->siguiente;
    }
}



// ---------- ArbolAVL ----------

ArbolAVL::ArbolAVL() : raiz(nullptr) {}
ArbolAVL::~ArbolAVL() { destruir(raiz); }

void ArbolAVL::destruir(NodoAVL* n) {
    if (!n) return;
    destruir(n->izq);
    destruir(n->der);
    delete n; // NO eliminar pasajero (propiedad del sistema)
}

int ArbolAVL::altura(NodoAVL* n) const { return n ? n->altura : 0; }
int ArbolAVL::balance(NodoAVL* n) const { return n ? altura(n->izq) - altura(n->der) : 0; }

NodoAVL* ArbolAVL::rotarDerecha(NodoAVL* y) {
    NodoAVL* x  = y->izq;
    NodoAVL* T2 = x->der;
    x->der = y;
    y->izq = T2;
    y->altura = 1 + std::max(altura(y->izq), altura(y->der));
    x->altura = 1 + std::max(altura(x->izq), altura(x->der));
    return x;
}

NodoAVL* ArbolAVL::rotarIzquierda(NodoAVL* x) {
    NodoAVL* y  = x->der;
    NodoAVL* T2 = y->izq;
    y->izq = x;
    x->der = T2;
    x->altura = 1 + std::max(altura(x->izq), altura(x->der));
    y->altura = 1 + std::max(altura(y->izq), altura(y->der));
    return y;
}

NodoAVL* ArbolAVL::insertar(NodoAVL* nodo, const std::string& clave, Pasajero* p) {
    if (!nodo) return new NodoAVL(clave, p);
    if (clave < nodo->clave)      nodo->izq = insertar(nodo->izq, clave, p);
    else if (clave > nodo->clave) nodo->der = insertar(nodo->der, clave, p);
    else { nodo->pasajero = p; return nodo; } // actualizar si misma clave

    nodo->altura = 1 + std::max(altura(nodo->izq), altura(nodo->der));
    int bal = balance(nodo);

    if (bal > 1 && clave < nodo->izq->clave)  return rotarDerecha(nodo);
    if (bal < -1 && clave > nodo->der->clave) return rotarIzquierda(nodo);
    if (bal > 1 && clave > nodo->izq->clave)  { nodo->izq = rotarIzquierda(nodo->izq); return rotarDerecha(nodo); }
    if (bal < -1 && clave < nodo->der->clave) { nodo->der = rotarDerecha(nodo->der);   return rotarIzquierda(nodo); }
    return nodo;
}

NodoAVL* ArbolAVL::minNodo(NodoAVL* n) const {
    while (n->izq) n = n->izq;
    return n;
}

NodoAVL* ArbolAVL::eliminar(NodoAVL* nodo, const std::string& clave) {
    if (!nodo) return nullptr;
    if (clave < nodo->clave)      nodo->izq = eliminar(nodo->izq, clave);
    else if (clave > nodo->clave) nodo->der = eliminar(nodo->der, clave);
    else {
        if (!nodo->izq || !nodo->der) {
            NodoAVL* tmp = nodo->izq ? nodo->izq : nodo->der;
            delete nodo;
            return tmp;
        }
        NodoAVL* suc = minNodo(nodo->der);
        nodo->clave   = suc->clave;
        nodo->pasajero = suc->pasajero;
        nodo->der = eliminar(nodo->der, suc->clave);
    }
    nodo->altura = 1 + std::max(altura(nodo->izq), altura(nodo->der));
    int bal = balance(nodo);
    if (bal > 1  && balance(nodo->izq) >= 0)  return rotarDerecha(nodo);
    if (bal > 1  && balance(nodo->izq) < 0)   { nodo->izq = rotarIzquierda(nodo->izq); return rotarDerecha(nodo); }
    if (bal < -1 && balance(nodo->der) <= 0)  return rotarIzquierda(nodo);
    if (bal < -1 && balance(nodo->der) > 0)   { nodo->der = rotarDerecha(nodo->der); return rotarIzquierda(nodo); }
    return nodo;
}

NodoAVL* ArbolAVL::buscar(NodoAVL* nodo, const std::string& clave) const {
    if (!nodo || nodo->clave == clave) return nodo;
    return (clave < nodo->clave) ? buscar(nodo->izq, clave)
                                 : buscar(nodo->der, clave);
}

void ArbolAVL::inorden(NodoAVL* nodo) const {
    if (!nodo) return;
    inorden(nodo->izq);
    std::cout << "  [" << nodo->clave << "] -> ";
    nodo->pasajero->mostrar();
    inorden(nodo->der);
}

// Interfaz publica
void ArbolAVL::insertar(const std::string& clave, Pasajero* p) {
    raiz = insertar(raiz, clave, p);
}

void ArbolAVL::eliminar(const std::string& clave) {
    raiz = eliminar(raiz, clave);
}

Pasajero* ArbolAVL::buscar(const std::string& clave) const {
    NodoAVL* n = buscar(raiz, clave);
    return n ? n->pasajero : nullptr;
}

void ArbolAVL::mostrarInorden() const {
    if (estaVacio()) { std::cout << "  (arbol vacio)\n"; return; }
    inorden(raiz);
}



// ---------- Incidencia ----------

Incidencia::Incidencia(TipoIncidencia t, const std::string& d,
                       const std::string& dni, const std::string& v,
                       const std::string& ts)
    : tipo(t), descripcion(d), pasajeroDni(dni), vueloId(v), timestamp(ts) {}

std::string Incidencia::tipoStr(TipoIncidencia t) {
    switch (t) {
        case TipoIncidencia::OVERBOOKING:         return "OVERBOOKING";
        case TipoIncidencia::CANCELACION:         return "CANCELACION";
        case TipoIncidencia::REUBICACION:         return "REUBICACION";
        case TipoIncidencia::PERDIDA_CONEXION:    return "PERDIDA_CONEXION";
        case TipoIncidencia::EQUIPAJE_EXTRAVIADO: return "EQUIPAJE_EXTRAVIADO";
        default:                                  return "OTRO";
    }
}

TipoIncidencia Incidencia::tipoDesdeString(const std::string& texto) {
    if (texto == "OVERBOOKING") return TipoIncidencia::OVERBOOKING;
    if (texto == "CANCELACION") return TipoIncidencia::CANCELACION;
    if (texto == "REUBICACION") return TipoIncidencia::REUBICACION;
    if (texto == "PERDIDA_CONEXION") return TipoIncidencia::PERDIDA_CONEXION;
    if (texto == "EQUIPAJE_EXTRAVIADO") return TipoIncidencia::EQUIPAJE_EXTRAVIADO;
    return TipoIncidencia::OTRO;
}

void Incidencia::mostrar() const {
    std::cout << "  [" << tipoStr(tipo) << "] "
              << timestamp << " | Vuelo: " << vueloId
              << " | DNI: " << pasajeroDni
              << " | " << descripcion << "\n";
}

std::string Incidencia::serializar() const {
    return tipoStr(tipo) + ";" + timestamp + ";" + vueloId + ";"
           + pasajeroDni + ";" + descripcion;
}



// ---------- GrafoRutas ----------

GrafoRutas::GrafoRutas() {}

// Busca el indice de un nodo por su codigo IATA
int GrafoRutas::buscarIndice(const std::string& codigo) const {
    for (int i = 0; i < (int)nodos.size(); i++) {
        if (nodos[i].codigo == codigo) return i;
    }
    return -1;
}

// Agrega un aeropuerto si no existe ya
void GrafoRutas::agregarAeropuerto(const std::string& codigo) {
    if (buscarIndice(codigo) == -1) {
        NodoAeropuerto nuevo;
        nuevo.codigo = codigo;
        nodos.push_back(nuevo);
    }
}

// Agrega una ruta dirigida ponderada (arista)
void GrafoRutas::agregarRuta(const std::string& origen,
                              const std::string& destino,
                              int peso,
                              const std::string& codigoVuelo) {
    agregarAeropuerto(origen);
    agregarAeropuerto(destino);
    int idx = buscarIndice(origen);

    for (int i = 0; i < (int)nodos[idx].aristas.size(); i++) {
        if (nodos[idx].aristas[i].destino == destino &&
            nodos[idx].aristas[i].codigoVuelo == codigoVuelo) {
            nodos[idx].aristas[i].peso = peso;
            return;
        }
    }

    Arista a;
    a.destino     = destino;
    a.peso        = peso;
    a.codigoVuelo = codigoVuelo;
    nodos[idx].aristas.push_back(a);
}

// Elimina una ruta (arista) entre dos aeropuertos
void GrafoRutas::eliminarRuta(const std::string& origen,
                               const std::string& destino) {
    int idx = buscarIndice(origen);
    if (idx == -1) return;
    std::vector<Arista>& lista = nodos[idx].aristas;
    int i = 0;
    while (i < (int)lista.size()) {
        if (lista[i].destino == destino) {
            // Eliminar desplazando el resto
            for (int j = i; j < (int)lista.size() - 1; j++)
                lista[j] = lista[j + 1];
            lista.pop_back();
        } else {
            i++;
        }
    }
}

//  Dijkstra - camino minimo (Algoritmo del temario S11)
//  Implementado con vector de distancias y busqueda del minimo
//  sin usar priority_queue con comparador externo ni lambdas
std::vector<std::string> GrafoRutas::rutaMasCorta(const std::string& origen,
                                                    const std::string& destino,
                                                    int& costoTotal) const {
    int n = (int)nodos.size();
    std::vector<std::string> ruta;

    int idxOrigen  = buscarIndice(origen);
    int idxDestino = buscarIndice(destino);

    if (idxOrigen == -1 || idxDestino == -1) {
        costoTotal = -1;
        return ruta;
    }

    // Distancias inicializadas a "infinito"
    const int INF = std::numeric_limits<int>::max();
    std::vector<int>  dist(n, INF);
    std::vector<int>  prev(n, -1);
    std::vector<bool> visitado(n, false);

    dist[idxOrigen] = 0;

    // n iteraciones: en cada una se elige el nodo no visitado con menor distancia
    for (int iter = 0; iter < n; iter++) {
        // Buscar el nodo no visitado con menor distancia (seleccion lineal del minimo)
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (!visitado[i] && dist[i] != INF) {
                if (u == -1 || dist[i] < dist[u]) u = i;
            }
        }
        if (u == -1) break;  // no quedan nodos alcanzables
        visitado[u] = true;

        // Relajar aristas del nodo u
        for (int j = 0; j < (int)nodos[u].aristas.size(); j++) {
            const Arista& a = nodos[u].aristas[j];
            int v = buscarIndice(a.destino);
            if (v == -1) continue;
            if (dist[u] != INF && dist[u] + a.peso < dist[v]) {
                dist[v] = dist[u] + a.peso;
                prev[v] = u;
            }
        }
    }

    costoTotal = (dist[idxDestino] == INF) ? -1 : dist[idxDestino];
    if (costoTotal < 0) return ruta;

    // Reconstruir ruta desde destino a origen
    std::vector<std::string> rutaInversa;
    int cur = idxDestino;
    while (cur != -1) {
        rutaInversa.push_back(nodos[cur].codigo);
        cur = prev[cur];
    }

    // Invertir para obtener ruta de origen a destino
    for (int i = (int)rutaInversa.size() - 1; i >= 0; i--)
        ruta.push_back(rutaInversa[i]);

    return ruta;
}

//  BFS - aeropuertos alcanzables (Algoritmo del temario S11)
//  Usa queue (Cola FIFO) tal como describe el temario
std::vector<std::string> GrafoRutas::alcanzables(const std::string& origen) const {
    std::vector<std::string> resultado;
    int idxOrigen = buscarIndice(origen);
    if (idxOrigen == -1) return resultado;

    int n = (int)nodos.size();
    std::vector<bool> visitado(n, false);
    std::queue<int>   cola;

    cola.push(idxOrigen);
    visitado[idxOrigen] = true;

    while (!cola.empty()) {
        int u = cola.front();
        cola.pop();
        resultado.push_back(nodos[u].codigo);

        for (int j = 0; j < (int)nodos[u].aristas.size(); j++) {
            int v = buscarIndice(nodos[u].aristas[j].destino);
            if (v != -1 && !visitado[v]) {
                visitado[v] = true;
                cola.push(v);
            }
        }
    }
    return resultado;
}

// Muestra el grafo por consola
void GrafoRutas::mostrar() const {
    for (int i = 0; i < (int)nodos.size(); i++) {
        std::cout << "  " << nodos[i].codigo << " -> ";
        if (nodos[i].aristas.empty()) {
            std::cout << "(sin conexiones)\n";
            continue;
        }
        for (int j = 0; j < (int)nodos[i].aristas.size(); j++) {
            const Arista& a = nodos[i].aristas[j];
            std::cout << a.destino << "(" << a.peso << "min," << a.codigoVuelo << ")";
            if (j + 1 < (int)nodos[i].aristas.size()) std::cout << "  ";
        }
        std::cout << "\n";
    }
}

// Devuelve lista de codigos IATA de todos los aeropuertos
std::vector<std::string> GrafoRutas::getAeropuertos() const {
    std::vector<std::string> lista;
    for (int i = 0; i < (int)nodos.size(); i++)
        lista.push_back(nodos[i].codigo);
    return lista;
}

// Devuelve la lista de aristas de un aeropuerto
const std::vector<Arista>& GrafoRutas::getAristas(const std::string& codigo) const {
    static std::vector<Arista> vacio;
    int idx = buscarIndice(codigo);
    return (idx != -1) ? nodos[idx].aristas : vacio;
}



// ---------- Vuelo ----------

Vuelo::Vuelo(const std::string& id_, const std::string& org,
             const std::string& dst, const std::string& fh,
             int cap, int capBodega)
    : id(id_), origen(org), destino(dst), fechaHora(fh),
      capacidad(cap > 0 ? cap : 1), activo(true),
      reservas(cap > 0 ? cap : 1), bodega(capBodega > 0 ? capBodega : 0) {}

Vuelo::~Vuelo() {
    for (Pasajero* p : pasajerosAllocated) delete p;
}

std::string Vuelo::timestampActual() const {
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", std::localtime(&t));
    return std::string(buf);
}

void Vuelo::registrarIncidencia(TipoIncidencia t,
                                 const std::string& desc,
                                 const std::string& dni) {
    incidencias.push_back(Incidencia(t, desc, dni, id, timestampActual()));
}

void Vuelo::agregarIncidenciaCargada(const Incidencia& inc) {
    incidencias.push_back(inc);
}

int Vuelo::venderBillete(Pasajero* p) {
    if (!p) return 2;

    if (buscarPorDni(p->getDni()) || buscarPorLocalizador(p->getLocalizador()) ||
        listaEspera.contieneDni(p->getDni()) || listaEspera.contieneLocalizador(p->getLocalizador())) {
        std::cout << "  ERROR: ya existe un pasajero con ese DNI o localizador en este vuelo.\n";
        return 2;
    }

    if (!reservas.estaLlena()) {
        int asiento = reservas.primerAsientoLibre();
        if (reservas.insertar(p, asiento)) {
            indiceAVL.insertar(p->getDni(), p);
            indiceAVL.insertar(p->getLocalizador(), p);
            std::cout << "  OK Billete vendido. Asiento " << asiento
                      << " asignado a " << p->getNombre() << "\n";
            return 0;
        }
    }

    listaEspera.encolar(p);
    registrarIncidencia(TipoIncidencia::OVERBOOKING,
                        "Pasajero en lista de espera por overbooking",
                        p->getDni());
    std::cout << "  AVISO: vuelo lleno. " << p->getNombre()
              << " anadido a lista de espera (posicion "
              << listaEspera.getTamano() << ").\n";
    return 1;
}

bool Vuelo::cargarReservaPersistida(Pasajero* p, int asiento) {
    if (!p) return false;
    if (buscarPorDni(p->getDni()) || buscarPorLocalizador(p->getLocalizador()) ||
        listaEspera.contieneDni(p->getDni()) || listaEspera.contieneLocalizador(p->getLocalizador())) {
        return false;
    }

    int asientoReal = asiento;
    if (asientoReal <= 0 || asientoReal > capacidad) {
        asientoReal = reservas.primerAsientoLibre();
    }
    if (!reservas.insertar(p, asientoReal)) {
        asientoReal = reservas.primerAsientoLibre();
        if (asientoReal <= 0 || !reservas.insertar(p, asientoReal)) {
            return false;
        }
    }
    indiceAVL.insertar(p->getDni(), p);
    indiceAVL.insertar(p->getLocalizador(), p);
    return true;
}

bool Vuelo::cancelarReserva(const std::string& localizador) {
    Pasajero* p = nullptr;
    if (!reservas.eliminar(localizador, p)) {
        std::cout << "  ERROR: no se encontro reserva con localizador " << localizador << ".\n";
        return false;
    }
    indiceAVL.eliminar(p->getDni());
    indiceAVL.eliminar(p->getLocalizador());
    registrarIncidencia(TipoIncidencia::CANCELACION,
                        "Reserva cancelada por " + p->getNombre(), p->getDni());
    std::cout << "  OK Reserva de " << p->getNombre() << " cancelada.\n";

    if (!listaEspera.estaVacia()) {
        Pasajero* siguiente = listaEspera.desencolar();
        int asiento = reservas.primerAsientoLibre();
        reservas.insertar(siguiente, asiento);
        indiceAVL.insertar(siguiente->getDni(), siguiente);
        indiceAVL.insertar(siguiente->getLocalizador(), siguiente);
        registrarIncidencia(TipoIncidencia::REUBICACION,
                            "Pasajero reasignado desde lista de espera",
                            siguiente->getDni());
        std::cout << "  OK " << siguiente->getNombre()
                  << " asignado automaticamente al asiento " << asiento
                  << " desde lista de espera.\n";
    }
    return true;
}

Pasajero* Vuelo::buscarPorDni(const std::string& dni) const {
    return indiceAVL.buscar(dni);
}

Pasajero* Vuelo::buscarPorLocalizador(const std::string& loc) const {
    return indiceAVL.buscar(loc);
}

bool Vuelo::cargarEquipaje(Equipaje* e) {
    if (!e) return false;
    if (!bodega.apilar(e)) {
        std::cout << "  ERROR: bodega llena. No se puede cargar el equipaje.\n";
        return false;
    }
    std::cout << "  OK Equipaje " << e->getId() << " cargado en bodega.\n";
    return true;
}

Equipaje* Vuelo::descargarEquipaje() {
    Equipaje* e = bodega.desapilar();
    if (!e) {
        std::cout << "  AVISO: bodega vacia.\n";
        return nullptr;
    }
    std::cout << "  OK Descargado: ";
    e->mostrar();
    return e;
}

void Vuelo::mostrarResumen() const {
    std::cout << "  Vuelo " << id << " (" << origen << " -> " << destino << ")"
              << " [" << fechaHora << "]\n"
              << "    Capacidad: " << reservas.getTamano() << "/" << capacidad
              << " | Espera: " << listaEspera.getTamano()
              << " | Bodega: " << bodega.getTamano() << "/" << bodega.getCapacidadMaxima()
              << " | Estado: " << (activo ? "ACTIVO" : "INACTIVO") << "\n";
}

void Vuelo::mostrarReservas() const {
    std::cout << "\n  Reservas vuelo " << id << "\n";
    reservas.mostrar();
}

void Vuelo::mostrarEspera() const {
    std::cout << "\n  Lista de espera vuelo " << id << "\n";
    listaEspera.mostrar();
}

void Vuelo::mostrarBodega() const {
    std::cout << "\n  Bodega vuelo " << id << "\n";
    bodega.mostrar();
}

void Vuelo::mostrarAVL() const {
    std::cout << "\n  Indice AVL vuelo " << id << " (inorden)\n";
    indiceAVL.mostrarInorden();
}

void Vuelo::mostrarIncidencias() const {
    std::cout << "\n  Incidencias vuelo " << id << "\n";
    if (incidencias.empty()) {
        std::cout << "  (sin incidencias)\n";
        return;
    }
    for (int i = 0; i < (int)incidencias.size(); i++) incidencias[i].mostrar();
}

std::string Vuelo::serializarCabecera() const {
    return "VUELO;" + id + ";" + origen + ";" + destino + ";"
           + fechaHora + ";" + std::to_string(capacidad) + ";"
           + std::to_string(getCapacidadBodega());
}



// ---------- GestorPersistencia ----------


GestorPersistencia::GestorPersistencia(const std::string& dir)
    : directorioBase(dir) {}

std::string GestorPersistencia::rutaVuelos() const { return "vuelos.dat"; }
std::string GestorPersistencia::rutaGrafo() const { return "grafo.dat"; }
std::string GestorPersistencia::rutaInforme() const { return "informe_ocupacion.txt"; }

bool GestorPersistencia::guardarSistema(const std::vector<Vuelo*>& vuelos,
                                         const GrafoRutas& grafo) const {
    std::ofstream f(rutaVuelos());
    if (!f.is_open()) {
        std::cerr << "  ERROR: no se pudo abrir " << rutaVuelos() << " para escritura.\n";
        return false;
    }

    for (int vi = 0; vi < (int)vuelos.size(); vi++) {
        Vuelo* v = vuelos[vi];
        f << v->serializarCabecera() << "\n";

        const NodoReserva* nodo = v->getReservas().getCabeza();
        while (nodo) {
            f << "RESERVA;" << nodo->asiento << ";"
              << nodo->pasajero->serializar() << "\n";
            nodo = nodo->siguiente;
        }

        const std::vector<Incidencia>& incs = v->getIncidencias();
        for (int i = 0; i < (int)incs.size(); i++) {
            f << "INCIDENCIA;" << incs[i].serializar() << "\n";
        }
        f << "FIN_VUELO\n";
    }
    f.close();

    guardarGrafo(grafo);
    std::cout << "  OK Sistema guardado en " << rutaVuelos() << "\n";
    return true;
}

std::vector<Vuelo*> GestorPersistencia::cargarVuelos() const {
    std::vector<Vuelo*> vuelos;
    std::ifstream f(rutaVuelos());
    if (!f.is_open()) {
        std::cout << "  (no hay datos previos)\n";
        return vuelos;
    }

    Vuelo* vActual = nullptr;
    std::string linea;
    while (std::getline(f, linea)) {
        if (linea.empty()) continue;

        std::istringstream ss(linea);
        std::string tipo;
        std::getline(ss, tipo, ';');

        if (tipo == "VUELO") {
            std::string id, org, dst, fh, capStr, capBodStr;
            std::getline(ss, id, ';');
            std::getline(ss, org, ';');
            std::getline(ss, dst, ';');
            std::getline(ss, fh, ';');
            std::getline(ss, capStr, ';');
            std::getline(ss, capBodStr, ';');

            int cap = capStr.empty() ? 1 : std::stoi(capStr);
            int capBod = capBodStr.empty() ? 60 : std::stoi(capBodStr);
            vActual = new Vuelo(id, org, dst, fh, cap, capBod);
            vuelos.push_back(vActual);
        } else if (tipo == "RESERVA" && vActual) {
            std::string asientoStr, tipoPas, nombre, dni, loc, extra1, extra2;
            std::getline(ss, asientoStr, ';');
            std::getline(ss, tipoPas, ';');
            std::getline(ss, nombre, ';');
            std::getline(ss, dni, ';');
            std::getline(ss, loc, ';');

            Pasajero* p = nullptr;
            if (tipoPas == "REGULAR") {
                p = new PasajeroRegular(nombre, dni, loc);
            } else if (tipoPas == "VIP") {
                std::getline(ss, extra1, ';');
                p = new PasajeroVIP(nombre, dni, loc, extra1);
            } else if (tipoPas == "CONEXION") {
                std::getline(ss, extra1, ';');
                std::getline(ss, extra2, ';');
                int tiempo = extra2.empty() ? 45 : std::stoi(extra2);
                p = new PasajeroConexion(nombre, dni, loc, extra1, tiempo);
            }

            if (p) {
                int asiento = asientoStr.empty() ? -1 : std::stoi(asientoStr);
                if (vActual->cargarReservaPersistida(p, asiento)) {
                    vActual->registrarPasajero(p);
                } else {
                    delete p;
                }
            }
        } else if (tipo == "INCIDENCIA" && vActual) {
            std::string tipoInc, timestamp, vueloId, dni, desc;
            std::getline(ss, tipoInc, ';');
            std::getline(ss, timestamp, ';');
            std::getline(ss, vueloId, ';');
            std::getline(ss, dni, ';');
            std::getline(ss, desc);
            vActual->agregarIncidenciaCargada(
                Incidencia(Incidencia::tipoDesdeString(tipoInc), desc, dni, vueloId, timestamp)
            );
        } else if (tipo == "FIN_VUELO") {
            vActual = nullptr;
        }
    }
    f.close();

    std::cout << "  OK " << vuelos.size() << " vuelo(s) cargado(s) desde fichero.\n";
    return vuelos;
}

bool GestorPersistencia::guardarGrafo(const GrafoRutas& grafo) const {
    std::ofstream f(rutaGrafo());
    if (!f.is_open()) return false;

    std::vector<std::string> apts = grafo.getAeropuertos();
    for (int i = 0; i < (int)apts.size(); i++) {
        const std::vector<Arista>& aristas = grafo.getAristas(apts[i]);
        for (int j = 0; j < (int)aristas.size(); j++) {
            f << apts[i] << ";" << aristas[j].destino << ";"
              << aristas[j].peso << ";" << aristas[j].codigoVuelo << "\n";
        }
    }
    f.close();
    return true;
}

void GestorPersistencia::cargarGrafo(GrafoRutas& grafo) const {
    std::ifstream f(rutaGrafo());
    if (!f.is_open()) return;

    std::string linea;
    while (std::getline(f, linea)) {
        if (linea.empty()) continue;
        std::istringstream ss(linea);
        std::string org, dst, pesoStr, cv;
        std::getline(ss, org, ';');
        std::getline(ss, dst, ';');
        std::getline(ss, pesoStr, ';');
        std::getline(ss, cv, ';');
        if (!org.empty() && !dst.empty() && !pesoStr.empty()) {
            grafo.agregarRuta(org, dst, std::stoi(pesoStr), cv);
        }
    }
    f.close();
}

bool GestorPersistencia::generarInformeOcupacion(const std::vector<Vuelo*>& vuelos) const {
    std::ofstream f(rutaInforme());
    if (!f.is_open()) return false;

    std::time_t t = std::time(nullptr);
    char buf[30];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

    f << "INFORME DE OCUPACION - AeroNode\n"
      << "Generado: " << buf << "\n\n";

    for (int i = 0; i < (int)vuelos.size(); i++) {
        Vuelo* v = vuelos[i];
        double pct = v->getCapacidad() > 0
                     ? (100.0 * v->getOcupacion() / v->getCapacidad()) : 0;
        f << "Vuelo: " << v->getId()
          << " (" << v->getOrigen() << " -> " << v->getDestino() << ")\n"
          << "Fecha/Hora : " << v->getFechaHora() << "\n"
          << "Ocupacion  : " << v->getOcupacion() << "/" << v->getCapacidad()
          << " (" << pct << "%)\n"
          << "En espera  : " << v->getEsperaSize() << "\n\n";
    }
    f.close();

    std::cout << "  OK Informe guardado en " << rutaInforme() << "\n";
    return true;
}



// ---------- SistemaAeroNode ----------

SistemaAeroNode::SistemaAeroNode(const std::string& dirDatos)
    : persistencia(dirDatos) {
    inicializarGrafoDemo();
}

SistemaAeroNode::~SistemaAeroNode() {
    for (int i = 0; i < (int)vuelos.size(); i++) delete vuelos[i];
}

void SistemaAeroNode::inicializarGrafoDemo() {
    grafo.agregarRuta("VGO", "MAD", 80,  "IB3841");
    grafo.agregarRuta("VGO", "BCN", 95,  "VY6701");
    grafo.agregarRuta("MAD", "BCN", 60,  "IB3011");
    grafo.agregarRuta("MAD", "LHR", 130, "IB3163");
    grafo.agregarRuta("MAD", "CDG", 120, "IB3451");
    grafo.agregarRuta("BCN", "CDG", 100, "VU8200");
    grafo.agregarRuta("BCN", "FCO", 110, "VY6021");
    grafo.agregarRuta("LHR", "CDG", 60,  "BA0304");
    grafo.agregarRuta("CDG", "FCO", 115, "AF1234");
    grafo.agregarRuta("LCG", "MAD", 85,  "IB3821");
    grafo.agregarRuta("LCG", "VGO", 30,  "VY3320");
    grafo.agregarRuta("MAD", "LCG", 85,  "IB3820");
    grafo.agregarRuta("VGO", "LCG", 30,  "VY3321");
}

std::string SistemaAeroNode::generarLocalizador() const {
    static int contador = 1000;
    std::ostringstream ss;
    ss << "AN" << ++contador;
    return ss.str();
}

bool SistemaAeroNode::crearVuelo(const std::string& id,
                                  const std::string& origen,
                                  const std::string& destino,
                                  const std::string& fechaHora,
                                  int capacidad,
                                  int capBodega) {
    if (id.empty() || origen.empty() || destino.empty()) {
        std::cout << "  ERROR: el ID, origen y destino no pueden estar vacios.\n";
        return false;
    }
    if (capacidad <= 0) {
        std::cout << "  ERROR: la capacidad de pasajeros debe ser mayor que 0.\n";
        return false;
    }
    if (capBodega < 0) {
        std::cout << "  ERROR: la capacidad de bodega no puede ser negativa.\n";
        return false;
    }
    if (buscarVuelo(id)) {
        std::cout << "  ERROR: ya existe un vuelo con ID " << id << ".\n";
        return false;
    }

    vuelos.push_back(new Vuelo(id, origen, destino, fechaHora, capacidad, capBodega));
    grafo.agregarAeropuerto(origen);
    grafo.agregarAeropuerto(destino);
    std::cout << "  OK Vuelo " << id << " creado ("
              << origen << " -> " << destino << ", " << capacidad << " plazas).\n";
    return true;
}

Vuelo* SistemaAeroNode::buscarVuelo(const std::string& id) const {
    for (int i = 0; i < (int)vuelos.size(); i++) {
        if (vuelos[i]->getId() == id) return vuelos[i];
    }
    return nullptr;
}

void SistemaAeroNode::listarVuelos() const {
    if (vuelos.empty()) {
        std::cout << "  (no hay vuelos registrados)\n";
        return;
    }
    for (int i = 0; i < (int)vuelos.size(); i++) vuelos[i]->mostrarResumen();
}

int SistemaAeroNode::venderBillete(const std::string& vueloId,
                                    const std::string& tipo,
                                    const std::string& nombre,
                                    const std::string& dni,
                                    const std::string& localizador,
                                    const std::string& extra) {
    Vuelo* v = buscarVuelo(vueloId);
    if (!v) {
        std::cout << "  ERROR: vuelo " << vueloId << " no encontrado.\n";
        return -1;
    }
    if (nombre.empty() || dni.empty() || localizador.empty()) {
        std::cout << "  ERROR: nombre, DNI y localizador son obligatorios.\n";
        return 2;
    }

    Pasajero* p = nullptr;
    if (tipo == "VIP") {
        p = new PasajeroVIP(nombre, dni, localizador,
                            extra.empty() ? "Sala VIP, embarque preferente" : extra);
    } else if (tipo == "CONEXION") {
        p = new PasajeroConexion(nombre, dni, localizador, extra, 45);
    } else {
        p = new PasajeroRegular(nombre, dni, localizador);
    }

    int resultado = v->venderBillete(p);
    if (resultado == 2) {
        delete p;
    } else {
        v->registrarPasajero(p);
    }
    return resultado;
}

bool SistemaAeroNode::cancelarReserva(const std::string& vueloId,
                                       const std::string& localizador) {
    Vuelo* v = buscarVuelo(vueloId);
    if (!v) {
        std::cout << "  ERROR: vuelo " << vueloId << " no encontrado.\n";
        return false;
    }
    return v->cancelarReserva(localizador);
}

bool SistemaAeroNode::cargarEquipaje(const std::string& vueloId,
                                      const std::string& tipoEq,
                                      const std::string& idEq,
                                      const std::string& propDni,
                                      double peso, double volumen,
                                      const std::string& extra) {
    Vuelo* v = buscarVuelo(vueloId);
    if (!v) {
        std::cout << "  ERROR: vuelo " << vueloId << " no encontrado.\n";
        return false;
    }
    if (idEq.empty() || propDni.empty() || peso <= 0 || volumen < 0) {
        std::cout << "  ERROR: datos de equipaje no validos.\n";
        return false;
    }

    Equipaje* e = nullptr;
    if (tipoEq == "MANO") {
        e = new EquipajeMano(idEq, propDni, peso, volumen);
    } else if (tipoEq == "ESPECIAL") {
        e = new EquipajeEspecial(idEq, propDni, peso, volumen, extra);
    } else {
        e = new EquipajeFacturado(idEq, propDni, peso, volumen, extra == "FRAGIL");
    }

    if (!v->cargarEquipaje(e)) {
        delete e;
        return false;
    }
    return true;
}

void SistemaAeroNode::descargarUnEquipaje(const std::string& vueloId) {
    Vuelo* v = buscarVuelo(vueloId);
    if (!v) {
        std::cout << "  ERROR: vuelo " << vueloId << " no encontrado.\n";
        return;
    }

    Equipaje* e = v->descargarEquipaje();
    if (e) delete e;
}

void SistemaAeroNode::descargarBodegaCompleta(const std::string& vueloId) {
    Vuelo* v = buscarVuelo(vueloId);
    if (!v) {
        std::cout << "  ERROR: vuelo " << vueloId << " no encontrado.\n";
        return;
    }

    std::cout << "  Iniciando descarga completa de bodega (LIFO)...\n";
    int n = 0;
    while (!v->bodegaEstaVacia()) {
        Equipaje* e = v->descargarEquipaje();
        if (!e) break;
        delete e;
        ++n;
    }
    std::cout << "  OK " << n << " pieza(s) descargada(s).\n";
}

void SistemaAeroNode::buscarPasajero(const std::string& clave) const {
    bool encontrado = false;
    for (int i = 0; i < (int)vuelos.size(); i++) {
        Pasajero* p = vuelos[i]->buscarPorDni(clave);
        if (!p) p = vuelos[i]->buscarPorLocalizador(clave);
        if (p) {
            std::cout << "  Encontrado en vuelo " << vuelos[i]->getId() << ": ";
            p->mostrar();
            encontrado = true;
        }
    }
    if (!encontrado) {
        std::cout << "  ERROR: pasajero no encontrado con clave " << clave << ".\n";
    }
}

void SistemaAeroNode::agregarRutaGrafo(const std::string& origen,
                                        const std::string& destino,
                                        int minutos,
                                        const std::string& codigoVuelo) {
    if (origen.empty() || destino.empty() || minutos <= 0) {
        std::cout << "  ERROR: datos de ruta no validos.\n";
        return;
    }
    grafo.agregarRuta(origen, destino, minutos, codigoVuelo);
    std::cout << "  OK Ruta " << origen << " -> " << destino
              << " (" << minutos << " min) anadida al grafo.\n";
}

void SistemaAeroNode::mostrarGrafo() const {
    std::cout << "\n-- Red de rutas (grafo ponderado) --\n";
    grafo.mostrar();
}

void SistemaAeroNode::mostrarRutaAlternativa(const std::string& origen,
                                              const std::string& destino) const {
    int costo = 0;
    std::vector<std::string> ruta = grafo.rutaMasCorta(origen, destino, costo);
    if (ruta.empty() || costo < 0) {
        std::cout << "  ERROR: no existe ruta entre " << origen << " y " << destino << ".\n";
        return;
    }

    std::cout << "  Ruta optima " << origen << " -> " << destino
              << " (" << costo << " min): ";
    for (int i = 0; i < (int)ruta.size(); i++) {
        std::cout << ruta[i];
        if (i + 1 < (int)ruta.size()) std::cout << " -> ";
    }
    std::cout << "\n";
}

void SistemaAeroNode::mostrarAlcanzables(const std::string& origen) const {
    std::vector<std::string> lista = grafo.alcanzables(origen);
    if (lista.empty()) {
        std::cout << "  ERROR: no hay aeropuertos alcanzables desde " << origen << ".\n";
        return;
    }

    std::cout << "  Aeropuertos alcanzables desde " << origen << ": ";
    for (int i = 0; i < (int)lista.size(); i++) {
        std::cout << lista[i];
        if (i + 1 < (int)lista.size()) std::cout << " -> ";
    }
    std::cout << "\n";
}

bool SistemaAeroNode::reubicarPasajero(const std::string& vueloOrigenId,
                                        const std::string& localizador) {
    Vuelo* vOrigen = buscarVuelo(vueloOrigenId);
    if (!vOrigen) {
        std::cout << "  ERROR: vuelo origen no encontrado.\n";
        return false;
    }

    Pasajero* p = vOrigen->buscarPorLocalizador(localizador);
    if (!p) {
        std::cout << "  ERROR: pasajero con localizador " << localizador << " no encontrado.\n";
        return false;
    }

    for (int i = 0; i < (int)vuelos.size(); i++) {
        Vuelo* vAlt = vuelos[i];
        if (vAlt->getId() == vueloOrigenId) continue;
        if (vAlt->getDestino() != vOrigen->getDestino()) continue;
        if (vAlt->getOcupacion() >= vAlt->getCapacidad()) continue;
        if (vAlt->buscarPorDni(p->getDni())) continue;

        Pasajero* pNuevo = nullptr;
        if (p->getTipo() == "VIP") {
            PasajeroVIP* vip = dynamic_cast<PasajeroVIP*>(p);
            pNuevo = new PasajeroVIP(p->getNombre(), p->getDni(),
                                     generarLocalizador(),
                                     vip ? vip->getBeneficios() : "Sala VIP");
        } else if (p->getTipo() == "CONEXION") {
            PasajeroConexion* pc = dynamic_cast<PasajeroConexion*>(p);
            pNuevo = new PasajeroConexion(p->getNombre(), p->getDni(),
                                          generarLocalizador(),
                                          pc ? pc->getVueloConexion() : "");
        } else {
            pNuevo = new PasajeroRegular(p->getNombre(), p->getDni(), generarLocalizador());
        }

        if (!vOrigen->cancelarReserva(localizador)) {
            delete pNuevo;
            return false;
        }

        int resultado = vAlt->venderBillete(pNuevo);
        if (resultado == 0 || resultado == 1) {
            vAlt->registrarPasajero(pNuevo);
            std::cout << "  OK " << p->getNombre()
                      << " reubicado en vuelo alternativo " << vAlt->getId() << ".\n";
            return true;
        }
        delete pNuevo;
        return false;
    }

    int costo = 0;
    std::vector<std::string> ruta = grafo.rutaMasCorta(
        vOrigen->getOrigen(), vOrigen->getDestino(), costo);
    if (!ruta.empty() && costo >= 0) {
        std::cout << "  Ruta alternativa sugerida mediante grafo: ";
        for (int i = 0; i < (int)ruta.size(); i++) {
            std::cout << ruta[i];
            if (i + 1 < (int)ruta.size()) std::cout << " -> ";
        }
        std::cout << " (" << costo << " min).\n";
        return true;
    }

    std::cout << "  ERROR: no se encontro vuelo ni ruta alternativa.\n";
    return false;
}

bool SistemaAeroNode::guardarSistema() const {
    return persistencia.guardarSistema(vuelos, grafo);
}

bool SistemaAeroNode::cargarSistema() {
    std::vector<Vuelo*> nuevos = persistencia.cargarVuelos();
    if (nuevos.empty()) {
        return false;
    }

    for (int i = 0; i < (int)vuelos.size(); i++) delete vuelos[i];
    vuelos = nuevos;

    grafo = GrafoRutas();
    inicializarGrafoDemo();
    persistencia.cargarGrafo(grafo);
    return true;
}

void SistemaAeroNode::mostrarInformeOcupacion() const {
    std::cout << "\nINFORME DE OCUPACION\n";
    if (vuelos.empty()) {
        std::cout << "  (no hay vuelos)\n";
        return;
    }

    for (int i = 0; i < (int)vuelos.size(); i++) {
        Vuelo* v = vuelos[i];
        double pct = v->getCapacidad() > 0
                     ? (100.0 * v->getOcupacion() / v->getCapacidad()) : 0.0;
        std::cout << "  " << v->getId()
                  << " (" << v->getOrigen() << " -> " << v->getDestino() << ")"
                  << " | Ocupacion: " << v->getOcupacion() << "/" << v->getCapacidad()
                  << " (" << static_cast<int>(pct) << "%)"
                  << " | Espera: " << v->getEsperaSize() << "\n";
    }
}

void SistemaAeroNode::mostrarTodasIncidencias() const {
    std::cout << "\nINCIDENCIAS GLOBALES\n";
    bool hayAlguna = false;
    for (int i = 0; i < (int)vuelos.size(); i++) {
        if (!vuelos[i]->getIncidencias().empty()) {
            vuelos[i]->mostrarIncidencias();
            hayAlguna = true;
        }
    }
    if (!hayAlguna) std::cout << "  (sin incidencias registradas)\n";
}

void SistemaAeroNode::generarInformeFichero() const {
    persistencia.generarInformeOcupacion(vuelos);
}

void SistemaAeroNode::mostrarDetalleVuelo(const std::string& id) const {
    Vuelo* v = buscarVuelo(id);
    if (!v) {
        std::cout << "  ERROR: vuelo " << id << " no encontrado.\n";
        return;
    }
    v->mostrarResumen();
    v->mostrarReservas();
    v->mostrarEspera();
    v->mostrarBodega();
    v->mostrarAVL();
    v->mostrarIncidencias();
}




static void limpiarCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int leerEntero(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            limpiarCin();
            return val;
        }
        std::cout << "  ERROR: entrada invalida. Introduce un numero.\n";
        limpiarCin();
    }
}

static double leerDouble(const std::string& prompt) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            limpiarCin();
            return val;
        }
        std::cout << "  ERROR: entrada invalida. Introduce un numero.\n";
        limpiarCin();
    }
}

static std::string leerCadena(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

static void imprimirCabecera(const std::string& titulo) {
    std::cout << "\n============================================\n";
    std::cout << "  " << titulo << "\n";
    std::cout << "============================================\n";
}

static void menuVuelos(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        imprimirCabecera("Gestion de Vuelos");
        std::cout << "  1. Crear vuelo\n"
                  << "  2. Listar vuelos activos\n"
                  << "  3. Ver detalle de un vuelo\n"
                  << "  0. Volver\n";
        op = leerEntero("  Opcion: ");

        switch (op) {
        case 1: {
            std::string id  = leerCadena("  ID vuelo (ej. IB1234): ");
            std::string org = leerCadena("  IATA origen (ej. VGO): ");
            std::string dst = leerCadena("  IATA destino (ej. MAD): ");
            std::string fh  = leerCadena("  Fecha/Hora (YYYY-MM-DD HH:MM): ");
            int cap = leerEntero("  Capacidad pasajeros: ");
            int bod = leerEntero("  Capacidad bodega (numero de maletas): ");
            sistema.crearVuelo(id, org, dst, fh, cap, bod);
            break;
        }
        case 2:
            sistema.listarVuelos();
            break;
        case 3: {
            std::string id = leerCadena("  ID vuelo: ");
            sistema.mostrarDetalleVuelo(id);
            break;
        }
        case 0:
            break;
        default:
            std::cout << "  Opcion no valida.\n";
        }
    } while (op != 0);
}

static void menuReservas(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        imprimirCabecera("Reservas y Pasajeros");
        std::cout << "  1. Vender billete\n"
                  << "  2. Cancelar reserva\n"
                  << "  3. Buscar pasajero (DNI/Localizador)\n"
                  << "  4. Ver arbol AVL de reservas\n"
                  << "  0. Volver\n";
        op = leerEntero("  Opcion: ");

        switch (op) {
        case 1: {
            std::string vid = leerCadena("  ID vuelo: ");
            std::cout << "  Tipo pasajero: 1.REGULAR  2.VIP  3.CONEXION\n";
            int t = leerEntero("  Tipo: ");
            std::string tipo = (t == 2) ? "VIP" : (t == 3) ? "CONEXION" : "REGULAR";
            std::string nom = leerCadena("  Nombre: ");
            std::string dni = leerCadena("  DNI/Pasaporte: ");
            std::string loc = leerCadena("  Localizador (Enter=autogenerar): ");
            if (loc.empty()) {
                loc = "AN" + std::to_string(std::rand() % 9000 + 1000);
            }

            std::string extra;
            if (tipo == "VIP") {
                extra = leerCadena("  Beneficios (Enter=default): ");
            } else if (tipo == "CONEXION") {
                extra = leerCadena("  Vuelo de conexion: ");
            }
            sistema.venderBillete(vid, tipo, nom, dni, loc, extra);
            break;
        }
        case 2: {
            std::string vid = leerCadena("  ID vuelo: ");
            std::string loc = leerCadena("  Localizador: ");
            sistema.cancelarReserva(vid, loc);
            break;
        }
        case 3: {
            std::string clave = leerCadena("  DNI o localizador: ");
            sistema.buscarPasajero(clave);
            break;
        }
        case 4: {
            std::string vid = leerCadena("  ID vuelo: ");
            sistema.mostrarDetalleVuelo(vid);
            break;
        }
        case 0:
            break;
        default:
            std::cout << "  Opcion no valida.\n";
        }
    } while (op != 0);
}

static void menuBodega(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        imprimirCabecera("Bodega de Equipaje (LIFO)");
        std::cout << "  1. Cargar equipaje\n"
                  << "  2. Descargar equipaje (1 pieza)\n"
                  << "  3. Descarga completa de bodega\n"
                  << "  0. Volver\n";
        op = leerEntero("  Opcion: ");

        switch (op) {
        case 1: {
            std::string vid = leerCadena("  ID vuelo: ");
            std::cout << "  Tipo equipaje: 1.FACTURADO  2.MANO  3.ESPECIAL\n";
            int t = leerEntero("  Tipo: ");
            std::string tipo = (t == 2) ? "MANO" : (t == 3) ? "ESPECIAL" : "FACTURADO";
            std::string eid = leerCadena("  ID equipaje: ");
            std::string pdni = leerCadena("  DNI propietario: ");
            double peso = leerDouble("  Peso (kg): ");
            double vol = leerDouble("  Volumen (L): ");

            std::string extra;
            if (tipo == "FACTURADO") {
                int fr = leerEntero("  Fragil? 1=Si 0=No: ");
                extra = (fr == 1) ? "FRAGIL" : "";
            } else if (tipo == "ESPECIAL") {
                extra = leerCadena("  Descripcion: ");
            }
            sistema.cargarEquipaje(vid, tipo, eid, pdni, peso, vol, extra);
            break;
        }
        case 2: {
            std::string vid = leerCadena("  ID vuelo: ");
            sistema.descargarUnEquipaje(vid);
            break;
        }
        case 3: {
            std::string vid = leerCadena("  ID vuelo: ");
            sistema.descargarBodegaCompleta(vid);
            break;
        }
        case 0:
            break;
        default:
            std::cout << "  Opcion no valida.\n";
        }
    } while (op != 0);
}

static void menuRutas(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        imprimirCabecera("Red de Rutas y Reubicaciones (Grafo)");
        std::cout << "  1. Mostrar red de rutas\n"
                  << "  2. Buscar ruta optima entre aeropuertos\n"
                  << "  3. Anadir nueva ruta al grafo\n"
                  << "  4. Reubicar pasajero en vuelo alternativo\n"
                  << "  5. Ver aeropuertos alcanzables desde un origen (BFS)\n"
                  << "  0. Volver\n";
        op = leerEntero("  Opcion: ");

        switch (op) {
        case 1:
            sistema.mostrarGrafo();
            break;
        case 2: {
            std::string org = leerCadena("  IATA origen: ");
            std::string dst = leerCadena("  IATA destino: ");
            sistema.mostrarRutaAlternativa(org, dst);
            break;
        }
        case 3: {
            std::string org = leerCadena("  IATA origen: ");
            std::string dst = leerCadena("  IATA destino: ");
            int min = leerEntero("  Tiempo (min): ");
            std::string cv = leerCadena("  Codigo vuelo: ");
            sistema.agregarRutaGrafo(org, dst, min, cv);
            break;
        }
        case 4: {
            std::string vid = leerCadena("  ID vuelo origen: ");
            std::string loc = leerCadena("  Localizador pasajero: ");
            sistema.reubicarPasajero(vid, loc);
            break;
        }
        case 5: {
            std::string org = leerCadena("  IATA origen: ");
            sistema.mostrarAlcanzables(org);
            break;
        }
        case 0:
            break;
        default:
            std::cout << "  Opcion no valida.\n";
        }
    } while (op != 0);
}

static void menuInformes(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        imprimirCabecera("Informes y Estadisticas");
        std::cout << "  1. Informe de ocupacion (consola)\n"
                  << "  2. Incidencias y reubicaciones\n"
                  << "  3. Guardar informe de ocupacion en fichero\n"
                  << "  0. Volver\n";
        op = leerEntero("  Opcion: ");

        switch (op) {
        case 1:
            sistema.mostrarInformeOcupacion();
            break;
        case 2:
            sistema.mostrarTodasIncidencias();
            break;
        case 3:
            sistema.generarInformeFichero();
            break;
        case 0:
            break;
        default:
            std::cout << "  Opcion no valida.\n";
        }
    } while (op != 0);
}

static void menuPrincipal(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        imprimirCabecera("AeroNode | Gestor Aereo | Grupo 4 UIE");
        std::cout << "  1. Gestion de Vuelos\n"
                  << "  2. Reservas y Pasajeros\n"
                  << "  3. Bodega de Equipaje (LIFO)\n"
                  << "  4. Red de Rutas y Reubicaciones (Grafo)\n"
                  << "  5. Informes y Estadisticas\n"
                  << "  6. Guardar sistema en fichero\n"
                  << "  7. Cargar sistema desde fichero\n"
                  << "  0. Salir\n";
        op = leerEntero("  Opcion: ");

        switch (op) {
        case 1:
            menuVuelos(sistema);
            break;
        case 2:
            menuReservas(sistema);
            break;
        case 3:
            menuBodega(sistema);
            break;
        case 4:
            menuRutas(sistema);
            break;
        case 5:
            menuInformes(sistema);
            break;
        case 6:
            sistema.guardarSistema();
            break;
        case 7:
            sistema.cargarSistema();
            break;
        case 0:
            std::cout << "\n  Guardando y cerrando AeroNode... Hasta pronto!\n\n";
            sistema.guardarSistema();
            break;
        default:
            std::cout << "  Opcion no valida. Introduce un numero del 0 al 7.\n";
        }
    } while (op != 0);
}

static void cargarDemoInicial(SistemaAeroNode& sistema) {
    std::cout << "\n  [DEMO] Cargando datos de demostracion...\n";

    sistema.crearVuelo("IB3841", "VGO", "MAD", "2026-04-20 08:30", 4, 8);
    sistema.crearVuelo("VY6701", "VGO", "BCN", "2026-04-20 10:15", 3, 6);
    sistema.crearVuelo("IB3011", "MAD", "BCN", "2026-04-20 12:00", 5, 10);

    sistema.venderBillete("IB3841", "REGULAR", "Ana Garcia", "12345678A", "LOC001");
    sistema.venderBillete("IB3841", "VIP", "Carlos Lopez", "87654321B", "LOC002", "Sala VIP");
    sistema.venderBillete("IB3841", "REGULAR", "Maria Fernandez", "11223344C", "LOC003");
    sistema.venderBillete("IB3841", "CONEXION", "Pedro Ruiz", "44332211D", "LOC004", "VY6701");
    sistema.venderBillete("IB3841", "REGULAR", "Sofia Martin", "55667788E", "LOC005");

    sistema.cargarEquipaje("IB3841", "FACTURADO", "EQ001", "12345678A", 22.0, 55.0, "");
    sistema.cargarEquipaje("IB3841", "FACTURADO", "EQ002", "87654321B", 18.5, 50.0, "FRAGIL");
    sistema.cargarEquipaje("IB3841", "ESPECIAL", "EQ003", "44332211D", 15.0, 60.0, "Guitarra");
    sistema.cargarEquipaje("IB3841", "FACTURADO", "EQ004", "11223344C", 20.0, 52.0, "");

    sistema.venderBillete("VY6701", "REGULAR", "Luis Gomez", "99887766F", "LOC006");
    sistema.venderBillete("VY6701", "VIP", "Elena Sanchez", "66778899G", "LOC007", "Sala VIP");

    std::cout << "  [DEMO] Datos cargados. Sistema listo!\n";
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "\n============================================\n"
              << "  Bienvenido a AeroNode\n"
              << "  Gestor de Aerolineas en C++\n"
              << "============================================\n";

    SistemaAeroNode sistema("data/");

    int opc = leerEntero("\n  Desea cargar datos desde fichero? (1=Si / 0=Demo): ");
    if (opc == 1) {
        bool cargado = sistema.cargarSistema();
        if (!cargado) {
            std::cout << "  No se encontraron datos previos. Se cargara la demo inicial.\n";
            cargarDemoInicial(sistema);
        }
    } else {
        cargarDemoInicial(sistema);
    }

    menuPrincipal(sistema);
    return 0;
}

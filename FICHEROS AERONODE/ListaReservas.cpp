#include "ListaReservas.h"
#include <iostream>

ListaReservas::ListaReservas(int capacidad)
    : cabeza(nullptr), cola(nullptr), tamano(0), capacidadMaxima(capacidad) {}

ListaReservas::~ListaReservas() {
    NodoReserva* actual = cabeza;
    while (actual) {
        NodoReserva* sig = actual->siguiente;
        // El Pasajero es propiedad del sistema, NO se elimina aquí
        delete actual;
        actual = sig;
    }
}

bool ListaReservas::insertar(Pasajero* p, int asiento) {
    if (estaLlena()) return false;
    // Verificar asiento duplicado
    NodoReserva* tmp = cabeza;
// HCR18
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
// HCR18

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
// HCR18
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
    // Busca el menor número de asiento libre entre 1 y capacidadMaxima
    for (int i = 1; i <= capacidadMaxima; ++i) {
        bool ocupado = false;
        NodoReserva* actual = cabeza;
        while (actual) {
            if (actual->asiento == i) { ocupado = true; break; }
            actual = actual->siguiente;
// HCR18
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

#include "Estructuras.h"
#include <iostream>

// ColaEspera (FIFO)
ColaEspera::ColaEspera() : frente(nullptr), final_(nullptr), tamano(0) {}

ColaEspera::~ColaEspera() {
    while (!estaVacia()) desencolar();
}

void ColaEspera::encolar(Pasajero* p) {
    NodoCola* nuevo = new NodoCola(p);
    if (!final_) { frente = final_ = nuevo; }
    else         { final_->siguiente = nuevo; final_ = nuevo; }
    ++tamano;
}

Pasajero* ColaEspera::desencolar() {
    if (estaVacia()) return nullptr;
    NodoCola* tmp = frente;
// HCR18
    Pasajero* p   = tmp->pasajero;
    frente = frente->siguiente;
    if (!frente) final_ = nullptr;
    delete tmp;
    --tamano;
    return p;
}

Pasajero* ColaEspera::verFrente() const {
    return frente ? frente->pasajero : nullptr;
}

void ColaEspera::mostrar() const {
    if (estaVacia()) { std::cout << "(cola vacía)\n"; return; }
    NodoCola* actual = frente;
    int pos = 1;
    while (actual) {
        std::cout << "  Pos " << pos++ << ": ";
        actual->pasajero->mostrar();
        actual = actual->siguiente;
// HCR18
    }
}

// PilaBodega (LIFO)
PilaBodega::PilaBodega(int cap) : tope(nullptr), tamano(0), capacidadMax(cap) {}

PilaBodega::~PilaBodega() {
    while (!estaVacia()) {
        Equipaje* e = desapilar();
        delete e;
    }
}

bool PilaBodega::apilar(Equipaje* e) {
    if (estaLlena()) return false;
    NodoPila* nuevo  = new NodoPila(e);
    nuevo->siguiente = tope;
    tope  = nuevo;
    ++tamano;
    return true;
// HCR18
}

Equipaje* PilaBodega::desapilar() {
    if (estaVacia()) return nullptr;
    NodoPila* tmp = tope;
    Equipaje* e   = tmp->equipaje;
    tope = tope->siguiente;
    delete tmp;
    --tamano;
    return e;
}

Equipaje* PilaBodega::verTope() const {
    return tope ? tope->equipaje : nullptr;
}

void PilaBodega::mostrar() const {
    if (estaVacia()) { std::cout << "(bodega vacía)\n"; return; }
    NodoPila* actual = tope;
    int nivel = tamano;
// HCR18
    std::cout << "[TOPE - último cargado / primero en descargar]\n";
    while (actual) {
        std::cout << "Nivel " << nivel-- << ": ";
        actual->equipaje->mostrar();
        actual = actual->siguiente;
    }
}

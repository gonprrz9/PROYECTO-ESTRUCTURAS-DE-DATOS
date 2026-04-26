#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include "Pasajero.h"
#include "Equipaje.h"
#include <iostream>

//  COLA FIFO  –  Lista de espera por overbooking
struct NodoCola {
    Pasajero* pasajero;
    NodoCola* siguiente;
    explicit NodoCola(Pasajero* p) : pasajero(p), siguiente(nullptr) {}
};

class ColaEspera {
    NodoCola* frente;
    NodoCola* final_;
    int       tamano;
// HCR18
public:
    ColaEspera();
    ~ColaEspera();

    void      encolar(Pasajero* p);
    Pasajero* desencolar();           // extrae el primero (FIFO)
    Pasajero* verFrente() const;
    bool      estaVacia() const { return tamano == 0; }
    int       getTamano() const { return tamano; }
    void      mostrar()   const;

    ColaEspera(const ColaEspera&)            = delete;
    ColaEspera& operator=(const ColaEspera&) = delete;
};

//  PILA LIFO  –  Bodega de equipaje
struct NodoPila {
    Equipaje* equipaje;
// HCR18
    NodoPila* siguiente;
    explicit NodoPila(Equipaje* e) : equipaje(e), siguiente(nullptr) {}
};

class PilaBodega {
    NodoPila* tope;
    int       tamano;
    int       capacidadMax;
public:
    explicit PilaBodega(int cap = 100);
    ~PilaBodega();

    bool      apilar(Equipaje* e);    // carga equipaje
    Equipaje* desapilar();            // descarga (último en entrar, primero en salir)
    Equipaje* verTope() const;
    bool      estaVacia() const { return tamano == 0; }
    bool      estaLlena() const { return tamano >= capacidadMax; }
    int       getTamano() const { return tamano; }
    void      mostrar()   const;

// HCR18
    PilaBodega(const PilaBodega&)            = delete;
    PilaBodega& operator=(const PilaBodega&) = delete;
};

#endif // ESTRUCTURAS_H

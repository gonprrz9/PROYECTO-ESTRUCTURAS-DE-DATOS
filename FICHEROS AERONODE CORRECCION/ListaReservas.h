#ifndef LISTA_RESERVAS_H
#define LISTA_RESERVAS_H

#include "Pasajero.h"
#include <string>
#include <iostream>

//  Nodo de la lista doblemente enlazada
struct NodoReserva {
    Pasajero*    pasajero;
    int          asiento;
    NodoReserva* siguiente;
    NodoReserva* anterior;

    NodoReserva(Pasajero* p, int a)
        : pasajero(p), asiento(a), siguiente(nullptr), anterior(nullptr) {}
};

//  Lista Doblemente Enlazada de Reservas
class ListaReservas {
    NodoReserva* cabeza;
    NodoReserva* cola;
    int          tamano;
    int          capacidadMaxima;

public:
    explicit ListaReservas(int capacidad);
    ~ListaReservas();

    // Operaciones principales
    bool        insertar(Pasajero* p, int asiento);
    bool        eliminar(const std::string& localizador, Pasajero*& pasajeroEliminado);
    NodoReserva* buscarPorLocalizador(const std::string& loc) const;
    NodoReserva* buscarPorDni(const std::string& dni)         const;

    // Consultas
    bool estaLlena()       const { return tamano >= capacidadMaxima; }
    bool estaVacia()       const { return tamano == 0; }
    int  getTamano()       const { return tamano; }
    int  getCapacidad()    const { return capacidadMaxima; }
    int  asientosLibres()  const { return capacidadMaxima - tamano; }

    // Primer asiento libre disponible
    int  primerAsientoLibre() const;

    // Mostrar
    void mostrar() const;

    // Iteración (para serialización)
    NodoReserva* getCabeza() const { return cabeza; }

    // Deshabilitar copia
    ListaReservas(const ListaReservas&)            = delete;
    ListaReservas& operator=(const ListaReservas&) = delete;
};

#endif // LISTA_RESERVAS_H

#ifndef VUELO_H
#define VUELO_H

#include "ListaReservas.h"
#include "Estructuras.h"
#include "ArbolAVL.h"
#include "Incidencia.h"
#include <string>
#include <vector>
#include <iostream>

//  Clase Vuelo  –  núcleo del sistema
class Vuelo {
    std::string    id;           // p.ej. "VY1234"
    std::string    origen;       // IATA origen
    std::string    destino;      // IATA destino
    std::string    fechaHora;
    int            capacidad;
    bool           activo;

    ListaReservas  reservas;
    ColaEspera     listaEspera;
    PilaBodega     bodega;
    ArbolAVL       indiceAVL;    // indice de busqueda por DNI y localizador

    std::vector<Incidencia> incidencias;

    // Propietario de los pasajeros en este vuelo
    std::vector<Pasajero*>  pasajerosAllocated;

public:
    Vuelo(const std::string& id,
          const std::string& origen,
          const std::string& destino,
          const std::string& fechaHora,
          int capacidad,
          int capacidadBodega = 60);
    ~Vuelo();

    // Getters
    std::string getId()        const { return id; }
    std::string getOrigen()    const { return origen; }
    std::string getDestino()   const { return destino; }
    std::string getFechaHora() const { return fechaHora; }
    int         getCapacidad() const { return capacidad; }
    bool        isActivo()     const { return activo; }
    int         getOcupacion() const { return reservas.getTamano(); }

    // Operaciones de reserva
    // Devuelve: 0=OK, 1=overbooking(añadido a espera), 2=error
    int  venderBillete(Pasajero* p);
    bool cancelarReserva(const std::string& localizador);

    // Búsqueda
    Pasajero*    buscarPorDni(const std::string& dni)         const;
    Pasajero*    buscarPorLocalizador(const std::string& loc) const;

    // Bodega
    bool     cargarEquipaje(Equipaje* e);
    Equipaje* descargarEquipaje();

    // Lista de espera
    int  getEsperaSize()  const { return listaEspera.getTamano(); }

    // Mostrar
    void mostrarResumen()     const;
    void mostrarReservas()    const;
    void mostrarEspera()      const;
    void mostrarBodega()      const;
    void mostrarAVL()         const;
    void mostrarIncidencias() const;

    // Serialización
    std::string serializarCabecera() const;
    const std::vector<Incidencia>& getIncidencias() const { return incidencias; }
    const ListaReservas& getReservas()              const { return reservas; }
    const ColaEspera&    getEspera()                const { return listaEspera; }

    // Tomar propiedad de puntero de pasajero
    void registrarPasajero(Pasajero* p) { pasajerosAllocated.push_back(p); }

    Vuelo(const Vuelo&)            = delete;
    Vuelo& operator=(const Vuelo&) = delete;

private:
    void registrarIncidencia(TipoIncidencia t,
                              const std::string& desc,
                              const std::string& dni);
    std::string timestampActual() const;
};

#endif // VUELO_H

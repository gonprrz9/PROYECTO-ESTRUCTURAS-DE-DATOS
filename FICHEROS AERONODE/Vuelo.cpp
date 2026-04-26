#include "Vuelo.h"
#include <iostream>
#include <ctime>
#include <sstream>

Vuelo::Vuelo(const std::string& id_, const std::string& org,
             const std::string& dst, const std::string& fh,
             int cap, int capBodega)
    : id(id_), origen(org), destino(dst), fechaHora(fh),
      capacidad(cap), activo(true),
      reservas(cap), bodega(capBodega) {}

Vuelo::~Vuelo() {
    for (Pasajero* p : pasajerosAllocated) delete p;
}

std::string Vuelo::timestampActual() const {
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", std::localtime(&t));
// HCR18
    return std::string(buf);
}

void Vuelo::registrarIncidencia(TipoIncidencia t,
                                 const std::string& desc,
                                 const std::string& dni) {
    incidencias.push_back(Incidencia(t, desc, dni, id, timestampActual()));
}

// Vender billete
int Vuelo::venderBillete(Pasajero* p) {
    if (!reservas.estaLlena()) {
        int asiento = reservas.primerAsientoLibre();
        if (reservas.insertar(p, asiento)) {
            indiceAVL.insertar(p->getDni(), p);
            indiceAVL.insertar(p->getLocalizador(), p);
            std::cout << "  ✓ Billete vendido. Asiento " << asiento
                      << " asignado a " << p->getNombre() << "\n";
            return 0;
        }
// HCR18
    }
    // Overbooking: añadir a cola de espera
    listaEspera.encolar(p);
    registrarIncidencia(TipoIncidencia::OVERBOOKING,
                        "Pasajero en lista de espera por overbooking",
                        p->getDni());
    std::cout << "  ⚠ Vuelo lleno. " << p->getNombre()
              << " añadido a lista de espera (pos "
              << listaEspera.getTamano() << ")\n";
    return 1;
}

// Cancelar reserva
bool Vuelo::cancelarReserva(const std::string& localizador) {
    Pasajero* p = nullptr;
    if (!reservas.eliminar(localizador, p)) {
        std::cout << "  ✗ No se encontro reserva con localizador " << localizador << "\n";
        return false;
    }
    indiceAVL.eliminar(p->getDni());
// HCR18
    indiceAVL.eliminar(p->getLocalizador());
    registrarIncidencia(TipoIncidencia::CANCELACION,
                        "Reserva cancelada por " + p->getNombre(), p->getDni());
    std::cout << "  ✓ Reserva de " << p->getNombre() << " cancelada.\n";

    // Asignar plaza automáticamente al primero de la cola FIFO
    if (!listaEspera.estaVacia()) {
        Pasajero* siguiente = listaEspera.desencolar();
        int asiento = reservas.primerAsientoLibre();
        reservas.insertar(siguiente, asiento);
        indiceAVL.insertar(siguiente->getDni(), siguiente);
        indiceAVL.insertar(siguiente->getLocalizador(), siguiente);
        registrarIncidencia(TipoIncidencia::REUBICACION,
                            "Pasajero reasignado desde lista de espera",
                            siguiente->getDni());
        std::cout << "  ✓ " << siguiente->getNombre()
                  << " asignado automaticamente al asiento " << asiento
                  << " desde lista de espera.\n";
    }
    return true;
// HCR18
}

// Búsqueda
Pasajero* Vuelo::buscarPorDni(const std::string& dni) const {
    return indiceAVL.buscar(dni);
}

Pasajero* Vuelo::buscarPorLocalizador(const std::string& loc) const {
    return indiceAVL.buscar(loc);
}

// Bodega
bool Vuelo::cargarEquipaje(Equipaje* e) {
    if (!bodega.apilar(e)) {
        std::cout << "  ✗ Bodega llena. No se puede cargar el equipaje.\n";
        return false;
    }
    std::cout << "  ✓ Equipaje " << e->getId() << " cargado en bodega.\n";
    return true;
}
// HCR18

Equipaje* Vuelo::descargarEquipaje() {
    Equipaje* e = bodega.desapilar();
    if (!e) { std::cout << "  ✗ Bodega vacia.\n"; return nullptr; }
    std::cout << "  ✓ Descargado: ";
    e->mostrar();
    return e;
}

// Mostrar
void Vuelo::mostrarResumen() const {
    std::cout << "┌─ Vuelo " << id << " (" << origen << " → " << destino << ")"
              << " [" << fechaHora << "]\n"
              << "│  Capacidad: " << reservas.getTamano() << "/" << capacidad
              << "| Espera: " << listaEspera.getTamano()
              << "| Bodega: " << bodega.getTamano()
              << "| " << (activo ? "ACTIVO" : "INACTIVO") << "\n└─\n";
}

void Vuelo::mostrarReservas() const {
// HCR18
    std::cout << "| Reservas vuelo " << id << " |\n";
    reservas.mostrar();
}

void Vuelo::mostrarEspera() const {
    std::cout << "| Lista de espera vuelo " << id << " |\n";
    listaEspera.mostrar();
}

void Vuelo::mostrarBodega() const {
    std::cout << "| Bodega vuelo " << id << " |\n";
    bodega.mostrar();
}

void Vuelo::mostrarAVL() const {
    std::cout << "| Indice AVL vuelo " << id << " (inorden) |\n";
    indiceAVL.mostrarInorden();
}

void Vuelo::mostrarIncidencias() const {
// HCR18
    std::cout << "| Incidencias vuelo " << id << " |\n";
    if (incidencias.empty()) { std::cout << "  (sin incidencias)\n"; return; }
    for (int i = 0; i < (int)incidencias.size(); i++) incidencias[i].mostrar();
}

std::string Vuelo::serializarCabecera() const {
    return "VUELO;" + id + ";" + origen + ";" + destino + ";"
           + fechaHora + ";" + std::to_string(capacidad);
}

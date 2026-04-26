#include "Pasajero.h"
#include <iostream>

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
// HCR18
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
// HCR18
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
// HCR18
              << " | Vuelo conexion: " << vueloConexion
              << " | Min: " << tiempoMinConex << "\n";
}

std::string PasajeroConexion::serializar() const {
    return "CONEXION;" + nombre + ";" + dni + ";" + localizador + ";"
           + vueloConexion + ";" + std::to_string(tiempoMinConex);
}

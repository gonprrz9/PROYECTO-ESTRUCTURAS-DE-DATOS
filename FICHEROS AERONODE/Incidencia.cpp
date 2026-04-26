#include "Incidencia.h"
#include <iostream>

Incidencia::Incidencia(TipoIncidencia t, const std::string& d,
                       const std::string& dni, const std::string& v,
                       const std::string& ts)
    : tipo(t), descripcion(d), pasajeroDni(dni), vueloId(v), timestamp(ts) {}

std::string Incidencia::tipoStr(TipoIncidencia t) {
    switch (t) {
        case TipoIncidencia::OVERBOOKING:        return "OVERBOOKING";
        case TipoIncidencia::CANCELACION:        return "CANCELACION";
        case TipoIncidencia::REUBICACION:        return "REUBICACION";
        case TipoIncidencia::PERDIDA_CONEXION:   return "PERDIDA_CONEXION";
        case TipoIncidencia::EQUIPAJE_EXTRAVIADO:return "EQUIPAJE_EXTRAVIADO";
        default:                                  return "OTRO";
    }
}

void Incidencia::mostrar() const {
// HCR18
    std::cout << "[" << tipoStr(tipo) << "] "
              << timestamp << " | Vuelo:" << vueloId
              << "| DNI:" << pasajeroDni
              << "| " << descripcion << "\n";
}

std::string Incidencia::serializar() const {
    return tipoStr(tipo) + ";" + timestamp + ";" + vueloId + ";"
           + pasajeroDni + ";" + descripcion;
}

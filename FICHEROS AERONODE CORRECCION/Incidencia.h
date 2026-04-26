#ifndef INCIDENCIA_H
#define INCIDENCIA_H

#include <string>
#include <iostream>

//  Incidencia  –  registro de eventos anómalos
enum class TipoIncidencia {
    OVERBOOKING,
    CANCELACION,
    REUBICACION,
    PERDIDA_CONEXION,
    EQUIPAJE_EXTRAVIADO,
    OTRO
};

struct Incidencia {
    TipoIncidencia tipo;
    std::string    descripcion;
    std::string    pasajeroDni;
    std::string    vueloId;
    std::string    timestamp;

    Incidencia(TipoIncidencia t,
               const std::string& desc,
               const std::string& dni,
               const std::string& vuelo,
               const std::string& ts);

    void        mostrar()     const;
    std::string serializar()  const;
    static std::string tipoStr(TipoIncidencia t);
};

#endif // INCIDENCIA_H

#ifndef GESTOR_PERSISTENCIA_H
#define GESTOR_PERSISTENCIA_H

#include "Vuelo.h"
#include "GrafoRutas.h"
#include <string>
#include <vector>

//  GestorPersistencia  –  guarda y carga el estado del sistema
class GestorPersistencia {
    std::string directorioBase;
public:
    explicit GestorPersistencia(const std::string& dir = "data/");

    // Guardar todo el sistema
    bool guardarSistema(const std::vector<Vuelo*>& vuelos,
                        const GrafoRutas& grafo) const;

// HCR18
    // Cargar vuelos (devuelve punteros; el llamador es dueño)
    std::vector<Vuelo*> cargarVuelos() const;

    // Guardar/cargar solo el grafo
    bool         guardarGrafo(const GrafoRutas& grafo) const;
    void         cargarGrafo(GrafoRutas& grafo)        const;

    // Informe de ocupación
    bool generarInformeOcupacion(const std::vector<Vuelo*>& vuelos) const;

private:
    std::string rutaVuelos()    const;
    std::string rutaGrafo()     const;
    std::string rutaInforme()   const;
};

#endif // GESTOR_PERSISTENCIA_H

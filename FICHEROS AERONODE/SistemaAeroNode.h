#ifndef SISTEMA_AERONODE_H
#define SISTEMA_AERONODE_H

#include "Vuelo.h"
#include "GrafoRutas.h"
#include "GestorPersistencia.h"
#include <vector>
#include <string>
#include <iostream>

//  SistemaAeroNode  –  gestor central multi-vuelo
class SistemaAeroNode {
    std::vector<Vuelo*>   vuelos;
    GrafoRutas            grafo;
    GestorPersistencia    persistencia;

public:
    explicit SistemaAeroNode(const std::string& dirDatos = "data/");
// HCR18
    ~SistemaAeroNode();

    // Gestión de vuelos
    bool   crearVuelo(const std::string& id,
                      const std::string& origen,
                      const std::string& destino,
                      const std::string& fechaHora,
                      int capacidad,
                      int capacidadBodega = 60);
    Vuelo* buscarVuelo(const std::string& id) const;
    void   listarVuelos()                      const;

    // Operaciones sobre vuelo
    // devuelve 0=OK, 1=overbooking, -1=vuelo no existe
    int  venderBillete(const std::string& vueloId,
                       const std::string& tipo,        // REGULAR|VIP|CONEXION
                       const std::string& nombre,
                       const std::string& dni,
                       const std::string& localizador,
                       const std::string& extra = ""); // beneficios / vueloConexion
// HCR18

    bool cancelarReserva(const std::string& vueloId,
                         const std::string& localizador);

    bool cargarEquipaje(const std::string& vueloId,
                        const std::string& tipoEquipaje, // MANO|FACTURADO|ESPECIAL
                        const std::string& idEquipaje,
                        const std::string& propDni,
                        double peso, double volumen,
                        const std::string& extra = "");

    void descargarBodegaCompleta(const std::string& vueloId);

    // Búsqueda
    void buscarPasajero(const std::string& clave) const; // DNI o localizador

    // Grafo de rutas
    void agregarRutaGrafo(const std::string& origen,
                          const std::string& destino,
                          int minutos,
// HCR18
                          const std::string& codigoVuelo = "");
    void mostrarGrafo()                          const;
    void mostrarRutaAlternativa(const std::string& origen,
                                const std::string& destino) const;

    // Reubicación inteligente
    bool reubicarPasajero(const std::string& vueloOrigenId,
                          const std::string& localizador);

    // Persistencia
    bool guardarSistema()  const;
    bool cargarSistema();

    // Informes
    void mostrarInformeOcupacion()    const;
    void mostrarTodasIncidencias()    const;
    void generarInformeFichero()      const;

    // Mostrar detalle vuelo
    void mostrarDetalleVuelo(const std::string& id) const;
// HCR18

    SistemaAeroNode(const SistemaAeroNode&)            = delete;
    SistemaAeroNode& operator=(const SistemaAeroNode&) = delete;

private:
    std::string generarLocalizador() const;
    void        inicializarGrafoDemo();
};

#endif // SISTEMA_AERONODE_H

#ifndef GRAFO_RUTAS_H
#define GRAFO_RUTAS_H

#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <queue>

//  Arista (conexion entre aeropuertos)
struct Arista {
    std::string destino;
    int         peso;        // minutos de vuelo o coste
    std::string codigoVuelo;
};

//  Nodo de la lista de aeropuertos (lista de adyacencia)
//  Representacion computacional segun temario S10/S11:
//  Lista de vertices + lista de aristas por vertice
struct NodoAeropuerto {
    std::string         codigo;   // IATA
    std::vector<Arista> aristas;  // lista de adyacencia del nodo
};

//  Grafo dirigido ponderado de aeropuertos
//  Representacion: lista de adyacencia (vector de NodoAeropuerto)
//  Algoritmos: Dijkstra (camino minimo) y BFS (alcanzables)
class GrafoRutas {
    std::vector<NodoAeropuerto> nodos;   // lista de vertices

    // Devuelve indice del nodo con ese codigo, o -1 si no existe
    int buscarIndice(const std::string& codigo) const;

public:
    GrafoRutas();

    void agregarAeropuerto(const std::string& codigo);
    void agregarRuta(const std::string& origen,
                     const std::string& destino,
                     int peso,
                     const std::string& codigoVuelo = "");
    void eliminarRuta(const std::string& origen, const std::string& destino);

    // Dijkstra: devuelve la ruta mas corta como vector de codigos IATA
    std::vector<std::string> rutaMasCorta(const std::string& origen,
                                          const std::string& destino,
                                          int& costoTotal) const;

    // BFS: todos los aeropuertos alcanzables desde origen
    std::vector<std::string> alcanzables(const std::string& origen) const;

    void mostrar() const;

    // Serializacion
    std::vector<std::string> getAeropuertos() const;
    const std::vector<Arista>& getAristas(const std::string& codigo) const;
};

#endif // GRAFO_RUTAS_H

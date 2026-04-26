#include "GrafoRutas.h"
#include <iostream>
#include <queue>
#include <vector>
#include <limits>
#include <string>

GrafoRutas::GrafoRutas() {}

// Busca el indice de un nodo por su codigo IATA
int GrafoRutas::buscarIndice(const std::string& codigo) const {
    for (int i = 0; i < (int)nodos.size(); i++) {
        if (nodos[i].codigo == codigo) return i;
    }
    return -1;
}

// Agrega un aeropuerto si no existe ya
void GrafoRutas::agregarAeropuerto(const std::string& codigo) {
    if (buscarIndice(codigo) == -1) {
        NodoAeropuerto nuevo;
        nuevo.codigo = codigo;
        nodos.push_back(nuevo);
    }
}

// Agrega una ruta dirigida ponderada (arista)
void GrafoRutas::agregarRuta(const std::string& origen,
                              const std::string& destino,
                              int peso,
                              const std::string& codigoVuelo) {
    agregarAeropuerto(origen);
    agregarAeropuerto(destino);
    int idx = buscarIndice(origen);

    for (int i = 0; i < (int)nodos[idx].aristas.size(); i++) {
        if (nodos[idx].aristas[i].destino == destino &&
            nodos[idx].aristas[i].codigoVuelo == codigoVuelo) {
            nodos[idx].aristas[i].peso = peso;
            return;
        }
    }

    Arista a;
    a.destino     = destino;
    a.peso        = peso;
    a.codigoVuelo = codigoVuelo;
    nodos[idx].aristas.push_back(a);
}

// Elimina una ruta (arista) entre dos aeropuertos
void GrafoRutas::eliminarRuta(const std::string& origen,
                               const std::string& destino) {
    int idx = buscarIndice(origen);
    if (idx == -1) return;
    std::vector<Arista>& lista = nodos[idx].aristas;
    int i = 0;
    while (i < (int)lista.size()) {
        if (lista[i].destino == destino) {
            // Eliminar desplazando el resto
            for (int j = i; j < (int)lista.size() - 1; j++)
                lista[j] = lista[j + 1];
            lista.pop_back();
        } else {
            i++;
        }
    }
}

//  Dijkstra - camino minimo (Algoritmo del temario S11)
//  Implementado con vector de distancias y busqueda del minimo
//  sin usar priority_queue con comparador externo ni lambdas
std::vector<std::string> GrafoRutas::rutaMasCorta(const std::string& origen,
                                                    const std::string& destino,
                                                    int& costoTotal) const {
    int n = (int)nodos.size();
    std::vector<std::string> ruta;

    int idxOrigen  = buscarIndice(origen);
    int idxDestino = buscarIndice(destino);

    if (idxOrigen == -1 || idxDestino == -1) {
        costoTotal = -1;
        return ruta;
    }

    // Distancias inicializadas a "infinito"
    const int INF = std::numeric_limits<int>::max();
    std::vector<int>  dist(n, INF);
    std::vector<int>  prev(n, -1);
    std::vector<bool> visitado(n, false);

    dist[idxOrigen] = 0;

    // n iteraciones: en cada una se elige el nodo no visitado con menor distancia
    for (int iter = 0; iter < n; iter++) {
        // Buscar el nodo no visitado con menor distancia (seleccion lineal del minimo)
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (!visitado[i] && dist[i] != INF) {
                if (u == -1 || dist[i] < dist[u]) u = i;
            }
        }
        if (u == -1) break;  // no quedan nodos alcanzables
        visitado[u] = true;

        // Relajar aristas del nodo u
        for (int j = 0; j < (int)nodos[u].aristas.size(); j++) {
            const Arista& a = nodos[u].aristas[j];
            int v = buscarIndice(a.destino);
            if (v == -1) continue;
            if (dist[u] != INF && dist[u] + a.peso < dist[v]) {
                dist[v] = dist[u] + a.peso;
                prev[v] = u;
            }
        }
    }

    costoTotal = (dist[idxDestino] == INF) ? -1 : dist[idxDestino];
    if (costoTotal < 0) return ruta;

    // Reconstruir ruta desde destino a origen
    std::vector<std::string> rutaInversa;
    int cur = idxDestino;
    while (cur != -1) {
        rutaInversa.push_back(nodos[cur].codigo);
        cur = prev[cur];
    }

    // Invertir para obtener ruta de origen a destino
    for (int i = (int)rutaInversa.size() - 1; i >= 0; i--)
        ruta.push_back(rutaInversa[i]);

    return ruta;
}

//  BFS - aeropuertos alcanzables (Algoritmo del temario S11)
//  Usa queue (Cola FIFO) tal como describe el temario
std::vector<std::string> GrafoRutas::alcanzables(const std::string& origen) const {
    std::vector<std::string> resultado;
    int idxOrigen = buscarIndice(origen);
    if (idxOrigen == -1) return resultado;

    int n = (int)nodos.size();
    std::vector<bool> visitado(n, false);
    std::queue<int>   cola;

    cola.push(idxOrigen);
    visitado[idxOrigen] = true;

    while (!cola.empty()) {
        int u = cola.front();
        cola.pop();
        resultado.push_back(nodos[u].codigo);

        for (int j = 0; j < (int)nodos[u].aristas.size(); j++) {
            int v = buscarIndice(nodos[u].aristas[j].destino);
            if (v != -1 && !visitado[v]) {
                visitado[v] = true;
                cola.push(v);
            }
        }
    }
    return resultado;
}

// Muestra el grafo por consola
void GrafoRutas::mostrar() const {
    for (int i = 0; i < (int)nodos.size(); i++) {
        std::cout << "  " << nodos[i].codigo << " -> ";
        if (nodos[i].aristas.empty()) {
            std::cout << "(sin conexiones)\n";
            continue;
        }
        for (int j = 0; j < (int)nodos[i].aristas.size(); j++) {
            const Arista& a = nodos[i].aristas[j];
            std::cout << a.destino << "(" << a.peso << "min," << a.codigoVuelo << ")";
        }
        std::cout << "\n";
    }
}

// Devuelve lista de codigos IATA de todos los aeropuertos
std::vector<std::string> GrafoRutas::getAeropuertos() const {
    std::vector<std::string> lista;
    for (int i = 0; i < (int)nodos.size(); i++)
        lista.push_back(nodos[i].codigo);
    return lista;
}

// Devuelve la lista de aristas de un aeropuerto
const std::vector<Arista>& GrafoRutas::getAristas(const std::string& codigo) const {
    static std::vector<Arista> vacio;
    int idx = buscarIndice(codigo);
    return (idx != -1) ? nodos[idx].aristas : vacio;
}

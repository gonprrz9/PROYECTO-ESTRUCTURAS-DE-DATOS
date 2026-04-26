#include "SistemaAeroNode.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>

SistemaAeroNode::SistemaAeroNode(const std::string& dirDatos)
    : persistencia(dirDatos) {
    inicializarGrafoDemo();
}

SistemaAeroNode::~SistemaAeroNode() {
    for (int i = 0; i < (int)vuelos.size(); i++) delete vuelos[i];
}

// Grafo de demostracion
void SistemaAeroNode::inicializarGrafoDemo() {
    grafo.agregarRuta("VGO", "MAD", 80,  "IB3841");
    grafo.agregarRuta("VGO", "BCN", 95,  "VY6701");
    grafo.agregarRuta("MAD", "BCN", 60,  "IB3011");
    grafo.agregarRuta("MAD", "LHR", 130, "IB3163");
    grafo.agregarRuta("MAD", "CDG", 120, "IB3451");
    grafo.agregarRuta("BCN", "CDG", 100, "VU8200");
    grafo.agregarRuta("BCN", "FCO", 110, "VY6021");
    grafo.agregarRuta("LHR", "CDG", 60,  "BA0304");
    grafo.agregarRuta("CDG", "FCO", 115, "AF1234");
    grafo.agregarRuta("LCG", "MAD", 85,  "IB3821");
    grafo.agregarRuta("LCG", "VGO", 30,  "VY3320");
    grafo.agregarRuta("MAD", "LCG", 85,  "IB3820");
    grafo.agregarRuta("VGO", "LCG", 30,  "VY3321");
}

// Generador de localizador unico
std::string SistemaAeroNode::generarLocalizador() const {
    static int contador = 1000;
    std::ostringstream ss;
    ss << "AN" << ++contador;
    return ss.str();
}

// Gestion de vuelos
bool SistemaAeroNode::crearVuelo(const std::string& id,
                                  const std::string& origen,
                                  const std::string& destino,
                                  const std::string& fechaHora,
                                  int capacidad,
                                  int capBodega) {
    if (buscarVuelo(id)) {
        std::cout << "  x Ya existe un vuelo con ID " << id << "\n";
        return false;
    }
    vuelos.push_back(new Vuelo(id, origen, destino, fechaHora, capacidad, capBodega));
    grafo.agregarAeropuerto(origen);
    grafo.agregarAeropuerto(destino);
    std::cout << "  OK Vuelo " << id << " creado ("
              << origen << "->" << destino << ", " << capacidad << " plazas)\n";
    return true;
}

Vuelo* SistemaAeroNode::buscarVuelo(const std::string& id) const {
    for (int i = 0; i < (int)vuelos.size(); i++)
        if (vuelos[i]->getId() == id) return vuelos[i];
    return nullptr;
}

void SistemaAeroNode::listarVuelos() const {
    if (vuelos.empty()) { std::cout << "  (no hay vuelos registrados)\n"; return; }
    for (int i = 0; i < (int)vuelos.size(); i++) vuelos[i]->mostrarResumen();
}

// Vender billete
int SistemaAeroNode::venderBillete(const std::string& vueloId,
                                    const std::string& tipo,
                                    const std::string& nombre,
                                    const std::string& dni,
                                    const std::string& localizador,
                                    const std::string& extra) {
    Vuelo* v = buscarVuelo(vueloId);
    if (!v) { std::cout << "  x Vuelo " << vueloId << " no encontrado.\n"; return -1; }

    Pasajero* p = nullptr;
    if (tipo == "VIP")
        p = new PasajeroVIP(nombre, dni, localizador,
                            extra.empty() ? "Sala VIP, embarque preferente" : extra);
    else if (tipo == "CONEXION")
        p = new PasajeroConexion(nombre, dni, localizador, extra, 45);
    else
        p = new PasajeroRegular(nombre, dni, localizador);

    v->registrarPasajero(p);
    return v->venderBillete(p);
}

// Cancelar reserva
bool SistemaAeroNode::cancelarReserva(const std::string& vueloId,
                                       const std::string& localizador) {
    Vuelo* v = buscarVuelo(vueloId);
    if (!v) { std::cout << "  x Vuelo " << vueloId << " no encontrado.\n"; return false; }
    return v->cancelarReserva(localizador);
}

// Cargar equipaje en bodega
bool SistemaAeroNode::cargarEquipaje(const std::string& vueloId,
                                      const std::string& tipoEq,
                                      const std::string& idEq,
                                      const std::string& propDni,
                                      double peso, double volumen,
                                      const std::string& extra) {
    Vuelo* v = buscarVuelo(vueloId);
    if (!v) { std::cout << "  x Vuelo " << vueloId << " no encontrado.\n"; return false; }

    Equipaje* e = nullptr;
    if (tipoEq == "MANO")
        e = new EquipajeMano(idEq, propDni, peso, volumen);
    else if (tipoEq == "ESPECIAL")
        e = new EquipajeEspecial(idEq, propDni, peso, volumen, extra);
    else
        e = new EquipajeFacturado(idEq, propDni, peso, volumen, extra == "FRAGIL");

    if (!v->cargarEquipaje(e)) { delete e; return false; }
    return true;
}

// Descargar bodega completa
void SistemaAeroNode::descargarBodegaCompleta(const std::string& vueloId) {
    Vuelo* v = buscarVuelo(vueloId);
    if (!v) { std::cout << "  x Vuelo " << vueloId << " no encontrado.\n"; return; }
    std::cout << "  Iniciando descarga completa de bodega (LIFO)...\n";
    int n = 0;
    while (true) {
        Equipaje* e = v->descargarEquipaje();
        if (!e) break;
        delete e;
        ++n;
    }
    std::cout << "  OK " << n << " pieza(s) descargada(s).\n";
}

// Buscar pasajero (DNI o localizador)
void SistemaAeroNode::buscarPasajero(const std::string& clave) const {
    bool encontrado = false;
    for (int i = 0; i < (int)vuelos.size(); i++) {
        Pasajero* p = vuelos[i]->buscarPorDni(clave);
        if (!p) p  = vuelos[i]->buscarPorLocalizador(clave);
        if (p) {
            std::cout << "  Encontrado en vuelo " << vuelos[i]->getId() << ": ";
            p->mostrar();
            encontrado = true;
        }
    }
    if (!encontrado)
        std::cout << "  x Pasajero no encontrado con clave: " << clave << "\n";
}

// Grafo
void SistemaAeroNode::agregarRutaGrafo(const std::string& origen,
                                        const std::string& destino,
                                        int minutos,
                                        const std::string& codigoVuelo) {
    grafo.agregarRuta(origen, destino, minutos, codigoVuelo);
    std::cout << "  OK Ruta " << origen << "->" << destino
              << " (" << minutos << " min) anadida al grafo.\n";
}

void SistemaAeroNode::mostrarGrafo() const {
    std::cout << "-- Red de rutas (Grafo ponderado) --\n";
    grafo.mostrar();
}

void SistemaAeroNode::mostrarRutaAlternativa(const std::string& origen,
                                              const std::string& destino) const {
    int costo = 0;
    std::vector<std::string> ruta = grafo.rutaMasCorta(origen, destino, costo);
    if (ruta.empty() || costo < 0) {
        std::cout << "  x No existe ruta entre " << origen << " y " << destino << "\n";
        return;
    }
    std::cout << "  Ruta optima " << origen << " -> " << destino
              << " (" << costo << " min):\n  ";
    for (int i = 0; i < (int)ruta.size(); i++) {
        std::cout << ruta[i];
        if (i + 1 < (int)ruta.size()) std::cout << " -> ";
    }
    std::cout << "\n";
}

// Reubicacion inteligente
bool SistemaAeroNode::reubicarPasajero(const std::string& vueloOrigenId,
                                        const std::string& localizador) {
    Vuelo* vOrigen = buscarVuelo(vueloOrigenId);
    if (!vOrigen) { std::cout << "  x Vuelo origen no encontrado.\n"; return false; }

    Pasajero* p = vOrigen->buscarPorLocalizador(localizador);
    if (!p) {
        std::cout << "  x Pasajero con localizador " << localizador << " no encontrado.\n";
        return false;
    }

    // Buscar vuelo alternativo con el mismo destino y plazas libres
    for (int i = 0; i < (int)vuelos.size(); i++) {
        Vuelo* vAlt = vuelos[i];
        if (vAlt->getId()      == vueloOrigenId)          continue;
        if (vAlt->getDestino() != vOrigen->getDestino())  continue;
        if (vAlt->getOcupacion() >= vAlt->getCapacidad()) continue;

        vOrigen->cancelarReserva(localizador);

        Pasajero* pNuevo = nullptr;
        if (p->getTipo() == "VIP")
            pNuevo = new PasajeroVIP(p->getNombre(), p->getDni(),
                                     generarLocalizador(),
                                     dynamic_cast<PasajeroVIP*>(p)->getBeneficios());
        else if (p->getTipo() == "CONEXION")
            pNuevo = new PasajeroConexion(p->getNombre(), p->getDni(),
                                          generarLocalizador(),
                                          dynamic_cast<PasajeroConexion*>(p)->getVueloConexion());
        else
            pNuevo = new PasajeroRegular(p->getNombre(), p->getDni(),
                                         generarLocalizador());

        vAlt->registrarPasajero(pNuevo);
        vAlt->venderBillete(pNuevo);
        std::cout << "  OK " << p->getNombre()
                  << " reubicado en vuelo alternativo " << vAlt->getId() << "\n";
        return true;
    }

    // Sin vuelo directo: intentar ruta alternativa en grafo
    int costo = 0;
    std::vector<std::string> ruta = grafo.rutaMasCorta(
        vOrigen->getOrigen(), vOrigen->getDestino(), costo);
    if (!ruta.empty() && costo >= 0) {
        std::cout << "  OK Ruta alternativa sugerida (grafo): ";
        for (int i = 0; i < (int)ruta.size(); i++) {
            std::cout << ruta[i];
            if (i + 1 < (int)ruta.size()) std::cout << " -> ";
        }
        std::cout << " (" << costo << " min)\n";
        return true;
    }

    std::cout << "  X No se encontro vuelo ni ruta alternativa.\n";
    return false;
}

// Persistencia
bool SistemaAeroNode::guardarSistema() const {
    return persistencia.guardarSistema(vuelos, grafo);
}

bool SistemaAeroNode::cargarSistema() {
    for (int i = 0; i < (int)vuelos.size(); i++) delete vuelos[i];
    vuelos.clear();
    vuelos = persistencia.cargarVuelos();
    persistencia.cargarGrafo(grafo);
    return true;
}

// Informes
void SistemaAeroNode::mostrarInformeOcupacion() const {
    std::cout << "\n INFORME DE OCUPACION \n";
    if (vuelos.empty()) { std::cout << "  (no hay vuelos)\n"; return; }
    for (int i = 0; i < (int)vuelos.size(); i++) {
        Vuelo* v = vuelos[i];
        double pct = v->getCapacidad() > 0
                     ? (100.0 * v->getOcupacion() / v->getCapacidad()) : 0.0;
        std::cout << "  " << v->getId()
                  << " (" << v->getOrigen() << "->" << v->getDestino() << ")"
                  << "  " << v->getOcupacion() << "/" << v->getCapacidad()
                  << " (" << static_cast<int>(pct) << "%)"
                  << "  Espera:" << v->getEsperaSize() << "\n";
    }
    std::cout << " \n";
}

void SistemaAeroNode::mostrarTodasIncidencias() const {
    std::cout << "\n INCIDENCIAS GLOBALES \n";
    bool hayAlguna = false;
    for (int i = 0; i < (int)vuelos.size(); i++) {
        if (!vuelos[i]->getIncidencias().empty()) {
            vuelos[i]->mostrarIncidencias();
            hayAlguna = true;
        }
    }
    if (!hayAlguna) std::cout << "  (sin incidencias registradas)\n";
    std::cout << " \n";
}

void SistemaAeroNode::generarInformeFichero() const {
    persistencia.generarInformeOcupacion(vuelos);
}

void SistemaAeroNode::mostrarDetalleVuelo(const std::string& id) const {
    Vuelo* v = buscarVuelo(id);
    if (!v) { std::cout << "  X Vuelo " << id << " no encontrado.\n"; return; }
    v->mostrarResumen();
    v->mostrarReservas();
    v->mostrarEspera();
    v->mostrarBodega();
    v->mostrarAVL();
    v->mostrarIncidencias();
}

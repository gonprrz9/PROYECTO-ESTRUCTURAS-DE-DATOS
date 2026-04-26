#include "GestorPersistencia.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <vector>
#ifdef _WIN32
    #include <direct.h>
    #define CREAR_DIR(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #define CREAR_DIR(path) mkdir(path, 0755)
#endif

GestorPersistencia::GestorPersistencia(const std::string& dir)
    : directorioBase(dir) {
    // Crear directorio si no existe
    CREAR_DIR(dir.c_str());
}

std::string GestorPersistencia::rutaVuelos()  const { return directorioBase + "vuelos.dat"; }
std::string GestorPersistencia::rutaGrafo()   const { return directorioBase + "grafo.dat"; }
std::string GestorPersistencia::rutaInforme() const { return directorioBase + "informe_ocupacion.txt"; }

// Guardar sistema completo
bool GestorPersistencia::guardarSistema(const std::vector<Vuelo*>& vuelos,
                                         const GrafoRutas& grafo) const {
// HCR18
    std::ofstream f(rutaVuelos());
    if (!f.is_open()) { std::cerr << "✗ No se pudo abrir" << rutaVuelos() << "\n"; return false; }

    for (int vi = 0; vi < (int)vuelos.size(); vi++) {
        Vuelo* v = vuelos[vi];
        f << v->serializarCabecera() << "\n";
        // Reservas
        const NodoReserva* nodo = v->getReservas().getCabeza();
        while (nodo) {
            f << "RESERVA;" << nodo->asiento << ";"
              << nodo->pasajero->serializar() << "\n";
            nodo = nodo->siguiente;
        }
        // Incidencias
        const std::vector<Incidencia>& incs = v->getIncidencias();
        for (int i = 0; i < (int)incs.size(); i++)
            f << "INCIDENCIA;" << incs[i].serializar() << "\n";
        f << "FIN_VUELO\n";
    }
    f.close();

    guardarGrafo(grafo);
// HCR18
    std::cout << "✓ Sistema guardado en" << rutaVuelos() << "\n";
    return true;
}

// Cargar vuelos
std::vector<Vuelo*> GestorPersistencia::cargarVuelos() const {
    std::vector<Vuelo*> vuelos;
    std::ifstream f(rutaVuelos());
    if (!f.is_open()) { std::cout << "(no hay datos previos)\n"; return vuelos; }

    Vuelo* vActual = nullptr;
    std::string linea;
    while (std::getline(f, linea)) {
        if (linea.empty()) continue;
        std::istringstream ss(linea);
        std::string tipo;
        std::getline(ss, tipo, ';');

        if (tipo == "VUELO") {
            std::string id, org, dst, fh, cap;
// HCR18
            std::getline(ss, id,  ';');
            std::getline(ss, org, ';');
            std::getline(ss, dst, ';');
            std::getline(ss, fh,  ';');
            std::getline(ss, cap, ';');
            vActual = new Vuelo(id, org, dst, fh, std::stoi(cap));
            vuelos.push_back(vActual);
        } else if (tipo == "RESERVA" && vActual) {
            std::string asientoStr, tipoPas, nombre, dni, loc, extra1, extra2;
            std::getline(ss, asientoStr, ';');
            std::getline(ss, tipoPas,   ';');
            std::getline(ss, nombre,    ';');
            std::getline(ss, dni,       ';');
            std::getline(ss, loc,       ';');

            Pasajero* p = nullptr;
            if (tipoPas == "REGULAR") {
                p = new PasajeroRegular(nombre, dni, loc);
            } else if (tipoPas == "VIP") {
                std::getline(ss, extra1, ';');
// HCR18
                p = new PasajeroVIP(nombre, dni, loc, extra1);
            } else if (tipoPas == "CONEXION") {
                std::getline(ss, extra1, ';');
                std::getline(ss, extra2, ';');
                p = new PasajeroConexion(nombre, dni, loc, extra1,
                                         extra2.empty() ? 45 : std::stoi(extra2));
            }
            if (p) {
                vActual->registrarPasajero(p);
                vActual->venderBillete(p);
            }
        } else if (tipo == "FIN_VUELO") {
            vActual = nullptr;
        }
    }
    f.close();
    std::cout << "✓ " << vuelos.size() << "vuelo(s) cargado(s) desde fichero.\n";
    return vuelos;
}

// HCR18
// Grafo
bool GestorPersistencia::guardarGrafo(const GrafoRutas& grafo) const {
    std::ofstream f(rutaGrafo());
    if (!f.is_open()) return false;
    std::vector<std::string> apts = grafo.getAeropuertos();
    for (int i = 0; i < (int)apts.size(); i++) {
        const std::vector<Arista>& aristas = grafo.getAristas(apts[i]);
        for (int j = 0; j < (int)aristas.size(); j++) {
            f << apts[i] << ";" << aristas[j].destino << ";"
              << aristas[j].peso << ";" << aristas[j].codigoVuelo << "\n";
        }
    }
    f.close();
    return true;
}

void GestorPersistencia::cargarGrafo(GrafoRutas& grafo) const {
    std::ifstream f(rutaGrafo());
    if (!f.is_open()) return;
    std::string linea;
    while (std::getline(f, linea)) {
        if (linea.empty()) continue;
        std::istringstream ss(linea);
// HCR18
        std::string org, dst, pesoStr, cv;
        std::getline(ss, org,     ';');
        std::getline(ss, dst,     ';');
        std::getline(ss, pesoStr, ';');
        std::getline(ss, cv,      ';');
        grafo.agregarRuta(org, dst, std::stoi(pesoStr), cv);
    }
    f.close();
}

// Informe de ocupación
bool GestorPersistencia::generarInformeOcupacion(const std::vector<Vuelo*>& vuelos) const {
    std::ofstream f(rutaInforme());
    if (!f.is_open()) return false;

    std::time_t t = std::time(nullptr);
    char buf[30];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

    f << " \n"
// HCR18
      << "INFORME DE OCUPACIÓN - AeroNode\n"
      << "Generado: " << buf << "\n"
      << " \n\n";

    for (int i = 0; i < (int)vuelos.size(); i++) {
        Vuelo* v = vuelos[i];
        double pct = v->getCapacidad() > 0
                     ? (100.0 * v->getOcupacion() / v->getCapacidad()) : 0;
        f << "Vuelo: " << v->getId()
          << "(" << v->getOrigen() << " -> " << v->getDestino() << ")\n"
          << "Fecha/Hora : " << v->getFechaHora() << "\n"
          << "Ocupacion  : " << v->getOcupacion() << "/" << v->getCapacidad()
          << "(" << pct << "%)\n"
          << "En espera  : " << v->getEsperaSize() << "\n\n";
    }
    f.close();
    std::cout << "Informe guardado en " << rutaInforme() << "\n";
    return true;
}

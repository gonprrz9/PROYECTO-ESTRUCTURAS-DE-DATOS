/*  
    AeroNode - Gestor de Aerolíneas
    Estructura de Datos  |  Grupo 4  |  UIE
    Implementa:
      · POO con herencia y polimorfismo (Pasajero, Equipaje)
      · Lista Doblemente Enlazada (reservas)
      · Cola FIFO (overbooking / lista de espera)
      · Pila LIFO (bodega de equipaje)
      · Árbol AVL (búsqueda eficiente)
      · Grafo ponderado + Dijkstra (rutas alternativas)
      · Persistencia en fichero (guardar / cargar)
      · Gestión multi-vuelo
    */

#include "SistemaAeroNode.h"
#include <iostream>
#include <limits>
#include <string>
#include <ctime>
#include <cstdlib>

// Utilidades de entrada
static void limpiarCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int leerEntero(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) { limpiarCin(); return val; }
        std::cout << "✗ Entrada inválida. Introduce un número.\n";
        limpiarCin();
    }
}

static double leerDouble(const std::string& prompt) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) { limpiarCin(); return val; }
        std::cout << "✗ Entrada inválida.\n";
        limpiarCin();
    }
}

static std::string leerCadena(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

// Submenús
static void menuVuelos(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        std::cout << "\n | Gestion de Vuelos | \n"
                  << "  1. Crear vuelo\n"
                  << "  2. Listar vuelos activos\n"
                  << "  3. Ver detalle de un vuelo\n"
                  << "  0. Volver\n";
        op = leerEntero("  Opción: ");
        switch (op) {
        case 1: {
            std::string id  = leerCadena("  ID vuelo (ej. IB1234): ");
            std::string org = leerCadena("  IATA origen (ej. VGO): ");
            std::string dst = leerCadena("  IATA destino (ej. MAD): ");
            std::string fh  = leerCadena("  Fecha/Hora (YYYY-MM-DD HH:MM): ");
            int cap  = leerEntero("  Capacidad pasajeros: ");
            int bod  = leerEntero("  Capacidad bodega (nº maletas): ");
            sistema.crearVuelo(id, org, dst, fh, cap, bod);
            break;
        }
        case 2: sistema.listarVuelos(); break;
        case 3: {
            std::string id = leerCadena("  ID vuelo: ");
            sistema.mostrarDetalleVuelo(id);
            break;
        }
        case 0: break;
        default: std::cout << "  Opcion no válida.\n";
        }
    } while (op != 0);
}

static void menuReservas(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        std::cout << "\n  | Gestion de Reservas  \n"
                  << "  1. Vender billete\n"
                  << "  2. Cancelar reserva\n"
                  << "  3. Buscar pasajero (DNI/Localizador)\n"
                  << "  4. Ver árbol AVL de reservas\n"
                  << "  0. Volver\n";
        op = leerEntero("  Opcion: ");
        switch (op) {
        case 1: {
            std::string vid = leerCadena("  ID vuelo: ");
            std::cout << "  Tipo pasajero: 1.REGULAR  2.VIP  3.CONEXION\n";
            int t = leerEntero("  Tipo: ");
            std::string tipo = (t == 2) ? "VIP" : (t == 3) ? "CONEXION" : "REGULAR";
            std::string nom  = leerCadena("  Nombre: ");
            std::string dni  = leerCadena("  DNI/Pasaporte: ");
            std::string loc  = leerCadena("  Localizador (Enter=autogen): ");
            if (loc.empty()) loc = "AN" + std::to_string(std::rand() % 9000 + 1000);
            std::string extra = "";
            if (tipo == "VIP")
                extra = leerCadena("  Beneficios (Enter=default): ");
            else if (tipo == "CONEXION")
                extra = leerCadena("  Vuelo de conexión: ");
            sistema.venderBillete(vid, tipo, nom, dni, loc, extra);
            break;
        }
        case 2: {
            std::string vid = leerCadena("  ID vuelo: ");
            std::string loc = leerCadena("  Localizador: ");
            sistema.cancelarReserva(vid, loc);
            break;
        }
        case 3: {
            std::string clave = leerCadena("  DNI o localizador: ");
            sistema.buscarPasajero(clave);
            break;
        }
        case 4: {
            // Muestra el detalle del vuelo, incluyendo el AVL
            std::string vid = leerCadena("  ID vuelo: ");
            sistema.mostrarDetalleVuelo(vid);
            break;
        }
        case 0: break;
        default: std::cout << "  Opcion no válida.\n";
        }
    } while (op != 0);
}

static void menuBodega(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        std::cout << "\n  | Gestión de Bodega (LIFO) | \n"
                  << "  1. Cargar equipaje\n"
                  << "  2. Descargar equipaje (1 pieza)\n"
                  << "  3. Descarga completa de bodega\n"
                  << "  0. Volver\n";
        op = leerEntero("  Opción: ");
        switch (op) {
        case 1: {
            std::string vid = leerCadena("  ID vuelo: ");
            std::cout << "  Tipo equipaje: 1.FACTURADO  2.MANO  3.ESPECIAL\n";
            int t = leerEntero("  Tipo: ");
            std::string tipo = (t == 2) ? "MANO" : (t == 3) ? "ESPECIAL" : "FACTURADO";
            std::string eid  = leerCadena("  ID equipaje: ");
            std::string pdni = leerCadena("  DNI propietario: ");
            double peso  = leerDouble("  Peso (kg): ");
            double vol   = leerDouble("  Volumen (L): ");
            std::string extra = "";
            if (tipo == "FACTURADO") {
                int fr = leerEntero("  ¿Fragil? 1=Si 0=No: ");
                extra = (fr == 1) ? "FRAGIL" : "";
            } else if (tipo == "ESPECIAL") {
                extra = leerCadena("  Descripcion: ");
            }
            sistema.cargarEquipaje(vid, tipo, eid, pdni, peso, vol, extra);
            break;
        }
        case 2: {
            std::string vid = leerCadena("  ID vuelo: ");
            // descargar 1 pieza
            sistema.descargarUnEquipaje(vid);
            break;
        }
        case 3: {
            std::string vid = leerCadena("  ID vuelo: ");
            sistema.descargarBodegaCompleta(vid);
            break;
        }
        case 0: break;
        default: std::cout << "  Opcion no válida.\n";
        }
    } while (op != 0);
}

static void menuRutas(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        std::cout << "\n  | Gestión de Rutas (Grafo) | \n"
                  << "  1. Mostrar red de rutas\n"
                  << "  2. Buscar ruta óptima entre aeropuertos\n"
                  << "  3. Añadir nueva ruta al grafo\n"
                  << "  4. Reubicar pasajero en vuelo alternativo\n"
                  << "  5. Ver aeropuertos alcanzables desde un origen\n"
                  << "  0. Volver\n";
        op = leerEntero("  Opcion: ");
        switch (op) {
        case 1: sistema.mostrarGrafo(); break;
        case 2: {
            std::string org = leerCadena("  IATA origen: ");
            std::string dst = leerCadena("  IATA destino: ");
            sistema.mostrarRutaAlternativa(org, dst);
            break;
        }
        case 3: {
            std::string org = leerCadena("  IATA origen: ");
            std::string dst = leerCadena("  IATA destino: ");
            int min = leerEntero("  Tiempo (min): ");
            std::string cv = leerCadena("  Código vuelo: ");
            sistema.agregarRutaGrafo(org, dst, min, cv);
            break;
        }
        case 4: {
            std::string vid = leerCadena("  ID vuelo origen: ");
            std::string loc = leerCadena("  Localizador pasajero: ");
            sistema.reubicarPasajero(vid, loc);
            break;
        }
        case 5: {
            std::string org = leerCadena("  IATA origen: ");
            sistema.mostrarAlcanzables(org);
            break;
        }
        case 0: break;
        default: std::cout << "  Opcion no válida.\n";
        }
    } while (op != 0);
}

static void menuInformes(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        std::cout << "\n  | Informes | \n"
                  << "  1. Informe de ocupacion (consola)\n"
                  << "  2. Incidencias y reubicaciones\n"
                  << "  3. Guardar informe de ocupacion en fichero\n"
                  << "  0. Volver\n";
        op = leerEntero("  Opción: ");
        switch (op) {
        case 1: sistema.mostrarInformeOcupacion();   break;
        case 2: sistema.mostrarTodasIncidencias();   break;
        case 3: sistema.generarInformeFichero();     break;
        case 0: break;
        default: std::cout << "  Opción no valida.\n";
        }
    } while (op != 0);
}

// Menú principal
static void menuPrincipal(SistemaAeroNode& sistema) {
    int op = 0;
    do {
        std::cout << "|        AeroNode  |  Gestor Aereo         |\n"
                  << "|   Estructura de Datos | Grupo 4 | UIE    |\n"
                  << " \n"
                  << "|  1. Gestion de Vuelos                    |\n"
                  << "|  2. Reservas y Pasajeros                 |\n"
                  << "|  3. Bodega de Equipaje (LIFO)            |\n"
                  << "|  4. Red de Rutas y Reubicaciones (Grafo) |\n"
                  << "|  5. Informes y Estadísticas              |\n"
                  << "|  6. Guardar sistema en fichero           |\n"
                  << "|  7. Cargar sistema desde fichero         |\n"
                  << "|  0. Salir                                |\n"
                  << " \n"
                  << " \n";
        op = leerEntero("  Opcion: ");
        switch (op) {
        case 1: menuVuelos(sistema);   break;
        case 2: menuReservas(sistema); break;
        case 3: menuBodega(sistema);   break;
        case 4: menuRutas(sistema);    break;
        case 5: menuInformes(sistema); break;
        case 6:
            if (sistema.guardarSistema())
                std::cout << "  Sistema guardado correctamente.\n";
            break;
        case 7:
            sistema.cargarSistema();
            break;
        case 0:
            std::cout << "\n  Guardando y cerrando AeroNode... ¡Hasta pronto!\n\n";
            sistema.guardarSistema();
            break;
        default:
            std::cout << "  Opción no válida. Introduce un numero del 0 al 7.\n";
        }
    } while (op != 0);
}

// Carga inicial de datos de demo
static void cargarDemoInicial(SistemaAeroNode& sistema) {
    std::cout << "\n  [DEMO] Cargando datos de demostracion...\n";

    // Crear vuelos de ejemplo
    sistema.crearVuelo("IB3841", "VGO", "MAD", "2026-04-20 08:30", 4, 8);
    sistema.crearVuelo("VY6701", "VGO", "BCN", "2026-04-20 10:15", 3, 6);
    sistema.crearVuelo("IB3011", "MAD", "BCN", "2026-04-20 12:00", 5, 10);

    // Vender billetes vuelo IB3841 (cap=4)
    sistema.venderBillete("IB3841","REGULAR","Ana García",    "12345678A","LOC001");
    sistema.venderBillete("IB3841","VIP",    "Carlos López",  "87654321B","LOC002","Sala VIP");
    sistema.venderBillete("IB3841","REGULAR","María Fernández","11223344C","LOC003");
    sistema.venderBillete("IB3841","CONEXION","Pedro Ruiz",   "44332211D","LOC004","VY6701");
    // 5º pasajero → overbooking → cola FIFO
    sistema.venderBillete("IB3841","REGULAR","Sofía Martín",  "55667788E","LOC005");

    // Cargar equipaje en bodega (LIFO)
    sistema.cargarEquipaje("IB3841","FACTURADO","EQ001","12345678A",22.0,55.0,"");
    sistema.cargarEquipaje("IB3841","FACTURADO","EQ002","87654321B",18.5,50.0,"FRAGIL");
    sistema.cargarEquipaje("IB3841","ESPECIAL", "EQ003","44332211D",15.0,60.0,"Guitarra");
    sistema.cargarEquipaje("IB3841","FACTURADO","EQ004","11223344C",20.0,52.0,"");

    // Vuelo VY6701
    sistema.venderBillete("VY6701","REGULAR","Luis Gómez",    "99887766F","LOC006");
    sistema.venderBillete("VY6701","VIP",    "Elena Sánchez", "66778899G","LOC007","Sala VIP");

    std::cout << "  [DEMO] Datos cargados. ¡Sistema listo!\n";
}

// Entry point
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "\n\n"
              << "|    Bienvenido a AeroNode                 |\n"
              << "|    Gestor de Aerolíneas en C++           |\n"
              << "============================================\n";

    SistemaAeroNode sistema("data/");

    std::cout << "\n  ¿Desea cargar datos desde fichero? (1=Si / 0=Demo): ";
    int opc;
    std::cin >> opc;
    limpiarCin();

    if (opc == 1) {
        sistema.cargarSistema();
    } else {
        cargarDemoInicial(sistema);
    }

    menuPrincipal(sistema);
    return 0;
}

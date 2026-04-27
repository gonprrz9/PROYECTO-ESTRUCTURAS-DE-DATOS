#ifndef AERONODE_H
#define AERONODE_H

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <vector>


/*
    AeroNode - Gestor de Aerolineas
    Version en dos archivos: declaraciones en AeroNode.h e implementacion en main.cpp

    Estructuras utilizadas:
      - Lista doblemente enlazada para reservas.
      - Cola FIFO para lista de espera.
      - Pila LIFO para bodega.
      - Arbol AVL para busqueda por DNI/localizador.
      - Grafo ponderado con Dijkstra y BFS.
      - POO con herencia y polimorfismo.
      - Persistencia local con ficheros de texto.
*/

// ============================================================
// DECLARACIONES DE CLASES Y ESTRUCTURAS
// ============================================================


// ---------- Pasajero.h ----------

//  Clase base abstracta: Pasajero
class Pasajero {
protected:
    std::string nombre;
    std::string dni;          // DNI / pasaporte / localizador unico
    std::string localizador;  // codigo de reserva
    int         prioridad;    // mayor numero = mayor prioridad en reubicacion

public:
    Pasajero(const std::string& nombre,
             const std::string& dni,
             const std::string& localizador,
             int prioridad = 0);
    virtual ~Pasajero();

    // Getters
    std::string getNombre() const;
    std::string getDni() const;
    std::string getLocalizador() const;
    int getPrioridad() const;

    // Setters
    void setNombre(const std::string& n);
    void setLocalizador(const std::string& l);

    // Polimorfismo: tipo de pasajero
    virtual std::string getTipo() const = 0;
    virtual void        mostrar() const;

    // Serializacion basica para fichero
    virtual std::string serializar() const;
};

//  PasajeroRegular
class PasajeroRegular : public Pasajero {
public:
    PasajeroRegular(const std::string& nombre,
                    const std::string& dni,
                    const std::string& localizador);
    std::string getTipo() const override;
    void        mostrar()     const override;
    std::string serializar()  const override;
};

//  PasajeroVIP  (prioridad alta en reubicacion)
class PasajeroVIP : public Pasajero {
    std::string beneficios;
public:
    PasajeroVIP(const std::string& nombre,
                const std::string& dni,
                const std::string& localizador,
                const std::string& beneficios = "Sala VIP, embarque preferente");
    std::string getTipo() const override;
    void        mostrar()     const override;
    std::string serializar()  const override;
    std::string getBeneficios() const;
};

//  PasajeroConexion  (necesita vuelo alternativo si pierde enlace)
class PasajeroConexion : public Pasajero {
    std::string vueloConexion;   // codigo del vuelo de conexion
    int         tiempoMinConex;  // minutos minimos necesarios
public:
    PasajeroConexion(const std::string& nombre,
                     const std::string& dni,
                     const std::string& localizador,
                     const std::string& vueloConexion,
                     int tiempoMinConex = 45);
    std::string getTipo() const override;
    void        mostrar()          const override;
    std::string serializar()       const override;
    std::string getVueloConexion() const;
    int getTiempoMin() const;
};



// ---------- Equipaje.h ----------

//  Clase base: Equipaje
class Equipaje {
protected:
    std::string id;
    std::string propietarioDni;
    double      peso;    // kg
    double      volumen; // litros
public:
    Equipaje(const std::string& id,
             const std::string& propietarioDni,
             double peso,
             double volumen);
    virtual ~Equipaje();

    std::string getId() const;
    std::string getPropietarioDni() const;
    double getPeso() const;
    double getVolumen() const;

    virtual std::string getTipo()     const = 0;
    virtual void        mostrar()     const;
    virtual std::string serializar()  const;
};

//  EquipajeMano  (< 10 kg, cabina)
class EquipajeMano : public Equipaje {
public:
    EquipajeMano(const std::string& id,
                 const std::string& propDni,
                 double peso, double volumen);
    std::string getTipo() const override;
    void        mostrar()    const override;
    std::string serializar() const override;
};

//  EquipajeFacturado  (bodega)
class EquipajeFacturado : public Equipaje {
    bool        fragil;
public:
    EquipajeFacturado(const std::string& id,
                      const std::string& propDni,
                      double peso, double volumen,
                      bool fragil = false);
    std::string getTipo() const override;
    bool esFragil() const;
    void        mostrar()    const override;
    std::string serializar() const override;
};

//  EquipajeEspecial  (animales, instrumentos, deportes)
class EquipajeEspecial : public Equipaje {
    std::string descripcion;
public:
    EquipajeEspecial(const std::string& id,
                     const std::string& propDni,
                     double peso, double volumen,
                     const std::string& descripcion);
    std::string getTipo() const override;
    std::string getDescripcion() const;
    void        mostrar()       const override;
    std::string serializar()    const override;
};



// ---------- Estructuras.h ----------

//  COLA FIFO  -  Lista de espera por overbooking
struct NodoCola {
    Pasajero* pasajero;
    NodoCola* siguiente;
    NodoCola(Pasajero* p);
};

class ColaEspera {
    NodoCola* frente;
    NodoCola* final_;
    int       tamano;
public:
    ColaEspera();
    ~ColaEspera();

    void      encolar(Pasajero* p);
    Pasajero* desencolar();           // extrae el primero (FIFO)
    Pasajero* verFrente() const;
    bool      contieneDni(const std::string& dni) const;
    bool      contieneLocalizador(const std::string& loc) const;
    bool estaVacia() const;
    int getTamano() const;
    void      mostrar()   const;

};

//  PILA LIFO  -  Bodega de equipaje
struct NodoPila {
    Equipaje* equipaje;
    NodoPila* siguiente;
    NodoPila(Equipaje* e);
};

class PilaBodega {
    NodoPila* tope;
    int       tamano;
    int       capacidadMax;
public:
    explicit PilaBodega(int cap = 100);
    ~PilaBodega();

    bool      apilar(Equipaje* e);    // carga equipaje
    Equipaje* desapilar();            // descarga (ultimo en entrar, primero en salir)
    Equipaje* verTope() const;
    bool estaVacia() const;
    bool estaLlena() const;
    int getTamano() const;
    int getCapacidadMaxima() const;
    void      mostrar()   const;

};



// ---------- ListaReservas.h ----------

//  Nodo de la lista doblemente enlazada
struct NodoReserva {
    Pasajero*    pasajero;
    int          asiento;
    NodoReserva* siguiente;
    NodoReserva* anterior;

    NodoReserva(Pasajero* p, int a);
};

//  Lista Doblemente Enlazada de Reservas
class ListaReservas {
    NodoReserva* cabeza;
    NodoReserva* cola;
    int          tamano;
    int          capacidadMaxima;

public:
    explicit ListaReservas(int capacidad);
    ~ListaReservas();

    // Operaciones principales
    bool        insertar(Pasajero* p, int asiento);
    bool        eliminar(const std::string& localizador, Pasajero*& pasajeroEliminado);
    NodoReserva* buscarPorLocalizador(const std::string& loc) const;
    NodoReserva* buscarPorDni(const std::string& dni)         const;

    // Consultas
    bool estaLlena() const;
    bool estaVacia() const;
    int getTamano() const;
    int getCapacidad() const;
    int asientosLibres() const;

    // Primer asiento libre disponible
    int  primerAsientoLibre() const;

    // Mostrar
    void mostrar() const;

    // Iteracion (para serializacion)
    NodoReserva* getCabeza() const;

    // Deshabilitar copia
};



// ---------- ArbolAVL.h ----------

//  Nodo AVL
struct NodoAVL {
    std::string clave;    // DNI o localizador
    Pasajero*   pasajero;
    int         altura;
    NodoAVL*    izq;
    NodoAVL*    der;

    NodoAVL(const std::string& k, Pasajero* p);
};

//  Arbol AVL
class ArbolAVL {
    NodoAVL* raiz;

    int      altura(NodoAVL* n) const;
    int      balance(NodoAVL* n) const;
    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);
    NodoAVL* insertar(NodoAVL* nodo, const std::string& clave, Pasajero* p);
    NodoAVL* eliminar(NodoAVL* nodo, const std::string& clave);
    NodoAVL* minNodo(NodoAVL* nodo) const;
    NodoAVL* buscar(NodoAVL* nodo, const std::string& clave) const;
    void     inorden(NodoAVL* nodo) const;
    void     destruir(NodoAVL* nodo);

public:
    ArbolAVL();
    ~ArbolAVL();

    void      insertar(const std::string& clave, Pasajero* p);
    void      eliminar(const std::string& clave);
    Pasajero* buscar(const std::string& clave) const;
    void      mostrarInorden() const;
    bool estaVacio() const;

};



// ---------- Incidencia.h ----------

//  Incidencia  -  registro de eventos anomalos
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
    static TipoIncidencia tipoDesdeString(const std::string& texto);
};



// ---------- GrafoRutas.h ----------

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



// ---------- Vuelo.h ----------

//  Clase Vuelo  -  nucleo del sistema
class Vuelo {
    std::string    id;           // p.ej. "VY1234"
    std::string    origen;       // IATA origen
    std::string    destino;      // IATA destino
    std::string    fechaHora;
    int            capacidad;
    bool           activo;

    ListaReservas  reservas;
    ColaEspera     listaEspera;
    PilaBodega     bodega;
    ArbolAVL       indiceAVL;    // indice de busqueda por DNI y localizador

    std::vector<Incidencia> incidencias;

    // Propietario de los pasajeros en este vuelo
    std::vector<Pasajero*>  pasajerosAllocated;

public:
    Vuelo(const std::string& id,
          const std::string& origen,
          const std::string& destino,
          const std::string& fechaHora,
          int capacidad,
          int capacidadBodega = 60);
    ~Vuelo();

    // Getters
    std::string getId() const;
    std::string getOrigen() const;
    std::string getDestino() const;
    std::string getFechaHora() const;
    int getCapacidad() const;
    int getCapacidadBodega() const;
    bool isActivo() const;
    int getOcupacion() const;

    // Operaciones de reserva
    // Devuelve: 0=OK, 1=overbooking(anadido a espera), 2=error
    int  venderBillete(Pasajero* p);
    bool cancelarReserva(const std::string& localizador);

    // Busqueda
    Pasajero*    buscarPorDni(const std::string& dni)         const;
    Pasajero*    buscarPorLocalizador(const std::string& loc) const;

    // Bodega
    bool     cargarEquipaje(Equipaje* e);
    Equipaje* descargarEquipaje();
    bool bodegaEstaVacia() const;

    // Lista de espera
    int getEsperaSize() const;

    // Mostrar
    void mostrarResumen()     const;
    void mostrarReservas()    const;
    void mostrarEspera()      const;
    void mostrarBodega()      const;
    void mostrarAVL()         const;
    void mostrarIncidencias() const;

    // Serializacion
    std::string serializarCabecera() const;
    bool cargarReservaPersistida(Pasajero* p, int asiento);
    void agregarIncidenciaCargada(const Incidencia& inc);
    const std::vector<Incidencia>& getIncidencias() const;
    const ListaReservas& getReservas() const;
    const ColaEspera& getEspera() const;

    // Tomar propiedad de puntero de pasajero
    void registrarPasajero(Pasajero* p);


private:
    void registrarIncidencia(TipoIncidencia t,
                              const std::string& desc,
                              const std::string& dni);
    std::string timestampActual() const;
};



// ---------- GestorPersistencia.h ----------

//  GestorPersistencia  -  guarda y carga vuelos, reservas y grafo
class GestorPersistencia {
    std::string directorioBase;
public:
    explicit GestorPersistencia(const std::string& dir = "");

    // Guardar vuelos, reservas, incidencias registradas y grafo
    bool guardarSistema(const std::vector<Vuelo*>& vuelos,
                        const GrafoRutas& grafo) const;

    // Cargar vuelos (devuelve punteros; el llamador es dueno)
    std::vector<Vuelo*> cargarVuelos() const;

    // Guardar/cargar solo el grafo
    bool         guardarGrafo(const GrafoRutas& grafo) const;
    void         cargarGrafo(GrafoRutas& grafo)        const;

    // Informe de ocupacion
    bool generarInformeOcupacion(const std::vector<Vuelo*>& vuelos) const;

private:
    std::string rutaVuelos()    const;
    std::string rutaGrafo()     const;
    std::string rutaInforme()   const;
};



// ---------- SistemaAeroNode.h ----------

//  SistemaAeroNode  -  gestor central multi-vuelo
class SistemaAeroNode {
    std::vector<Vuelo*>   vuelos;
    GrafoRutas            grafo;
    GestorPersistencia    persistencia;

public:
    explicit SistemaAeroNode(const std::string& dirDatos = "");
    ~SistemaAeroNode();

    // Gestion de vuelos
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

    bool cancelarReserva(const std::string& vueloId,
                         const std::string& localizador);

    bool cargarEquipaje(const std::string& vueloId,
                        const std::string& tipoEquipaje, // MANO|FACTURADO|ESPECIAL
                        const std::string& idEquipaje,
                        const std::string& propDni,
                        double peso, double volumen,
                        const std::string& extra = "");

    void descargarUnEquipaje(const std::string& vueloId);
    void descargarBodegaCompleta(const std::string& vueloId);

    // Busqueda
    void buscarPasajero(const std::string& clave) const; // DNI o localizador

    // Grafo de rutas
    void agregarRutaGrafo(const std::string& origen,
                          const std::string& destino,
                          int minutos,
                          const std::string& codigoVuelo = "");
    void mostrarGrafo()                          const;
    void mostrarRutaAlternativa(const std::string& origen,
                                const std::string& destino) const;
    void mostrarAlcanzables(const std::string& origen) const;

    // Reubicacion inteligente
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


private:
    std::string generarLocalizador() const;
    void        inicializarGrafoDemo();
};

#endif // AERONODE_H

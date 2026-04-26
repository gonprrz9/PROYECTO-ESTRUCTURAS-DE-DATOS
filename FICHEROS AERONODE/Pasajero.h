#ifndef PASAJERO_H
#define PASAJERO_H

#include <string>
#include <iostream>

//  Clase base abstracta: Pasajero
class Pasajero {
protected:
    std::string nombre;
    std::string dni;          // DNI / pasaporte / localizador único
    std::string localizador;  // código de reserva
    int         prioridad;    // mayor número = mayor prioridad en reubicación

public:
    Pasajero(const std::string& nombre,
             const std::string& dni,
             const std::string& localizador,
// HCR18
             int prioridad = 0);
    virtual ~Pasajero() = default;

    // Getters
    std::string getNombre()      const { return nombre; }
    std::string getDni()         const { return dni; }
    std::string getLocalizador() const { return localizador; }
    int         getPrioridad()   const { return prioridad; }

    // Setters
    void setNombre(const std::string& n)      { nombre = n; }
    void setLocalizador(const std::string& l) { localizador = l; }

    // Polimorfismo: tipo de pasajero
    virtual std::string getTipo() const = 0;
    virtual void        mostrar() const;

    // Serialización básica para fichero
    virtual std::string serializar() const;
};
// HCR18

//  PasajeroRegular
class PasajeroRegular : public Pasajero {
public:
    PasajeroRegular(const std::string& nombre,
                    const std::string& dni,
                    const std::string& localizador);
    std::string getTipo()     const override { return "REGULAR"; }
    void        mostrar()     const override;
    std::string serializar()  const override;
};

//  PasajeroVIP  (prioridad alta en reubicación)
class PasajeroVIP : public Pasajero {
    std::string beneficios;
public:
// HCR18
    PasajeroVIP(const std::string& nombre,
                const std::string& dni,
                const std::string& localizador,
                const std::string& beneficios = "Sala VIP, embarque preferente");
    std::string getTipo()     const override { return "VIP"; }
    void        mostrar()     const override;
    std::string serializar()  const override;
    std::string getBeneficios() const { return beneficios; }
};

//  PasajeroConexion  (necesita vuelo alternativo si pierde enlace)
class PasajeroConexion : public Pasajero {
    std::string vueloConexion;   // código del vuelo de conexión
    int         tiempoMinConex;  // minutos mínimos necesarios
public:
    PasajeroConexion(const std::string& nombre,
                     const std::string& dni,
                     const std::string& localizador,
// HCR18
                     const std::string& vueloConexion,
                     int tiempoMinConex = 45);
    std::string getTipo()          const override { return "CONEXION"; }
    void        mostrar()          const override;
    std::string serializar()       const override;
    std::string getVueloConexion() const { return vueloConexion; }
    int         getTiempoMin()     const { return tiempoMinConex; }
};

#endif // PASAJERO_H

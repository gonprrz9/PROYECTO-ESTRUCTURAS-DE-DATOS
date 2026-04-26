#ifndef EQUIPAJE_H
#define EQUIPAJE_H

#include <string>
#include <iostream>

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
// HCR18
    virtual ~Equipaje() = default;

    std::string getId()             const { return id; }
    std::string getPropietarioDni() const { return propietarioDni; }
    double      getPeso()           const { return peso; }
    double      getVolumen()        const { return volumen; }

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
// HCR18
    std::string getTipo()    const override { return "MANO"; }
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
    std::string getTipo()    const override { return "FACTURADO"; }
    bool        esFragil()   const { return fragil; }
    void        mostrar()    const override;
    std::string serializar() const override;
};
// HCR18

//  EquipajeEspecial  (animales, instrumentos, deportes)
class EquipajeEspecial : public Equipaje {
    std::string descripcion;
public:
    EquipajeEspecial(const std::string& id,
                     const std::string& propDni,
                     double peso, double volumen,
                     const std::string& descripcion);
    std::string getTipo()       const override { return "ESPECIAL"; }
    std::string getDescripcion() const { return descripcion; }
    void        mostrar()       const override;
    std::string serializar()    const override;
};

#endif // EQUIPAJE_H

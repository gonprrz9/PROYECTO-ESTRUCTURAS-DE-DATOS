#include "Equipaje.h"
#include <iostream>

// Equipaje base
Equipaje::Equipaje(const std::string& id, const std::string& pd,
                   double p, double v)
    : id(id), propietarioDni(pd), peso(p), volumen(v) {}

void Equipaje::mostrar() const {
    std::cout << "[" << getTipo() << "] ID:" << id
              << " | Prop:" << propietarioDni
              << " | " << peso << "kg " << volumen << "L\n";
}

std::string Equipaje::serializar() const {
    return getTipo() + ";" + id + ";" + propietarioDni + ";"
           + std::to_string(peso) + ";" + std::to_string(volumen);
}

// EquipajeMano
EquipajeMano::EquipajeMano(const std::string& id, const std::string& pd,
                           double p, double v)
    : Equipaje(id, pd, p, v) {}

void EquipajeMano::mostrar() const {
    std::cout << "[MANO] ID:" << id << " | Prop:" << propietarioDni
              << " | " << peso << "kg\n";
}

std::string EquipajeMano::serializar() const {
    return "MANO;" + id + ";" + propietarioDni + ";"
           + std::to_string(peso) + ";" + std::to_string(volumen);
}

// EquipajeFacturado
EquipajeFacturado::EquipajeFacturado(const std::string& id,
                                     const std::string& pd,
                                     double p, double v, bool f)
    : Equipaje(id, pd, p, v), fragil(f) {}

void EquipajeFacturado::mostrar() const {
    std::cout << "[FACTURADO] ID:" << id << "| Prop:" << propietarioDni
              << "| " << peso << "kg"
              << (fragil ? "[FRAGIL]" : "") << "\n";
}

std::string EquipajeFacturado::serializar() const {
    return "FACTURADO;" + id + ";" + propietarioDni + ";"
           + std::to_string(peso) + ";" + std::to_string(volumen) + ";"
           + (fragil ? "1" : "0");
}

// EquipajeEspecial
EquipajeEspecial::EquipajeEspecial(const std::string& id,
                                   const std::string& pd,
                                   double p, double v,
                                   const std::string& desc)
    : Equipaje(id, pd, p, v), descripcion(desc) {}

void EquipajeEspecial::mostrar() const {
    std::cout << "[ESPECIAL] ID:" << id << "| Prop:" << propietarioDni
              << "| " << peso << "kg | " << descripcion << "\n";
}

std::string EquipajeEspecial::serializar() const {
    return "ESPECIAL;" + id + ";" + propietarioDni + ";"
           + std::to_string(peso) + ";" + std::to_string(volumen) + ";"
           + descripcion;
}
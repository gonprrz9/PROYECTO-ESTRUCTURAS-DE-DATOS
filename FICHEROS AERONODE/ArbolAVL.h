#ifndef ARBOL_AVL_H
#define ARBOL_AVL_H

#include "Pasajero.h"
#include <string>
#include <iostream>
#include <algorithm>

//  Nodo AVL
struct NodoAVL {
    std::string clave;    // DNI o localizador
    Pasajero*   pasajero;
    int         altura;
    NodoAVL*    izq;
    NodoAVL*    der;

    NodoAVL(const std::string& k, Pasajero* p)
        : clave(k), pasajero(p), altura(1), izq(nullptr), der(nullptr) {}
// HCR18
};

//  Árbol AVL
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
// HCR18
    ArbolAVL();
    ~ArbolAVL();

    void      insertar(const std::string& clave, Pasajero* p);
    void      eliminar(const std::string& clave);
    Pasajero* buscar(const std::string& clave) const;
    void      mostrarInorden() const;
    bool      estaVacio() const { return raiz == nullptr; }

    ArbolAVL(const ArbolAVL&)            = delete;
    ArbolAVL& operator=(const ArbolAVL&) = delete;
};

#endif // ARBOL_AVL_H

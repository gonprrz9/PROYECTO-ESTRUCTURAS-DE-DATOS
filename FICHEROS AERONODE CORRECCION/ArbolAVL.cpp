#include "ArbolAVL.h"
#include <iostream>
#include <algorithm>

ArbolAVL::ArbolAVL() : raiz(nullptr) {}
ArbolAVL::~ArbolAVL() { destruir(raiz); }

void ArbolAVL::destruir(NodoAVL* n) {
    if (!n) return;
    destruir(n->izq);
    destruir(n->der);
    delete n; // NO eliminar pasajero (propiedad del sistema)
}

int ArbolAVL::altura(NodoAVL* n) const { return n ? n->altura : 0; }
int ArbolAVL::balance(NodoAVL* n) const { return n ? altura(n->izq) - altura(n->der) : 0; }

NodoAVL* ArbolAVL::rotarDerecha(NodoAVL* y) {
    NodoAVL* x  = y->izq;
    NodoAVL* T2 = x->der;
    x->der = y;
    y->izq = T2;
    y->altura = 1 + std::max(altura(y->izq), altura(y->der));
    x->altura = 1 + std::max(altura(x->izq), altura(x->der));
    return x;
}

NodoAVL* ArbolAVL::rotarIzquierda(NodoAVL* x) {
    NodoAVL* y  = x->der;
    NodoAVL* T2 = y->izq;
    y->izq = x;
    x->der = T2;
    x->altura = 1 + std::max(altura(x->izq), altura(x->der));
    y->altura = 1 + std::max(altura(y->izq), altura(y->der));
    return y;
}

NodoAVL* ArbolAVL::insertar(NodoAVL* nodo, const std::string& clave, Pasajero* p) {
    if (!nodo) return new NodoAVL(clave, p);
    if (clave < nodo->clave)      nodo->izq = insertar(nodo->izq, clave, p);
    else if (clave > nodo->clave) nodo->der = insertar(nodo->der, clave, p);
    else { nodo->pasajero = p; return nodo; } // actualizar si misma clave

    nodo->altura = 1 + std::max(altura(nodo->izq), altura(nodo->der));
    int bal = balance(nodo);

    if (bal > 1 && clave < nodo->izq->clave)  return rotarDerecha(nodo);
    if (bal < -1 && clave > nodo->der->clave) return rotarIzquierda(nodo);
    if (bal > 1 && clave > nodo->izq->clave)  { nodo->izq = rotarIzquierda(nodo->izq); return rotarDerecha(nodo); }
    if (bal < -1 && clave < nodo->der->clave) { nodo->der = rotarDerecha(nodo->der);   return rotarIzquierda(nodo); }
    return nodo;
}

NodoAVL* ArbolAVL::minNodo(NodoAVL* n) const {
    while (n->izq) n = n->izq;
    return n;
}

NodoAVL* ArbolAVL::eliminar(NodoAVL* nodo, const std::string& clave) {
    if (!nodo) return nullptr;
    if (clave < nodo->clave)      nodo->izq = eliminar(nodo->izq, clave);
    else if (clave > nodo->clave) nodo->der = eliminar(nodo->der, clave);
    else {
        if (!nodo->izq || !nodo->der) {
            NodoAVL* tmp = nodo->izq ? nodo->izq : nodo->der;
            delete nodo;
            return tmp;
        }
        NodoAVL* suc = minNodo(nodo->der);
        nodo->clave   = suc->clave;
        nodo->pasajero = suc->pasajero;
        nodo->der = eliminar(nodo->der, suc->clave);
    }
    nodo->altura = 1 + std::max(altura(nodo->izq), altura(nodo->der));
    int bal = balance(nodo);
    if (bal > 1  && balance(nodo->izq) >= 0)  return rotarDerecha(nodo);
    if (bal > 1  && balance(nodo->izq) < 0)   { nodo->izq = rotarIzquierda(nodo->izq); return rotarDerecha(nodo); }
    if (bal < -1 && balance(nodo->der) <= 0)  return rotarIzquierda(nodo);
    if (bal < -1 && balance(nodo->der) > 0)   { nodo->der = rotarDerecha(nodo->der); return rotarIzquierda(nodo); }
    return nodo;
}

NodoAVL* ArbolAVL::buscar(NodoAVL* nodo, const std::string& clave) const {
    if (!nodo || nodo->clave == clave) return nodo;
    return (clave < nodo->clave) ? buscar(nodo->izq, clave)
                                 : buscar(nodo->der, clave);
}

void ArbolAVL::inorden(NodoAVL* nodo) const {
    if (!nodo) return;
    inorden(nodo->izq);
    std::cout << "  [" << nodo->clave << "] -> ";
    nodo->pasajero->mostrar();
    inorden(nodo->der);
}

// Interfaz pública
void ArbolAVL::insertar(const std::string& clave, Pasajero* p) {
    raiz = insertar(raiz, clave, p);
}

void ArbolAVL::eliminar(const std::string& clave) {
    raiz = eliminar(raiz, clave);
}

Pasajero* ArbolAVL::buscar(const std::string& clave) const {
    NodoAVL* n = buscar(raiz, clave);
    return n ? n->pasajero : nullptr;
}

void ArbolAVL::mostrarInorden() const {
    if (estaVacio()) { std::cout << "  (árbol vacío)\n"; return; }
    inorden(raiz);
}

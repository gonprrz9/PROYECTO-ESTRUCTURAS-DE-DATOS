# AeroNode - Gestor de aerolíneas

Proyecto de Estructuras de Datos en C++.

## Compilación recomendada

```bash
make
```

También puede compilarse manualmente por módulos:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -c ArbolAVL.cpp Equipaje.cpp Estructuras.cpp GestorPersistencia.cpp GrafoRutas.cpp Incidencia.cpp ListaReservas.cpp Pasajero.cpp SistemaAeroNode.cpp Vuelo.cpp main.cpp
g++ ArbolAVL.o Equipaje.o Estructuras.o GestorPersistencia.o GrafoRutas.o Incidencia.o ListaReservas.o Pasajero.o SistemaAeroNode.o Vuelo.o main.o -o AeroNode
```

## Ejecución

```bash
./AeroNode
```

## Estructuras de datos usadas

- Lista doblemente enlazada para reservas.
- Cola FIFO para lista de espera por overbooking.
- Pila LIFO para bodega de equipaje.
- Árbol AVL para búsqueda por DNI y localizador.
- Grafo dirigido ponderado para red de rutas.
- Dijkstra para ruta óptima.
- BFS para aeropuertos alcanzables.

## Nota sobre persistencia

La persistencia guarda y carga vuelos, reservas principales y la red de rutas. La cola de espera y el estado completo de bodega quedan como posibles mejoras futuras.

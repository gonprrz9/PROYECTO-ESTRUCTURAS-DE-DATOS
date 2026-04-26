CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = AeroNode
SOURCES = ArbolAVL.cpp Equipaje.cpp Estructuras.cpp GestorPersistencia.cpp GrafoRutas.cpp Incidencia.cpp ListaReservas.cpp Pasajero.cpp SistemaAeroNode.cpp Vuelo.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

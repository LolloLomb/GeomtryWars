# Nome dell'eseguibile
TARGET = game

# File sorgenti
SOURCES = main.cpp Game.cpp Entity.cpp EntityManager.cpp Vec2.cpp

# File oggetto generati
OBJECTS = $(SOURCES:.cpp=.o)

# Compilatore e flag
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Regola principale
all: $(TARGET)

# Creazione dell'eseguibile
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Regola per compilare i file .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pulizia
clean:
	rm -f $(OBJECTS) $(TARGET)

# Pulizia dei file generati (.o e .gch)
deepclean: clean
	rm -f *.gch

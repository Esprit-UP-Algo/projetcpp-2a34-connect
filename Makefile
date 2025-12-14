# Makefile for Banking System

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = banque_app
SOURCES = main_banque.cpp CompteBancaire.cpp CompteEpargne.cpp CompteCourant.cpp Banque.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

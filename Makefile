# Variáveis
CXX = g++
CXXFLAGS = -Wall -O2
CPPFILES = main.cpp Instancia.cpp
HPPFILES = Instancia.hpp
TARGET = programa

# Regras
$(TARGET): $(CPPFILES) $(HPPFILES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(CPPFILES)

# Regra para limpeza
clean:
	rm -f $(TARGET)

# Define o alvo padrão
all: $(TARGET)

# Define quais regras não são nomes de arquivos
.PHONY: clean all

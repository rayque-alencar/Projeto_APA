# Variáveis
CXX = g++
CXXFLAGS = -Wall -O2
CPPFILES = main.cpp Instancia.cpp Solucao.cpp
HPPFILES = Instancia.hpp Solucao.hpp
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

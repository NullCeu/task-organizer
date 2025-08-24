# Makefile para TaskCLI
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = taskcli
SOURCE = task_manager.cpp
INSTALL_DIR = /usr/local/bin

# Detectar sistema operacional
UNAME_S := $(shell uname -s)

# Configurações específicas por sistema
ifeq ($(UNAME_S),Linux)
    CXXFLAGS += -D_GNU_SOURCE
endif

ifeq ($(UNAME_S),Darwin)
    CXXFLAGS += -D_DARWIN_C_SOURCE
endif

# Targets principais
all: $(TARGET)

$(TARGET): $(SOURCE)
	@echo "🔨 Compilando TaskCLI..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)
	@echo "✅ Compilação concluída! Execute com: ./$(TARGET)"

# Compilação com debug
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)
	@echo "🐛 Versão debug compilada!"

# Instalação no sistema (Linux/macOS)
install: $(TARGET)
	@echo "📦 Instalando TaskCLI no sistema..."
	sudo cp $(TARGET) $(INSTALL_DIR)/
	sudo chmod +x $(INSTALL_DIR)/$(TARGET)
	@echo "✅ TaskCLI instalado! Use 'taskcli' de qualquer lugar."

# Desinstalação
uninstall:
	@echo "🗑️  Removendo TaskCLI do sistema..."
	sudo rm -f $(INSTALL_DIR)/$(TARGET)
	@echo "✅ TaskCLI removido do sistema."

# Limpeza
clean:
	@echo "🧹 Limpando arquivos..."
	rm -f $(TARGET)
	rm -f *.o
	rm -f tasks.txt
	rm -f *.csv
	@echo "✅ Limpeza concluída!"

# Executar testes básicos
test: $(TARGET)
	@echo "🧪 Executando testes básicos..."
	./$(TARGET) add "Teste de funcionalidade" -p alta
	./$(TARGET) add "Segunda tarefa" -p baixa -d 2025-08-25
	./$(TARGET) list
	./$(TARGET) done 1
	./$(TARGET) list
	./$(TARGET) export test_export.csv
	@echo "✅ Testes concluídos!"

# Mostrar informações do build
info:
	@echo "📋 Informações do build:"
	@echo "   Compilador: $(CXX)"
	@echo "   Flags: $(CXXFLAGS)"
	@echo "   Target: $(TARGET)"
	@echo "   Sistema: $(UNAME_S)"

# Release (compilação otimizada)
release: CXXFLAGS += -DNDEBUG -O3 -s
release: $(TARGET)
	@echo "🚀 Versão release compilada!"

# Verificar dependências
check:
	@echo "🔍 Verificando dependências..."
	@which $(CXX) > /dev/null || (echo "❌ g++ não encontrado!" && exit 1)
	@echo "✅ Todas as dependências encontradas!"

# Help
help:
	@echo "🚀 TaskCLI - Makefile Help"
	@echo "========================="
	@echo "Targets disponíveis:"
	@echo "  all      - Compilar o projeto (padrão)"
	@echo "  debug    - Compilar com símbolos de debug"
	@echo "  release  - Compilar versão otimizada"
	@echo "  install  - Instalar no sistema (requer sudo)"
	@echo "  uninstall- Remover do sistema (requer sudo)"
	@echo "  test     - Executar testes básicos"
	@echo "  clean    - Limpar arquivos gerados"
	@echo "  check    - Verificar dependências"
	@echo "  info     - Mostrar informações do build"
	@echo "  help     - Mostrar esta ajuda"

.PHONY: all debug release install uninstall clean test check info help

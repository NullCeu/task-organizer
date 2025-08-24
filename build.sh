#!/bin/bash

# Script de build automático para TaskCLI
# Autor: TaskCLI Project
# Versão: 1.0

set -e  # Parar em caso de erro

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# Funções utilitárias
print_header() {
    echo -e "${BLUE}${BOLD}"
    echo "=================================="
    echo "  🚀 TaskCLI Build Script v1.0"
    echo "=================================="
    echo -e "${NC}"
}

print_step() {
    echo -e "${YELLOW}${BOLD}[STEP]${NC} $1"
}

print_success() {
    echo -e "${GREEN}${BOLD}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}${BOLD}[ERROR]${NC} $1"
}

check_dependencies() {
    print_step "Verificando dependências..."
    
    # Verificar g++
    if ! command -v g++ &> /dev/null; then
        print_error "g++ não encontrado! Instale um compilador C++."
        echo "Ubuntu/Debian: sudo apt install build-essential"
        echo "CentOS/RHEL: sudo yum install gcc-c++"
        echo "macOS: xcode-select --install"
        exit 1
    fi
    
    # Verificar versão do g++
    GCC_VERSION=$(g++ -dumpversion | cut -d. -f1)
    if [ "$GCC_VERSION" -lt 7 ]; then
        print_error "g++ versão muito antiga! Requer versão 7+ para C++17."
        exit 1
    fi
    
    # Verificar make (opcional)
    if command -v make &> /dev/null; then
        HAS_MAKE=true
        print_success "g++ $(g++ -dumpversion) e make encontrados!"
    else
        HAS_MAKE=false
        print_success "g++ $(g++ -dumpversion) encontrado! (make não disponível)"
    fi
}

build_project() {
    print_step "Compilando TaskCLI..."
    
    if [ "$HAS_MAKE" = true ] && [ -f "Makefile" ]; then
        make clean 2>/dev/null || true
        make
    else
        # Build manual sem Makefile
        g++ -std=c++17 -Wall -Wextra -O2 -o taskcli task_manager.cpp
    fi
    
    if [ -f "taskcli" ]; then
        print_success "Compilação concluída com sucesso!"
        chmod +x taskcli
    else
        print_error "Falha na compilação!"
        exit 1
    fi
}

run_tests() {
    print_step "Executando testes básicos..."
    
    # Limpar dados de teste anteriores
    rm -f tasks.txt test_export.csv 2>/dev/null || true
    
    # Testes básicos
    ./taskcli add "Teste de funcionalidade básica" -p alta
    ./taskcli add "Segunda tarefa de teste" -p baixa -d 2025-08-25
    ./taskcli add "Terceira tarefa" -p media
    
    # Verificar se as tarefas foram criadas
    if [ ! -f "tasks.txt" ]; then
        print_error "Arquivo de tarefas não foi criado!"
        exit 1
    fi
    
    # Marcar uma como concluída
    ./taskcli done 1
    
    # Exportar para CSV
    ./taskcli export test_export.csv
    
    if [ ! -f "test_export.csv" ]; then
        print_error "Exportação CSV falhou!"
        exit 1
    fi
    
    print_success "Todos os testes passaram!"
    
    # Mostrar resultado final
    echo -e "\n${BOLD}Resultado dos testes:${NC}"
    ./taskcli list
    
    # Limpeza dos testes
    rm -f tasks.txt test_export.csv 2>/dev/null || true
}

show_usage() {
    echo -e "${BOLD}Uso:${NC} $0 [opções]"
    echo ""
    echo "Opções:"
    echo "  -h, --help       Mostrar esta ajuda"
    echo "  -t, --test       Executar testes após build"
    echo "  -i, --install    Instalar no sistema após build"
    echo "  -c, --clean      Limpar antes de compilar"
    echo "  -r, --release    Build de release otimizado"
    echo "  -d, --debug      Build com símbolos de debug"
    echo ""
    echo "Exemplos:"
    echo "  $0                 # Build padrão"
    echo "  $0 --test          # Build + testes"
    echo "  $0 --release       # Build otimizado"
    echo "  $0 --install       # Build + instalar"
}

# Análise de argumentos
BUILD_TYPE="default"
RUN_TESTS=false
INSTALL_AFTER=false
CLEAN_FIRST=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -t|--test)
            RUN_TESTS=true
            shift
            ;;
        -i|--install)
            INSTALL_AFTER=true
            shift
            ;;
        -c|--clean)
            CLEAN_FIRST=true
            shift
            ;;
        -r|--release)
            BUILD_TYPE="release"
            shift
            ;;
        -d|--debug)
            BUILD_TYPE="debug"
            shift
            ;;
        *)
            print_error "Opção desconhecida: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Script principal
main() {
    print_header
    
    # Verificar se estamos no diretório correto
    if [ ! -f "task_manager.cpp" ]; then
        print_error "task_manager.cpp não encontrado! Execute no diretório do projeto."
        exit 1
    fi
    
    check_dependencies
    
    # Limpeza se solicitada
    if [ "$CLEAN_FIRST" = true ]; then
        print_step "Limpando arquivos anteriores..."
        rm -f taskcli *.o tasks.txt *.csv 2>/dev/null || true
        print_success "Limpeza concluída!"
    fi
    
    # Build baseado no tipo
    case $BUILD_TYPE in
        "release")
            print_step "Compilando versão RELEASE..."
            if [ "$HAS_MAKE" = true ]; then
                make release
            else
                g++ -std=c++17 -Wall -Wextra -O3 -DNDEBUG -s -o taskcli task_manager.cpp
            fi
            ;;
        "debug")
            print_step "Compilando versão DEBUG..."
            if [ "$HAS_MAKE" = true ]; then
                make debug
            else
                g++ -std=c++17 -Wall -Wextra -g -DDEBUG -o taskcli task_manager.cpp
            fi
            ;;
        *)
            build_project
            ;;
    esac
    
    # Executar testes se solicitado
    if [ "$RUN_TESTS" = true ]; then
        run_tests
    fi
    
    # Instalar se solicitado
    if [ "$INSTALL_AFTER" = true ]; then
        print_step "Instalando no sistema..."
        if [ "$HAS_MAKE" = true ]; then
            make install
        else
            sudo cp taskcli /usr/local/bin/
            sudo chmod +x /usr/local/bin/taskcli
            print_success "TaskCLI instalado em /usr/local/bin/"
        fi
    fi
    
    # Informações finais
    echo -e "\n${GREEN}${BOLD}🎉 Build concluído com sucesso!${NC}"
    echo -e "Execute: ${BLUE}${BOLD}./taskcli help${NC} para começar"
    
    if [ "$INSTALL_AFTER" = true ]; then
        echo -e "Ou use: ${BLUE}${BOLD}taskcli help${NC} de qualquer lugar"
    fi
    
    # Mostrar informações do binário
    if [ -f "taskcli" ]; then
        BINARY_SIZE=$(du -h taskcli | cut -f1)
        echo -e "\nInformações do binário:"
        echo -e "  Tamanho: ${BINARY_SIZE}"
        echo -e "  Tipo: $BUILD_TYPE"
        echo -e "  Localização: $(pwd)/taskcli"
    fi
}

# Executar função principal
main "$@"

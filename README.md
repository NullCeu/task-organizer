# 🚀 TaskCLI - Gerenciador de Tarefas em C++

[![C++](https://img.shields.io/badge/C++-17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)]()

Um gerenciador de tarefas poderoso e elegante para linha de comando, desenvolvido em C++17/20. Organize suas tarefas com estilo, prioridades, datas e muito mais!

## ✨ Funcionalidades

- ✅ **Adicionar tarefas** com descrição personalizada
- 📋 **Listar tarefas** com formatação colorida e organizada  
- ✔️ **Marcar como concluída** tarefas específicas
- 🗑️ **Remover tarefas** individualmente
- 📊 **Sistema de prioridades** (Alta, Média, Baixa)
- 📅 **Suporte a datas** para organização temporal
- 💾 **Persistência automática** em arquivo
- 📈 **Exportação para CSV** (Excel/Google Sheets)
- 🎨 **Interface colorida** e intuitiva
- 📊 **Resumo estatístico** das tarefas
- 🧹 **Limpeza automática** de tarefas concluídas

## 🛠️ Tecnologias Utilizadas

- **C++17/20** - Linguagem principal
- **STL** - Standard Template Library para estruturas de dados
- **fstream** - Manipulação de arquivos
- **ANSI Colors** - Cores no terminal
- **Cross-platform** - Funciona em Linux, macOS e Windows

## 📦 Instalação

### Compilação Manual

```bash
# Clone o repositório
git clone https://github.com/seuusuario/taskcli.git
cd taskcli

# Compile o projeto
make

# Execute
./taskcli help
```

### Instalação no Sistema

```bash
# Compile e instale
make install

# Agora você pode usar de qualquer lugar
taskcli help
```

### Dependências

- **g++** com suporte a C++17 ou superior
- **make** (opcional, mas recomendado)

## 🚀 Uso Rápido

```bash
# Adicionar uma nova tarefa
./taskcli add "Estudar C++ às 19h"

# Adicionar com prioridade e data
./taskcli add "Reunião importante" -p alta -d 2025-08-25

# Listar todas as tarefas
./taskcli list

# Marcar tarefa como concluída
./taskcli done 1

# Remover uma tarefa
./taskcli remove 2

# Exportar para CSV
./taskcli export minhas_tarefas.csv

# Organizar por prioridade
./taskcli sort

# Limpar tarefas concluídas
./taskcli clear
```

## 📖 Comandos Detalhados

### 📝 Adicionar Tarefas

```bash
# Tarefa simples
./taskcli add "Comprar leite"

# Com prioridade
./taskcli add "Apresentação projeto" -p alta

# Com data específica
./taskcli add "Consulta médica" -d 2025-08-30

# Completo (descrição + prioridade + data)
./taskcli add "Entrega relatório" -p alta -d 2025-08-28
```

**Prioridades disponíveis:** `alta`, `media`, `baixa`

### 📋 Listar Tarefas

```bash
./taskcli list
```

**Saída exemplo:**
```
📋 LISTA DE TAREFAS
================================================================================
  1. ❌ PENDENTE [ALTA] Apresentação projeto 📅 2025-08-25
  2. ✔️  CONCLUÍDA [MÉDIA] Comprar leite
  3. ❌ PENDENTE [BAIXA] Organizar arquivos

================================================================================
📊 RESUMO: Total: 3 | Concluídas: 1 | Pendentes: 2
```

### ✅ Gerenciar Status

```bash
# Marcar como concluída
./taskcli done 1

# Remover tarefa
./taskcli remove 2

# Limpar todas as concluídas
./taskcli clear
```

### 📊 Exportar e Organizar

```bash
# Exportar para CSV (Excel/Google Sheets)
./taskcli export tarefas.csv

# Organizar por prioridade
./taskcli sort
```

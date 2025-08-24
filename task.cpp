#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <map>

// Códigos ANSI para cores no terminal
namespace Colors {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string BOLD = "\033[1m";
    const std::string UNDERLINE = "\033[4m";
}

enum class Priority { BAIXA = 1, MEDIA = 2, ALTA = 3 };

struct Task {
    std::string description;
    bool completed;
    Priority priority;
    std::string date;
    std::time_t createdAt;
    
    Task(const std::string& desc, Priority prio = Priority::MEDIA, const std::string& taskDate = "") 
        : description(desc), completed(false), priority(prio), date(taskDate) {
        createdAt = std::time(nullptr);
    }
    
    std::string getPriorityString() const {
        switch(priority) {
            case Priority::ALTA: return "ALTA";
            case Priority::MEDIA: return "MÉDIA";
            case Priority::BAIXA: return "BAIXA";
            default: return "MÉDIA";
        }
    }
    
    std::string getPriorityColor() const {
        switch(priority) {
            case Priority::ALTA: return Colors::RED;
            case Priority::MEDIA: return Colors::YELLOW;
            case Priority::BAIXA: return Colors::GREEN;
            default: return Colors::RESET;
        }
    }
};

class TaskManager {
private:
    std::vector<Task> tasks;
    const std::string filename = "tasks.txt";
    
public:
    TaskManager() {
        loadTasks();
    }
    
    ~TaskManager() {
        saveTasks();
    }
    
    void addTask(const std::string& description, Priority priority = Priority::MEDIA, const std::string& date = "") {
        tasks.emplace_back(description, priority, date);
        std::cout << Colors::GREEN << "✅ Tarefa adicionada com sucesso!" << Colors::RESET << std::endl;
        saveTasks();
    }
    
    void listTasks() const {
        if (tasks.empty()) {
            std::cout << Colors::YELLOW << "📝 Nenhuma tarefa encontrada!" << Colors::RESET << std::endl;
            return;
        }
        
        std::cout << Colors::BOLD << Colors::CYAN << "\n📋 LISTA DE TAREFAS" << Colors::RESET << std::endl;
        std::cout << std::string(80, '=') << std::endl;
        
        for (size_t i = 0; i < tasks.size(); ++i) {
            const auto& task = tasks[i];
            std::string status = task.completed ? Colors::GREEN + "✔️  CONCLUÍDA" : Colors::RED + "❌ PENDENTE";
            std::string priority = task.getPriorityColor() + "[" + task.getPriorityString() + "]";
            
            std::cout << Colors::BOLD << std::setw(3) << (i + 1) << ". " << Colors::RESET;
            std::cout << status << Colors::RESET << " ";
            std::cout << priority << Colors::RESET << " ";
            std::cout << Colors::BOLD << task.description << Colors::RESET;
            
            if (!task.date.empty()) {
                std::cout << Colors::BLUE << " 📅 " << task.date << Colors::RESET;
            }
            std::cout << std::endl;
        }
        
        std::cout << std::string(80, '=') << std::endl;
        showSummary();
    }
    
    void markCompleted(int index) {
        if (index < 1 || index > static_cast<int>(tasks.size())) {
            std::cout << Colors::RED << "❌ Número de tarefa inválido!" << Colors::RESET << std::endl;
            return;
        }
        
        tasks[index - 1].completed = true;
        std::cout << Colors::GREEN << "✅ Tarefa #" << index << " marcada como concluída!" << Colors::RESET << std::endl;
        saveTasks();
    }
    
    void removeTask(int index) {
        if (index < 1 || index > static_cast<int>(tasks.size())) {
            std::cout << Colors::RED << "❌ Número de tarefa inválido!" << Colors::RESET << std::endl;
            return;
        }
        
        std::cout << Colors::YELLOW << "🗑️  Removendo: \"" << tasks[index - 1].description << "\"" << Colors::RESET << std::endl;
        tasks.erase(tasks.begin() + index - 1);
        std::cout << Colors::GREEN << "✅ Tarefa removida com sucesso!" << Colors::RESET << std::endl;
        saveTasks();
    }
    
    void exportToCSV(const std::string& csvFilename = "tasks_export.csv") const {
        std::ofstream file(csvFilename);
        if (!file.is_open()) {
            std::cout << Colors::RED << "❌ Erro ao criar arquivo CSV!" << Colors::RESET << std::endl;
            return;
        }
        
        file << "Numero,Descricao,Status,Prioridade,Data,Criado_Em\n";
        
        for (size_t i = 0; i < tasks.size(); ++i) {
            const auto& task = tasks[i];
            file << (i + 1) << ","
                 << "\"" << task.description << "\","
                 << (task.completed ? "Concluida" : "Pendente") << ","
                 << task.getPriorityString() << ","
                 << (task.date.empty() ? "N/A" : task.date) << ","
                 << std::ctime(&task.createdAt);
        }
        
        file.close();
        std::cout << Colors::GREEN << "📊 Tarefas exportadas para " << csvFilename << Colors::RESET << std::endl;
    }
    
    void sortByPriority() {
        std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            if (a.completed != b.completed) return !a.completed && b.completed;
            return static_cast<int>(a.priority) > static_cast<int>(b.priority);
        });
        std::cout << Colors::GREEN << "📈 Tarefas organizadas por prioridade!" << Colors::RESET << std::endl;
        saveTasks();
    }
    
    void clearCompleted() {
        auto initial_size = tasks.size();
        tasks.erase(std::remove_if(tasks.begin(), tasks.end(), 
                                  [](const Task& t) { return t.completed; }), 
                   tasks.end());
        
        auto removed = initial_size - tasks.size();
        std::cout << Colors::GREEN << "🧹 " << removed << " tarefas concluídas removidas!" << Colors::RESET << std::endl;
        saveTasks();
    }
    
private:
    void saveTasks() const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << Colors::RED << "❌ Erro ao salvar tarefas!" << Colors::RESET << std::endl;
            return;
        }
        
        for (const auto& task : tasks) {
            file << task.description << "|"
                 << task.completed << "|"
                 << static_cast<int>(task.priority) << "|"
                 << task.date << "|"
                 << task.createdAt << std::endl;
        }
        file.close();
    }
    
    void loadTasks() {
        std::ifstream file(filename);
        if (!file.is_open()) return;
        
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string desc, completed_str, priority_str, date, created_str;
            
            if (std::getline(ss, desc, '|') &&
                std::getline(ss, completed_str, '|') &&
                std::getline(ss, priority_str, '|') &&
                std::getline(ss, date, '|') &&
                std::getline(ss, created_str)) {
                
                Task task(desc, static_cast<Priority>(std::stoi(priority_str)), date);
                task.completed = (completed_str == "1");
                task.createdAt = std::stoll(created_str);
                tasks.push_back(task);
            }
        }
        file.close();
    }
    
    void showSummary() const {
        int total = tasks.size();
        int completed = std::count_if(tasks.begin(), tasks.end(), 
                                     [](const Task& t) { return t.completed; });
        int pending = total - completed;
        
        std::cout << Colors::BOLD << "\n📊 RESUMO: " << Colors::RESET;
        std::cout << Colors::CYAN << "Total: " << total << Colors::RESET << " | ";
        std::cout << Colors::GREEN << "Concluídas: " << completed << Colors::RESET << " | ";
        std::cout << Colors::RED << "Pendentes: " << pending << Colors::RESET << std::endl << std::endl;
    }
};

Priority parsePriority(const std::string& priority) {
    std::string p = priority;
    std::transform(p.begin(), p.end(), p.begin(), ::tolower);
    
    if (p == "alta" || p == "high" || p == "3") return Priority::ALTA;
    if (p == "baixa" || p == "low" || p == "1") return Priority::BAIXA;
    return Priority::MEDIA;
}

void showHelp() {
    std::cout << Colors::BOLD << Colors::CYAN << "\n🚀 TaskCLI - Gerenciador de Tarefas" << Colors::RESET << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << Colors::BOLD << "USO:" << Colors::RESET << " ./taskcli [comando] [argumentos]" << std::endl << std::endl;
    
    std::cout << Colors::YELLOW << "COMANDOS DISPONÍVEIS:" << Colors::RESET << std::endl;
    std::cout << "  " << Colors::GREEN << "add" << Colors::RESET << " \"descrição\"           - Adicionar nova tarefa" << std::endl;
    std::cout << "  " << Colors::GREEN << "list" << Colors::RESET << "                        - Listar todas as tarefas" << std::endl;
    std::cout << "  " << Colors::GREEN << "done" << Colors::RESET << " [número]               - Marcar tarefa como concluída" << std::endl;
    std::cout << "  " << Colors::GREEN << "remove" << Colors::RESET << " [número]             - Remover tarefa" << std::endl;
    std::cout << "  " << Colors::GREEN << "export" << Colors::RESET << " [arquivo.csv]        - Exportar para CSV" << std::endl;
    std::cout << "  " << Colors::GREEN << "sort" << Colors::RESET << "                        - Ordenar por prioridade" << std::endl;
    std::cout << "  " << Colors::GREEN << "clear" << Colors::RESET << "                       - Remover tarefas concluídas" << std::endl;
    std::cout << "  " << Colors::GREEN << "help" << Colors::RESET << "                        - Mostrar esta ajuda" << std::endl << std::endl;
    
    std::cout << Colors::YELLOW << "OPÇÕES PARA 'add':" << Colors::RESET << std::endl;
    std::cout << "  -p [alta|media|baixa]      - Definir prioridade" << std::endl;
    std::cout << "  -d [YYYY-MM-DD]            - Definir data" << std::endl << std::endl;
    
    std::cout << Colors::YELLOW << "EXEMPLOS:" << Colors::RESET << std::endl;
    std::cout << "  ./taskcli add \"Estudar C++ às 19h\" -p alta -d 2025-08-25" << std::endl;
    std::cout << "  ./taskcli list" << std::endl;
    std::cout << "  ./taskcli done 1" << std::endl;
    std::cout << "  ./taskcli export minhas_tarefas.csv" << std::endl << std::endl;
}

int main(int argc, char* argv[]) {
    TaskManager taskManager;
    
    if (argc < 2) {
        showHelp();
        return 0;
    }
    
    std::string command = argv[1];
    
    if (command == "add") {
        if (argc < 3) {
            std::cout << Colors::RED << "❌ Uso: add \"descrição da tarefa\" [-p prioridade] [-d data]" << Colors::RESET << std::endl;
            return 1;
        }
        
        std::string description = argv[2];
        Priority priority = Priority::MEDIA;
        std::string date = "";
        
        // Parse argumentos opcionais
        for (int i = 3; i < argc; i++) {
            if (std::string(argv[i]) == "-p" && i + 1 < argc) {
                priority = parsePriority(argv[++i]);
            } else if (std::string(argv[i]) == "-d" && i + 1 < argc) {
                date = argv[++i];
            }
        }
        
        taskManager.addTask(description, priority, date);
        
    } else if (command == "list") {
        taskManager.listTasks();
        
    } else if (command == "done") {
        if (argc < 3) {
            std::cout << Colors::RED << "❌ Uso: done [número da tarefa]" << Colors::RESET << std::endl;
            return 1;
        }
        taskManager.markCompleted(std::stoi(argv[2]));
        
    } else if (command == "remove") {
        if (argc < 3) {
            std::cout << Colors::RED << "❌ Uso: remove [número da tarefa]" << Colors::RESET << std::endl;
            return 1;
        }
        taskManager.removeTask(std::stoi(argv[2]));
        
    } else if (command == "export") {
        std::string filename = argc > 2 ? argv[2] : "tasks_export.csv";
        taskManager.exportToCSV(filename);
        
    } else if (command == "sort") {
        taskManager.sortByPriority();
        
    } else if (command == "clear") {
        taskManager.clearCompleted();
        
    } else if (command == "help") {
        showHelp();
        
    } else {
        std::cout << Colors::RED << "❌ Comando desconhecido: " << command << Colors::RESET << std::endl;
        std::cout << "Use " << Colors::CYAN << "'./taskcli help'" << Colors::RESET << " para ver os comandos disponíveis." << std::endl;
        return 1;
    }
    
    return 0;
}
